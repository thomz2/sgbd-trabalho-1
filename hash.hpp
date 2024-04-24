#include <math.h>
#pragma once

int hash(int input, int bits = 3) {
    return input & ((1 << bits) - 1);
    //input and 0b_111
}

// 1 << 3 = 1000 - 1 = 0111  