Base64Io
======
Base64 encode or decode from standard input to standard output.

Build
-----
    cmake . && cmake --build .

Usage
-----
    (echo A; sleep 1; echo B) | base64io | base64io -d

Options
-------
    -d       decode data
    -b SIZE  encode buffer size
    -w SIZE  encode wrap size
    -t TIME  encode reading time
