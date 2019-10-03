import http.client
import threading
from http.server import HTTPServer, BaseHTTPRequestHandler


class Handler (BaseHTTPRequestHandler):
    
    def _set_headers (self):
        self.send_response (200)
        self.send_header ("Content-type", "text/html")
        self.end_headers ()

    def _html (self, message):
        content = f"<html><body><h1>{message}</h1></body></html>"
        return content.encode("utf8")

    def do_GET (self):
        if (self.path == '/stream/start'):
            pass
        elif (self.path == '/stream/stop'):
            pass
        else:
            self._set_headers ()
            self.wfile.write (self._html ("GET!"))

    def do_HEAD (self):
        self._set_headers ()

    def do_POST (self):
        self._set_headers ()
        self.wfile.write (self._html ("POST!"))


def run (server_class = HTTPServer, handler_class = Handler):
    server_address = ("127.0.0.1", 80)
    httpd = server_class (server_address, handler_class)

    httpd.serve_forever ()


if __name__ == "__main__":
    run ()