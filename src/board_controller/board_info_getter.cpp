#include <string.h>

#include "board_controller.h"

#define ARRAY_SIZE(a) (sizeof (a) / sizeof (a[0]))

// here its the only way to avoid ~1 thousand lines of copypasting, your function argument mush has
// the same name as class field (even clang format doesnt like it)

// clang-format off

#define GENERATE_BODY_SINGLE_ARG(param_name) switch (board_id)                                     \
    {                                                                                              \
        case CYTON_BOARD:                                                                          \
            *param_name = brainflow::detail::CYTON::param_name;                                    \
            return STATUS_OK;                                                                      \
        case GANGLION_BOARD:                                                                       \
            *param_name = brainflow::detail::GANGLION::param_name;                                 \
            return STATUS_OK;                                                                      \
        case SYNTHETIC_BOARD:                                                                      \
            *param_name = brainflow::detail::SYNTHETIC::param_name;                                \
            return STATUS_OK;                                                                      \
        case CYTON_DAISY_BOARD:                                                                    \
            *param_name = brainflow::detail::CYTON_DAISY::param_name;                              \
            return STATUS_OK;                                                                      \
        case NOVAXR_BOARD:                                                                         \
            *param_name = brainflow::detail::NOVAXR::param_name;                                   \
            return STATUS_OK;                                                                      \
        case CYTON_WIFI_BOARD:                                                                     \
            *param_name = brainflow::detail::CYTON_WIFI::param_name;                               \
            return STATUS_OK;                                                                      \
        case CYTON_DAISY_WIFI_BOARD:                                                               \
            *param_name = brainflow::detail::CYTON_DAISY_WIFI::param_name;                         \
            return STATUS_OK;                                                                      \
        case GANGLION_WIFI_BOARD:                                                                  \
            *param_name = brainflow::detail::GANGLION_WIFI::param_name;                            \
            return STATUS_OK;                                                                      \
        default:                                                                                   \
            return UNSUPPORTED_BOARD_ERROR;                                                        \
    }

#define GENERATE_BODY_ARRAY_ARG(param_name) switch (board_id)                                       \
    {                                                                                               \
        case CYTON_BOARD:                                                                           \
            return brainflow::detail::prepare_board_info_data (                                     \
                brainflow::detail::CYTON::param_name,                                               \
                (brainflow::detail::CYTON::param_name[0] == -1) ?                                   \
                    0 :                                                                             \
                    ARRAY_SIZE (brainflow::detail::CYTON::param_name),                              \
                param_name, len);                                                                   \
        case GANGLION_BOARD:                                                                        \
            return brainflow::detail::prepare_board_info_data (                                     \
                brainflow::detail::GANGLION::param_name,                                            \
                (brainflow::detail::GANGLION::param_name[0] == -1) ?                                \
                    0 :                                                                             \
                    ARRAY_SIZE (brainflow::detail::GANGLION::param_name),                           \
                param_name, len);                                                                   \
        case SYNTHETIC_BOARD:                                                                       \
            return brainflow::detail::prepare_board_info_data (                                     \
                brainflow::detail::SYNTHETIC::param_name,                                           \
                (brainflow::detail::SYNTHETIC::param_name[0] == -1) ?                               \
                    0 :                                                                             \
                    ARRAY_SIZE (brainflow::detail::SYNTHETIC::param_name),                          \
                param_name, len);                                                                   \
        case CYTON_DAISY_BOARD:                                                                     \
            return brainflow::detail::prepare_board_info_data (                                     \
                brainflow::detail::CYTON_DAISY::param_name,                                         \
                (brainflow::detail::CYTON_DAISY::param_name[0] == -1) ?                             \
                    0 :                                                                             \
                    ARRAY_SIZE (brainflow::detail::CYTON_DAISY::param_name),                        \
                param_name, len);                                                                   \
        case NOVAXR_BOARD:                                                                          \
            return brainflow::detail::prepare_board_info_data (                                     \
                brainflow::detail::NOVAXR::param_name,                                              \
                (brainflow::detail::NOVAXR::param_name[0] == -1) ?                                  \
                    0 :                                                                             \
                    ARRAY_SIZE (brainflow::detail::NOVAXR::param_name),                             \
                param_name, len);                                                                   \
        case CYTON_WIFI_BOARD:                                                                      \
            return brainflow::detail::prepare_board_info_data (                                     \
                brainflow::detail::CYTON_WIFI::param_name,                                          \
                (brainflow::detail::CYTON_WIFI::param_name[0] == -1) ?                              \
                    0 :                                                                             \
                    ARRAY_SIZE (brainflow::detail::CYTON_WIFI::param_name),                         \
                param_name, len);                                                                   \
        case CYTON_DAISY_WIFI_BOARD:                                                                \
            return brainflow::detail::prepare_board_info_data (                                     \
                brainflow::detail::CYTON_DAISY_WIFI::param_name,                                    \
                (brainflow::detail::CYTON_DAISY_WIFI::param_name[0] == -1) ?                        \
                    0 :                                                                             \
                    ARRAY_SIZE (brainflow::detail::CYTON_DAISY_WIFI::param_name),                   \
                param_name, len);                                                                   \
        case GANGLION_WIFI_BOARD:                                                                   \
            return brainflow::detail::prepare_board_info_data (                                     \
                brainflow::detail::GANGLION_WIFI::param_name,                                       \
                (brainflow::detail::GANGLION_WIFI::param_name[0] == -1) ?                           \
                    0 :                                                                             \
                    ARRAY_SIZE (brainflow::detail::GANGLION_WIFI::param_name),                      \
                param_name, len);                                                                   \
        default:                                                                                    \
            return UNSUPPORTED_BOARD_ERROR;                                                         \
    }
