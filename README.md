# Colourful Sound Bars

Just another sound visualizer that takes the DFT of sound samples. The visualizer produces a lot of rapidly changing colours and lights, so please do not use this program or look at the example below if you are sensitive to those type of things.

## Prerequisites
This project uses the [SDL2](https://www.libsdl.org/) library, which you can install [here](https://www.libsdl.org/download-2.0.php). You must, of course, also have a C compiler, and Make.

## Installation
 - Download or clone this repository
 - Open a terminal in the directory location of this file, and type `make`.

## Usage
Currently this program only supports wav files. To play a wav file, type:
```
./sound_view /path/to/sound-file.wav
```

<img src="https://raw.githubusercontent.com/marl0ny/colourful-sound-bars/master/musical-colour-bars.gif" />

The pulsating wave near the top of the window corresponds sound wave data in time. The colour bars at the bottom correspond to the DFT of the sound data. The height and colour of the colour bars are determined by the magnitude and complex argument of the corresponding DFT amplitudes. Only the positive frequency terms are shown, in ascending order from lower to higher frequency. 

### Keyboard Controls
- <b>Space bar</b> - pause playback
- <b>up</b> or <b>down keys</b> - increase or decrease height of colour bars
- <b>left</b> or <b>right keys</b> - increase or decrease width of 
colour bars 

## References:
FFT algorithm:
- [Wikipedia - Cooley–Tukey FFT algorithm](https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm)
- [Numerical Recipes Chapter 12](https://websites.pmc.ucsc.edu/~fnimmo/eart290c_17/NumericalRecipesinF77.pdf)
- [Wolfram MathWorld - Fast Fourier Transform](http://mathworld.wolfram.com/FastFourierTransform.html)

Info on how to read a .wav file header:
- [WAVE PCM soundfile format ](http://hummer.stanford.edu/sig/doc/classes/SoundHeader/WaveFormat/)

Playing audio in SDL:
- [Play a sound with SDL2 (no SDL_Mixer) ](https://gist.github.com/armornick/3447121)

Colour hue to RGB values:
- [Wikipedia - Hue](https://en.wikipedia.org/wiki/Hue)
- [HSV-RGB-comparison](https://en.wikipedia.org/wiki/File:HSV-RGB-comparison.svg)
