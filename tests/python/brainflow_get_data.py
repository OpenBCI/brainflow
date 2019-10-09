import argparse
import time
import brainflow
import brainflow.board_shim


def main ():
    parser = argparse.ArgumentParser ()
    parser.add_argument ('--port', type = str, help  = 'port name, for synthetic board port_name doesnt matter, just pass smth', required = True)
    parser.add_argument ('--board', type = int, help  = 'board id, check docs to get a list of supported board ids', required = True)
    parser.add_argument ('--log', action = 'store_true')
    args = parser.parse_args ()

    if (args.log):
        brainflow.board_shim.BoardShim.enable_dev_board_logger ()
    else:
        brainflow.board_shim.BoardShim.disable_board_logger ()

    board = brainflow.board_shim.BoardShim (args.board, args.port)
    board.prepare_session ()
    
    # disable second channel, note emulator doesnt handle such commands, run with real board to validate
    # different board have different data formats
    if args.board == brainflow.board_shim.BoardIds.CYTON.value:
        board.config_board ('x2100000X')
    elif args.board == brainflow.board_shim.BoardIds.GANGLION.value:
        board.config_board ('2')

    board.start_stream ()
    time.sleep (10)
    data = board.get_board_data ()
    board.stop_stream ()
    board.release_session ()

    print (data)

if __name__ == "__main__":
    main ()
