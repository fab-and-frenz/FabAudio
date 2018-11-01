#ifndef FA_PCM_H
#define FA_PCM_H

/*
 * The FA_PCM type represents an array of raw PCM audio data.
 * 
 * SampleRate defines the number of samples for 1 second of audio.
 * BitDepth defines the size of one sample in bits.
 * NumChannels defines the number of channels of audio.
 * Data is a pointer to the raw PCM data.
 * Size defines the size of the data pointed to by Data in bytes.
 *
 * The Data is formatted as an array of signed integers of size BitDepth.
 * e.g. if BitDepth is 8, Data is an array of int8_t values
 *
 * If there are multiple channels of audio, the samples for each channel
 * are placed after eachother consecutively.
 * e.g. if NumChannels is 2, and Data is casted as a pointer of to the correct
 * type of integer, it follows that:
 * Data[0] = channel 0 sample 0,
 * Data[1] = channel 1 sample 0,
 * Data[2] = channel 0 sample 1,
 * Data[3] = channel 1 sample 1,
 * Data[4] = channel 0 sample 2,
 * Data[5] = channel 1 sample 2,
 * ...
 * Data[(NumChannels * S) + C] = channel C, sample S
 *
 * So if we have stereo 16-bit audio, then NumChannels = 2, and
 * BitDepth = 16. If we want the 5th audio sample of the right audio
 * channel, we can find it like this:
 * 
 * FabPCM pcm;
 * int16_t sample = ((int16_t*)pcm.Data)[(pcm.NumChannels * 5) + 1];
*/
typedef struct {
    size_t  SampleRate;
    size_t  BitDepth;
    size_t  NumChannels;
    size_t  Size;
    void   *Data;
} FA_PCM;

#endif

