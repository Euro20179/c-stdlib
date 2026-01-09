#include "string.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

void string_new(string* strout, size_t len)
{
    strout->allocated = len;
    strout->len = 0;

    strout->data = calloc(len, 1);

    strout->initialized = true;
}

string* string_new2(size_t len)
{
    string* out = malloc(sizeof(string));
    out->data = calloc(len, 1);
    out->len = 0;
    out->allocated = len;
    out->initialized = true;
    return out;
}

void string_del2(string* str)
{
    if (str->initialized != true)
        return;
    str->initialized = 0;
    str->len = 0;
    str->allocated = 0;
    free(str->data);
    free(str);
}

void string_del(string* str)
{
    if (str->initialized != true)
        return;
    str->initialized = 0;
    str->len = 0;
    str->allocated = 0;
    free(str->data);
}

void string_concat(string* str, const char* text, size_t textlen)
{
    if (str->initialized != true) {
        return;
    }
    if (textlen + str->len > str->allocated) {
        if (string_extend(str, textlen) < 0) {
            return;
        }
    }

    memcpy(str->data + str->len, text, textlen);
    str->len += textlen;
}

void string2_concat(string* to, string* from)
{
    if (from->len + to->len > to->allocated) {
        if (string_extend(to, from->len) < 0) {
            return;
        }
    }

    memcpy(to->data + to->len, from->data, from->len);

    to->len += from->len;
}

void string_set(string* str, const char* text, size_t n)
{
    if (str->initialized != true)
        return;

    if (n > str->allocated) {
        n = str->allocated;
    }

    str->len = n;

    memcpy(str->data, text, n);
}

void string_set_char_at(string* str, char c, int idx)
{
    if (idx > str->allocated)
        return;
    if (idx >= str->len) {
        str->len = idx + 1;
    }
    str->data[idx] = c;
}

void string_concat_char(string* str, char c)
{
    if (str->len >= str->allocated) {
        if (string_extend(str, str->allocated || 1) < 0) {
            return;
        }
    }
    str->data[str->len] = c;
    str->len++;
}

int string_extend(string* str, size_t amount)
{
    if (str->initialized != true)
        return -1;

    if (str->allocated == 0) {
        str->data = realloc(str->data, 1);
        if (str->data == NULL) {
            return -2;
        }

        str->allocated = amount;
        return 0;
    }

    int new_amount = str->allocated + amount;
    void* temp = realloc(str->data, new_amount);
    if (temp == NULL) {
        return -2;
    }
    str->allocated = new_amount;
    str->data = temp;
    return 0;
}

void string_to_cstr(string* str, char* out)
{
    if (str->initialized != true || str->len < 1) {
        out[0] = 0;
        return;
    }
    memcpy(out, str->data, str->len);
    out[str->len] = 0;
}

char* string_mkcstr(string* str)
{
    string_concat_char(str, '\0');
    return str->data;
}

string* string_from_cstr(const char* str, size_t n)
{
    string* s = string_new2(n);
    string_set(s, str, n);
    return s;
}

void string_clear(string* str)
{
    str->len = 0;
}

void string_copy(string* str, string* ostring)
{
    ostring->allocated = str->allocated;
    ostring->initialized = str->initialized;
    ostring->len = str->len;
    if (str->initialized) {
        *(ostring->data) = *(str->data);
        // ostring->data = str->data;
        // memcpy(ostring->data, str->data, str->len);
    }
}

char string_at(string* str, size_t pos)
{
    if (str->initialized != true || pos >= str->len) {
        return 0;
    }
    return str->data[pos];
}

__int64_t string_find(string* str, char ch)
{
    for (__int64_t i = 0; i < string_len(str); i++) {
        if (string_at(str, i) == ch) {
            return i;
        }
    }
    return -1;
}

void string_split(string* str, char sep, void* userdata, void(cb)(string*, size_t, void*))
{
    string cur;
    string_new(&cur, 0);

    size_t count = 0;

    for (size_t i = 0; i < str->len; i++) {
        char ch = string_at(str, i);
        if (ch == sep) {
            cb(&cur, count++, userdata);
            string_clear(&cur);
            cur.len = 0;
        } else {
            string_concat_char(&cur, ch);
        }
    }

    if (cur.len > 0) {
        cb(&cur, count, userdata);
    }

    string_del(&cur);
}

void string_slice_suffix(string* str, size_t amount)
{
    if (amount > str->len) {
        amount = str->len;
    }
    str->len -= amount;
}

void string_slice_prefix(string* str, size_t amount)
{
    if (amount > str->len) {
        amount = str->len;
    }
    str->len -= amount;
    memcpy(str->data, str->data + amount, str->len);
}

int string_slice(string* str, size_t start, int64_t end)
{
    if (start >= str->len) {
        return -1;
    }

    if (end < 0) {
        end = str->len + end;
    }

    if (end == 0) {
        end = str->len;
    }

    if (start > end) {
        return -2;
    }

    str->len = end - start;
    memcpy(str->data, str->data + start, end - start);
    return 0;
}

size_t string_len(string* str)
{
    return str->len;
}

void string_tr(string* str, char from, char to)
{
    for (size_t i = 0; i < str->len; i++) {
        if (string_at(str, i) == from) {
            string_set_char_at(str, to, i);
        }
    }
}

