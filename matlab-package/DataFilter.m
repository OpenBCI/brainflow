classdef DataFilter
    properties
        libname
    end
    methods
        function obj = DataFilter ()
            if ispc
                obj.libname = 'DataHandler';
                if not (libisloaded ('DataHandler'))
                    loadlibrary ('./lib/DataHandler.dll', './inc/data_handler.h');
                end
            elseif ismac
                obj.libname = 'libDataHandler';
                if not (libisloaded ('libDataHandler'))
                    loadlibrary ('./lib/libDataHandler.dylib', './inc/data_handler.h');
                end
            elseif isunix
                obj.libname = 'libDataHandler';
                if not (libisloaded ('libDataHandler'))
                    loadlibrary ('./lib/libDataHandler.so', './inc/data_handler.h');
                end
            else
                error ('OS not supported!')
            end
        end

        function check_ec (obj, ec, task_name)
            if (ExitCodes.STATUS_OK.ne(ec))
                error ('%s non zero ec: %d', task_name, ec)
            end
        end

        function filtered_data = perform_lowpass (obj, data, sampling_rate, cutoff, order, filter_type, ripple)
            task_name = 'perform_lowpass';
            buf = libpointer ('doublePtr', data);
            len = size (data, 2);
            exit_code = calllib (obj.libname, task_name, buf, len, sampling_rate, cutoff, order, int32 (filter_type), ripple);
            obj.check_ec (exit_code, task_name);
            filtered_data = buf.Value;
        end
        
        function filtered_data = perform_highpass (obj, data, sampling_rate, cutoff, order, filter_type, ripple)
            task_name = 'perform_highpass';
            buf = libpointer ('doublePtr', data);
            len = size (data, 2);
            exit_code = calllib (obj.libname, task_name, buf, len, sampling_rate, cutoff, order, int32 (filter_type), ripple);
            obj.check_ec (exit_code, task_name);
            filtered_data = buf.Value;
        end
        
        function filtered_data = perform_bandpass (obj, data, sampling_rate, center_freq, band_width, order, filter_type, ripple)
            task_name = 'perform_bandpass';
            buf = libpointer ('doublePtr', data);
            len = size (data, 2);
            exit_code = calllib (obj.libname, task_name, buf, len, sampling_rate, center_freq, band_width, order, int32 (filter_type), ripple);
            obj.check_ec (exit_code, task_name);
            filtered_data = buf.Value;
        end
        
        function filtered_data = perform_bandstop (obj, data, sampling_rate, center_freq, band_width, order, filter_type, ripple)
            task_name = 'perform_bandstop';
            buf = libpointer ('doublePtr', data);
            len = size (data, 2);
            exit_code = calllib (obj.libname, task_name, buf, len, sampling_rate, center_freq, band_width, order, int32 (filter_type), ripple);
            obj.check_ec (exit_code, task_name);
            filtered_data = buf.Value;
        end
    end
end