// clang-format on

namespace brainflow
{
    // just to ensure that novody will use it directly from C++ binding and keep global namespace
    // cleaner
    namespace detail
    {
        // it looks bad by we need this information to be available staticly so we can not move it
        // to Board class, also its better than hardcoding values direcltly in get_* methods and
        // allows to use dirty hack with defines
        class CYTON
        {
        public:
            static const int sampling_rate = 250;
            static const int package_num_channel = 0;
            static const int num_rows = 13;
            static const int timestamp_channel = 12;
            static const int eeg_channels[8];
            static const int emg_channels[8];
            static const int eog_channels[8];
            static const int ecg_channels[8];
            static const int ppg_channels[];
            static const int eda_channels[];
            static const int accel_channels[];
            static const int gyro_channels[];
            static const int other_channels[];
        };

        // convention - for majority of boards we can not split emg\eeg\... return the same values
        // for all of them, use -1 to mark that there is no such channels
        const int CYTON::eeg_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int CYTON::emg_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int CYTON::ecg_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int CYTON::eog_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int CYTON::eda_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int CYTON::ppg_channels[] = {-1};
        const int CYTON::gyro_channels[] = {-1};
        const int CYTON::accel_channels[] = {9, 10, 11};
        const int CYTON::other_channels[] = {-1};


        class GANGLION
        {
        public:
            static const int sampling_rate = 200;
            static const int package_num_channel = 0;
            static const int num_rows = 9;
            static const int timestamp_channel = 8;
            static const int eeg_channels[];
            static const int emg_channels[];
            static const int eog_channels[];
            static const int ecg_channels[];
            static const int ppg_channels[];
            static const int eda_channels[];
            static const int accel_channels[];
            static const int gyro_channels[];
            static const int other_channels[];
        };

        const int GANGLION::eeg_channels[] = {1, 2, 3, 4};
        const int GANGLION::emg_channels[] = {1, 2, 3, 4};
        const int GANGLION::ecg_channels[] = {1, 2, 3, 4};
        const int GANGLION::eog_channels[] = {1, 2, 3, 4};
        const int GANGLION::eda_channels[] = {1, 2, 3, 4};
        const int GANGLION::ppg_channels[] = {-1};
        const int GANGLION::gyro_channels[] = {-1};
        const int GANGLION::accel_channels[] = {5, 6, 7};
        const int GANGLION::other_channels[] = {-1};


        class CYTON_DAISY
        {
        public:
            static const int sampling_rate = 125;
            static const int package_num_channel = 0;
            static const int num_rows = 21;
            static const int timestamp_channel = 20;
            static const int eeg_channels[];
            static const int emg_channels[];
            static const int eog_channels[];
            static const int ecg_channels[];
            static const int ppg_channels[];
            static const int eda_channels[];
            static const int accel_channels[];
            static const int gyro_channels[];
            static const int other_channels[];
        };

