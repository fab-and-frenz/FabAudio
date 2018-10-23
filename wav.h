#ifndef __WAV_H__
#define __WAV_H__

#include <stdint.h>

#define WAV_HEADER_SIZE 44

typedef struct {
    uint32_t  chunk_id;
    uint32_t  chunk_size;
    uint32_t  format;

    uint32_t  subchunk1_id;
    uint32_t  subchunk1_size;
    uint16_t  audio_format;
    uint16_t  num_channels;
    uint32_t  sample_rate;
    uint32_t  byte_rate;
    uint16_t  block_align;
    uint16_t  bits_per_sample;

    uint32_t  subchunk2_id;
    uint32_t  subchunk2_size;
    uint8_t  *data;
} Wav;

Wav *
wav_from_buffer (const uint8_t *data,
                 size_t         data_size);

Wav *
wav_from_file (const char *filepath);

uint8_t *
wav_to_buffer (const Wav *wav,
               size_t    *size);

void
wav_to_file (const Wav  *wav,
             const char *filepath);

void
wav_free (Wav *wav);

#endif

