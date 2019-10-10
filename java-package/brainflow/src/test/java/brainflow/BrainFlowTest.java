package brainflow;

import java.util.Arrays;

public class BrainFlowTest {

    public static void main (String[] args) throws Exception {

        BoardShim.enable_board_logger ();
        int board_id = Integer.parseInt (args[0]);
        BoardShim board_shim = new BoardShim (board_id, args[1]);
        board_shim.prepare_session ();
        board_shim.start_stream (3600);
        Thread.sleep (1000);
        board_shim.stop_stream ();
        System.out.println (board_shim.get_board_data_count ());
        int num_rows = BoardShim.get_num_rows (board_id);
        double[][] data = board_shim.get_current_board_data (50);
        for (int i = 0; i < num_rows; i++) {
        	System.out.println (Arrays.toString (data[i]));
        }
        board_shim.release_session ();
    }
}
