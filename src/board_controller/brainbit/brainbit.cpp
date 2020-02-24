#include <stdint.h>
#include <string.h>

#include "brainbit.h"

////////////////////////////////
// Implementation for Windows //
////////////////////////////////

#ifdef _WIN32

#include "cparams.h"
#include "cscanner.h"
#include "sdk_error.h"
#include "stdio.h"
#include "windows.h"


BrainBit::BrainBit (struct BrainFlowInputParams params) : Board ((int)BRAINBIT_BOARD, params)
{
    this->is_streaming = false;
    this->keep_alive = false;
    this->initialized = false;
    device = NULL;
}

BrainBit::~BrainBit ()
{
    skip_logs = true;
    release_session ();
}

int BrainBit::prepare_session ()
{
    if (initialized)
    {
        safe_logger (spdlog::level::info, "Session is already prepared");
        return STATUS_OK;
    }
    if (params.other_info.empty ())
    {
        safe_logger (
            spdlog::level::err, "You need to provide BrainBit serial number to other_info field!");
        return INVALID_ARGUMENTS_ERROR;
    }
    long long serial_number = 0;
    try
    {
        serial_number = std::stoll (params.other_info);
    }
    catch (...)
    {
        safe_logger (
            spdlog::level::err, "You need to provide BrainBit serial number to other_info field!");
        return INVALID_ARGUMENTS_ERROR;
    }

    int res = find_device (serial_number);
    if (res != STATUS_OK)
    {
        return res;
    }

    res = connect_device ();
    if (res == STATUS_OK)
    {
        initialized = true;
    }
    else
    {
        device_delete (device);
        device = NULL;
    }

    return res;
}

int BrainBit::find_device (long long serial_number)
{
    DeviceEnumerator *enumerator = create_device_enumerator (DeviceTypeBrainbit);
    if (enumerator == NULL)
    {
        char error_msg[1024];
        sdk_last_error_msg (error_msg, 1024);
        safe_logger (spdlog::level::err, "create enumerator error {}", error_msg);
        return BOARD_NOT_READY_ERROR;
    }

    int attempts = 20;
    int found = false;
    int res = STATUS_OK;
    DeviceInfo device_info;
    do
    {
        if (find_device_info (enumerator, serial_number, &device_info) != 0)
        {
            Sleep (300);
            continue;
        }

        safe_logger (spdlog::level::info, "Device with SN {} found", serial_number);
        found = true;
        break;
    } while (attempts-- > 0);

    if (found)
    {
        device = create_Device (enumerator, device_info);
        if (device == NULL)
        {
            char error_msg[1024];
            sdk_last_error_msg (error_msg, 1024);
            safe_logger (spdlog::level::err, "create Device error {}", error_msg);
            res = BOARD_NOT_READY_ERROR;
        }
    }
    else
    {
        safe_logger (spdlog::level::err, "Device with SN {} not found", serial_number);
        res = BOARD_NOT_READY_ERROR;
    }

    enumerator_delete (enumerator);

    return res;
}

int BrainBit::find_device_info (
    DeviceEnumerator *enumerator, uint64_t serial_number, DeviceInfo *out_device_info)
{
    DeviceInfoArray device_info_array;
    const int result_code = enumerator_get_device_list (enumerator, &device_info_array);
    if (result_code != SDK_NO_ERROR)
    {
        return -1;
    }

    for (size_t i = 0; i < device_info_array.info_count; ++i)
    {
        if (device_info_array.info_array[i].SerialNumber == serial_number)
        {
            *out_device_info = device_info_array.info_array[i];
            free_DeviceInfoArray (device_info_array);
            return 0;
        }
    }

    free_DeviceInfoArray (device_info_array);
    return -1;
}

int BrainBit::connect_device ()
{
    if (device == NULL)
    {
        safe_logger (spdlog::level::err, "Device is not created.");
        return BOARD_NOT_CREATED_ERROR;
    }

    device_connect (device);

    DeviceState device_state;
    int return_code = device_read_State (device, &device_state);
    if (return_code != SDK_NO_ERROR)
    {
        char error_msg[1024];
        sdk_last_error_msg (error_msg, 1024);
        safe_logger (spdlog::level::err, "device read state error {}", error_msg);
        return BOARD_NOT_READY_ERROR;
    }

    if (device_state != DeviceStateConnected)
    {
        safe_logger (spdlog::level::err, "Device is not connected.");
        return BOARD_NOT_READY_ERROR;
    }

    return STATUS_OK;
}

