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
            // use it to run from VS solution wo cmd line args
            // int board_id = (int)BoardIds.SYNTHETIC_BOARD;
            // board_shim = new BoardShim (board_id, "");
            int board_id = Int32.Parse (args[0]);
            if (args.Length == 2)
                board_shim = new BoardShim (board_id, args[1]);
            else
                board_shim = new BoardShim (board_id, null);

            board_shim.prepare_session ();
            board_shim.start_stream (3600);
            System.Threading.Thread.Sleep (5000);
            board_shim.stop_stream ();
            Console.WriteLine ("data count: {0}", board_shim.get_board_data_count ());
            double[,] unprocessed_data = board_shim.get_current_board_data (50);
            int[] eeg_channels = BoardShim.get_eeg_channels (board_id);
            Console.WriteLine ("Before processing:");
            foreach (var index in eeg_channels)
                Console.WriteLine ("[{0}]", string.Join (", ", unprocessed_data.GetRow (index)));
            board_shim.release_session ();

            // for demo apply different filters to different channels
            double[] filtered;
            for (int i = 0; i < eeg_channels.Length; i++)
            {
                switch (i)
                {
                    case 0:
                        filtered = DataFilter.perform_lowpass (unprocessed_data.GetRow(eeg_channels[i]), BoardShim.get_sampling_rate (board_id), 20.0, 4, (int)FilterTypes.BESSEL, 0.0);
                        Console.WriteLine ("Filtered channel " + eeg_channels[i]);
                        Console.WriteLine ("[{0}]", string.Join (", ", filtered));
                        break;
                    case 1:
                        filtered = DataFilter.perform_highpass (unprocessed_data.GetRow (eeg_channels[i]), BoardShim.get_sampling_rate (board_id), 2.0, 4, (int)FilterTypes.BUTTERWORTH, 0.0);
                        Console.WriteLine ("Filtered channel " + eeg_channels[i]);
                        Console.WriteLine ("[{0}]", string.Join (", ", filtered));
                        break;
                    case 2:
                        filtered = DataFilter.perform_bandpass (unprocessed_data.GetRow (eeg_channels[i]), BoardShim.get_sampling_rate (board_id), 20.0, 5.0, 2, (int)FilterTypes.BUTTERWORTH, 0.0);
                        Console.WriteLine ("Filtered channel " + eeg_channels[i]);
                        Console.WriteLine ("[{0}]", string.Join (", ", filtered));
                        break;
                    case 3:
                        filtered = DataFilter.perform_bandstop (unprocessed_data.GetRow (eeg_channels[i]), BoardShim.get_sampling_rate (board_id), 20.0, 1.0, 6, (int)FilterTypes.CHEBYSHEV_TYPE_1, 1.0);
                        Console.WriteLine ("Filtered channel " + eeg_channels[i]);
                        Console.WriteLine ("[{0}]", string.Join (", ", filtered));
                        break;
                }
            }
        }
    }
}
