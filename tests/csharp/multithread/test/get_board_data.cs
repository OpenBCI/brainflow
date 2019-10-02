using System;
using brainflow;

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
            BoardShim board_shim;
            if (args.Length == 2)
                board_shim = new BoardShim (Int32.Parse (args[0]), args[1]);
            else
                board_shim = new BoardShim (Int32.Parse (args[0]), null);

            // initialize thread_collectors
            for (int i = 0; i < thread_number; i++)
            {
                System.Threading.Thread.Sleep (1000);
                thread_collector_array[i] = new ThreadCollector (board_shim, args);
            }

            // prepare and start streaming
            board_shim.prepare_session ();
            Console.WriteLine ("Session is ready");

            board_shim.start_stream (3600);
            Console.WriteLine ("Started");
            System.Threading.Thread.Sleep (5000);

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

            board_shim.release_session ();
            Console.WriteLine ("Released");
        }
    }
}