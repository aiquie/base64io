#ifndef BASE46_H
#define BASE46_H

struct base64_state {
    char counts;
    unsigned char buffer[4];
};

// dst length = srclen * 4 / 3;
int base64_encode_chunk(char* dst, char const* src, int srclen, struct base64_state* state);
// dst length = 4 + 1;
int base64_encode_finish(char* dst, struct base64_state* state);

// dst length = srclen * 3 / 4;
int base64_decode_chunk(char* dst, char const* src, int srclen, struct base64_state* state);
// dst length = 0;
int base64_decode_finish(char* dst, struct base64_state* state);

struct base64_conv {
    int(*chunk )(char*, char const*, int, struct base64_state*);
    int(*finish)(char*,                   struct base64_state*);
};

extern struct base64_conv const base64encode;
extern struct base64_conv const base64decode;

#endif //BASE46_H