        const int CYTON_DAISY::eeg_channels[] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        const int CYTON_DAISY::emg_channels[] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        const int CYTON_DAISY::ecg_channels[] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        const int CYTON_DAISY::eog_channels[] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        const int CYTON_DAISY::eda_channels[] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        const int CYTON_DAISY::ppg_channels[] = {-1};
        const int CYTON_DAISY::gyro_channels[] = {-1};
        const int CYTON_DAISY::accel_channels[] = {17, 18, 19};
        const int CYTON_DAISY::other_channels[] = {-1};


        class SYNTHETIC
        {
        public:
            static const int sampling_rate = 256;
            static const int package_num_channel = 0;
            static const int num_rows = 13;
            static const int timestamp_channel = 12;
            static const int eeg_channels[];
            static const int emg_channels[];
            static const int eog_channels[];
            static const int ecg_channels[];
            static const int ppg_channels[];
            static const int eda_channels[];
            static const int accel_channels[];
            static const int gyro_channels[];
            static const int other_channels[];
        };

        const int SYNTHETIC::eeg_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int SYNTHETIC::emg_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int SYNTHETIC::ecg_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int SYNTHETIC::eda_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int SYNTHETIC::eog_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int SYNTHETIC::ppg_channels[] = {-1};
        const int SYNTHETIC::gyro_channels[] = {-1};
        const int SYNTHETIC::accel_channels[] = {9, 10, 11};
        const int SYNTHETIC::other_channels[] = {-1};


        class NOVAXR
        {
        public:
            static const int sampling_rate = 2000;
            static const int package_num_channel = 0;
            static const int num_rows = 26;
            static const int timestamp_channel = 25;
            static const int eeg_channels[];
            static const int emg_channels[];
            static const int eog_channels[];
            static const int ecg_channels[];
            static const int ppg_channels[];
            static const int eda_channels[];
            static const int accel_channels[];
            static const int gyro_channels[];
            static const int other_channels[];
        };

