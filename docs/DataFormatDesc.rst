Data Format Description
=========================

**Methods like:**

.. code-block:: python

   get_board_data ()
   get_current_boards_data (100)

**Return 2d double array [num_channels x num_data_points], rows of this array contain data for EEG, EMG, Accel, Timesteps and other kinds of data.**

Exact format for this array is board specific but to keep API uniform we have methods like:

.. code-block:: python

   # these methods return an array of rows in this 2d array containing eeg\emg\ecg\accel data
   get_eeg_channels (board_id)
   get_emg_channels (board_id)
   get_ecg_channels (board_id)
   get_accel_channels (board_id)
   # and so on, check docs for full list
   # also we have methods to get sampling rate from board id, get number of timestamp channel and others
   get_sampling_rate (board_id)
   get_timestamp_channel (board_id)
   # and so on

Using methods above you can write completely board agnostic code and switch boards using single parameter! Even if you have only one board using these methods you can easily switch to Synthetic board for development and run code without real hardware.

Special channels for Cyton Based boards
-----------------------------------------

`Cyton based boards from OpenBCI <https://docs.openbci.com/Tutorials/00-Tutorials>`_ suport different output formats like described `here <https://docs.openbci.com/docs/02Cyton/CytonDataFormat#firmware-version-200-fall-2016-to-now-1>`_.

**For Cyton based boards we add Cyton End byte to a first channel from:**

.. code-block:: python

   get_other_channels (board_id)

**If Cyton End bytes is equal to 0xC0 we add accel data, to get rows which contain accel data use:**

.. code-block:: python

   get_accel_channels (board_id)

**If Cyton End bytes is equal to 0xC1 we add analog data, to get rows which contain analog data use:**

.. code-block:: python

   get_analog_channels (board_id)

For analog data we return int32 values but from low level API we return double array so these values were casted to double wo any changes.

**If Cyton End Byte is between 0xC2 and 0xC6 we add raw unprocessed bytes to the second and following channels returned by:**

.. code-block:: python

   get_other_channels (board_id)

For such data we return unprocessed raw bytes you should cast them to integer or floats

If Cyton End Byte outside `this range <https://docs.openbci.com/docs/02Cyton/CytonDataFormat#firmware-version-200-fall-2016-to-now-1>`_ we drop entire package.