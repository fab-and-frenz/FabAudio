#include "FA_WAV.h"

/*
 * This file is just to test the ReadWavFile and WriteWavFile functions.
 * 
 * After adding the two functions into FA_WAV.c, compile this code with:
 *
 *     gcc main.c FA_PCM.c FA_WAV.c -o wav-tester
 * 
 * You can test the program with:
 *
 *     ./wav-tester /path/to/wav/file.wav out.wav
 *
 * Make sure the input file exists! If both functions work properly,
 * out.wav should sound the same as file.wav if you play them both
 * with a media player.
 */
int
main (int   argc,
      char *argv[])
{
    // Make sure the user specifies the input and output files.
    if (argc < 3) {
        printf ("USAGE: %s <infile> <outfile>\n", argv[0]);
        return 1;
    }

    // Filename for the input file
    const char *infile = argv[1];

    // Filename for the output file
    const char *outfile = argv[2];

    // Read the Wav File
    FA_PCM pcm = FA_ReadWavFile(infile);

    // Write a new Wav file with the same audio data
    FA_WriteWavFile (outfile, pcm);

    return 0;
}

