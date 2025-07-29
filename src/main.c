#include "search.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: rgrep '<regex pattern>' <file or location>\n");
        return 1;
    }

    int res = search(argv[1], argv[2]);

    return res;
}
