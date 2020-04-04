#include "permutation.h"

bitset<32> permutation::do_P(bitset<32> block)
{
    bitset<32> res;

    for (size_t i = 0; i < 32; i++)
        res[i] = block[P[i] - 1];

    return res;
}

bitset<64> permutation::do_IP(bitset<64> key)
{
    bitset<64> key_IP;

    for (size_t i = 0; i < 64; i++)
        key_IP[i] = key[IP[i] - 1];

    return key_IP;
}

bitset<64> permutation::do_IP_R(bitset<64> key)
{
    bitset<64> key_IP_R;

    for (size_t i = 0; i < 64; i++)
        key_IP_R[i] = key[IP_R[i] - 1];

    return key_IP_R;
}

bitset<48> permutation::do_EP(bitset<32> key)
{
    bitset<48> key_EP;

    for (size_t i = 0; i < 48; i++)
        key_EP[i] = key[EP[i] - 1];

    return key_EP;
}

bitset<56> permutation::circular_left_shift(bitset<56> &key)
{
    bitset<1> tmp;
    bitset<56> res;

    int i = 0;
    res[27] = key[0];
    for (i = 0; i < 27; i++)
        res[i] = key[i + 1];

    res[55] = key[28];
    for (i = 28; i < 55; i++)
        res[i] = key[i + 1];

    return res;
}

const int permutation::s_box(size_t box, int position)
{
    switch (box)
    {
    case 1:
        return (s1[position]);
        break;

    case 2:
        return (s2[position]);
        break;

    case 3:
        return (s3[position]);
        break;

    case 4:
        return (s4[position]);
        break;

    case 5:
        return (s5[position]);
        break;

    case 6:
        return (s6[position]);
        break;

    case 7:
        return (s7[position]);
        break;

    case 8:
        return (s8[position]);
        break;

    default:
        perror("s_box invalide");
        exit(0);
    }
}

bitset<56> permutation::do_CP1(bitset<64> K)
{
    bitset<56> k56;

    for (size_t i = 0; i < 56; i++)
        k56[i] = K[CP1[i] - 1];

    return k56;
}

bitset<48> permutation::do_CP2(bitset<56> K)
{
    bitset<48> k48;

    for (size_t i = 0; i < 48; i++)
        k48[i] = K[CP2[i] - 1];

    return k48;
}
