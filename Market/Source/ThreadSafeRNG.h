/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/22/21
*
* This is a thread safe random number generator tbz533 created
*/
#pragma once
#include "PCH.h"

// Thread Safe RNG
std::mt19937_64 my_rng; // Defines an engine
std::uniform_real_distribution<double> my_unif_real_dist(0.0, 1.0); // Define distribution
std::mutex my_rng_mutex; // a mutex to guard my_rng


// This is the function to call if you want a random number in the interval [0,1)
float Rand_ThreadSafeRNG() {
    std::lock_guard<std::mutex> lock(my_rng_mutex);
    return my_unif_real_dist(my_rng);
    // mutex is released when lock goes out of scope
}


/* Function to seed the random number generator from main file
 * This function again locks the mutex for when a thread decides
 * to re-seed the RNG.
 */
void Seed_ThreadSafeRNG(uint16_t seed) {
    std::lock_guard<std::mutex> lock(my_rng_mutex);
    my_rng.seed(seed);
}