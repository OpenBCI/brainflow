board_shim = BoardShim (BoardIDs.SYNTHETIC_BOARD, '');
board_shim.prepare_session ();
board_shim.start_stream (3600);
pause (5)
board_shim.stop_stream ()
data = board_shim.get_board_data ();
disp (data);

board_shim.release_session ()
