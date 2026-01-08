#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

//return 0 for EOF
typedef size_t(*stream_reader_t)(void*, uint8_t*, size_t);
typedef size_t(*stream_writer_t)(void*, uint8_t*, size_t);
typedef int(*stream_seek_t)(void*, size_t);

#define reader_fn(fn) (size_t(*)(void*, uint8_t*, size_t)) (fn)
#define writer_fn(fn) (size_t(*)(void*, uint8_t*, size_t)) (fn)
#define seeker_fn(fn) (int(*)(void*, size_t)) (fn)

// reads the entire reader stream, and writes to the writer
void stream_stream(void* readfrom, void* writeto, 
        stream_reader_t reader, stream_writer_t writer);

// reads n bytes from reader and puts them into writer
void stream_nstream(stream_reader_t, stream_writer_t, size_t n);

// seeks n bytes using the provided seeker function
// return values:
//   0 - seeked the correct amount
//   -1 - failed to seek
//   -2 - seeked to EOF
int stream_seek(void*,
        stream_seek_t, size_t n);

// turns fwrite into a stream_writer_t
size_t stream_fwrite(FILE*, uint8_t*, size_t);

// turns fread into a stream_reader_t
size_t stream_fread(FILE* file, uint8_t * buf, size_t bufsize);
