#ifndef CONVBUF_H
#define CONVBUF_H

struct convbuf {
    char * p;
    int   sz;
};

extern struct convbuf const encbuf;
extern struct convbuf const decbuf;

#endif // CONVBUF_H

