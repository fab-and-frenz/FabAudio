package pcm

type PCM struct {
    SampleRate      uint
    BitsPerSample   uint
    NumChannels     uint

    Data          []byte
}

