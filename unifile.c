#include "unifile.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

int set_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL);
    return flags != -1 ? fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) : -1;
}

int is_ready(int fd, double tm) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    struct timeval tv, *ptv = NULL;
    if(tm >= 0) {
        tv.tv_sec = tm;
        tv.tv_usec = (tm - tv.tv_sec) * 1000000;
        ptv = &tv;
    }

    if(select(fd + 1, &fds, NULL, NULL, ptv) == -1) return -1;
    return FD_ISSET(fd, &fds) ? 1 : 0;
}

int get_ready(int * fds, int fdc) {
    fd_set rfds;
    FD_ZERO(&rfds);
    int i = 0;

    int maxfd = -1;
    for(i = 0; i < fdc; ++i) {
        if(fds[i] != -1) {
            FD_SET(fds[i], &rfds);
            maxfd = fds[i] > maxfd ? fds[i] : maxfd;
        }
    }

    if(maxfd == -1) return -1;
    if(select(maxfd + 1, &rfds, NULL, NULL, NULL) == -1) return -1;
    for(i = 0; i < fdc; ++i) {
        if(fds[i] != -1 && FD_ISSET(fds[i], &rfds)) return fds[i];
    }
    return -1;
}

ssize_t safe_read(int fd, void * buff, size_t size) {
    ssize_t res = 0;
    while(size) {
        ssize_t ret = read(fd, buff, size);
        if(ret == -1 && errno == EAGAIN) ret = 0;
        if(ret == -1) return -1;
        if(ret ==  0) break;
        res  += ret;
        buff += ret;
        size -= ret;
    }
    return res;
}

ssize_t safe_write(int fd, const void * buff, size_t size) {
    ssize_t res = size;
    while(size) {
        ssize_t ret = write(fd, buff, size);
        if(ret == -1 && errno == EAGAIN) ret = 0;
        if(ret == -1) return -1;
        buff += ret;
        size -= ret;
    }
    return res;
}

ssize_t wrap_read( int fd, void * buff, size_t buffsize, size_t readsize, size_t wrapsize) {
    if(buffsize && readsize != -1) {
        size_t leftsize = wrapsize - readsize;
        ssize_t res = safe_read(fd, buff, buffsize < leftsize ? buffsize : leftsize);   
        if(res == -1) return -1;
        readsize += res;
    }
    return readsize;
}


ssize_t wrap_write(int fd, const void * buff, size_t buffsize, size_t writesize, size_t wrapsize) {
    while(buffsize && writesize != -1) {
        size_t leftsize = wrapsize - writesize;
        ssize_t res = safe_write(fd, buff, buffsize < leftsize ? buffsize : leftsize);
        if(res == -1) return -1;
        buff      += res;
        buffsize  -= res;
        writesize += res;
        if(writesize == wrapsize) writesize = write_endl(fd);
    }
    return writesize;
}

int write_endl(int fd) {
    return safe_write(fd, "\n", 1) != -1 ? 0 : -1;    
}
