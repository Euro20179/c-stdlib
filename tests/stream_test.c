#include <stdio.h>

#include "../string.h"
#include "../stream.h"
#include "../tests.h"

mktest(test_stream_fwrite, {
    string* str = string_from_cstr("Cool", 4);
    string_stream* ins = string_stream_open(str);

    FILE* f = fopen("./test_stream_fwrite", "w");

    stream_stream(ins, f, reader_fn(string_stream_read), writer_fn(stream_fwrite));

    string_stream_close(ins);
    string_clear(str);
    string_stream* outs = string_stream_open(str);
    fclose(f);

    f = fopen("./test_stream_fwrite", "r");
    stream_stream(f, outs, reader_fn(stream_fread), writer_fn(string_stream_write));
    fclose(f);

    remove("./test_stream_fwrite");

    string_stream_close(outs);

    t("After writing to file and reading from file, string is 'Cool'", "%d",
            string_eq(str, "Cool"), eq, 1);

    string_del2(str);
})

int main() {
    TEST(test_stream_fwrite);

    printf("\n--------------------\n\x1b[32;1m[✓]\x1b[0m %zu \x1b[31;1m[✗]\x1b[0m %zu\n", pass_count, err_count);
}