void string_cpy(string* to, string* from)
{
    if (to->allocated < from->allocated) {
        if (string_extend(to, from->len) < 0) {
            return;
        }
    }
    to->len = from->len;
    memcpy(to->data, from->data, from->len);
}

void string_uri_encode(string* str, string* out)
{
    for (size_t i = 0; i < string_len(str); i++) {
        char ch = string_at(str, i);
        switch (ch) {
        case ':':
        case '/':
        case '#':
        case '[':
        case ']':
        case '@':
        case '!':
        case '$':
        case '&':
        case '\'':
        case '(':
        case ')':
        case '*':
        case '+':
        case ',':
        case ';':
        case '=':
        case '%':
        case ' ': {
            char buf[4];
            snprintf(buf, 4, "%%%X", ch);
            string_concat(out, buf, 3);
            break;
        }
        default:
            string_concat_char(out, ch);
        }
    }
}

void string_nconcatf(string* str, size_t maxlen, const char* fmt, ...)
{
    char* buf = calloc(maxlen, 1);
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, maxlen, fmt, args);
    va_end(args);
    int len = 0;
    for (int i = 0; i < maxlen; i++) {
        if (buf[i] == 0) {
            break;
        }
        len++;
    }
    string_concat(str, buf, len);
    free(buf);
}

void string_replace(string* str, char needle, char repl)
{
    for (size_t i = 0; i < string_len(str); i++) {
        if (str->data[i] == needle) {
            str->data[i] = repl;
        }
    }
}

bool cstr_includes(const char* haystack, const char* needle)
{
    int nlen = strlen(needle);
    int stacklen = strlen(haystack);
    for (size_t i = 0; i < stacklen; i += nlen) {
        if (strncmp(haystack + i, needle, nlen) == 0) {
            return true;
        }
    }
    return false;
}

int string_prepend(string* str, string* toprepend)
{
    if (str->len + toprepend->len > str->allocated) {
        int res = string_extend(str, str->len + toprepend->len);
        if (res < 0) {
            return -1;
        }
    }

    // 1. move the string data out of the way
    //(moving it by toprepend->len only works if toprepend is shorter than str
    // because otherwise weird overlapping happens)
    memcpy(str->data + toprepend->len, str->data, str->len);
    // 2. move toprepend to the beginning
    memcpy(str->data, toprepend->data, toprepend->len);

    str->len += toprepend->len;
    return 0;
}

bool string_startswith(string* str, string* needle)
{
    for (size_t i = 0; i < string_len(needle); i++) {
        if (string_at(str, i) != string_at(needle, i)) {
            return false;
        }
    }
    return true;
}

void string_trimstart(string* str, const char* mask)
{
    const int masklen = strlen(mask);
    bool not_found_all = true;
    while (not_found_all) {
        for (int i = 0; i < masklen; i++) {
            char ch = string_at(str, 0);
            if (ch != mask[i]) {
                not_found_all = false;
                continue;
            }

            not_found_all = true;
            while (string_at(str, 0) == mask[i]) {
                string_slice_prefix(str, 1);
            }
        }
    }
}

bool string_eq(string* str, const char* cmp)
{
    const int cmp_len = strlen(cmp);

    if (cmp_len != str->len) {
        return false;
    }

    for (size_t i = 0; i < cmp_len; i++) {
        if (string_at(str, i) != cmp[i]) {
            return false;
        }
    }

    return true;
}

// string stream stuff {{{

struct string_stream {
    string* str;
    size_t pos;
};

string_stream* string_stream_open(string* str)
{
    string_stream* stream = malloc(sizeof(string_stream));
    stream->str = str;
    stream->pos = 0;
    return stream;
}

void string_stream_close(string_stream* stream)
{
    free(stream);
}

size_t string_stream_read(string_stream* stream, uint8_t* out, size_t maxlen)
{
    if (stream->pos >= string_len(stream->str)) {
        return 0;
    }

    size_t len = string_len(stream->str) - stream->pos;
    if (maxlen >= len) {
        memcpy(out, stream->str->data + stream->pos, len);
        stream->pos += len;
        return len;
    }

    memcpy(out, stream->str->data + stream->pos, maxlen);
    stream->pos += maxlen;
    return maxlen;
}

size_t string_stream_write(string_stream* stream, uint8_t* in, size_t byte_count)
{
    if (byte_count > stream->str->allocated - stream->pos || stream->pos > stream->str->allocated) {
        if (string_extend(stream->str, stream->str->allocated * 2 + (stream->str->allocated - string_len(stream->str))) < 0) {
            return 0;
        }
    }

    memcpy(stream->str->data + stream->pos, in, byte_count);
    if(stream->pos > stream->str->len) {
        stream->str->len = stream->pos + byte_count;
    } else if (stream->pos + byte_count > stream->str->len) {
        stream->str->len += (stream->pos + byte_count) - stream->str->len;
    }
    stream->pos += byte_count;

    return byte_count;
}

int string_stream_seek(string_stream* stream, size_t bytes)
{
    if (bytes > string_len(stream->str) - stream->pos) {
        stream->pos = string_len(stream->str);
        return -2;
    }

    stream->pos += bytes;
    return 0;
}
// }}}
