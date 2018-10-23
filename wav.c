#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "wav.h"

static uint16_t
extract_uint16_t (const uint8_t **ptr)
{
    uint16_t val =  0x0000 |
                   ((*ptr)[1] << 8) |
                    (*ptr)[0];
    *ptr = *ptr + 2; 
    return val;
}

static void
put_uint16_t (uint16_t   val,
              uint8_t  **buf)
{
    (*buf)[1] = (0xff00 & val) >> 8;
    (*buf)[0] = (0x00ff & val);
    *buf = *buf + 2;
}

static uint32_t
extract_uint32_t (const uint8_t **ptr)
{
    uint32_t val = 0x00000000 |
                  ((*ptr)[1] << 24) |
                  ((*ptr)[0] << 16) |
                  ((*ptr)[3] << 8)  |
                   (*ptr)[2];
    *ptr = *ptr + 4;
    return val;
}

static void
put_uint32_t (uint32_t   val,
              uint8_t  **buf)
{
    (*buf)[1] = (0xff000000 & val) >> 24;
    (*buf)[0] = (0x00ff0000 & val) >> 16;
    (*buf)[3] = (0x0000ff00 & val) >> 8;
    (*buf)[2] = (0x000000ff & val);
    *buf = *buf + 4;
}

Wav *
wav_from_buffer (const uint8_t *data,
                 size_t         data_size)
{
    if (data_size < WAV_HEADER_SIZE) return NULL;

    Wav *wav = malloc (sizeof (Wav));

    wav->chunk_id = extract_uint32_t (&data);
    wav->chunk_size = extract_uint32_t (&data);
    wav->format = extract_uint32_t (&data);

    wav->subchunk1_id = extract_uint32_t (&data);
    wav->subchunk1_size = extract_uint32_t (&data);
    wav->audio_format = extract_uint16_t (&data);
    wav->num_channels = extract_uint16_t (&data);
    wav->sample_rate = extract_uint32_t (&data);
    wav->byte_rate = extract_uint32_t (&data);
    wav->block_align = extract_uint16_t (&data);
    wav->bits_per_sample = extract_uint16_t (&data);
    
    wav->subchunk2_id = extract_uint32_t (&data);
    wav->subchunk2_size = extract_uint32_t (&data);

    wav->data = (uint8_t*) malloc (wav->subchunk2_size);
    memcpy (wav->data, data + WAV_HEADER_SIZE, wav->subchunk2_size);

    return wav;
}

Wav *
wav_from_file (const char *filepath)
{
    FILE *fh = fopen (filepath, "r");

    fseek (fh, 0, SEEK_END);
    size_t fsize = ftell (fh);
    uint8_t *buf = (uint8_t*) malloc (fsize);
    fseek (fh, 0, SEEK_SET);
    fread (buf, sizeof (uint8_t), fsize, fh);

    Wav *wav = wav_from_buffer (buf, fsize);

    free (buf);
    fclose (fh);

    return wav;
}

uint8_t *
wav_to_buffer (const Wav *wav,
               size_t    *size)
{
    size_t data_size = wav->subchunk2_size + WAV_HEADER_SIZE;
    uint8_t *ret = (uint8_t*) malloc (data_size);
    uint8_t *data = ret;

    put_uint32_t (wav->chunk_id, &data);
    put_uint32_t (wav->chunk_size, &data);
    put_uint32_t (wav->format, &data);

    put_uint32_t (wav->subchunk1_id, &data);
    put_uint32_t (wav->subchunk1_size, &data);
    put_uint16_t (wav->audio_format, &data);
    put_uint16_t (wav->num_channels, &data);
    put_uint32_t (wav->sample_rate, &data);
    put_uint32_t (wav->byte_rate, &data);
    put_uint16_t (wav->block_align, &data);
    put_uint16_t (wav->bits_per_sample, &data);
    
    put_uint32_t (wav->subchunk2_id, &data);
    put_uint32_t (wav->subchunk2_size, &data);

    memcpy (data, wav->data, wav->subchunk2_size);

    *size = data_size;

    return ret;
}

void
wav_to_file (const Wav  *wav,
             const char *filepath)
{
    FILE *fh = fopen (filepath, "w");

    size_t size;
    uint8_t *buf = wav_to_buffer (wav, &size);

    fwrite (buf, sizeof (uint8_t), size, fh);

    fclose (fh);
}

