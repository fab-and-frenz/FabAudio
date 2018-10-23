#ifndef __FA_PCM_SLICE_H__
#define __FA_PCM_SLICE_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    unsigned int  num_channels;
    unsigned int  sample_rate;
    int           bits_per_sample;

    size_t        size;
    void         *data;
} FA_PcmSlice;


FA_PcmSlice          fa_pcm_slice_new_from_buffer                (const void         *buffer,
                                                                  size_t              buffer_size,
                                                                  unsigned int        num_channels,
                                                                  unsigned int        sample_rate,
                                                                  int                 bits_per_sample);
FA_PcmSlice          fa_pcm_slice_deep_copy                      (FA_PcmSlice         slice);
FA_PcmSlice          fa_pcm_slice_subslice                       (FA_PcmSlice         slice,
                                                                  int                 start,
                                                                  int                 end);
void                 fa_pcm_slice_free                           (FA_PcmSlice        *slice);
void                 fa_pcm_slice_invalidate                     (FA_PcmSlice        *slice);
bool                 fa_pcm_slice_is_valid                       (FA_PcmSlice         slice);

#endif

