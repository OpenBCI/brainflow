#include <condition_variable>
#include <ctype.h>
#include <mutex>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#include "helpers.h"
#include "uart.h"

#include "GanglionNativeInterface.h"

#define FIRST_HANDLE 0x0001
#define LAST_HANDLE 0xffff

extern volatile int exit_code;
extern volatile bd_addr connect_addr;
extern volatile uint8 connection;
extern char uart_port[1024];
extern volatile State state;
extern std::mutex m;
extern std::condition_variable cv;

void output (uint8 len1, uint8 *data1, uint16 len2, uint8 *data2)
{
    if (uart_tx (len1, data1) || uart_tx (len2, data2))
    {
        exit_code = (int)GanglionLibNative::PORT_OPEN_ERROR;
    }
}

// reads messages and calls required callbacks
int read_message (int timeout_ms)
{
    unsigned char data[256]; // enough for BLE
    struct ble_header hdr;
    int r;

    r = uart_rx (sizeof (hdr), (unsigned char *)&hdr, UART_TIMEOUT);
    if (!r)
    {
        return -1; // timeout
    }
    else if (r < 0)
    {
        exit_code = (int)GanglionLibNative::PORT_OPEN_ERROR;
        return 1; // fails to read
    }
    if (hdr.lolen)
    {
        r = uart_rx (hdr.lolen, data, UART_TIMEOUT);
        if (r <= 0)
        {
            exit_code = (int)GanglionLibNative::PORT_OPEN_ERROR;
            return 1; // fails to read
        }
    }

    const struct ble_msg *msg = ble_get_msg_hdr (hdr);

    if (!msg)
    {
        exit_code = (int)GanglionLibNative::GENERAL_ERROR;
        return 1;
    }

    msg->handler (data);

    return 0;
}

std::string get_dongle_port ()
{
    const char *val = getenv (GANGLION_DONGLE_PORT);
    if (val == NULL)
    {
        return "";
    }
    else
    {
        return val;
    }
}

int open_ble_dev ()
{
    exit_code = (int)GanglionLibNative::SYNC_ERROR;
    // send command to connect
    state = State::initial_connection;
    ble_cmd_gap_connect_direct (&connect_addr, gap_address_type_random, 40, 60, 100, 0);
    int res = wait_for_callback (15);
    if (res != (int)GanglionLibNative::STATUS_OK)
    {
        return res;
    }
    state = State::open_called;

    exit_code = (int)GanglionLibNative::SYNC_ERROR;
    uint8 primary_service_uuid[] = {0x00, 0x28};
    ble_cmd_attclient_read_by_group_type (
        connection, FIRST_HANDLE, LAST_HANDLE, 2, primary_service_uuid);

    return wait_for_callback (15);
}

int wait_for_callback (int num_attempts)
{
    for (int i = 0; (i < num_attempts) && (exit_code == (int)GanglionLibNative::SYNC_ERROR); i++)
    {
        if (read_message (UART_TIMEOUT) > 0)
        {
            break;
        }
    }
    return exit_code;
}

int reset_ble_dev ()
{
    // Reset dongle to get it into known state
    ble_cmd_system_reset (0);
    uart_close ();
    int i;
    for (i = 0; i < 5; i++)
    {
        usleep (500000); // 0.5s
        if (!uart_open (uart_port))
        {
            break;
        }
    }
    if (i == 5)
    {
        return (int)GanglionLibNative::PORT_OPEN_ERROR;
    }
    return (int)GanglionLibNative::STATUS_OK;
}