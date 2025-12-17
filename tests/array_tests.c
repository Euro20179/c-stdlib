#include "../tests.h"
#include "../array.h"
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
        assert(*cur == 3, "current item in foreach is not 3");
    })

    array_clear(arr);
    for(x = 0; x < 10; x++) {
        array_append(arr, &x);
    }

    int i = 0;
    foreach(int*, arr, array, {
        assert(*cur == i++, "current item in foreach is not sequential from 0..10");
    })
})

mkarrtest(clear, const char*, {
    array_append(arr, "Hello");

    t("array len is correct", "%lu",
            array_len(arr), eq, 1);

    array_clear(arr);

    t("array was cleared", "%lu",
            array_len(arr), eq, 0);
})

int main() {
    TEST(test_clear);
    TEST(test_iter);
    printf("\n--------------------\n\x1b[32;1m[✓]\x1b[0m %zu \x1b[31;1m[✗]\x1b[0m %zu\n", pass_count, err_count);
}
