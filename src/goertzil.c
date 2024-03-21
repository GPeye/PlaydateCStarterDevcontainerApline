#include <math.h>
#include <stdint.h>

#include "constants.h"
#include "goertzel.h"

// Function to run the Goertzel algorithm on a set of samples for a specific frequency.
// Returns the magnitude squared of the targeted frequency component.
float goertzel_mag(int numSamples, float targetFrequency, float sampleRate, const int16_t* data)
{
	int k;
	float omega, sine, cosine, coeff, q0, q1, q2, real, imag, magnitudeSquared;

	// Calculate the Goertzel parameters
	k = (int)(0.5f + ((numSamples * targetFrequency) / sampleRate));
	omega = (2.0f * M_PI * k) / (float)numSamples;
	sine = sinf(omega);
	cosine = cosf(omega);
	coeff = 2.0f * cosine;

	q0 = 0;
	q1 = 0;
	q2 = 0;

	// Process the samples
	for (int i = 0; i < numSamples; i++)
	{
		q0 = coeff * q1 - q2 + (float)data[i];
		q2 = q1;
		q1 = q0;
	}

	// Perform the DFT calculation to get the complex output
	real = q1 - q2 * cosine;
	imag = q2 * sine;

	// Calculate the magnitude squared
	magnitudeSquared = real * real + imag * imag;

	return magnitudeSquared;
}

int goertzel_value(int numSamples, float sampleRate, float targetFrequency1, float targetFrequency2, const int16_t* data)
{
	// Bail out if we have mostly silence
	float power = calculate_rms_power(data, numSamples);
	if (power < MIN_POWER) {
		return -1;
	}
	
	// Run the Goertzel algorithm for each target frequency
	float magnitudeSquared1 = goertzel_mag(numSamples, targetFrequency1, sampleRate, data);
	float magnitudeSquared2 = goertzel_mag(numSamples, targetFrequency2, sampleRate, data);

	// Compare and determine which frequency is present
	if (magnitudeSquared1 > magnitudeSquared2) {
		return 0;
	} else  {
		return 1;
	}
}

float calculate_rms_power(const int16_t* samples, size_t numSamples) {
	float sumOfSquares = 0.0;

	// Sum the squares of the individual samples
	for (size_t i = 0; i < numSamples; ++i) {
		float sampleValue = (float)samples[i];
		sumOfSquares += sampleValue * sampleValue;
	}

	// Calculate the mean of the squares
	float meanOfSquares = sumOfSquares / (float)numSamples;

	// Return the square root of the mean of the squares (RMS)
	return (float)sqrt(meanOfSquares);
}