/**
 * desc: 公共方法测试
 * file: utils_test.c
 *
 * author:  myw31415926
 * date:    20181005
 * version: V0.1
 *
 * the closer you look, the less you see
 */

#include "utils.h"

/* 测试：产生0~1之间的随机数，需要先srand()初始化随机种子 */
void test_rand_1()
{
    fprintf(stdout, "### test start: test_rand_1() ###\n");
    srand(time(NULL));

    for (int i = 0; i < 10; ++i) {
        fprintf(stdout, "\t%d: %0.6f", i, rand_1());
    }

    fprintf(stdout, "### test end: test_rand_1() ###\n");
}

/* 测试：产生0~n之间的随机整数，需要先srand()初始化随机种子 */
void test_rand_n(int n)
{
    fprintf(stdout, "### test start: test_rand_n() ###\n");
    srand(time(NULL));

    for (int i = 0; i < 10; ++i) {
        fprintf(stdout, "\t%d: %d", i, rand_n(1000));
    }

    fprintf(stdout, "### test end: test_rand_n() ###\n");
}

/* 测试：打印buf中的二进制数据，每行显示16个数据 */
void test_print_binary_data()
{
    int buf[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    print_binary_data((unsigned char*)buf, sizeof(buf));
}

int main(int argc, char *argv[])
{
    fprintf(stdout, "************* test_rand_1() *************\n");
    test_rand_1();

    fprintf(stdout, "************* test_rand_n() *************\n");
    test_rand_n();

    fprintf(stdout, "************* test_print_binary_data() *************\n");
    test_print_binary_data();

    return 0;
}
