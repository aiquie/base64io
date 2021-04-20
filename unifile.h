#ifndef UNIFILE_H
#define UNIFILE_H

#include <unistd.h>

int set_nonblock(int fd);
int is_ready(int fd, double tm);
int get_ready(int * fds, int fdc);

ssize_t safe_read( int fd, void       * buff, size_t size);
ssize_t safe_write(int fd, const void * buff, size_t size);

ssize_t wrap_read( int fd, void       * buff, size_t buffsize, size_t readsize,  size_t wrapsize);
ssize_t wrap_write(int fd, const void * buff, size_t buffsize, size_t writesize, size_t wrapsize);

int write_endl(int fd);

#endif //UNIFILE_H

