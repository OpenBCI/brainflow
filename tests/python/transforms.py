import argparse
import time
import brainflow
import numpy as np

from brainflow.board_shim import BoardShim, BrainFlowInputParams, LogLevels, BoardIds
from brainflow.data_filter import DataFilter, FilterTypes, AggOperations


def main ():
    BoardShim.enable_dev_board_logger ()

    # use synthetic board for demo
    params = BrainFlowInputParams ()
    board_id = BoardIds.SYNTHETIC_BOARD.value
    board = BoardShim (board_id, params)
    board.prepare_session ()
    board.start_stream ()
    BoardShim.log_message (LogLevels.LEVEL_INFO.value, 'start sleeping in the main thread')
    time.sleep (10)
    data = board.get_board_data ()
    board.stop_stream ()
    board.release_session ()

    eeg_channels = BoardShim.get_eeg_channels (board_id)
    # demo for transforms
    for count, channel in enumerate (eeg_channels):
        print ('Original data for channel %d:' % channel)
        print (data[channel])
        wavelet_coeffs = DataFilter.perform_wavelet_transform (data[channel], 'db5')
        # first halp - approximation coeffs, second hanf - detailed coeffs
        app_coefs = wavelet_coeffs[0: int (wavelet_coeffs.shape[0] / 2)]
        detailed_coeffs = wavelet_coeffs[int (wavelet_coeffs.shape[0] / 2) : ]
        # you can do smth with wavelet coeffs here, for example denoising works via thresholds 
        # for wavelets coefficients
        restored_data = DataFilter.perform_inverse_wavelet_transform (wavelet_coeffs, data[channel].shape[0], 'db5')
        print ('Restored data for channel %d:' % channel)
        print (restored_data)


if __name__ == "__main__":
    main ()
