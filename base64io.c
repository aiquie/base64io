#include "base64.h"
#include "unifile.h"
#include "convbuf.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * const argv[]) {
    int decode   = 0;
    int buffsize = 0;
    int wrapsize = 0;
    double readtime = -1;

    int opt;
    while((opt = getopt(argc, argv, "db:w:t:h")) != -1) {
        switch (opt) {
        case 'd': decode = 1;              break;
        case 'b': buffsize = atoi(optarg); break;
        case 'w': wrapsize = atoi(optarg); break;
        case 't': readtime = atof(optarg); break;
        case 'h':
        default:
            fprintf(stderr, "Usage: %s [OPTIONS]\n", argv[0]);
            fprintf(stderr, "Base64 encode or decode from standard input to standard output.\n");
            fprintf(stderr, "\n");
            fprintf(stderr, "    -d       decode data\n");
            fprintf(stderr, "    -b SIZE  encode buffer size\n");
            fprintf(stderr, "    -w SIZE  encode wrap size\n");
            fprintf(stderr, "    -t TIME  encode reading time\n");
            return EXIT_FAILURE;
        }
    }

    if(decode && (buffsize || wrapsize || readtime != -1)) {
        fprintf(stderr, "Error: encode options are not compatible with decode data.\n");
        return EXIT_FAILURE;
    }

    buffsize = buffsize > 0 ? buffsize : INT_MAX;
    wrapsize = wrapsize > 0 ? wrapsize : INT_MAX;

    struct convbuf     const srcbuf     = decode ? encbuf : decbuf;
    struct convbuf     const dstbuf     = decode ? decbuf : encbuf;
    struct base64_conv const base64conv = decode ? base64decode : base64encode;

    struct timespec curtime = {0};
    struct base64_state base64state = {0};

    if(set_nonblock(STDIN_FILENO) == -1) return errno;

    double timeout = -1;
    for(;;) {
        is_ready(STDIN_FILENO, timeout);

        int readsize  = 0;
        int writesize = 0;
        int initsize = -1;
        while(initsize != readsize) {
            initsize = readsize;
            readsize = wrap_read(STDIN_FILENO, srcbuf.p, srcbuf.sz, readsize, buffsize);
            if(readsize == -1) return errno;
            int chunksize = base64conv.chunk(dstbuf.p, srcbuf.p, readsize - initsize, &base64state);
            writesize = wrap_write(STDOUT_FILENO, dstbuf.p, chunksize, writesize, wrapsize);
            if(writesize == -1) return errno;
        }

        if(timeout != -1) timeout = -1;
        else if(readsize) timeout = readtime;
        else break;

        if(timeout < 0) {
            int chunksize = base64conv.finish(dstbuf.p, &base64state);
            writesize = wrap_write(STDOUT_FILENO, dstbuf.p, chunksize, writesize, wrapsize);
            if(!decode && writesize > 0) writesize = write_endl(STDOUT_FILENO);
            if(writesize == -1) return errno;
        }
    }

    return EXIT_SUCCESS;
}
