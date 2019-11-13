﻿using System;

using Accord.Math;

namespace brainflow
{
    /// <summary>
    /// DataFilter class to perform signal processing
    /// </summary>
    public class DataFilter
    {
        // accord GetRow returns a copy instead pointer, so we can not easily update data in place like in other bindings

        /// <summary>
        /// perform lowpass filter, unlike other bindings instead in-place calculation it returns new array
        /// </summary>
        /// <param name="data"></param>
        /// <param name="sampling_rate"></param>
        /// <param name="cutoff"></param>
        /// <param name="order"></param>
        /// <param name="filter_type"></param>
        /// <param name="ripple"></param>
        /// <returns>filtered data</returns>
        public static double[] perform_lowpass (double[] data, int sampling_rate, double cutoff, int order, int filter_type, double ripple)
        {
            double[] filtered_data = new double[data.Length];
            Array.Copy (data, filtered_data, data.Length);
            int res = DataHandlerLibrary.perform_lowpass (filtered_data, data.Length, sampling_rate, cutoff, order, filter_type, ripple);
            if (res != (int)CustomExitCodes.STATUS_OK)
            {
                throw new BrainFlowException (res);
            }
            return filtered_data;
        }

        /// <summary>
        /// perform highpass filter, unlike other bindings instead in-place calculation it returns new array
        /// </summary>
        /// <param name="data"></param>
        /// <param name="sampling_rate"></param>
        /// <param name="cutoff"></param>
        /// <param name="order"></param>
        /// <param name="filter_type"></param>
        /// <param name="ripple"></param>
        /// <returns>filtered data</returns>
        public static double[] perform_highpass (double[] data, int sampling_rate, double cutoff, int order, int filter_type, double ripple)
        {
            double[] filtered_data = new double[data.Length];
            Array.Copy (data, filtered_data, data.Length);
            int res = DataHandlerLibrary.perform_highpass (filtered_data, data.Length, sampling_rate, cutoff, order, filter_type, ripple);
            if (res != (int)CustomExitCodes.STATUS_OK)
            {
                throw new BrainFlowException (res);
            }
            return filtered_data;
        }

        /// <summary>
        /// perform bandpass filter, unlike other bindings instead in-place calculation it returns new array
        /// </summary>
        /// <param name="data"></param>
        /// <param name="sampling_rate"></param>
        /// <param name="center_freq"></param>
        /// <param name="band_width"></param>
        /// <param name="order"></param>
        /// <param name="filter_type"></param>
        /// <param name="ripple"></param>
        /// <returns>filtered data</returns>
        public static double[] perform_bandpass (double[] data, int sampling_rate, double center_freq, double band_width, int order, int filter_type, double ripple)
        {
            double[] filtered_data = new double[data.Length];
            Array.Copy (data, filtered_data, data.Length);
            int res = DataHandlerLibrary.perform_bandpass (filtered_data, data.Length, sampling_rate, center_freq, band_width, order, filter_type, ripple);
            if (res != (int)CustomExitCodes.STATUS_OK)
            {
                throw new BrainFlowException (res);
            }
            return filtered_data;
        }

        /// <summary>
        /// perform bandstop filter, unlike other bindings instead in-place calculation it returns new array
        /// </summary>
        /// <param name="data"></param>
        /// <param name="sampling_rate"></param>
        /// <param name="center_freq"></param>
        /// <param name="band_width"></param>
        /// <param name="order"></param>
        /// <param name="filter_type"></param>
        /// <param name="ripple"></param>
        /// <returns>filtered data</returns>
        public static double[] perform_bandstop (double[] data, int sampling_rate, double center_freq, double band_width, int order, int filter_type, double ripple)
        {
            double[] filtered_data = new double[data.Length];
            Array.Copy (data, filtered_data, data.Length);
            int res = DataHandlerLibrary.perform_bandstop (filtered_data, data.Length, sampling_rate, center_freq, band_width, order, filter_type, ripple);
            if (res != (int)CustomExitCodes.STATUS_OK)
            {
                throw new BrainFlowException (res);
            }
            return filtered_data;
        }

        /// <summary>
        /// perform moving average or moving median filter, unlike other bindings instead in-place calculation it returns new array
        /// </summary>
        /// <param name="data"></param>
        /// <param name="period"></param>
        /// <param name="operation"></param>
        /// <returns>filered data</returns>
        public static double[] perform_rolling_filter (double[] data, int period, int operation)
        {
            double[] filtered_data = new double[data.Length];
            Array.Copy (data, filtered_data, data.Length);
            int res = DataHandlerLibrary.perform_rolling_filter (filtered_data, data.Length, period, operation);
            if (res != (int)CustomExitCodes.STATUS_OK)
            {
                throw new BrainFlowException (res);
            }
            return filtered_data;
        }

        /// <summary>
        /// write data to csv file, data will be transposed
        /// </summary>
        /// <param name="data"></param>
        /// <param name="file_name"></param>
        /// <param name="file_mode"></param>
        public static void write_file (double[,] data, string file_name, string file_mode)
        {
            int num_rows = data.Rows();
            int res = DataHandlerLibrary.write_file (data.Flatten(), data.Rows (), data.Columns (), file_name, file_mode);
            if (res != (int)CustomExitCodes.STATUS_OK)
            {
                throw new BrainFlowException (res);
            }
        }

        /// <summary>
        /// read data from file, data will be transposed back to original format
        /// </summary>
        /// <param name="file_name"></param>
        /// <returns></returns>
        public static double[,] read_file (string file_name)
        {
            int[] num_elements = new int[1];
            int res = DataHandlerLibrary.get_num_elements_in_file (file_name, num_elements);
            if (res != (int)CustomExitCodes.STATUS_OK)
            {
                throw new BrainFlowException (res);
            }
            double[] data_arr = new double[num_elements[0]];
            int[] num_rows = new int[1];
            int[] num_cols = new int[1];
            res = DataHandlerLibrary.read_file (data_arr, num_rows, num_cols, file_name, num_elements[0]);
            if (res != (int)CustomExitCodes.STATUS_OK)
            {
                throw new BrainFlowException (res);
            }

            double[,] result = new double[num_rows[0], num_cols[0]];
            for (int i = 0; i < num_rows[0]; i++)
            {
                for (int j = 0; j < num_cols[0]; j++)
                {
                    result[i, j] = data_arr[i * num_cols[0] + j];
                }
            }
            return result;
        }
    }
}
