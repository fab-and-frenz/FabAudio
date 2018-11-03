package main

import(
    "os"
    "fmt"
    "math"
    "io/ioutil"
    "unsafe"
    "encoding/binary"
)

func main() {
    if len(os.Args) < 2 {
        fmt.Printf ("USAGE: %s <output file>\n", os.Args[0])
        return
    }

    outFile := os.Args[1]

    var audioFormat uint16 = 1
    var numChannels uint16 = 1
    var sampleRate uint32 = 48000
    var bitDepth uint16 = 16
    var byteRate uint32 = sampleRate * uint32(numChannels) * uint32(bitDepth) / uint32(8)
    var blockAlign uint16 = numChannels * bitDepth / 8
    var numSamples uint32 = sampleRate * 5
    var pcmSize uint32 = numSamples * uint32(blockAlign)
    var fileSize uint32 = pcmSize + 44
    var chunkSize uint32 = fileSize - 8
    var fmtSize uint32 = 16

    riffId := []byte("RIFF")
    format := []byte("WAVE")
    fmtId := []byte("fmt ")
    dataId := []byte("data")

    fileBuffer := make([]byte, fileSize)

    copy(fileBuffer[0:], riffId)
    binary.LittleEndian.PutUint32(fileBuffer[4:], chunkSize)
    copy(fileBuffer[8:], format)
    copy(fileBuffer[12:], fmtId)
    binary.LittleEndian.PutUint32(fileBuffer[16:], fmtSize)
    binary.LittleEndian.PutUint16(fileBuffer[20:], audioFormat)
    binary.LittleEndian.PutUint16(fileBuffer[22:], numChannels)
    binary.LittleEndian.PutUint32(fileBuffer[24:], sampleRate)
    binary.LittleEndian.PutUint32(fileBuffer[28:], byteRate)
    binary.LittleEndian.PutUint16(fileBuffer[32:], blockAlign)
    binary.LittleEndian.PutUint16(fileBuffer[34:], bitDepth)
    copy(fileBuffer[36:], dataId)
    binary.LittleEndian.PutUint32(fileBuffer[40:], pcmSize)

    pcmData := fileBuffer[44:]

    for i := 0; i < int(numSamples); i++ {
        sample := int16(math.MaxInt16 * math.Sin(float64(i) / math.Pi))

        sample /= 4

        uSample := *(*uint16)(unsafe.Pointer(&sample))
        binary.LittleEndian.PutUint16(pcmData[i * 2:], uSample)
    }

    ioutil.WriteFile(outFile, fileBuffer, 0666)
}

