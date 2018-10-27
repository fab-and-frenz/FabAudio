// 
// sine.c
//
// This program creates a wav file with five-seconds of a sine wave at
// 1/4 maximum volume to prevent your ears from bleeding.
//
// To compile this code, use:
//
//     gcc sine.c -o sine -lm
// 
// To run the executable, use:
//
//     ./sine /path/to/outputfile/here.wav
//
// You can then listen to the wav file with the
// media player of your choice.
//
// For information about the LPCM audio format see:
//
//     https://en.wikipedia.org/wiki/Pulse-code_modulation
//
// For information about the basic structure of a wav file see:
// 
//     http://soundfile.sapp.org/doc/WaveFormat/ 
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

int
main (int   argc,
      char *argv[])
{
    if (argc < 2) {
        printf ("USAGE: %s <output file>\n", argv[0]);
        return 1;
    }

    // The path to our output file
    const char *outfile = argv[1];
   
    // Audio format 1 specifies pcm data in a wav file
    uint16_t audioformat = 1;

    // 1 channel (mono) audio
    uint16_t numchannels = 1;

    // The resolution of our data on the x-axis (time)
    // 48000 samples / 1 second
    uint32_t samplerate = 48000;

    // The resolution of our data on the y-axis (size of a sample in bits)
    // 16 bits / 1 channel / 1 sample
    uint16_t bitdepth = 16;

    // bytes / 1 second
    uint32_t byterate = samplerate * numchannels * bitdepth / 8;

    // bytes / 1 sample
    uint16_t blockalign = numchannels * bitdepth / 8;

    // five seconds worth of audio samples
    uint32_t numsamples = samplerate * 5;

    // the size of our pcm data in bytes
    uint32_t pcmsize = numsamples * blockalign;

    // The size of the file we are creating in bytes.
    // The file is a 44-byte wav header, followed by our PCM data.
    uint32_t filesize = pcmsize + 44;

    // Allocate memory for our Wav data
    uint8_t *filebuffer = (uint8_t*) malloc (filesize);

    // The size of the main 'chunk'
    uint32_t chunksize = filesize - 8;

    // The size of the 'fmt' subchunk (always 16 for pcm data)
    uint32_t fmtsize = 16;

    // These strings are the only big-endian values in a Wav header.
    const char *riff_id = "RIFF";
    const char *format = "WAVE";
    const char *fmt_id = "fmt ";
    const char *data_id = "data";

    // Copy our wav header into the beginning of the buffer.
    //
    //      destination      source         size of data to copy
    //     ------------------------------------------------------
    memcpy (filebuffer,       riff_id,      strlen (riff_id));
    memcpy (filebuffer +  4, &chunksize,    sizeof (chunksize));
    memcpy (filebuffer +  8,  format,       strlen (format));
    memcpy (filebuffer + 12,  fmt_id,       strlen (fmt_id));
    memcpy (filebuffer + 16, &fmtsize,      sizeof (fmtsize));
    memcpy (filebuffer + 20, &audioformat,  sizeof (audioformat));
    memcpy (filebuffer + 22, &numchannels,  sizeof (numchannels));
    memcpy (filebuffer + 24, &samplerate,   sizeof (samplerate));
    memcpy (filebuffer + 28, &byterate,     sizeof (byterate));
    memcpy (filebuffer + 32, &blockalign,   sizeof (blockalign));
    memcpy (filebuffer + 34, &bitdepth,     sizeof (bitdepth));
    memcpy (filebuffer + 36,  data_id,      strlen (data_id));
    memcpy (filebuffer + 40, &pcmsize,      sizeof (pcmsize));

    // Create a pointer to the section of our buffer that contains the PCM data.
    int16_t *pcm_data = (int16_t*) (filebuffer + 44);

    for (int i = 0; i < numsamples; i++) {
        // Create a Sin wave at maximum volume.
        // 
        // It is maximum volume because our sample data is of size int16,
        // and we are multiplying the sin function's output (bounded by [-1.0, 1.0])
        // by INT16_MAX (the largest possible 16-bit integer), giving us an output
        // bounded by [INT16_MIN, INT16_MAX].
        pcm_data[i] = (int16_t) (INT16_MAX * sin ((double) i / M_PI));

        // I divide the amplitude by four, lowering the volume to save your eardrums.
        pcm_data[i] /= 4;
    }

    // Write the wav data to disk
    FILE *fh = fopen (outfile, "w");
    fwrite (filebuffer, filesize, 1, fh);
    fclose (fh);

    return 0;
}
