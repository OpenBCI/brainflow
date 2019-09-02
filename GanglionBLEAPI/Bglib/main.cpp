#include <chrono>
#include <condition_variable>
#include <ctype.h>
#include <mutex>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#include "cmd_def.h"
#include "helpers.h"
#include "uart.h"

#include <iostream>

#include "GanglionNativeInterface.h"

// read Bluetooth_Smart_Software_v1.3.1_API_Reference.pdf to understand at least smth here

volatile int exit_code = (int)GanglionLibNative::SYNC_ERROR;
char uart_port[1024];
volatile bd_addr connect_addr;
volatile uint8 connection = 0;
volatile uint16 ganglion_handle_start = 0; // I have no idea what it is but seems like its important
volatile uint16 ganglion_handle_end = 0;
volatile uint16 ganglion_handle_recv = 0;
volatile uint16 ganglion_handle_send = 0;
volatile State state =
    State::none; // same callbacks are triggered by different methods we need to differ them

std::thread
    read_message_thread; // read_message should be executed in a loop but connection has
                         // small timeout and need to be updated via callback for disconnect
                         // so we need to keep connection up to date I run read_message
                         // during all execution, not only when we call start\stop and other merhods
volatile bool keep_alive = false;
std::mutex m;
std::condition_variable cv;

bool initialized = false;


void read_message_worker ()
{
    while (keep_alive)
    {
        read_message (UART_TIMEOUT);
    }
}

namespace GanglionLibNative
{
    int initialize_native (void *param)
    {
        // there is no release method, ensure that we init all this staff once
        if (!initialized)
        {
            std::string dongle_port = get_dongle_port ();
            if (dongle_port.empty ())
            {
                return (int)CustomExitCodesNative::GANGLION_DONGLE_PORT_IS_NOT_SET_ERROR;
            }
            strcpy (uart_port, dongle_port.c_str ());
            bglib_output = output;
            exit_code = (int)CustomExitCodesNative::SYNC_ERROR;
            if (uart_open (uart_port))
            {
                return (int)CustomExitCodesNative::GANGLION_NOT_FOUND_ERROR;
            }
            // Reset dongle to get it into known state
            ble_cmd_system_reset (0);
            uart_close ();
            do
            {
                usleep (500000); // 0.5s
            } while (uart_open (uart_port));
            initialized = true;
        }
        return (int)CustomExitCodesNative::STATUS_OK;
    }

    int open_ganglion_native (void *param)
    {
        if (keep_alive)
        {
            return (int)CustomExitCodesNative::GANGLION_ALREADY_OPEN_ERROR;
        }
        exit_code = (int)CustomExitCodesNative::SYNC_ERROR;
        keep_alive = true;
        read_message_thread = std::thread (read_message_worker);

        state = State::open_called;
        ble_cmd_gap_discover (gap_discover_observation);

        int res = wait_for_callback (5);
        if (res != (int)CustomExitCodesNative::STATUS_OK)
        {
            return res;
        }
        ble_cmd_gap_end_procedure ();
        return open_ble_dev ();
    }

    int open_ganglion_mac_addr_native (void *param)
    {
        if (keep_alive)
        {
            return (int)CustomExitCodesNative::GANGLION_ALREADY_OPEN_ERROR;
        }
        exit_code = (int)CustomExitCodesNative::SYNC_ERROR;
        keep_alive = true;
        read_message_thread = std::thread (read_message_worker);

        state = State::open_called;
        char *mac_addr = (char *)param;
        // convert string mac addr to bd_addr struct
        for (int i = 0; i < strlen (mac_addr); i++)
        {
            mac_addr[i] = tolower (mac_addr[i]);
        }
        short unsigned int addr[6];
        if (sscanf (mac_addr, "%02hx:%02hx:%02hx:%02hx:%02hx:%02hx", &addr[5], &addr[4], &addr[3],
                &addr[2], &addr[1], &addr[0]) == 6)
        {
            for (int i = 0; i < 6; i++)
            {
                connect_addr.addr[i] = addr[i];
            }
        }
        else
        {
            return (int)CustomExitCodesNative::INVALID_MAC_ADDR_ERROR;
        }
        return open_ble_dev ();
    }

    int stop_stream_native (void *param)
    {
        return config_board_native ((void *)"s");
    }

    int start_stream_native (void *param)
    {
        return config_board_native ((void *)"b");
    }

    int close_ganglion_native (void *param)
    {
        if (!keep_alive)
        {
            return (int)CustomExitCodesNative::GANGLION_IS_NOT_OPEN_ERROR;
        }
        state = State::close_called;

        stop_stream_native (NULL);

        keep_alive = false;
        if (read_message_thread.joinable ())
        {
            read_message_thread.join ();
        }

        uart_close ();

        connection = 0;
        ganglion_handle_start = 0;
        ganglion_handle_end = 0;
        ganglion_handle_recv = 0;
        ganglion_handle_send = 0;
        state = State::none;
        return (int)CustomExitCodesNative::STATUS_OK;
    }

    int get_data_native (void *param)
    {
        if (!keep_alive)
        {
            return (int)CustomExitCodesNative::GANGLION_IS_NOT_OPEN_ERROR;
        }
        state = State::get_data_called;
        return (int)CustomExitCodesNative::NOT_IMPLEMENTED_ERROR;
    }

    int config_board_native (void *param)
    {
        if (!keep_alive)
        {
            return (int)CustomExitCodesNative::GANGLION_IS_NOT_OPEN_ERROR;
        }
        exit_code = (int)CustomExitCodesNative::SYNC_ERROR;
        char *config = (char *)param;
        int len = strlen (config);
        state = State::config_called;
        if (!ganglion_handle_send)
        {
            return (int)CustomExitCodesNative::SEND_CHARACTERISTIC_NOT_FOUND_ERROR;
        }
        ble_cmd_attclient_attribute_write (connection, ganglion_handle_send, len, (uint8 *)&config);
        int res = wait_for_callback (5);
        return res;
    }

} // GanglionLibNative
