#ifndef FA_WAV_H
#define FA_WAV_H

#include "FA_PCM.h"

/**
 * Reads a wav file and creates an FA_PCM structure containing the raw PCM data
 * of the wav file.
 *
 * @param filename the name of the file to read from.
 * @return An FA_PCM structure that contains the audio data from the wav file.
 */
FA_PCM
FA_ReadWavFile (const char *filename);

/**
 * Creates a wav file from the raw PCM data contained in an FA_PCM structure.
 *
 * @param filename the name of the wav file to create.
 * @param pcm a PCM structure to write into a wav file.
 */
void
FA_WriteWavFile (const char *filename,
                 FA_PCM      pcm);

#endif

