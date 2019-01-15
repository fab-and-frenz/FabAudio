package fabaudio

import (
    "io"
)

type Sink interface {
    io.Writer
    io.ReaderFrom
}

