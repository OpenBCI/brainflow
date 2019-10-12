import argparse
import time
import brainflow
import numpy as np

import pandas as pd
import matplotlib
matplotlib.use ('Agg')
import matplotlib.pyplot as plt

from brainflow.board_shim import BoardShim
from brainflow.data_filter import DataFilter, FilterTypes

def main ():
    parser = argparse.ArgumentParser ()
    parser.add_argument ('--port', type = str, help  = 'port name, for synthetic board port_name doesnt matter, just pass smth', required = True)
    parser.add_argument ('--board', type = int, help  = 'board id, check docs to get a list of supported boards', required = True)
    parser.add_argument ('--log', action = 'store_true')
    args = parser.parse_args ()

    if (args.log):
        BoardShim.enable_dev_board_logger ()
    else:
        BoardShim.disable_board_logger ()

    # demo how to read data as 2d numpy array
    board = BoardShim (args.board, args.port)
    board.prepare_session ()
    board.start_stream ()
    time.sleep (10)
    # data = board.get_current_board_data (256) # get latest 256 packages or less, doesnt remove them from internal buffer
    data = board.get_board_data () # get all data and remove it from internal buffer
    board.stop_stream ()
    board.release_session ()

    # demo how to convert it to pandas DF and plot data
    eeg_channels = BoardShim.get_eeg_channels (args.board)
    df = pd.DataFrame (np.transpose (data))
    print (df.head ())
    plt.figure ()
    df[eeg_channels].plot (subplots = True)
    plt.savefig ('before_processing.png')

    # demo how to perform signal processing
    for count, channel in enumerate (eeg_channels):
        # e.g. apply bessel bandpass filter, order - 4, central freq - 15.0, band width - 6
        if count == 0:
            DataFilter.perform_bandpass (data[channel], BoardShim.get_sampling_rate (args.board), 15.0, 6.0, 4, FilterTypes.BESSEL.value, 0)
        elif count == 1:
            DataFilter.perform_bandstop (data[channel], BoardShim.get_sampling_rate (args.board), 5.0, 1.0, 3, FilterTypes.BUTTERWORTH.value, 0)
        elif count == 2:
            DataFilter.perform_lowpass (data[channel], BoardShim.get_sampling_rate (args.board), 9.0, 5, FilterTypes.CHEBYSHEV_TYPE_1.value, 1)
        elif count == 3:
            DataFilter.perform_highpass (data[channel], BoardShim.get_sampling_rate (args.board), 3.0, 4, FilterTypes.BUTTERWORTH.value, 0)

    df = pd.DataFrame (np.transpose (data))
    print (df.head ())
    plt.figure ()
    df[eeg_channels].plot (subplots = True)
    plt.savefig ('after_processing.png')


if __name__ == "__main__":
    main ()
