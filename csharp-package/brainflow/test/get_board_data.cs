using System;
using brainflow;

using Accord.Math;

namespace test
{
    class GetBoardData
    {
        static void Main (string[] args)
        {
            BoardShim.enable_dev_board_logger ();
            BoardShim board_shim;
            if (args.Length == 2)
                board_shim = new BoardShim (Int32.Parse(args[0]), args[1]);
            else
                board_shim = new BoardShim (Int32.Parse (args[0]), null);

            board_shim.prepare_session ();
            board_shim.start_stream (3600);
            System.Threading.Thread.Sleep (5000);
            board_shim.stop_stream ();
            Console.WriteLine ("data count: {0}", board_shim.get_board_data_count ());
            double[,] unprocessed_data = board_shim.get_current_board_data (10);
            int[] eeg_channels = BoardShim.get_eeg_channels (Int32.Parse (args[0]));
            foreach (var index in eeg_channels)
                Console.WriteLine ("[{0}]", string.Join (", ", unprocessed_data.GetRow(index)));

            board_shim.release_session ();
        }
    }
}
