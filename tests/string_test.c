#include "../string.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static uint64_t err_count;
static uint64_t pass_count;

#define TEST(name) { \
    printf("TESTING \x1b[4m%s\x1b[0m\n", #name); \
    const char* res = name(); \
    if(res[0] != 0) { \
        printf("\x1b[31m%s\n\x1b[0m", res); \
        printf("\x1b[31;1mERR ✗ \x1b[0m\n-------\n"); \
    } else { \
        printf("\x1b[32;1mOK ✓ \x1b[0m\n-------\n"); \
    }\
}

static char assert_text_buf[256];
#define asserteq(left, right, ty)\
    ((left) == (right));\
    if (!((left) == (right))) { \
        snprintf(assert_text_buf, 255, #left " does not equal " #right ", got: " ty, left); \
    }

#define assertgt(left, right, ty)\
    ((left) > (right));\
    if(!((left) > (right))) { \
        snprintf(assert_text_buf, 255, #left " is not > " #right ", got: " ty, left); \
    }


#define assert(expr, msg) if(!(expr)) return msg

#define mktest(name, body) const char* name() { \
    body \
    return ""; \
}

#define t(name, ty, left, testty, right) { \
    bool n = assert##testty(left, right, ty); \
    if(n == 0) { \
        printf("\t\x1b[31;1m[✗]\x1b[0m subtest: %s\n\t\t" #left " " #testty " " #right "\n", name); \
        err_count++;\
        return assert_text_buf; \
    } \
    pass_count++;\
    printf("\t\x1b[32;1m[✓]\x1b[0m subtest: %s\n\t\t" #left " " #testty " " #right "\n", name); \
}

#define mkstrtest(name, body)\
        mktest(\
                string_test_##name,\
                string* str = string_new2(30);\
                body\
                string_del2(str); \
           )

mktest(string_test_new, {
    const char* text = "Hello";
    string k;
    string_new(&k, 100);
    string_set(&k, text, strlen(text));

    for(int i = 0; i < strlen(text); i++) {
        assert(string_at(&k, i) == text[i], "string_new sets chars incorrectly");
    }

    string_del(&k);
})

mktest(string_test_slice, {
    string* str = string_new2(30);
    string_set(str, "Hello", 5);
    string_slice_prefix(str, 100);

    assert(string_len(str) == 0, "over slicing should make the len 0");
    string_set(str, "Hello", 5);
    string_slice_suffix(str, 100);

    assert(string_len(str) == 0, "over slicing should make the len 0");
    string_del2(str);
})

mktest(string_test_tr, {
    string* str = string_new2(100);
    string_set(str, "very cool", strlen("very cool"));
    string_tr(str, 'o', 'a');
    assert(string_find(str, 'o') != 0, "string_tr did not translate o to a");
})

mktest(string_test_slice_single, {
    string* str = string_new2(30);
    string_set(str, "Hello", 5);

    string_slice(str, 1, 3);
    t(
        "str[1:3]",
        "%zu", string_len(str), eq, 2
    );

    string_concat(str, " yes", 4);

    string_slice(str, 3, 0);
    t(
        "str[3:0]",
        "%zu", string_len(str), eq, 3
    );

    string_concat(str, " no", 3);

    string_slice(str, 4, -1);
    t(
        "str[4:-1]",
       "%zu", string_len(str), eq, 1
    );

    t(
        "string[0] is 'n'",
        "%c", string_at(str, 0), eq, 'n'
    );

    string_concat(str, "yes", 3);

    t(
        "start > str->len",
       "%d", string_slice(str, 5, 1), eq, -1
    );

    t(
        "start > end",
       "%d", string_slice(str, 2, 1), eq, -2
    );

    string_del2(str);
})

mkstrtest(basics, {
    string_set(str, "Hello", 5);
    t(
        "at(0)",
          "%c", string_at(str, 0), eq, 'H'
    );

    t(
        "len",
          "%zu", string_len(str), eq, 5
    );

    string_concat_char(str, 'a');
    t(
        "concat('a')",
        "%c", string_at(str, 5), eq, 'a'
    );

    string_set(str, "Hello", 5);

    string_mkcstr(str);
    t(
        "mkcstr (len)",
          "%zu", string_len(str), eq, 6
    );
    t(
        "mkcstr (null terminator)",
          "%c", string_at(str, 5), eq, '\0'
    );

    string_clear(str);
    t(
        "string clear",
        "%zu", string_len(str), eq, 0
    );
})

mkstrtest(concat, {
    string_del2(str);
    str = string_new2(0);
    string_concat_char(str, 'a');
    t(
        "concat and extend (len)",
        "%zu", string_len(str), eq, 1
    );
    t(
        "concat and extend (char)",
          "%c", string_at(str, 0), eq, 'a'
    );

    t(
        "concat and extend (capacity)",
          "%zu", str->allocated, gt, 0
    );

    string_concat(str, "bc", 2);

    t(
        "concat string and extend (len)",
          "%zu", string_len(str), eq, 3
    );
    t(
        "concat string and extend (capacity)",
          "%zu", str->allocated, gt, 2
    );
})

mkstrtest(from_cstr, {
    string* s = string_from_cstr("Hello", 5);
    t(
        "corret len",
          "%zu", string_len(s), eq, 5
    );

    string_del2(s);

    s = string_from_cstr("Hello", 0);
    t(
        "from \"Hello\" (0 len)",
          "%zu", string_len(s), eq, 0
    );
})

mkstrtest(cstr_includes, {
    string_set(str, "yes no maybe so", strlen("yes no maybe so"));

    const char* c_str = string_mkcstr(str);
    t(
        "'yes no maybe so' includes 'yes'",
          "%d", cstr_includes(c_str, "yes"), eq, 1
    );

    t(
        "'yes no maybe so' includes 'kk'",
          "%d", cstr_includes(c_str, "kk"), eq, 0
    );

    t(
        "'yes no maybe so' includes ''",
          "%d", cstr_includes(c_str, ""), eq, 1
    );

    //this is the same as empty string
    t(
        "'yes no maybe so' includes '\\0' (empty string)",
          "%d", cstr_includes(c_str, "\0"), eq, 1
    );

})

mkstrtest(prepend, {
    string_set(str, "no", 2);

    string* str2 = string_new2(4);
    string_set(str2, "yes ", 4);

    string_prepend(str, str2);

    t(
        "'yes ' prepended to 'no'",
          "%d", string_eq(str, "yes no"), eq, 1
    );

    string_del2(str2);

    str2 = string_new2(4);
    string_set(str2, "a", 1);
    string_prepend(str, str2);

    t(
        "'a' prepended to 'yes no'",
          "%d", string_eq(str, "ayes no"),eq, 1
    );
})

int main() {
    TEST(string_test_new);
    TEST(string_test_concat);
    TEST(string_test_basics);
    TEST(string_test_slice);
    TEST(string_test_tr);
    TEST(string_test_slice_single);
    TEST(string_test_from_cstr);
    TEST(string_test_cstr_includes);
    TEST(string_test_prepend);

    printf("\n--------------------\n\x1b[32;1m[✓]\x1b[0m %zu \x1b[31;1m[✗]\x1b[0m %zu\n", pass_count, err_count);
}
