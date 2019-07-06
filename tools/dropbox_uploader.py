import dropbox
import argparse


class TransferData:

    def __init__ (self, access_token):
        self.access_token = access_token

    def upload_file (self, file_from, file_to):
        dbx = dropbox.Dropbox (self.access_token)
        with open (file_from, 'rb') as f:
            dbx.files_upload (f.read (), file_to)

def main ():

    parser = argparse.ArgumentParser ()
    parser.add_argument ('--token', type = str, help  = 'access token', required = True)
    parser.add_argument ('--local-file', type = str, help  = 'local file to send', required = True)
    parser.add_argument ('--remote-file', type = str, help = 'remote file location', required = True)
    args = parser.parse_args ()

    transferData = TransferData (args.token)
    transferData.upload_file (args.local_file, args.remote_file)

if __name__ == '__main__':
    main ()
