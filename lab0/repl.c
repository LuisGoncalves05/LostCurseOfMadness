#include <stdio.h>
#include <string.h>

#define BIT(b) (1 << (b))
#define BYTE(b) (0xFF << ((b) << 3))

int main(void) {
    while (42) {
        unsigned res = 0;
        char *op;
        unsigned a, b;

        scanf("%s %u %u", op, &a, &b);

        if (strcmp(op, "and") == 0) {
            res = a & b;
        } else if (strcmp(op, "or") == 0) {
            res = a | b;
        } else if (strcmp(op, "set") == 0) {
            res = a | BIT(b);
        } else if (strcmp(op, "unset") == 0) {
            res = a & ~BIT(b);
        } else if (strcmp(op, "byte") == 0) {
            res = (a & BYTE(b)) >> (b << 3);
        } else {
            break;
        }

        printf("%i\n", res);
    }

    return 0;
}