﻿using System;
using System.IO;
using System.Reflection;

namespace brainflow
{
    public class DataFilter
    {
        static DataFilter ()
        {
            // in nuget we can put unmanaged libraries to 'lib' folder and this folder is on top level from brainflow.dll
            // set PATH env variable to this folder and unmanaged libraries will be loaded wo copypaste
            // need to dubplicate this code for all classes which loads c++ libs
            string assembly_folder = Path.GetDirectoryName (Assembly.GetExecutingAssembly ().Location);
            DirectoryInfo directory_info = new DirectoryInfo (assembly_folder);
            foreach (var dir in directory_info.Parent.EnumerateDirectories ())
            {
                string lib_location = "lib";
                if (dir.ToString ().EndsWith (lib_location))
                {
                    string path_var = Environment.GetEnvironmentVariable ("PATH");
                    Environment.SetEnvironmentVariable ("PATH", path_var + ";" + dir.FullName);
                    path_var = Environment.GetEnvironmentVariable ("PATH");
                }
            }
        }

        // accord GetRow returns a copy instead pointer, so we can not easily update data in place like in other bindings
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
    }
}
