# include <stdio.h>
# include <stdlib.h>


# ifndef _WAVE_H_
# define _WAVE_H_

/*
* Information about the wav file.
* This is retrieved from its header.
*
* riff: contains the letters RIFF
* wave: contains the letters WAVES
* fmt: contains the letters FMT
* followed by a space.
* chunk_size: the size of the actual sound
* data minus 36.
* format: The format of the wav file
* i.e. 1 for mono and 2 for stereo.
* num_channels: The number of channels.
* sample_rate: The sample rate, in sound
* samples per second.
* bits_per_sample: The number of bits
* allocated to each sample.
* subchunk2_size: The size of each sub chunk.
* ---------------------------------------------
*
* References:
* Stuart, Craig. (2001). WAVE PCM soundfile format. Web.
* http://hummer.stanford.edu/sig/doc/classes/SoundHeader/WaveFormat/
*
*
*/
struct WavInfo {
    char riff[5];
    char wave[5];
    char fmt[5]; 
    int chunk_size;
    signed short format;
    signed short num_channels;
    int sample_rate;
    signed short bits_per_sample;
    int subchunk2_size;
};

/*
* Wav struct that contains
* the sound data of the wav file.
*
* header: information from
* the wav file header.
* data: this contains the actual
* sound data.
*/
struct Wav {
    struct WavInfo header;
    // i8: 8 bit signed
    // i16: 16 bit signed
    // i32: 32 bit signed
    union data {
        signed char *i8;
        signed short *i16; 
        int *i32;
    }data;
};

/*
* Extract info about a wav file from the wav file itself.
*
* file: the .wav file.
* wav_info: the wav info struct that contains the information
* from the wav header.
*/
void wav_info_from_file(struct WavInfo *wav_info, FILE *file);

/*
* Get the sound data from the wav file itself.
*
* filename: the name of the filename.
*
* returns the wav struct which contains the sound data.
*/
struct Wav *get_wav(const char *filename);

#ifndef _NUMBER_TYPEDEFS
#define _NUMBER_TYPEDEFS

#include <complex.h>

typedef double __complex__ cmplx;
typedef double real;

#endif

/*
* Get a data point from the sound data.
*
* wav: the wav struct for the sound data.
* index: the index of the data point.
*
* returns the sound data at the input index as
* a floating point value.
*/
real wav_get_point(const struct Wav *wav, size_t index);

/*
* Print information about the wav sound data.
*
* wav_info: the wav info struct containing information
* about the wav file.
*/
void print_wav_info(const struct WavInfo *wav_info);

/*
* Copy a filename to a string buffer, given
* the entire path.
*
* filename: the name of a file.
* path: the entire path to a file.
*/
void copy_filename(char *filename, const char *path);

#endif
