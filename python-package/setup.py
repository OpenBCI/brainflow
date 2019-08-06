import os
import platform
import sys
from setuptools import setup, find_packages
import struct
import ctypes
import pkg_resources
#### check lib file
if platform.system () == 'Windows':
    if struct.calcsize ("P") * 8 == 64:
        dll_paths = ['brainflow\\lib\\BoardController.dll', 'brainflow\\lib\\GanglionLib.dll', 'brainflow\\lib\\GanglionLibNative64.dll']
    else:
        dll_paths = ['brainflow\\lib\\BoardController32.dll', 'brainflow\\lib\\GanglionLibNative32.dll']
elif platform.system () == 'Darwin':
    dll_paths = ['brainflow/lib/libBoardController.dylib']
else:
    dll_paths = ['brainflow/lib/libBoardController.so']
for dll_path in dll_paths:
    full_path = pkg_resources.resource_filename (__name__, dll_path)
    if os.path.isfile (full_path):
        lib = ctypes.cdll.LoadLibrary (full_path)
    else:
        raise FileNotFoundError ('Dynamic library %s is missed, did you forget to compile brainflow before installation of python package?' % full_path)
this_directory = os.path.abspath (os.path.dirname (__file__))
#### check lib file
with open (os.path.join (this_directory, 'README.md')) as f:
    long_description = f.read ()

setup (
    name = 'brainflow',
    version = os.environ.get('BRAINFLOW_VERSION', '1.0.8'),
    description = 'Library to get data from OpenBCI boards',
    long_description = long_description,
    long_description_content_type = 'text/markdown',
    url = 'https://github.com/Andrey1994/brainflow',
    author = 'Andrey Parfenov',
    author_email = 'a1994ndrey@gmail.com',
    packages = find_packages (),
    classifiers = [
        'Development Status :: 2 - Pre-Alpha',
        'Topic :: Utilities'
    ],
    install_requires = [
        'numpy', 'scipy', 'pandas', 'enum-compat'
    ],
    package_data = {
        'brainflow': [
            # I think we can add 32bit version only for windows at least for now
            os.path.join ('lib', 'BoardController.dll'),
            os.path.join ('lib', 'BoardController32.dll'),
            os.path.join ('lib', 'libBoardController.so'),
            os.path.join ('lib', 'GanglionLib.dll'),
            os.path.join ('lib', 'GanglionLibNative64.dll'),
            os.path.join ('lib', 'GanglionLibNative32.dll'),
            os.path.join ('lib', 'libBoardController.dylib'),
        ]
    },
    zip_safe = True,
    python_requires = '>=2.7'
)
