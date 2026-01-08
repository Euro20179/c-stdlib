#include "stream.h"
#include <stdio.h>

void stream_stream(void* readfrom, void* writeto,
        stream_reader_t reader, stream_writer_t writer) {
    uint8_t out[1028] = { 0 };
    size_t bytes_read;
    while((bytes_read = reader(readfrom, out, 1028)) != 0) {
        writer(writeto, out, bytes_read);
    }
}

int stream_seek(void* toseek,
        stream_seek_t seeker, size_t bytes) {
    return seeker(toseek, bytes);
}

size_t stream_fwrite(FILE* file, uint8_t * buf, size_t bufsize) {
    return fwrite(buf, 1, bufsize, file);
}

size_t stream_fread(FILE* file, uint8_t * buf, size_t bufsize) {
    return fread(buf, 1, bufsize, file);
}