        const int NOVAXR::eeg_channels[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        const int NOVAXR::emg_channels[] = {11, 12, 13, 14, 15, 16};
        const int NOVAXR::ecg_channels[] = {-1};
        const int NOVAXR::eog_channels[] = {-1};
        const int NOVAXR::eda_channels[] = {18};
        const int NOVAXR::ppg_channels[] = {17};
        const int NOVAXR::accel_channels[] = {19, 20, 21};
        const int NOVAXR::gyro_channels[] = {22, 23, 24};
        const int NOVAXR::other_channels[] = {-1};


        class CYTON_WIFI
        {
        public:
            static const int sampling_rate = 1000;
            static const int package_num_channel = 0;
            static const int num_rows = 13;
            static const int timestamp_channel = 12;
            static const int eeg_channels[];
            static const int emg_channels[];
            static const int eog_channels[];
            static const int ecg_channels[];
            static const int ppg_channels[];
            static const int eda_channels[];
            static const int accel_channels[];
            static const int gyro_channels[];
            static const int other_channels[];
        };

        const int CYTON_WIFI::eeg_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int CYTON_WIFI::emg_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int CYTON_WIFI::ecg_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int CYTON_WIFI::eda_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int CYTON_WIFI::eog_channels[] = {1, 2, 3, 4, 5, 6, 7, 8};
        const int CYTON_WIFI::ppg_channels[] = {-1};
        const int CYTON_WIFI::accel_channels[] = {9, 10, 11};
        const int CYTON_WIFI::gyro_channels[] = {-1};
        const int CYTON_WIFI::other_channels[] = {-1};


        class CYTON_DAISY_WIFI
        {
        public:
            static const int sampling_rate = 1000;
            static const int package_num_channel = 0;
            static const int num_rows = 21;
            static const int timestamp_channel = 20;
            static const int eeg_channels[];
            static const int emg_channels[];
            static const int eog_channels[];
            static const int ecg_channels[];
            static const int ppg_channels[];
            static const int eda_channels[];
            static const int accel_channels[];
            static const int gyro_channels[];
            static const int other_channels[];
        };

        const int CYTON_DAISY_WIFI::eeg_channels[] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        const int CYTON_DAISY_WIFI::emg_channels[] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        const int CYTON_DAISY_WIFI::ecg_channels[] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        const int CYTON_DAISY_WIFI::eog_channels[] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        const int CYTON_DAISY_WIFI::eda_channels[] = {
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
        const int CYTON_DAISY_WIFI::ppg_channels[] = {-1};
        const int CYTON_DAISY_WIFI::accel_channels[] = {17, 18, 19};
        const int CYTON_DAISY_WIFI::gyro_channels[] = {-1};
        const int CYTON_DAISY_WIFI::other_channels[] = {-1};


        class GANGLION_WIFI
        {
        public:
            static const int sampling_rate = 1600;
            static const int package_num_channel = 0;
            static const int num_rows = 9;
            static const int timestamp_channel = 8;
            static const int eeg_channels[];
            static const int emg_channels[];
            static const int eog_channels[];
            static const int ecg_channels[];
            static const int ppg_channels[];
            static const int eda_channels[];
            static const int accel_channels[];
            static const int gyro_channels[];
            static const int other_channels[];
        };

        const int GANGLION_WIFI::eeg_channels[] = {1, 2, 3, 4};
        const int GANGLION_WIFI::emg_channels[] = {1, 2, 3, 4};
        const int GANGLION_WIFI::ecg_channels[] = {1, 2, 3, 4};
        const int GANGLION_WIFI::eda_channels[] = {1, 2, 3, 4};
        const int GANGLION_WIFI::eog_channels[] = {1, 2, 3, 4};
        const int GANGLION_WIFI::ppg_channels[] = {-1};
        const int GANGLION_WIFI::accel_channels[] = {5, 6, 7};
        const int GANGLION_WIFI::gyro_channels[] = {-1};
        const int GANGLION_WIFI::other_channels[] = {-1};


        // helper function to pass info to bindings
        int prepare_board_info_data (
            const int *initial_array, int intial_array_size, int *target_array, int *len)
        {
            if (initial_array[0] != -1)
            {
                memcpy (target_array, initial_array, sizeof (int) * intial_array_size);
            }
            *len = intial_array_size;
            return STATUS_OK;
        }

    } // namespace detail
} // namespace brainflow


int get_sampling_rate (int board_id, int *sampling_rate)
{
    GENERATE_BODY_SINGLE_ARG (sampling_rate);
}

int get_package_num_channel (int board_id, int *package_num_channel)
{
    GENERATE_BODY_SINGLE_ARG (package_num_channel);
}

int get_num_rows (int board_id, int *num_rows)
{
    GENERATE_BODY_SINGLE_ARG (num_rows);
}

int get_timestamp_channel (int board_id, int *timestamp_channel)
{
    GENERATE_BODY_SINGLE_ARG (timestamp_channel);
}

int get_eeg_channels (int board_id, int *eeg_channels, int *len)
{
    GENERATE_BODY_ARRAY_ARG (eeg_channels);
}

int get_emg_channels (int board_id, int *emg_channels, int *len)
{
    GENERATE_BODY_ARRAY_ARG (emg_channels);
}

int get_ecg_channels (int board_id, int *ecg_channels, int *len)
{
    GENERATE_BODY_ARRAY_ARG (ecg_channels);
}

int get_eog_channels (int board_id, int *eog_channels, int *len)
{
    GENERATE_BODY_ARRAY_ARG (eog_channels);
}

int get_eda_channels (int board_id, int *eda_channels, int *len)
{
    GENERATE_BODY_ARRAY_ARG (eda_channels);
}

int get_ppg_channels (int board_id, int *ppg_channels, int *len)
{
    GENERATE_BODY_ARRAY_ARG (ppg_channels);
}

int get_accel_channels (int board_id, int *accel_channels, int *len)
{
    GENERATE_BODY_ARRAY_ARG (accel_channels);
}

int get_gyro_channels (int board_id, int *gyro_channels, int *len)
{
    GENERATE_BODY_ARRAY_ARG (gyro_channels);
}

int get_other_channels (int board_id, int *other_channels, int *len)
{
    GENERATE_BODY_ARRAY_ARG (other_channels);
}

#undef ARRAY_SIZE
#undef GENERATE_BODY_ARRAY_ARG
#undef GENERATE_BODY_SINGLE_ARG
