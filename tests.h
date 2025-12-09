#include <stdint.h>

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

#define assert(expr, msg) if(!(expr)) { \
    printf("\t\x1b[31;1m[✗]\x1b[0m assert: " #expr "\n\t\t%s\n", msg); \
    err_count++; \
    return msg; \
} else { \
    printf("\t\x1b[32;1m[✓]\x1b[0m assert: " #expr "\n"); \
    pass_count++;\
}

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

/*
 * vim: ft=c
 * */
