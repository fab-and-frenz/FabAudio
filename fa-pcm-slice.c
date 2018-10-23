#include <string.h>
#include <stdlib.h>

#include "pcm-slice.h"

FA_PcmSlice
fa_pcm_slice_new_from_buffer (const void    *buffer,
                              size_t         buffer_size,
                              unsigned int   num_channels,
                              unsigned int   sample_rate,
                              int            bits_per_sample)
{
    FA_PcmSlice slice = {0};
    slice.num_channels = num_channels;
    slice.sample_rate = sample_rate;
    slice.bits_per_sample = bits_per_sample;
    slice.data = malloc (buffer_size);
    memcpy (slice.data, buffer, buffer_size);
    return slice;
}

FA_PcmSlice
fa_pcm_slice_deep_copy (FA_PcmSlice slice)
{
    FA_PcmSlice new_slice = slice;
    new_slice.data = malloc (slice.size);
    memcpy (new_slice.data, slice.data, slice.size);
    return new_slice;
}

FA_PcmSlice
fa_pcm_slice_subslice (FA_PcmSlice  slice,
                       int          start,
                       int          end)
{
    FA_PcmSlice new_slice = slice;
  
    uint8_t *start_ptr = slice.data;
    uint8_t *end_ptr = ((uint8_t*)slice.data) + slice.size;

    int new_size = end - start;

    if (new_size <= 0 || end_ptr) {
        fa_pcm_slice_invalidate (&slice);
        return slice;
    }

    uint8_t *new_start;
    if (start < 0) {
        new_start = end_ptr + start;
    }
    else {
        new_start = start_ptr + start;
    }
        
    new_slice.data = new_start;
    new_slice.size = new_size;
    
    return new_slice;
}

void
fa_pcm_slice_free (FA_PcmSlice *slice)
{
    free (slice->data);
    fa_pcm_slice_invalidate (slice);
}

void
fa_pcm_slice_invalidate (FA_PcmSlice *slice)
{
    memset (slice, 0, sizeof (FA_PcmSlice));
}

bool
fa_pcm_slice_is_valid (FA_PcmSlice slice)
{
    return slice.size > 0;
}

