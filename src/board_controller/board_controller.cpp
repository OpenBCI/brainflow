#ifdef _WIN32
#include <windows.h>
#endif

#include <map>
#include <memory>
#include <mutex>
#include <utility>

#include "board.h"
#include "board_controller.h"
#include "cyton.h"
#include "cyton_daisy.h"
#include "ganglion.h"
#include "synthetic_board.h"

std::map<std::pair<int, std::string>, std::shared_ptr<Board>> boards;
std::mutex mutex;

int prepare_session (int board_id, char *port_name)
{

#ifndef _WIN32
    // temp stub for ganglion on linux and macos
    if (board_id == GANGLION_BOARD)
        return UNSUPPORTED_BOARD_ERROR;
#endif

    std::lock_guard<std::mutex> lock (mutex);

    std::string str_port;
    if (port_name == NULL)
    {
        str_port = "";
    }
    else
    {
        str_port = port_name;
    }
    std::pair<int, std::string> key = std::make_pair (board_id, str_port);
    if (boards.find (key) != boards.end ())
    {
        Board::board_logger->error (
            "Board with id {} and port {} already created", board_id, port_name);
        return PORT_ALREADY_OPEN_ERROR;
    }

    int res = STATUS_OK;
    std::shared_ptr<Board> board = NULL;
    switch (board_id)
    {
        case CYTON_BOARD:
            board = std::shared_ptr<Board> (new Cyton (port_name));
            break;
        case GANGLION_BOARD:
            board = std::shared_ptr<Board> (new Ganglion (port_name));
            break;
        case SYNTHETIC_BOARD:
            board = std::shared_ptr<Board> (new SyntheticBoard (port_name));
            break;
        case CYTON_DAISY_BOARD:
            board = std::shared_ptr<Board> (new CytonDaisy (port_name));
            break;
        default:
            return UNSUPPORTED_BOARD_ERROR;
    }
    res = board->prepare_session ();
    if (res != STATUS_OK)
    {
        board = NULL;
    }
    boards[key] = board;
    return res;
}

int start_stream (int buffer_size, int board_id, char *port_name)
{
    std::lock_guard<std::mutex> lock (mutex);

    std::string str_port;
    if (port_name == NULL)
    {
        str_port = "";
    }
    else
    {
        str_port = port_name;
    }
    std::pair<int, std::string> key = std::make_pair (board_id, str_port);
    auto board_it = boards.find (key);
    if (board_it == boards.end ())
    {
        Board::board_logger->error (
            "Board with id {} and port {} is not created", board_id, port_name);
        return BOARD_NOT_CREATED_ERROR;
    }

    return board_it->second->start_stream (buffer_size);
}

int stop_stream (int board_id, char *port_name)
{
    std::lock_guard<std::mutex> lock (mutex);

    std::string str_port;
    if (port_name == NULL)
    {
        str_port = "";
    }
    else
    {
        str_port = port_name;
    }
    std::pair<int, std::string> key = std::make_pair (board_id, str_port);
    auto board_it = boards.find (key);
    if (board_it == boards.end ())
    {
        Board::board_logger->error (
            "Board with id {} and port {} is not created", board_id, port_name);
        return BOARD_NOT_CREATED_ERROR;
    }

    return board_it->second->stop_stream ();
}

int release_session (int board_id, char *port_name)
{
    std::lock_guard<std::mutex> lock (mutex);

    std::string str_port;
    if (port_name == NULL)
    {
        str_port = "";
    }
    else
    {
        str_port = port_name;
    }
    std::pair<int, std::string> key = std::make_pair (board_id, str_port);
    auto board_it = boards.find (key);
    if (board_it == boards.end ())
    {
        Board::board_logger->error (
            "Board with id {} and port {} is not created", board_id, port_name);
        return BOARD_NOT_CREATED_ERROR;
    }

    int res = board_it->second->release_session ();
    boards.erase (board_it);

    return res;
}

int get_current_board_data (int num_samples, float *data_buf, double *ts_buf, int *returned_samples,
    int board_id, char *port_name)
{
    std::lock_guard<std::mutex> lock (mutex);

    std::string str_port;
    if (port_name == NULL)
    {
        str_port = "";
    }
    else
    {
        str_port = port_name;
    }
    std::pair<int, std::string> key = std::make_pair (board_id, str_port);
    auto board_it = boards.find (key);
    if (board_it == boards.end ())
    {
        Board::board_logger->error (
            "Board with id {} and port {} is not created", board_id, port_name);
        return BOARD_NOT_CREATED_ERROR;
    }

    return board_it->second->get_current_board_data (
        num_samples, data_buf, ts_buf, returned_samples);
}

int get_board_data_count (int *result, int board_id, char *port_name)
{
    std::lock_guard<std::mutex> lock (mutex);

    std::string str_port;
    if (port_name == NULL)
    {
        str_port = "";
    }
    else
    {
        str_port = port_name;
    }
    std::pair<int, std::string> key = std::make_pair (board_id, str_port);
    auto board_it = boards.find (key);
    if (board_it == boards.end ())
    {
        Board::board_logger->error (
            "Board with id {} and port {} is not created", board_id, port_name);
        return BOARD_NOT_CREATED_ERROR;
    }

    return board_it->second->get_board_data_count (result);
}

int get_board_data (int data_count, float *data_buf, double *ts_buf, int board_id, char *port_name)
{
    std::lock_guard<std::mutex> lock (mutex);

    std::string str_port;
    if (port_name == NULL)
    {
        str_port = "";
    }
    else
    {
        str_port = port_name;
    }
    std::pair<int, std::string> key = std::make_pair (board_id, str_port);
    auto board_it = boards.find (key);
    if (board_it == boards.end ())
    {
        Board::board_logger->error (
            "Board with id {} and port {} is not created", board_id, port_name);
        return BOARD_NOT_CREATED_ERROR;
    }

    return board_it->second->get_board_data (data_count, data_buf, ts_buf);
}

int set_log_level (int log_level)
{
    std::lock_guard<std::mutex> lock (mutex);
    return Board::set_log_level (log_level);
}

int config_board (char *config, int board_id, char *port_name)
{
    std::lock_guard<std::mutex> lock (mutex);

    std::string str_port;
    if (port_name == NULL)
    {
        str_port = "";
    }
    else
    {
        str_port = port_name;
    }
    std::pair<int, std::string> key = std::make_pair (board_id, str_port);
    auto board_it = boards.find (key);
    if (board_it == boards.end ())
    {
        Board::board_logger->error (
            "Board with id {} and port {} is not created", board_id, port_name);
        return BOARD_NOT_CREATED_ERROR;
    }

    return board_it->second->config_board (config);
}
