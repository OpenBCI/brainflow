#include "cyton_wifi.h"
#include "custom_cast.h"
#include "timestamp.h"


#define START_BYTE 0xA0
#define END_BYTE_STANDARD 0xC0
#define END_BYTE_ANALOG 0xC1
#define END_BYTE_MAX 0xC6


void CytonWifi::read_thread ()
{
    /*
        Byte 1: 0xA0
        Byte 2: Sample Number
        Bytes 3-5: Data value for EEG channel 1
        Bytes 6-8: Data value for EEG channel 2
        Bytes 9-11: Data value for EEG channel 3
        Bytes 12-14: Data value for EEG channel 4
        Bytes 15-17: Data value for EEG channel 5
        Bytes 18-20: Data value for EEG channel 6
        Bytes 21-23: Data value for EEG channel 6
        Bytes 24-26: Data value for EEG channel 8
        Aux Data Bytes 27-32: 6 bytes of data
        Byte 33: 0xCX where X is 0-F in hex
    */
    int res;
    unsigned char b[OpenBCIWifiShieldBoard::transaction_size];
    double accel[3] = {0.};
    while (keep_alive)
    {
        // check start byte
        res = server_socket->recv (b, OpenBCIWifiShieldBoard::transaction_size);
        if (res != OpenBCIWifiShieldBoard::transaction_size)
        {
            continue;
        }
        for (int cur_package = 0;
             cur_package < OpenBCIWifiShieldBoard::num_packages_per_transaction; cur_package++)
        {
            int offset = cur_package * OpenBCIWifiShieldBoard::package_size;
            if (b[0 + offset] != START_BYTE)
            {
                continue;
            }
            unsigned char *bytes = b + 1; // for better consistency between plain cyton and wifi, in
                                          // plain cyton index is shifted by 1

            if ((bytes[31 + offset] < END_BYTE_STANDARD) || (bytes[31 + offset] > END_BYTE_MAX))
            {
                safe_logger (spdlog::level::warn, "Wrong end byte {}", bytes[31 + offset]);
                continue;
            }

            double package[22] = {0.};
            // package num
            package[0] = (double)bytes[0 + offset];
            // eeg
            for (int i = 0; i < 8; i++)
            {
                package[i + 1] = eeg_scale * cast_24bit_to_int32 (bytes + 1 + 3 * i + offset);
            }
            package[12] = (double)bytes[31 + offset]; // end byte
            // place unprocessed bytes for all modes to other_channels
            package[13] = (double)bytes[25 + offset];
            package[14] = (double)bytes[26 + offset];
            package[15] = (double)bytes[27 + offset];
            package[16] = (double)bytes[28 + offset];
            package[17] = (double)bytes[29 + offset];
            package[18] = (double)bytes[30 + offset];
            // place processed bytes for accel
            if (bytes[31 + offset] == END_BYTE_STANDARD)
            {
                double accelTemp[3] = {0.};
                accelTemp[0] = accel_scale * cast_16bit_to_int32 (bytes + 25 + offset);
                accelTemp[1] = accel_scale * cast_16bit_to_int32 (bytes + 27 + offset);
                accelTemp[2] = accel_scale * cast_16bit_to_int32 (bytes + 29 + offset);

                for (int i = 0; i < 3; i++)
                {
                    if (accelTemp[i] != 0)
                    {
                        accel[i] = accelTemp[i];
                    }
                }

                package[9] = accel[0];
                package[10] = accel[1];
                package[11] = accel[2];
            }
            // place processed bytes for analog
            if (bytes[31 + offset] == END_BYTE_ANALOG)
            {
                package[19] = cast_16bit_to_int32 (bytes + 25 + offset);
                package[20] = cast_16bit_to_int32 (bytes + 27 + offset);
                package[21] = cast_16bit_to_int32 (bytes + 29 + offset);
            }

            double timestamp = get_timestamp ();
            db->add_data (timestamp, package);
            streamer->stream_data (package, 22, timestamp);
        }
    }
}
