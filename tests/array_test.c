#include "../tests.h"
#include "../array.h"
#include "../string.h"

#include <string.h>
#include <stdbool.h>
#include <stdio.h>


#define mkarrtest(name, arrty, body)\
        mktest(\
                test_##name,\
                array* arr = array_new2(30, sizeof (arrty));\
                body\
                array_del2(arr); \
           )

mkarrtest(iter, int*, {
    int x = 3;
    array_append(arr, &x);
    array_append(arr, &x);
    foreach(int*, arr, array, {
        assert(**((int**)cur) == 3, "current item in foreach is not 3");
    })

    array_clear(arr);
    for(x = 0; x < 10; x++) {
        array_append(arr, &x);
    }

    int i = 0;
    foreach(int*, arr, array, {
        assert(**((int**)cur) == 10, "current item in foreach is not sequential from 0..10");
    })
})

mkarrtest(double_pointer, int**, {
    int x = 3;
    int* y = &x;
    array_append(arr, &y);
    t("array at 0 is 3", "%d",
            **(int**)*array_at(arr, 0), eq, 3);
})

mkarrtest(non_pointer, int, {
    array_append(arr, (void*)3);
    t("array at 0 is 3", "%d",
            *(int*)array_at(arr, 0), eq, 3);
})

mkarrtest(clear, const char*, {
    array_append(arr, "Hello");

    t("array len is correct", "%lu",
            array_len(arr), eq, 1);

    array_clear(arr);

    t("array was cleared", "%lu",
            array_len(arr), eq, 0);
})


mkarrtest(string_arr, string*, {
    string* k = string_new2(30);

    string_set(k, "Hello", 5);

    array_append(arr, k);
    array_append(arr, k);

    t("string at 1 says 'Hello'", "%d",
            strcmp(string_mkcstr(*array_at(arr, 0)), "Hello"), eq, 0);
})
int main() {
    TEST(test_clear);
    TEST(test_iter);
    TEST(test_double_pointer);
    TEST(test_non_pointer);
    TEST(test_string_arr);
    printf("\n--------------------\n\x1b[32;1m[✓]\x1b[0m %zu \x1b[31;1m[✗]\x1b[0m %zu\n", pass_count, err_count);
}
