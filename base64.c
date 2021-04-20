#include "base64.h"

char base46_map[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                     'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
                     '='}; /* termination character */

int base64_encode_chunk(char* dst, char const* src, int srclen, struct base64_state* state) {
    int res = 0;
    int i   = 0;
    for(; i < srclen; i++) {
        state->buffer[state->counts++] = src[i];
        if(state->counts == 3) {
            *dst++ = base46_map[state->buffer[0] >> 2];
            *dst++ = base46_map[((state->buffer[0] & 0x03) << 4) + (state->buffer[1] >> 4)];
            *dst++ = base46_map[((state->buffer[1] & 0x0f) << 2) + (state->buffer[2] >> 6)];
            *dst++ = base46_map[state->buffer[2] & 0x3f];
            state->counts = 0;
            res += 4;
        }
    }
    return res;
}

int base64_encode_finish(char* dst, struct base64_state* state) {
    int res = 0;
    if(state->counts > 0) {
        *dst++ = base46_map[state->buffer[0] >> 2];
        if(state->counts == 1) {
            *dst++ = base46_map[(state->buffer[0] & 0x03) << 4];
            *dst++ = '=';
        } else { // state->counts == 2
            *dst++ = base46_map[((state->buffer[0] & 0x03) << 4) + (state->buffer[1] >> 4)];
            *dst++ = base46_map[ (state->buffer[1] & 0x0f) << 2];
        }
        *dst++ = '=';
        state->counts = 0;
        res += 4;
    }
    return res;
}

int base64_decode_chunk(char* dst, char const* src, int srclen, struct base64_state* state) {
    int res = 0;
    int i   = 0;
    for(; i < srclen; i++) {
        char k;
        for(k = 0 ; k < sizeof(base46_map) && base46_map[k] != src[i]; k++);
        if(k == sizeof(base46_map)) continue;
        state->buffer[state->counts++] = k;
        if(state->counts == 4) {
            {
                *dst++ = (state->buffer[0] << 2) + (state->buffer[1] >> 4);
                res++;
            }
            if(state->buffer[2] != 64) {
                *dst++ = (state->buffer[1] << 4) + (state->buffer[2] >> 2);
                res++;
            }
            if(state->buffer[3] != 64) {
                *dst++ = (state->buffer[2] << 6) +  state->buffer[3];
                res++;
            }
            state->counts = 0;
        }
    }
    return res;
}

int base64_decode_finish(char* dst, struct base64_state* state) {
    return 0;
}

struct base64_conv const base64encode = { base64_encode_chunk, base64_encode_finish };
struct base64_conv const base64decode = { base64_decode_chunk, base64_decode_finish };
