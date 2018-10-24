package main

import(
    "fmt"
    "flag"
    "io/ioutil"
    "github.com/fab-and-frenz/fabaudio/pcm"
)

var(
    infile  string
    outfile string
)

func init() {
    flag.StringVar(&infile, "i", "", "The wav file to read")
    flag.StringVar(&outfile, "o", "", "The wav file to create")
    flag.Parse()
}

func main() {
    wavData, _ := ioutil.ReadFile(infile)
    pcmSlice, _ := pcm.NewPCMFromWav(wavData)

    fmt.Println("Sample Rate:", pcmSlice.SampleRate)
    fmt.Println("Bits Per Sample:", pcmSlice.BitsPerSample)
    fmt.Println("Num Channels:", pcmSlice.NumChannels)
    fmt.Println("Data Size:", len(pcmSlice.Data))

    outData, _ := pcmSlice.ToWav()
    ioutil.WriteFile(outfile, outData, 0666)
}

