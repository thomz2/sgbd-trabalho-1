#pragma once

int hash(int input) {
    return input & 7;
    //input and 0b_111
}