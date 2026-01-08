#include "stream.h"

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
