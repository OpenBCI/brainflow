board_shim = BoardShim (-1, '');
board_shim.prepare_session ();
board_shim.start_stream (3600);
pause (5)
board_shim.stop_stream ()
[data, ts] = board_shim.get_current_board_data (250);
disp (data);
board_shim.release_session ()