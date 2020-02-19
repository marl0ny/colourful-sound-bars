#include <SDL2/SDL.h>

#ifndef _SOUND_VIEW_H
#define _SOUND_VIEW_H


#define FFT_SIZE 2048

/*
* Struct that controls the
* playing of and visualization
* of the audio.
*
* wav: pointer to the wav struct.
* start: pointer to the start of the sound data.
* data: pointer to the current position
* in the sound data.
* length: the length remaining in the sound data.
* position: the current position in the sound data,
* relative to number of samples in the data.
* is_paused: whether playback is paused or not.
* is_paused_cooldown: a timer that controls when
* the user can pause or un-pause the audio playback.
*/
struct AudioControl {
    struct Wav *wav;
    Uint8 *start;
    Uint8 *data;
    int length;
    int position;
    int is_paused;
    int is_paused_cooldown;
};

/*
* RGB colour encoded with the
* integer values 0 - 255.
* The members of this struct are
* red, green, and blue.
*/
struct Colour {
    int red;
    int green;
    int blue;
};

/*
* The audio callback function that is used by
* SDL to play the audio.
*
* user_data: this is casted to a pointer to the AudioControl struct.
* stream: the audio stream controlled by SDL.
* delta: the amount of sound data to play in the given instance.
*/
void audio_callback(void *user_data, Uint8 *stream, int delta);

#ifndef _NUMBER_TYPEDEFS
#define _NUMBER_TYPEDEFS

#include <complex.h>

typedef double __complex__ cmplx;
typedef double real;

#endif

/*
* Convert the argument of a complex number into a
* colour hue.
*
* col: the colour struct.
* number: the complex number.
*
* ---------------------------------------------------------
* References:
*
* Wikipedia contributors. (2020). Hue. 
* In Wikipedia, The Free Encyclopedia.
* https://en.wikipedia.org/wiki/Hue
*
*/
void complex_to_colour(struct Colour *col, cmplx number);

#endif