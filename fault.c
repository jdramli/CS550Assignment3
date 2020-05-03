#include <stdlib.h>

int main(void)
{
    while (1) {
        char* p = malloc(1 * 1024 * 1024 * 1024);
        for (int i = 0; i < 1024; ++i) {
            p[i * 1024 * 1024] = 0;     /* touch the pages */
        }
    }
    return 0;
}
