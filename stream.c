#include "stream.h"
#include "array.h"
#include <stdarg.h>
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
    uint8_t out[256] = { 0 };
    size_t bytes_read,
           total_bytes_read = 0;
    while(n > 0 && (bytes_read = reader(readfrom, out, chunk_size)) != 0) {
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

void stream_tee(void *ins, stream_reader_t reader, stream_tee_output writers[]) {

    array* outs = array_new2(10, sizeof(void*));
    array* writer_fns = array_new2(10, sizeof(stream_writer_t));

    unsigned int writer_count = 0;
    for(unsigned int i = 0; writers[i].stream != NULL; i++) {
        writer_count++;
    }

    uint8_t buf[256];
    size_t bytes_read;
    while((bytes_read = reader(ins, buf, 256)) != 0) {
        for(unsigned int i = 0; i < writer_count; i++) {
            stream_writer_t writer = writers[i].writer;
            void* into = writers[i].stream;
            writer(into, buf, bytes_read);
        }
    }

    array_del2(outs);
    array_del2(writer_fns);
}
