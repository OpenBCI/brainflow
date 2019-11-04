using System;
using brainflow;
using Accord.Math;

namespace test
{
    class GetBoardData
    {
        static void Main (string[] args)
        {
            int thread_number = 5;
            System.Threading.Thread[] thread_array = new System.Threading.Thread[thread_number];
            ThreadCollector[] thread_collector_array = new ThreadCollector[thread_number];

            BoardShim.enable_dev_board_logger ();

            BrainFlowInputParams input_params = new BrainFlowInputParams ();
            int board_id = parse_args (args, input_params);

            BoardShim board_shim = new BoardShim (board_id, input_params);

            // initialize thread_collectors
            for (int i = 0; i < thread_number; i++)
            {
                System.Threading.Thread.Sleep (1000);
                thread_collector_array[i] = new ThreadCollector (board_shim, board_id);
            }

            // prepare and start streaming
            board_shim.prepare_session ();
            Console.WriteLine ("Session is ready");

            board_shim.start_stream (3600);
            Console.WriteLine ("Started");

            // collect data in threads
            for (int i = 0; i < thread_number; i++)
            {
                System.Threading.Thread.Sleep (500);
                thread_array[i] = new System.Threading.Thread (thread_collector_array[i].collect_data);
                thread_array[i].Start ();
                Console.WriteLine ("Thread " + i.ToString () + " Start!");
            }

            // wait for 10s
            System.Threading.Thread.Sleep (10000);

            // stop collecting data in threads
            for (int i = 0; i < thread_number; i++)
            {
                System.Threading.Thread.Sleep (500);
                thread_collector_array[i].keep_alive = false;
                thread_array[i].Join ();
                Console.WriteLine ("Thread " + i.ToString () + " stopped!");
            }

            // wait for 1s
            System.Threading.Thread.Sleep (1000);

            // stop streaming and release
            board_shim.stop_stream ();
            Console.WriteLine ("Stopped");
            double[,] unprocessed_data = board_shim.get_current_board_data (50);
            int[] eeg_channels = BoardShim.get_eeg_channels (board_id);
            foreach (var index in eeg_channels)
                Console.WriteLine ("[{0}]", string.Join (", ", unprocessed_data.GetRow(index)));
            board_shim.release_session ();
            Console.WriteLine ("Released");
        }

        static int parse_args (string[] args, BrainFlowInputParams input_params)
        {
            int board_id = (int)BoardIds.SYNTHETIC_BOARD; //assume synthetic board by default
            // use docs to get params for your specific board, e.g. set serial_port for Cyton
            for (int i = 0; i < args.Length; i++)
            {
                if (args[i].Equals ("--ip-address"))
                {
                    input_params.ip_address = args[i + 1];
                }
                if (args[i].Equals ("--mac-address"))
                {
                    input_params.mac_address = args[i + 1];
                }
                if (args[i].Equals ("--serial-port"))
                {
                    input_params.serial_port = args[i + 1];
                }
                if (args[i].Equals ("--other-info"))
                {
                    input_params.other_info = args[i + 1];
                }
                if (args[i].Equals ("--ip-port"))
                {
                    input_params.ip_port = Convert.ToInt32 (args[i + 1]);
                }
                if (args[i].Equals ("--ip-protocol"))
                {
                    input_params.ip_protocol = Convert.ToInt32 (args[i + 1]);
                }
                if (args[i].Equals ("--board-id"))
                {
                    board_id = Convert.ToInt32 (args[i + 1]);
                }
            }
            return board_id;
        }
    }
}
