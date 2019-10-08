#include <iostream>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "board_shim.h"
#include "data_handler.h"

using namespace std;


void print_head (double **data_buf, int num_channels, int num_data_points)
{
    std::cout << "Total Channels for this board: " << num_channels << std::endl;
    int num_points = (num_data_points < 5) ? num_data_points : 5;
    for (int i = 0; i < num_channels; i++)
    {
        std::cout << "Channel " << i << ": ";
        for (int j = 0; j < num_points; j++)
        {
            std::cout << data_buf[i][j] << ",";
        }
        std::cout << std::endl;
    }
}

int main (int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "board id and port name are required" << endl;
        return -1;
    }

    BoardShim::enable_dev_board_logger ();

    int board_id = atoi (argv[1]);
    BoardShim *board = new BoardShim (board_id, argv[2]);
    DataHandler *dh = new DataHandler (board_id);
    double **data = NULL;
    int *eeg_channels = NULL;
    int num_rows = 0;
    int res = 0;

    try
    {
        board->prepare_session ();
        board->start_stream ();

#ifdef _WIN32
        Sleep (5000);
#else
        sleep (5);
#endif
        board->stop_stream ();
        int data_count = 0;
        data = board->get_board_data (&data_count);
        std::cout << "received " << data_count << " packages" << std::endl;
        board->release_session ();
        num_rows = BoardShim::get_num_rows (board_id);
        print_head (data, num_rows, data_count);

        int eeg_num_channels = 0;
        eeg_channels = BoardShim::get_eeg_channels (board_id, &eeg_num_channels);
        dh->preprocess_data (data, eeg_channels, eeg_num_channels, data_count);
        print_head (data, num_rows, data_count);
    }
    catch (const BrainFlowException &err)
    {
        std::cout << err.what () << std::endl;
        res = err.get_exit_code ();
    }

    if (data != NULL)
    {
        for (int i = 0; i < num_rows; i++)
        {
            delete[] data[i];
        }
        delete[] data;
    }
    delete[] eeg_channels;
    delete dh;
    delete board;

    return res;
}