#include <iostream>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "board_shim.h"
#include "data_filter.h"

using namespace std;

void print_one_row (double *data, int num_data_points);

int main (int argc, char *argv[])
{
    struct BrainFlowInputParams params;
    // use synthetic board for demo
    int board_id = SYNTHETIC_BOARD;

    BoardShim::enable_dev_board_logger ();

    BoardShim *board = new BoardShim (board_id, params);
    double **data = NULL;
    int *eeg_channels = NULL;
    int num_rows = 0;
    int res = 0;

    try
    {
        board->prepare_session ();
        board->start_stream ();
        BoardShim::log_message ((int)LogLevels::LEVEL_INFO, "Start sleeping in the main thread");
#ifdef _WIN32
        Sleep (5000);
#else
        sleep (5);
#endif

        board->stop_stream ();
        int data_count = 0;
        data = board->get_current_board_data (64, &data_count);
        BoardShim::log_message ((int)LogLevels::LEVEL_INFO, "read %d packages", data_count);
        board->release_session ();
        num_rows = BoardShim::get_num_rows (board_id);

        int eeg_num_channels = 0;
        eeg_channels = BoardShim::get_eeg_channels (board_id, &eeg_num_channels);
        int filtered_size = 0;
        double *downsampled_data = NULL;
        for (int i = 0; i < eeg_num_channels; i++)
        {
            // demo for wavelet transform
            int wavelet_size = 0;
            int *decomposition_lengths = NULL;
            // format for wavelet_data is [A(J) D(J) D(J-1) ..... D(1)] where J is a decomposition
            // level, A - app coeffs, D - detailed coeffs, decomposition_lengths stores length for
            // each block, len(decomposition_length) = decomposition_level + 1
            double *wavelet_data = DataFilter::perform_wavelet_transform (
                data[eeg_channels[i]], data_count, "db4", 3, &wavelet_size, &decomposition_lengths);

            // you can do smth with wavelet coeffs here, for example denoising works via thresholds
            // for wavelet coefficients
            std::cout << "approximation coefficients:" << std::endl;
            for (int i = 0; i < decomposition_lengths[0]; i++)
            {
                std::cout << wavelet_data[i] << " ";
            }
            std::cout << std::endl;
            std::cout << "first block of detailed coefficients:" << std::endl;
            for (int i = decomposition_lengths[0];
                 i < decomposition_lengths[0] + decomposition_lengths[1]; i++)
            {
                std::cout << wavelet_data[i] << " ";
            }
            std::cout << std::endl;

            double *restored_data = DataFilter::perform_inverse_wavelet_transform (
                wavelet_data, wavelet_size, data_count, "db4", 3, decomposition_lengths);

            std::cout << "Original data:" << std::endl;
            print_one_row (data[eeg_channels[i]], data_count);
            std::cout << "Restored after inverse transform data:" << std::endl;
            print_one_row (restored_data, data_count);

            delete[] wavelet_data;
            delete[] restored_data;
            delete[] decomposition_lengths;
        }
    }
    catch (const BrainFlowException &err)
    {
        BoardShim::log_message ((int)LogLevels::LEVEL_ERROR, err.what ());
        res = err.exit_code;
    }

    if (data != NULL)
    {
        for (int i = 0; i < num_rows; i++)
        {
            delete[] data[i];
        }
    }
    delete[] data;
    delete[] eeg_channels;
    delete board;

    return res;
}

void print_one_row (double *data, int num_data_points)
{
    for (int i = 0; i < num_data_points; i++)
    {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}