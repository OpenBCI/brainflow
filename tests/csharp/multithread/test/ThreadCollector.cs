using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using brainflow;

namespace test
{
    class ThreadCollector
    {
        public int sleep_ms = 2000;
        public BoardShim board_shim;
        public int board_id;
        public bool keep_alive = true;
        public DateTime thread_start_time;

        public ThreadCollector (BoardShim board, int id)
        {
            thread_start_time = DateTime.Now;
            board_shim = board;
            board_id = id;
        }

        public void collect_data ()
        {
            while (keep_alive)
            {
                System.Threading.Thread.Sleep (sleep_ms);
                Console.WriteLine ("data count: {0}", board_shim.get_board_data_count ());
                double[,] unprocessed_data = board_shim.get_current_board_data (250);
            }

        }

        public void stop_collecting ()
        {
            keep_alive = false;
        }
    }
}
