package pcm

import(
    "encoding/binary"
)

type PCM struct {
    SampleRate      uint32
    BitsPerSample   uint16
    NumChannels     uint16

    Data          []byte
}

func NewPCMFromWav(data []byte) (PCM, error) {
    var pcm PCM

    pcm.SampleRate = binary.LittleEndian.Uint32(data[24:28])
    pcm.BitsPerSample = binary.LittleEndian.Uint16(data[34:36])
    pcm.NumChannels = binary.LittleEndian.Uint16(data[22:24])

    start := 0
    end := 4

    for string(data[start:end]) != "data" && end <= len(data) {
        start += 1
        end += 1
    }

    start += 4
    end += 4

    dataSize := binary.LittleEndian.Uint32(data[start:end])

    start += 4
    end += 4 + int(dataSize)

    pcm.Data = data[start:end]

    return pcm, nil
}

func (pcm PCM) ByteRate () uint32 {
    return pcm.SampleRate * uint32(pcm.BlockAlign())
}

func (pcm PCM) BlockAlign () uint16 {
    return pcm.NumChannels * pcm.BitsPerSample / 8
}

func (pcm PCM) ToWav () ([]byte, error) {
    size := 44 + len(pcm.Data)
    buf := make([]byte, size)

    copy(buf, "RIFF")
    binary.LittleEndian.PutUint32(buf[4:8], uint32(size) - 8)
    copy(buf[8:12], "WAVE")

    copy(buf[12:16], "fmt ")
    binary.LittleEndian.PutUint32(buf[16:20], 16)
    binary.LittleEndian.PutUint16(buf[20:22], 1)
    binary.LittleEndian.PutUint16(buf[22:24], pcm.NumChannels)
    binary.LittleEndian.PutUint32(buf[24:28], pcm.SampleRate)
    binary.LittleEndian.PutUint32(buf[28:32], pcm.ByteRate())
    binary.LittleEndian.PutUint16(buf[32:34], pcm.BlockAlign())
    binary.LittleEndian.PutUint16(buf[34:36], pcm.BitsPerSample)

    copy(buf[36:40], "data")
    binary.LittleEndian.PutUint32(buf[40:44], uint32(len(pcm.Data)))
    copy(buf[44:], pcm.Data)

    return buf, nil
}

