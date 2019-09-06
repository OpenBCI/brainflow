#include <condition_variable>
#include <ctype.h>
#include <math.h>
#include <mutex>
#include <queue>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "GanglionNativeInterface.h"
#include "cmd_def.h"
#include "helpers.h"
#include "timestamp.h"
#include "uart.h"


#define GANGLION_SERVICE_UUID 0xfe84
// seems like in this api chars uuids are not the same as in microsoft api
#define GANGLION_SEND_CHAR_UUID 0x2800
#define GANGLION_RECV_CHAR_UUID 0x2803

extern volatile int exit_code;
extern volatile bd_addr connect_addr;
extern volatile uint8 connection;
extern volatile uint16 ganglion_handle_start;
extern volatile uint16 ganglion_handle_end;
// recv and send chars handles
extern volatile uint16 ganglion_handle_recv;
extern volatile uint16 ganglion_handle_send;
extern volatile State state;

extern std::mutex m;
extern std::condition_variable cv;

extern std::queue<struct GanglionLibNative::GanglionDataNative> data_queue;

void ble_evt_gap_scan_response (const struct ble_msg_gap_scan_response_evt_t *msg)
{
    char *name = NULL;
    for (int i = 0; i < msg->data.len;)
    {
        int8 len = msg->data.data[i++];
        if (!len)
        {
            continue;
        }
        if (i + len > msg->data.len)
        {
            break; // not enough data
        }
        uint8 type = msg->data.data[i++];
        switch (type)
        {
            case 0x09:
                name = (char *)malloc (len);
                memcpy (name, msg->data.data + i, len - 1);
                name[len - 1] = '\0';
        }

        i += len - 1;
    }

    if (name)
    {
        if (strstr (name, "anglion") != NULL)
        {
            memcpy ((void *)connect_addr.addr, msg->sender.addr, sizeof (bd_addr));
            {
                std::lock_guard<std::mutex> lk (m);
                exit_code = (int)GanglionLibNative::STATUS_OK;
            }
            cv.notify_one ();
        }
    }

    free (name);
}

void ble_evt_connection_status (const struct ble_msg_connection_status_evt_t *msg)
{
    // New connection
    if (msg->flags & connection_connected)
    {
        connection = msg->connection;
        // this method is called from ble_evt_connection_disconnected after each connection timeout
        // expiration, need to set exit code only when we call this methid from open_ble_device
        if (state == State::initial_connection)
        {
            {
                std::lock_guard<std::mutex> lk (m);
                exit_code = (int)GanglionLibNative::STATUS_OK;
            }
            cv.notify_one ();
        }
    }
}

void ble_evt_connection_disconnected (const struct ble_msg_connection_disconnected_evt_t *msg)
{
    // atempt to reconnect
    ble_cmd_gap_connect_direct (&connect_addr, gap_address_type_random, 40, 60, 100, 0);
}

// ble_evt_attclient_group_found and ble_evt_attclient_procedure_completed are called after the same
// command(ble_cmd_attclient_read_by_group_type)
void ble_evt_attclient_group_found (const struct ble_msg_attclient_group_found_evt_t *msg)
{
    if (msg->uuid.len == 0)
    {
        return;
    }
    uint16 uuid = (msg->uuid.data[1] << 8) | msg->uuid.data[0];
    if (uuid == GANGLION_SERVICE_UUID)
    {
        ganglion_handle_start = msg->start;
        ganglion_handle_end = msg->end;
    }
}

void ble_evt_attclient_procedure_completed (
    const struct ble_msg_attclient_procedure_completed_evt_t *msg)
{
    if (state == State::open_called)
    {
        if ((ganglion_handle_start) && (ganglion_handle_end))
        {
            ble_cmd_attclient_find_information (msg->connection, ganglion_handle_start,
                ganglion_handle_end); // triggers ble_evt_attclient_find_information_found
        }
    }
    // triggered after ble_cmd_attclient_attribute_write
    else if (state == State::config_called)
    {
        if (msg->result == 0)
        {
            {
                std::lock_guard<std::mutex> lk (m);
                exit_code = (int)GanglionLibNative::STATUS_OK;
            }
            cv.notify_one ();
        }
    }
}

// finds characteristic handles and set exit_code for open_ganglion call
void ble_evt_attclient_find_information_found (
    const struct ble_msg_attclient_find_information_found_evt_t *msg)
{
    if (state == State::open_called)
    {
        if (msg->uuid.len == 2)
        {
            uint16 uuid = (msg->uuid.data[1] << 8) | msg->uuid.data[0];
            if (uuid == GANGLION_RECV_CHAR_UUID)
            {
                ganglion_handle_recv = msg->chrhandle;
            }
            else if (uuid == GANGLION_SEND_CHAR_UUID)
            {
                ganglion_handle_send = msg->chrhandle;
            }
        }
        if ((ganglion_handle_send) && (ganglion_handle_recv))
        {
            {
                std::lock_guard<std::mutex> lk (m);
                exit_code = (int)GanglionLibNative::STATUS_OK;
            }
            cv.notify_one ();
        }
    }
}

void ble_evt_attclient_attribute_value (const struct ble_msg_attclient_attribute_value_evt_t *msg)
{
    if ((int)msg->value.len >= 18)
    {
        unsigned char values[20] = {0};
        memcpy (values, msg->value.data + 1,
            (msg->value.len - 1) * sizeof (unsigned char)); // fist byte is a lenght
        struct GanglionLibNative::GanglionDataNative data (values, (long)get_timestamp ());
        data_queue.push (data);
    }
}
