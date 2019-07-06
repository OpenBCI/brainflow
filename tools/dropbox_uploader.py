import argparse
import os

import dropbox
import glob


class TransferData:

    def __init__ (self, access_token):
        self.access_token = access_token

    def upload_file (self, file_from, file_to):
        dbx = dropbox.Dropbox (self.access_token)
        # good enought for our task
        if file_from.endswith ('.h') or file_from.endswith ('.cmake'):
            with open (file_from, 'r') as f:
                dbx.files_upload (f.read (), file_to)
        else:
            with open (file_from, 'rb') as f:
                dbx.files_upload (f.read (), file_to)

def main ():

    parser = argparse.ArgumentParser ()
    parser.add_argument ('--token', type = str, help  = 'access token', required = True)
    parser.add_argument ('--local-files', type = str, help  = 'local file to send', required = True)
    parser.add_argument ('--remote-dir', type = str, help = 'remote file location', required = True)
    args = parser.parse_args ()

    transferData = TransferData (args.token)
    for file in glob.glob (args.local_files):
        remote_file = args.remote_dir + '/' + os.path.split (file)[1]
        transferData.upload_file (file, remote_file)

if __name__ == '__main__':
    main ()
