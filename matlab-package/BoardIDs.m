<<<<<<< HEAD
classdef BoardIDs < uint32
   enumeration
      CYTON_BOARD (0)
      GANGLION_BOARD (1)
=======
classdef BoardIDs < int32
   enumeration
      SYNTHETIC_BOARD (-1)
      CYTON_BOARD (0)
      GANGLION_BOARD (1)
      CYTON_DAISY_BOARD (2)
>>>>>>> fb96aa6396fdf5788f8d5f87573203229210de01
   end
end