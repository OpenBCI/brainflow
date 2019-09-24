using System;
using brainflow;
using System.Threading;

namespace test
{
    class GetBoardData
    {
        private BoardShim _board;

        private Thread monitorThread;

        private volatile bool keepAlive;

        private void GetSignal()
        {
            int SleepMs = 5000;
            Thread.Sleep(SleepMs);

            Console.WriteLine("data count: {0}", board_shim.get_board_data_count());
            double[,] unprocessed_data = board_shim.get_board_data();

            // check serialization
            DataHandler dh = new DataHandler(Int32.Parse(args[0]), data_from_board: unprocessed_data);
            dh.save_csv("before_processing.csv");
            dh = new DataHandler(Int32.Parse(args[0]), csv_file: "before_processing.csv");
            dh.save_csv("before_preprocessing2.csv");
            // check preprocessing
            dh.remove_dc_offset();
            dh.bandpass(1.0, 50.0);
            dh.save_csv("after_preprocessing.csv");

        }

        static void Main (string[] args)
        {
            boardshim.enable_dev_board_logger();
            boardshim board_shim;
            if (args.Length == 2)
                board_shim = new BoardShim(Int32.Parse(args[0]), args[1]);
            else
                board_shim = new BoardShim(Int32.Parse(args[0]), null);

            board_shim.prepare_session();
            Console.WriteLine("Session is ready");

            board_shim.start_stream(3600);
            Console.WriteLine("Started");

            keepAlive = true;
            monitorThread = new Thread(GetSignal);
            monitorThread.Start();
            System.Threading.Thread.Sleep(6000);
            monitorThread.Join();

            board_shim.stop_stream();
            Console.WriteLine("Stopped");
            
            board_shim.release_session();
            Console.WriteLine("Released");



            //BoardShim.enable_dev_board_logger ();
            //BoardShim board_shim;
            //if (args.Length == 2)
            //    board_shim = new BoardShim (Int32.Parse(args[0]), args[1]);
            //else
            //    board_shim = new BoardShim (Int32.Parse (args[0]), null);

            //board_shim.prepare_session ();
            //Console.WriteLine ("Session is ready");

            //board_shim.start_stream (3600);
            //Console.WriteLine ("Started");
            //System.Threading.Thread.Sleep (5000);

            //board_shim.stop_stream ();
            //Console.WriteLine ("Stopped");

            //Console.WriteLine ("data count: {0}", board_shim.get_board_data_count ());
            //double[,] unprocessed_data = board_shim.get_board_data ();

            //// check serialization
            //DataHandler dh = new DataHandler (Int32.Parse(args[0]), data_from_board: unprocessed_data);
            //dh.save_csv ("before_processing.csv");
            //dh = new DataHandler (Int32.Parse(args[0]), csv_file: "before_processing.csv");
            //dh.save_csv ("before_preprocessing2.csv");
            //// check preprocessing
            //dh.remove_dc_offset ();
            //dh.bandpass (1.0, 50.0);
            //dh.save_csv ("after_preprocessing.csv");

            //board_shim.release_session ();
            //Console.WriteLine ("Released");
        }




    }
}