int BrainBit::config_board (char *config)
{
    if (device == NULL)
    {
        return BOARD_NOT_CREATED_ERROR;
    }
    if (config == NULL)
    {
        return INVALID_ARGUMENTS_ERROR;
    }

    Command cmd = CommandStartSignal;
    bool parsed = false;
    // get Command enum value from value name as char *
    // this enum has more values but looks like they are not supported by brainbit board and used by
    // other devices
    if (strcmp (config, "CommandStartSignal") == 0)
    {
        parsed = true;
        cmd = CommandStartSignal;
    }
    if (strcmp (config, "CommandStopSignal") == 0)
    {
        parsed = true;
        cmd = CommandStopSignal;
    }
    if (strcmp (config, "CommandStartResist") == 0)
    {
        parsed = true;
        cmd = CommandStartResist;
    }
    if (strcmp (config, "CommandStopResist") == 0)
    {
        parsed = true;
        cmd = CommandStopResist;
    }

    if (!parsed)
    {
        safe_logger (spdlog::level::err,
            "Invalid value for config, Supported values: CommandStartSignal, CommandStopSignal, "
            "CommandStartResist, CommandStopResist");
        return INVALID_ARGUMENTS_ERROR;
    }

    int ec = device_execute (device, cmd);
    if (ec != SDK_NO_ERROR)
    {
        char error_msg[1024];
        sdk_last_error_msg (error_msg, 1024);
        safe_logger (spdlog::level::err, error_msg);
        return GENERAL_ERROR;
    }

    return STATUS_OK;
}

int BrainBit::start_stream (int buffer_size, char *streamer_params)
{
    if (is_streaming)
    {
        safe_logger (spdlog::level::err, "Streaming thread already running");
        return STREAM_ALREADY_RUN_ERROR;
    }
    if (buffer_size <= 0 || buffer_size > MAX_CAPTURE_SAMPLES)
    {
        safe_logger (spdlog::level::err, "invalid array size");
        return INVALID_BUFFER_SIZE_ERROR;
    }

    if (db)
    {
        delete db;
        db = NULL;
    }
    if (streamer)
    {
        delete streamer;
        streamer = NULL;
    }

    int res = prepare_streamer (streamer_params);
    if (res != STATUS_OK)
    {
        return res;
    }
    db = new DataBuffer (8, buffer_size); // todo determine number of channels 8 is a guess
    if (!db->is_ready ())
    {
        safe_logger (spdlog::level::err, "unable to prepare buffer");
        delete db;
        db = NULL;
        return INVALID_BUFFER_SIZE_ERROR;
    }

    res = config_board ((char *)"CommandStartSignal");
    if (res != STATUS_OK)
    {
        return res;
    }

    keep_alive = true;
    streaming_thread = std::thread ([this] { this->read_thread (); });
    is_streaming = true;
    return STATUS_OK;
}

int BrainBit::stop_stream ()
{
    if (is_streaming)
    {
        keep_alive = false;
        is_streaming = false;
        streaming_thread.join ();
        if (streamer)
        {
            delete streamer;
            streamer = NULL;
        }
        int res = config_board ((char *)"CommandStopSignal");
        return res;
    }
    else
    {
        return STREAM_THREAD_IS_NOT_RUNNING;
    }
}

int BrainBit::release_session ()
{
    if (initialized)
    {
        if (is_streaming)
        {
            stop_stream ();
        }
        initialized = false;
        if (device)
        {
            device_disconnect (device);
            device_delete (device);
            device = NULL;
        }
    }
    return STATUS_OK;
}

void BrainBit::read_thread ()
{
    // todo, waiting for samples from https://sdk.brainbit.com/windows-data-receiving-c/
    while (keep_alive)
    {
        Sleep (1);
    }
}

///////////////////
// Stub for Unix //
///////////////////

#else

BrainBit::BrainBit (struct BrainFlowInputParams params) : Board ((int)BRAINBIT_BOARD, params)
{
}

BrainBit::~BrainBit ()
{
}

int BrainBit::prepare_session ()
{
    safe_logger (spdlog::level::err, "BrainBit supports only Windows.");
    return UNSUPPORTED_BOARD_ERROR;
}

int BrainBit::config_board (char *config)
{
    safe_logger (spdlog::level::err, "BrainBit supports only Windows.");
    return UNSUPPORTED_BOARD_ERROR;
}

int BrainBit::release_session ()
{
    safe_logger (spdlog::level::err, "BrainBit supports only Windows.");
    return UNSUPPORTED_BOARD_ERROR;
}

int BrainBit::stop_stream ()
{
    safe_logger (spdlog::level::err, "BrainBit supports only Windows.");
    return UNSUPPORTED_BOARD_ERROR;
}

int BrainBit::start_stream (int buffer_size, char *streamer_params)
{
    safe_logger (spdlog::level::err, "BrainBit supports only Windows.");
    return UNSUPPORTED_BOARD_ERROR;
}

#endif
