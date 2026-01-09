/*
stream.h

The goal of this library is to make it so that anything can be a reader, writer
or be able to be seeked.

Reader functions:
A reader function reads an object from the current cursor position
and outputs the data into a buffer.
PARAMS:
    object to read
    output buffer
    max bytes to read
RETURNS:
    the number of bytes read

Writer functions:
A writer function writes n bytes from a buffer to an object at the current cursor
position.
PARAMS:
    object to read
    input buffer
    bytes to write from input buffer
RETURNS:
    the number of bytes written (may be less than requested as if for example
                                the object ran out of space)

Seeker functions:
Moves the cursor either forward or backward depending on seek FLAGS
If a seeker fucntion doesn't implement a flag it should return -3 - bitpos(flag)
eg if the flag is 0b10, it should return -10 - bitpos(0b10) -> -12
PARAMS:
    object to seek
    amount to seek (unsigned)
RETURNS:
    0 - seeked the requested amount
    -1 - failed to seek
    -2 - seeked to EOF or SOF (start of file)
         EOF is 1 byte past the end
         SOF is the first byte
    -10 - bitpos(flag) - invalid flag
*/
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <strings.h>
#include <stdio.h>

//return 0 for EOF
typedef size_t(*stream_reader_t)(void*, uint8_t*, size_t);
typedef size_t(*stream_writer_t)(void*, uint8_t*, size_t);
typedef int(*stream_seek_t)(void*, size_t);

#define reader_fn(fn) (size_t(*)(void*, uint8_t*, size_t)) (fn)
#define writer_fn(fn) (size_t(*)(void*, uint8_t*, size_t)) (fn)
#define seeker_fn(fn) (int(*)(void*, size_t)) (fn)

#define stream(rf, wt, r, w) stream_stream((rf), (wt), reader_fn(r), writer_fn(w))
#define stream_n(rf, wt, r, w, n) stream_nstream((rf), (wt), reader_fn(r), writer_fn(w), n)


// reads the entire reader stream, and writes to the writer
// RETURNS:
// number of bytes read
size_t stream_stream(void* readfrom, void* writeto, 
        stream_reader_t reader, stream_writer_t writer);

// reads n bytes from reader and puts them into writer
// RETURNS:
// number of bytes read (if less than n, EOF was hit)
size_t stream_nstream(void* readfrom, void* writeto, stream_reader_t, stream_writer_t, size_t n);

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

typedef struct {
    void* stream;
    stream_writer_t writer;
} stream_tee_output;


// reads ins using reader and writes it to all the following
// outs outs_writer pairs.
// last item in writers must be NULL
void stream_tee(void* ins, stream_reader_t reader, stream_tee_output writers[]);

// similar to stream_tee, but read at most n bytes from ins
// RETURNS:
// the amount of bytes read
size_t stream_ntee(void* ins, stream_reader_t reader, size_t n, stream_tee_output writers[]);

// a writer that writes to the list of writers provided
void stream_tee_writer(stream_tee_output writers[], uint8_t*, size_t);
