#include "stream.h"
#include <stdio.h>

size_t stream_stream(void* readfrom, void* writeto,
        stream_reader_t reader, stream_writer_t writer) {
    uint8_t out[256] = { 0 };
    size_t total_bytes_read = 0,
           bytes_read;
    while((bytes_read = reader(readfrom, out, 256)) != 0) {
        writer(writeto, out, bytes_read);
        total_bytes_read += bytes_read;
    }
    return total_bytes_read;
}

size_t stream_nstream(void *readfrom, void *writeto,
        stream_reader_t reader, stream_writer_t writer, size_t n) {
    const int chunk_size = n > 255 ? 256 : n;
    uint8_t out[256];
    size_t bytes_read,
           total_bytes_read = 0;
    while((bytes_read = reader(readfrom, out, chunk_size)) != 0 && n > 0) {
        n -= chunk_size;
        writer(writeto, out, bytes_read);
        total_bytes_read += bytes_read;
    }
    return total_bytes_read;
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
