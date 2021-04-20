#include "convbuf.h"

#define ENC_BUF_SIZE 4096
#define DEC_BUF_SIZE 3072

char ENC_BUF[ENC_BUF_SIZE];
char DEC_BUF[DEC_BUF_SIZE];

struct convbuf const encbuf = { ENC_BUF, ENC_BUF_SIZE };
struct convbuf const decbuf = { DEC_BUF, DEC_BUF_SIZE };
