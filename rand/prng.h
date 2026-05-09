
#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t u32;
typedef uint64_t u64;
typedef float    f32;

void prng_seed(uint64_t initstate, uint64_t initseq);
u32  prng_rand(void); 

//************************
// Stats Distributions
//************************
f32  prng_runif(void);              // ~ Uniform(0, 1)
f32  prng_rnorm(f32 mu, f32 sigma); // ~ Normal(mu, sigma)
f32  prng_rexp(f32 lambda);         // ~ Exponential(lambda) (mu = 1/lambda)
u32  prng_rpois(f32 mu);            // ~ Poisson(mu) (sd = mu)
bool prng_rbern(f32 p);             // ~ Bernoulli(p)
u32  prng_rbinom(u32 n, f32 p);     // ~ Binomial(n, p)


#endif // PRNG_H
