classdef BoardShim
    properties
        libname
        board_id
        port_name
    end
    methods
        function obj = BoardShim (board_id, port_name)
            if ispc
                obj.libname = 'BoardController';
                if not (libisloaded ('BoardController'))
                    loadlibrary ('./lib/BoardController.dll', './inc/board_controller.h', 'addheader', './inc/board_info_getter.h');
                end
            elseif ismac
                obj.libname = 'libBoardController';
                if not (libisloaded ('libBoardController'))
                    loadlibrary ('./lib/libBoardController.dylib', './inc/board_controller.h', 'addheader', './inc/board_info_getter.h');
                end
            elseif isunix
                obj.libname = 'libBoardController';
                if not (libisloaded ('libBoardController'))
                    loadlibrary ('./lib/libBoardController.so', './inc/board_controller.h', 'addheader', './inc/board_info_getter.h');
                end
            else
                error ('OS not supported!')
            end
            obj.port_name = port_name;
            obj.board_id = int32 (board_id);
        end

        function check_ec (obj, ec, task_name)
            if (ExitCodes.STATUS_OK.ne(ec))
                error ('%s non zero ec: %d', task_name, ec)
            end
        end

        function prepare_session (obj)
            task_name = 'prepare_session';
            exit_code = calllib (obj.libname, task_name, obj.board_id, obj.port_name);
            obj.check_ec (exit_code, task_name);
        end

        function config_board (obj, config)
            task_name = 'config_board';
            exit_code = calllib (obj.libname, task_name, config, obj.board_id, obj.port_name);
            obj.check_ec (exit_code, task_name);
        end

        function start_stream (obj, buffer_size)
            task_name = 'start_stream';
            exit_code = calllib (obj.libname, task_name, buffer_size, obj.board_id, obj.port_name);
            obj.check_ec (exit_code, task_name);
        end

        function stop_stream (obj)
            task_name = 'stop_stream';
            exit_code = calllib (obj.libname, task_name, obj.board_id, obj.port_name);
            obj.check_ec (exit_code, task_name);
        end

        function release_session (obj)
            task_name = 'release_session';
            exit_code = calllib (obj.libname, task_name, obj.board_id, obj.port_name);
            obj.check_ec (exit_code, task_name);
        end

        function set_log_level (obj, log_level)
            task_name = 'set_log_level';
            exit_code = calllib (obj.libname, task_name, log_level);
            obj.check_ec (exit_code, task_name);
        end

        function set_log_file (obj, log_file)
            task_name = 'set_log_file';
            exit_code = calllib (obj.libname, task_name, log_file);
            obj.check_ec (exit_code, task_name);
        end

        function enable_board_logger (obj)
            obj.set_log_level (2)
        end

        function enable_dev_board_logger (obj)
            obj.set_log_level (0)
        end

        function disable_board_logger (obj)
            obj.set_log_level (6)
        end

        function sampling_rate = get_sampling_rate (obj)
            task_name = 'get_sampling_rate';
            res = libpointer ('int32Ptr', 0);
            exit_code = calllib (obj.libname, task_name, obj.board_id, res);
            obj.check_ec (exit_code, task_name);
            sampling_rate = res.Value;
        end

        function package_num_channel = get_package_num_channel (obj)
            task_name = 'get_package_num_channel';
            res = libpointer ('int32Ptr', 0);
            exit_code = calllib (obj.libname, task_name, obj.board_id, res);
            obj.check_ec (exit_code, task_name);
            package_num_channel = res.Value;
        end

        function timestamp_channel = get_timestamp_channel (obj)
            task_name = 'get_timestamp_channel';
            res = libpointer ('int32Ptr', 0);
            exit_code = calllib (obj.libname, task_name, obj.board_id, res);
            obj.check_ec (exit_code, task_name);
            timestamp_channel = res.Value;
        end

        function num_rows = get_num_rows (obj)
            task_name = 'get_num_rows';
            res = libpointer ('int32Ptr', 0);
            exit_code = calllib (obj.libname, task_name, obj.board_id, res);
            obj.check_ec (exit_code, task_name);
            num_rows = res.Value;
        end

        function num_data_point = get_board_data_count (obj)
            task_name = 'get_board_data_count';
            data_count = libpointer ('int32Ptr', 0);
            exit_code = calllib (obj.libname, task_name, data_count, obj.board_id, obj.port_name);
            obj.check_ec (exit_code, task_name);
            num_data_point = data_count.value;
        end

        function eeg_channels = get_eeg_channels (obj)
            task_name = 'get_emg_channels';
            len = libpointer ('int32Ptr', 0);
            channels = libpointer ('int32Ptr', zeros (1, 256));
            exit_code = calllib (obj.libname, task_name, obj.board_id, channels, len);
            obj.check_ec (exit_code, task_name);
            eeg_channels = channels.Value (1, 1:len.Value);
        end

        function emg_channels = get_emg_channels (obj)
            task_name = 'get_emg_channels';
            len = libpointer ('int32Ptr', 0);
            channels = libpointer ('int32Ptr', zeros (1, 256));
            exit_code = calllib (obj.libname, task_name, obj.board_id, channels, len);
            obj.check_ec (exit_code, task_name);
            emg_channels = channels.Value (1, 1:len.Value);
        end

        function ecg_channels = get_ecg_channels (obj)
            task_name = 'get_ecg_channels';
            len = libpointer ('int32Ptr', 0);
            channels = libpointer ('int32Ptr', zeros (1, 256));
            exit_code = calllib (obj.libname, task_name, obj.board_id, channels, len);
            obj.check_ec (exit_code, task_name);
            ecg_channels = channels.Value (1, 1:len.Value);
        end

        function eog_channels = get_eog_channels (obj)
            task_name = 'get_eog_channels';
            len = libpointer ('int32Ptr', 0);
            channels = libpointer ('int32Ptr', zeros (1, 256));
            exit_code = calllib (obj.libname, task_name, obj.board_id, channels, len);
            obj.check_ec (exit_code, task_name);
            eog_channels = channels.Value (1, 1:len.Value);
        end

        function eda_channels = get_eda_channels (obj)
            task_name = 'get_eda_channels';
            len = libpointer ('int32Ptr', 0);
            channels = libpointer ('int32Ptr', zeros (1, 256));
            exit_code = calllib (obj.libname, task_name, obj.board_id, channels, len);
            obj.check_ec (exit_code, task_name);
            eda_channels = channels.Value (1, 1:len.Value);
        end

        function ppg_channels = get_ppg_channels (obj)
            task_name = 'get_eda_channels';
            len = libpointer ('int32Ptr', 0);
            channels = libpointer ('int32Ptr', zeros (1, 256));
            exit_code = calllib (obj.libname, task_name, obj.board_id, channels, len);
            obj.check_ec (exit_code, task_name);
            ppg_channels = channels.Value (1, 1:len.Value);
        end

        function accel_channels = get_accel_channels (obj)
            task_name = 'get_eda_channels';
            len = libpointer ('int32Ptr', 0);
            channels = libpointer ('int32Ptr', zeros (1, 256));
            exit_code = calllib (obj.libname, task_name, obj.board_id, channels, len);
            obj.check_ec (exit_code, task_name);
            accel_channels = channels.Value (1, 1:len.Value);
        end

        function gyro_channels = get_gyro_channels (obj)
            task_name = 'get_eda_channels';
            len = libpointer ('int32Ptr', 0);
            channels = libpointer ('int32Ptr', zeros (1, 256));
            exit_code = calllib (obj.libname, task_name, obj.board_id, channels, len);
            obj.check_ec (exit_code, task_name);
            gyro_channels = channels.Value (1, 1:len.Value);
        end

        function other_channels = get_other_channels (obj)
            task_name = 'get_eda_channels';
            len = libpointer ('int32Ptr', 0);
            channels = libpointer ('int32Ptr', zeros (1, 256));
            exit_code = calllib (obj.libname, task_name, obj.board_id, channels, len);
            obj.check_ec (exit_code, task_name);
            other_channels = channels.Value (1, 1:len.Value);
        end

        function data_buf = get_board_data (obj)
            task_name = 'get_board_data';
            data_count = obj.get_board_data_count ();
            num_rows = obj.get_num_rows();
            data = libpointer ('doublePtr', zeros (1, data_count * num_rows));
            exit_code = calllib (obj.libname, task_name, data_count, data, obj.board_id, obj.port_name);
            obj.check_ec (exit_code, task_name);
            data_buf = transpose(reshape (data.Value, [data_count, num_rows]));
        end

        function data_buf = get_current_board_data (obj, num_samples)
            task_name = 'get_current_board_data';
            data_count = libpointer ('int32Ptr', 0);
            num_rows = obj.get_num_rows();
            data = libpointer ('doublePtr', zeros (1, num_samples * num_rows));
            exit_code = calllib (obj.libname, task_name, num_samples, data, data_count, obj.board_id, obj.port_name);
            obj.check_ec (exit_code, task_name);
            data_buf = transpose (reshape (data.Value (1,1:data_count.Value * num_rows), [data_count.Value, num_rows]));
        end
    end
end