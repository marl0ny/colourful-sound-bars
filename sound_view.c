#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "wave.h"
#include "sound_view.h"
#include "fft.h"

#define GETTIME(t1, t2) \
  ((double)(t2 - t1))/(double)(CLOCKS_PER_SEC)


int main(int argc, char **argv) {
    char *audio_file;
    char *audio_filename;
    if (argc == 2) {
        audio_file = argv[1];
        audio_filename = calloc(1, strlen(argv[1]) + 1);
        copy_filename(audio_filename, argv[1]);
    } else {
        fprintf(stderr, "Usage: ./sound_view /path/to/file.wav\n");
        exit(1);
    }
    struct Wav *wav = get_wav(audio_file);
    if (wav == NULL) {
        free(audio_filename);
        perror("");
        exit(1);
    }
    print_wav_info((struct WavInfo *)wav);
    int sample_size = wav->header.sample_rate/50;
    int width = 1280;
    int height = 720;
    SDL_Window* window = NULL;
    real freqs[FFT_SIZE] = {0.0};
    frequencies(freqs, FFT_SIZE);
    cmplx z[FFT_SIZE] = {0.0};

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        free(audio_filename);
        fprintf(stderr, 
                "Unable to initialize SDL2 audio: %s\n", SDL_GetError());
        exit(1);
    }
    struct AudioControl audio;
    audio.wav = wav;
    audio.start = (Uint8 *)wav->data.i8;
    audio.data = (Uint8 *)wav->data.i8;
    audio.position = 0;
    audio.length = wav->header.chunk_size - 36;
    audio.is_paused = 0;
    audio.is_paused_cooldown = 0;
    SDL_AudioSpec spec;
    spec.userdata = (void *)&audio;
    spec.callback = audio_callback;
    spec.channels = wav->header.num_channels;
    spec.freq = wav->header.sample_rate;
    spec.size = wav->header.sample_rate;

    /*
    * Open the audio device
    * 
    * A very good and succinct example to play audio files
    * without using SDL_Mixer is given here:
    * 
    * Play a sound with SDL2 (no SDL_Mixer)
    * https://gist.github.com/armornick/3447121
    * by GitHub User armornick
    * https://github.com/armornick
    */
	if ( SDL_OpenAudio(&spec, NULL) < 0 ){
        free(audio_filename);
        fprintf(stderr, "Can't open audio: %s\n", SDL_GetError());
        exit(-1);
	}
    SDL_PauseAudio(0);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        free(audio_filename);
        fprintf(stderr, "Unable to initialize video: %s\n", SDL_GetError());
        exit(1);
    } 
    window = SDL_CreateWindow(audio_filename,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height,
                              SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        free(audio_filename);
        fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 
                                                SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        free(audio_filename);
        fprintf(stderr, "Unable to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
    const Uint8 *keyboard_state;
    int returned = 0;
    struct Colour phase_colour;
    double t1 = 0.0;
    double t2 = 0.0;
    double delta_t = 0.0;
    int bar_width = 4;
    int bar_width_cooldown = 0;
    real bar_height_scale = 1.0;
    int bar_height_scale_cooldown = 0;
    SDL_Rect background_rect = {.x = 0, .y = 0, .w = width, .h = height,};
    do {
        t1 = clock();
        SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
        SDL_GetWindowSize(window, &width, &height);
        background_rect.w = width;
        background_rect.h = height;
        SDL_RenderFillRect(renderer, &background_rect);
        SDL_PumpEvents();
        keyboard_state = SDL_GetKeyboardState(NULL);
        if (keyboard_state == NULL) {
            fprintf(stderr, "Unable to get Keyboard state: %s\n", 
                    SDL_GetError());
        }
        if (keyboard_state[SDL_SCANCODE_RETURN] || SDL_QuitRequested()) {
            returned = 1;
        }
        audio.is_paused_cooldown += (audio.is_paused_cooldown <= 0)? 0: -1; 
        if (keyboard_state[SDL_SCANCODE_SPACE] && 
            audio.is_paused_cooldown <= 0) {
            if (!audio.is_paused) {
                SDL_PauseAudio(1);
            } else {
                SDL_PauseAudio(0);
            }
            audio.is_paused = !audio.is_paused;
            audio.is_paused_cooldown = 7;
        }
        bar_width_cooldown += (bar_width_cooldown <= 0)? 0: -1;
        if (keyboard_state[SDL_SCANCODE_LEFT] && 
            bar_width_cooldown <= 0) {
            bar_width -= (bar_width  > 1)? 1: 0;
            bar_width_cooldown = 7;
        }
        if (keyboard_state[SDL_SCANCODE_RIGHT] &&
            bar_width_cooldown <= 0) {
            bar_width += 1;
            bar_width_cooldown = 7;
        }
        bar_height_scale_cooldown += (
            bar_height_scale_cooldown <= 0)? 0: -1;
        if (keyboard_state[SDL_SCANCODE_DOWN] &&
            bar_height_scale_cooldown <= 0) {
            if (bar_height_scale <= 0.0) {
                bar_height_scale = 0.0;
            } else {
                bar_height_scale -= 0.1;
            }
            bar_height_scale_cooldown = 7;
        }
        if (keyboard_state[SDL_SCANCODE_UP] &&
            bar_height_scale_cooldown <= 0) {
            bar_height_scale += 0.1;
            bar_height_scale_cooldown = 7;
        }
        if ((audio.position + sample_size) < 
            (wav->header.chunk_size - 36)/(wav->header.bits_per_sample/8)) {
            for (int i = 0; i < FFT_SIZE; i++) {
                if (i < FFT_SIZE/2) {
                    z[i] = wav_get_point(wav, audio.position+i)/(5*(double)
                                                                 FFT_SIZE);
                } else {
                    z[i] = wav_get_point(wav, audio.position+i)/(5*(double)
                                                                 FFT_SIZE);
                    // z[i] = 0.0;
                }
            }
            inplace_fft(z, FFT_SIZE);
            z[0] = 0.0;
            SDL_SetRenderDrawColor(renderer, 200, 200, 255, 200);
            if ((audio.position + width) < 
                (wav->header.chunk_size - 36)/(wav->header.bits_per_sample/8)){
                for (int i = 1; i < width; i++) {
                    SDL_RenderDrawLine(renderer, i-1 , 
                                       height/4 + (int)wav_get_point(
                                       wav, audio.position+i-1)/300, 
                                       i, height/4 + (int)wav_get_point(
                                       wav, audio.position+i)/200); 
                }
            }
            for (int i = 0; i < width/bar_width; i++) {

                complex_to_colour(&phase_colour, z[i]);
                SDL_SetRenderDrawColor(renderer, 
                                       phase_colour.red, 
                                       phase_colour.green,
                                       phase_colour.blue,
                                       255);
                for (int j = 0; 
                     j < bar_width && (j + i) < bar_width*FFT_SIZE/2; j++) {
                    SDL_RenderDrawLine(renderer, bar_width*(int)freqs[i] + j, 
                                       (int)(height 
                                       - bar_height_scale*cabs(z[i])), 
                                       bar_width*(int)freqs[i] + j, height);
                }
            }
        } else {
            audio.data = audio.start;
            audio.length = wav->header.chunk_size - 36;
            audio.position = 0;
        }
        SDL_RenderPresent(renderer);
        t2 = clock();
        delta_t = GETTIME(t1, t2);
        SDL_Delay(abs(20 - (int)roundf(delta_t*1000.0)));
    } while(!returned);
    SDL_CloseAudio();
    free(audio_filename);
    free(wav->data.i8);
    free(wav);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}


void audio_callback(void *user_data, Uint8 *stream, int delta) {
    struct AudioControl *audio = (struct AudioControl *)user_data;
    int bytes_per_sample = ((int)(audio->wav->header.bits_per_sample))/8;
    if (audio == NULL || stream == NULL) {
        fprintf(stderr, "Audio is null.");
        exit(1);
    }
    SDL_memcpy(stream, audio->data, delta);
    audio->data += delta;
    audio->position += delta/bytes_per_sample;
    audio->length -= delta;
}


void complex_to_colour(struct Colour *col, cmplx number) {
    real pi = 3.141592653589793;
    real arg_val = cargf(number);
    int max_col = 255;
    int min_col = 50;
    int col_range = max_col - min_col;
    if (arg_val <= pi/3 && arg_val >= 0) {
        col->red = max_col;
        col->green = min_col + (int)(col_range*arg_val/(pi/3));
        col->blue = min_col;
    } else if (arg_val > pi/3 && arg_val <= 2*pi/3){
        col->red = max_col - (int)(col_range*(arg_val - pi/3)/(pi/3));
        col->green = max_col;
        col->blue = min_col;
    } else if (arg_val > 2*pi/3 && arg_val <= pi){
        col->red = min_col;
        col->green = max_col;
        col->blue = min_col + (int)(col_range*(arg_val - 2*pi/3)/(pi/3));
    } else if (arg_val < 0.0 && arg_val > -pi/3){
        col->red = max_col;
        col->blue = min_col - (int)(col_range*arg_val/(pi/3));
        col->green = min_col;
    } else if (arg_val <= -pi/3 && arg_val > -2*pi/3){
        col->red = max_col + (int)(col_range*(arg_val + pi/3)/(pi/3));
        col->blue = max_col;
        col->green = min_col;
    } else if (arg_val <= -2*pi/3 && arg_val >= -pi){
        col->red = min_col;
        col->blue = max_col;
        col->green = min_col - (int)(col_range*(arg_val + 2*pi/3)/(pi/3));
    }
    else {
        col->red = min_col;
        col->green = max_col;
        col->blue = max_col;
    }
}
