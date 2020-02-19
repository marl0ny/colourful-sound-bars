# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "wave.h"
# include "fft.h"


#define CHECK_FREAD(fread_call, val) \
do {\
    if(fread_call != (val) ) { \
        fscanf(stderr, "Unable to properly read file.\n");\
        return; \
    }\
}while(0)

#define CHECK_FSEEK(fseek_call) \
do {\
    if (fseek_call != 0) {\
        fscanf(stderr, "Unable to seek file position.\n");\
        return;\
    }\
}while(0)

void wav_info_from_file(struct WavInfo *wav_info, FILE *file) {
    CHECK_FREAD(fread(&wav_info->riff, 1, 4, file), 4);
    wav_info->riff[4] = '\0';
    CHECK_FSEEK(fseek(file, 4, SEEK_SET));
    CHECK_FREAD(fread(&wav_info->chunk_size, 1, 4, file), 4);
    CHECK_FSEEK(fseek(file, 8, SEEK_SET));
    CHECK_FREAD(fread(&wav_info->wave, 1, 4, file), 4);
    wav_info->wave[4] = '\0';
    CHECK_FSEEK(fseek(file, 12, SEEK_SET));
    CHECK_FREAD(fread(&wav_info->fmt, 1, 4, file), 4);
    wav_info->fmt[4] = '\0';
    CHECK_FSEEK(fseek(file, 20, SEEK_SET));
    CHECK_FREAD(fread(&wav_info->format, 1, 2, file), 2);
    CHECK_FSEEK(fseek(file, 22, SEEK_SET));
    CHECK_FREAD(fread(&wav_info->num_channels, 1, 2, file), 2);
    CHECK_FSEEK(fseek(file, 24, SEEK_SET));
    CHECK_FREAD(fread(&wav_info->sample_rate, 1, 4, file), 4);
    CHECK_FSEEK(fseek(file, 34, SEEK_SET));
    CHECK_FREAD(fread(&wav_info->bits_per_sample, 1, 2, file), 2);
    CHECK_FSEEK(fseek(file, 40, SEEK_SET));
    CHECK_FREAD(fread(&wav_info->subchunk2_size, 1, 4, file), 4);
}

#undef CHECK_FSEEK
#undef CHECK_FREAD


real wav_get_point(const struct Wav *wav, size_t index) {
    if (index < (wav->header.chunk_size - 36)) {
        switch(wav->header.bits_per_sample) {
            case 8:
            return (real)wav->data.i8[index];
            case 16:
            return (real)wav->data.i16[index];
            case 32:
            return (real)wav->data.i32[index];
        }
    } else {
        fprintf(stderr, "Invalid index: %d\n", (int)index);
        return 0.0;
    }
    return 0.0;
}


struct Wav *get_wav(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file %s: ", filename);
        return NULL;
    }
    struct Wav *wav = calloc(1, sizeof(struct Wav));
    if (wav == NULL) {
        fclose(file);
        perror("Unable to allocate memory");
        return NULL;
    }
    wav_info_from_file((struct WavInfo *)wav, file);
    if (strcmp(((struct WavInfo *)wav)->riff, "RIFF") != 0 ||
        strcmp(((struct WavInfo *)wav)->wave, "WAVE") != 0 ||
        strcmp(((struct WavInfo *)wav)->fmt, "fmt ") != 0) {
        fprintf(stderr, "Not a wav file.\n");
        free(wav);
        fclose(file);
        return NULL;
    }
    fseek(file, 44, SEEK_SET);
    signed char *wav_data = malloc(
        (((struct WavInfo *)wav)->chunk_size - 36)*sizeof(signed char));
    if (wav_data == NULL) {
        free(wav);
        fclose(file);
        perror("Unable to allocate sound data");
        return NULL;
    }
    if (fread(wav_data, 1, 
        ((struct WavInfo *)wav)->chunk_size-36, 
        file) != (((struct WavInfo *)wav)->chunk_size-36)) {
        fprintf(stderr, "Unable to read wav data\n");
        free(wav);
        return NULL;
    }
    wav->data.i8 = wav_data;
    fclose(file);
    return wav;
}


void print_wav_info(const struct WavInfo *wav_info) {
    printf("Filetype: %s %s %s\n", wav_info->riff, 
           wav_info->wave, wav_info->fmt);
    printf("Chunk size: %d\n", wav_info->chunk_size);
    printf("Format: %d\n", (int)wav_info->format);
    printf("Number of channels: %d\n", 
           (int)wav_info->num_channels);
    printf("Sample Rate: %d\n", wav_info->sample_rate);
    printf("Bits per Sample: %d\n", 
           (int)wav_info->bits_per_sample);
    printf("Data Chunk Size: %d\n", wav_info->subchunk2_size);
}


void copy_filename(char *filename, const char *path) {
    const char *path2 = path;
    while(*path2++);
    while(path2 --> path && *path2 != '/');
    path2 = (*path2 == '/')? path2+1: path;
    while((*filename++ = *path2++));
    *filename = '\0';
}