#include "permutation.h"

bitset<10> permutation::do_P10(bitset<10> key)
{
    bitset<10> key_P10;

    for (size_t i = 0; i < 10; i++)
        key_P10[i] = key[P10[i] - 1];

    return key_P10;
}

bitset<8> permutation::do_P8(bitset<10> key)
{
    bitset<8> key_P8;

    for (size_t i = 0; i < 8; i++)
        key_P8[i] = key[P8[i] - 1];

    return key_P8;
}

bitset<4> permutation::do_P4(bitset<4> key)
{
    bitset<4> key_P4;

    for (size_t i = 0; i < 4; i++)
        key_P4[i] = key[3 - (P4[i] - 1)];

    return key_P4;
}

bitset<8> permutation::do_IP(bitset<8> key)
{
    bitset<8> key_IP;

    for (size_t i = 0; i < 8; i++)
        key_IP[i] = key[IP[i] - 1];

    return key_IP;
}

bitset<8> permutation::do_IP_R(bitset<8> key)
{
    bitset<8> key_IP_R;

    for (size_t i = 0; i < 8; i++)
        key_IP_R[i] = key[IP_R[i] - 1];

    return key_IP_R;
}

bitset<8> permutation::do_EP(bitset<4> key)
{
    bitset<8> key_EP;

    for (size_t i = 0; i < 8; i++)
        key_EP[i] = key[EP[i] - 1];

    return key_EP;
}

void permutation::circular_left_shift(bitset<10> &key)
{
    bitset<1> tmp;
    int i;

    tmp[0] = key[0];
    for (i = 0; i < 4; i++)
        key[i] = key[i + 1];

    key[4] = tmp[0];

    tmp[0] = key[5];
    for (i = 5; i < 9; i++)
        key[i] = key[i + 1];

    key[9] = tmp[0];
}
