import enum


class StreamExitCodes (enum.Enum):
    """Enum to store all possible exit codes"""
    STATUS_OK = 0
    PORT_ALREADY_OPEN_ERROR = 1
    UNABLE_TO_OPEN_PORT_ERROR = 2
    SER_PORT_ERROR = 3
    BOARD_WRITE_ERROR = 4
    INCOMMING_MSG_ERROR = 5
    INITIAL_MSG_ERROR = 6
    BOARD_NOT_READY_ERROR = 7
    STREAM_ALREADY_RUN_ERROR = 8
    INVALID_BUFFER_SIZE_ERROR = 9
    STREAM_THREAD_ERROR = 10
    STREAM_THREAD_IS_NOT_RUNNING = 11
    EMPTY_BUFFER_ERROR = 12
    INVALID_ARGUMENTS_ERROR = 13
    UNSUPPORTED_BOARD_ERROR = 14
    BOARD_NOT_CREATED_ERROR = 15
    ANOTHER_BOARD_IS_CREATED_ERROR = 16
    GENERAL_ERROR = 17
    SYNC_TIMEOUT_ERROR = 18
    GANGLION_DONGLE_PORT_IS_NOT_SET_ERROR = 19
