board_id = BoardIDs.SYNTHETIC_BOARD;
board_shim = BoardShim (board_id, '');
board_shim.prepare_session ();
board_shim.start_stream (3600);
pause (5);
board_shim.stop_stream ();
data = board_shim.get_current_board_data (50);
board_shim.release_session ();

eeg_channels = board_shim.get_eeg_channels ();
data_filter = DataFilter ();
filtered_first_channel = data_filter.perform_lowpass(data(eeg_channels(1),:), board_shim.get_sampling_rate(), 20.0, 4, FilterTypes.BESSEL, 0.0);
filtered_second_channel = data_filter.perform_highpass(data(eeg_channels(2),:), board_shim.get_sampling_rate(), 2.0, 4, FilterTypes.BUTTERWORTH, 0.0);
filtered_third_channel = data_filter.perform_bandpass(data(eeg_channels(3),:), board_shim.get_sampling_rate(), 10.0, 5.0, 4, FilterTypes.CHEBYSHEV_TYPE_1, 1.0);
filtered_fourth_channel = data_filter.perform_bandstop(data(eeg_channels(4),:), board_shim.get_sampling_rate(), 2.0, 1.0, 4, FilterTypes.BUTTERWORTH, 0.0);