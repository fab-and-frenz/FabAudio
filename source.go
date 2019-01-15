package fabaudio

import (
    "io"
)

type Source interface {
    io.Reader
    io.WriterTo
}

