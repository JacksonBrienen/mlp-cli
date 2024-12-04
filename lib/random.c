// PCG algorithm - derived from https://github.com/imneme/pcg-c-basic
#include <mlp/random.h>

typedef struct {
    uint64_t state;
    uint64_t seq;
} pcg_state64;

pcg_state64 pcg_state = {0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL};

uint32_t random_u32() {
    uint64_t oldstate = pcg_state.state;
    pcg_state.state = oldstate * 6364136223846793005ULL + pcg_state.seq;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

void srandf(uint64_t seed) {
    pcg_state = (pcg_state64){0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL};
    pcg_state.seq = seed;
    uint32_t h = random_u32();
    uint32_t l = random_u32();
    pcg_state.seq = h;
    pcg_state.seq <<= 32;
    pcg_state.seq += l;
    pcg_state.seq = (pcg_state.seq << 1u) | 1u;
}

float randf() {
    return ((float)random_u32() / UINT32_MAX);
}