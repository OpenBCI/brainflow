using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using brainflow;

namespace multithread
{
    class ThreadCollector
    {
        public int sleep_ms = 2000;
        public BoardShim board_shim;
        public int board_id;
        public bool keep_alive = true;
        public DateTime thread_start_time;

        public ThreadCollector (BoardShim board, string[] args)
        {
            thread_start_time = DateTime.Now;
            board_shim = board;
            board_id = Int32.Parse (args[0]);
        }

        public void collect_data ()
        {
            while (keep_alive)
            {
                System.Threading.Thread.Sleep (sleep_ms);
                Console.WriteLine ("data count: {0}", board_shim.get_board_data_count ());
            }
            double[,] unprocessed_data = board_shim.get_board_data ();
            // check serialization
            DataHandler dh = new DataHandler (board_id, data_from_board: unprocessed_data);
            dh.save_csv (thread_start_time.ToString ("HH-mm-ss") + "before_processing.csv");
            dh = new DataHandler (board_id, csv_file: thread_start_time.ToString ("HH-mm-ss") + "before_processing.csv");
            dh.save_csv (thread_start_time.ToString ("HH-mm-ss") + "before_preprocessing2.csv");
            // check preprocessing
            dh.remove_dc_offset ();
            dh.bandpass (1.0, 50.0);
            dh.save_csv (thread_start_time.ToString ("HH-mm-ss") + "after_preprocessing.csv");
        }

        public void stop_collecting ()
        {
            keep_alive = false;
        }
    }
}
