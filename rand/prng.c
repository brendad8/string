/* 
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 */

#include "../basic.h"
#include <math.h>
#include <stdio.h>

typedef struct {
    u64 state;         
    u64 inc;
    f32 prev_norm;
} prng_state;

void prng_seed(u64 initstate, u64 initseq);
void prng_seed_r(prng_state* rng, u64 initstate,
                 u64 initseq);

u32 prng_rand(void);
u32 prng_rand_r(prng_state* rng);

f32 prng_runif(void);
f32 prng_runif_r(prng_state* rng);

f32 prng_rnorm(f32 mu, f32 sigma);
f32 prng_rnorm_r(prng_state* rng, f32 mu, f32 sigma);

f32 prng_rexp(f32 lambda);
f32 prng_rexp_r(prng_state* rng, f32 lambda);

u32 prng_rpois(f32 mu);
u32 prng_rpois_r(prng_state* rng, f32 mu);

u32 prng_rbern(f32 p);
u32 prng_rbern_r(prng_state* rng, f32 p);

u32 prng_rbinom_r(prng_state* rng, u32 n, f32 p);
u32 prng_rbinom(u32 n, f32 p);


u32 prng_boundedrand(u32 bound);
u32 prng_boundedrand_r(prng_state* rng, u32 bound);

static prng_state s_prng_state = {
    0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL, NAN
};

int main(void)
{

    prng_seed(8, 22);
    for (int i = 0; i < 10; i++)
        printf("%u, %d, %f\n", prng_rbinom(10, 0.5), prng_rpois(5), prng_rexp(3));

    return 0;
}

void prng_seed_r(prng_state* rng, u64 initstate, u64 initseq)
{
    rng->state = 0U;
    rng->inc = (initseq << 1u) | 1u;
    prng_rand_r(rng);
    rng->state += initstate;
    prng_rand_r(rng);
}

void prng_seed(uint64_t seed, uint64_t seq)
{
    prng_seed_r(&s_prng_state, seed, seq);
}

u32 prng_rand_r(prng_state* rng)
{
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

u32 prng_rand(void)
{
    return prng_rand_r(&s_prng_state);
}

f32 prng_runif_r(prng_state* rng)
{
    return prng_rand_r(rng) / (f32)UINT32_MAX;
}

f32 prng_runif(void)
{
    return prng_runif_r(&s_prng_state);
}

f32 prng_rnorm_r(prng_state* rng, f32 mu, f32 sigma)
{
    f32 u1 = 1.0f, u2 = 0.0f;

    if (!isnan(rng->prev_norm))
    {
        f32 out = rng->prev_norm;
        rng->prev_norm = NAN;
        return out;
    }

    do {
        u1 = prng_runif_r(rng);
    } while (u1 == 0.0f);

    u2 = prng_runif_r(rng);
    f32 mag = sigma * sqrt(-2.0f * log(u1));
    f32 z0 = mag * cos(2 * M_PI * u2) + mu;
    f32 z1 = mag * sin(2 * M_PI * u2) + mu;

    rng->prev_norm = z1;
    return z0;
}

f32 prng_rnorm(f32 mu, f32 sigma)
{
    return prng_rnorm_r(&s_prng_state, mu, sigma);
}

f32 prng_rexp_r(prng_state* rng, f32 lambda)
{
    return (-1 / lambda) * log(prng_runif_r(rng));
}

f32 prng_rexp(f32 lambda)
{
    return prng_rexp_r(&s_prng_state, lambda);
}

u32 prng_rpois_r(prng_state* rng, f32 mu)
{
    u32 k = 0;
    f32 t = 0.0f;
    while (t < mu)
    {
        t += prng_rexp_r(&s_prng_state, 1);
        k += 1;
    }
    return k;
}

u32 prng_rpois(f32 mu)
{
    return prng_rpois_r(&s_prng_state, mu);
}


u32 prng_rbern_r(prng_state* rng, f32 p)
{
    return prng_runif_r(&s_prng_state) < p;
}

u32 prng_rbern(f32 p)
{
    return prng_rbern_r(&s_prng_state, p);
}


u32 prng_rbinom_r(prng_state* rng, u32 n, f32 p)
{
    u32 x = 0;
    for (int i = 0; i < n; i++)
        x += prng_rbern_r(rng, p);

    return x;
}

u32 prng_rbinom(u32 n, f32 p)
{
    return prng_rbinom_r(&s_prng_state, n, p);
}

// u32 prng_boundedrand_r(prng_state* rng, uint32_t bound)
// {
//     uint32_t threshold = -bound % bound;
//     for (;;) {
//         uint32_t r = prng_rand_r(rng);
//         if (r >= threshold)
//             return r % bound;
//     }
// }
//
// u32 prng_boundedrand(uint32_t bound)
// {
//     return prng_boundedrand_r(&s_prng_state, bound);
// }
