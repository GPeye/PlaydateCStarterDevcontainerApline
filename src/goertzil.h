#ifndef _goertzel_h_
#define _goertzel_h_

#include <stdint.h>
#include <stdlib.h>

// Determined experimentally
#define MIN_POWER 100

float goertzel_mag(int numSamples, float targetFrequency, float sampleRate, const int16_t* data);
int goertzel_value(int numSamples, float sampleRate, float targetFrequency1, float targetFrequency2, const int16_t* data);
float calculate_rms_power(const int16_t* samples, size_t numSamples);

#endif