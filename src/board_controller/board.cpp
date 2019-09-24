#include "board.h"
#include "board_controller.h"

#define LOGGER_NAME "board_logger"

std::shared_ptr<spdlog::logger> Board::board_logger = spdlog::stderr_logger_mt (LOGGER_NAME);

int Board::set_log_level (int level)
{
    int log_level = level;
    if (level > 6)
        log_level = 6;
    if (level < 0)
        log_level = 0;
    Board::board_logger->set_level (spdlog::level::level_enum (log_level));
    Board::board_logger->flush_on (spdlog::level::level_enum (log_level));
    return STATUS_OK;
}

int Board::set_log_file (char *log_file)
{
    spdlog::level::level_enum level = Board::board_logger->level ();
    spdlog::drop (LOGGER_NAME);
    Board::board_logger = spdlog::basic_logger_mt (LOGGER_NAME, log_file);
    Board::board_logger->set_level (level);
    Board::board_logger->flush_on (level);
    return STATUS_OK;
}

int Board::get_current_board_data (
    int num_samples, float *data_buf, double *ts_buf, int *returned_samples)
{
    if (db && data_buf && ts_buf && returned_samples)
    {
        size_t result = db->get_current_data (num_samples, ts_buf, data_buf);
        (*returned_samples) = int(result);
        return STATUS_OK;
    }
    else
        return INVALID_ARGUMENTS_ERROR;
}

int Board::get_board_data_count (int *result)
{
    if (!db)
        return EMPTY_BUFFER_ERROR;
    if (!result)
        return INVALID_ARGUMENTS_ERROR;

    *result = int(db->get_data_count ());
    return STATUS_OK;
}

int Board::get_board_data (int data_count, float *data_buf, double *ts_buf)
{
    if (!db)
        return EMPTY_BUFFER_ERROR;
    if ((!data_buf) || !(ts_buf))
        return INVALID_ARGUMENTS_ERROR;

    db->get_data (data_count, ts_buf, data_buf);
    return STATUS_OK;
}