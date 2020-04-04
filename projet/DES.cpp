#include "DES.h"

// On déclare un objet permutation, celui-ci nous permettra de réaliser l'ensemble des permutations nécessaires au S-DES (P8, P10, ...)
permutation permut;

string bitset_to_string(bitset<64> message)
{
    string message_string = "";
    for (size_t j = 0; j < 8; j++)
    {
        char c = 0;
        for (int i = 0; i < 8; i++)
            c += message[j * 8 + i] * pow(2, i);
        message_string += c;
    }
    reverse(message_string.begin(), message_string.end());
    return message_string;
}

string char_to_booleans(char c)
{
    bitset<8> bit_char(c);
    return bit_char.to_string();
}

bitset<64> message_to_bitsets(string message_string, vector<bitset<64>> *vect)
{
    unsigned int i = 0, j = 0;

    unsigned int nb_messages = (message_string.length() / 8);
    if ((message_string.length() % 8 != 0) || (strcmp(message_string.c_str(), "") == 0))
        nb_messages++;

    string boolean_string;
    for (i = 0; i < nb_messages - 1; i++)
    {
        boolean_string = "";

        for (j = 0; j < 8; j++)
            boolean_string += char_to_booleans(message_string[i * 8 + j]);

        bitset<64> mess(boolean_string);
        vect->push_back(mess);
    }

    boolean_string = "";
    char c = (char)0;

    for (j = 0; j < message_string.length() - i * 8; j++)
        boolean_string += char_to_booleans(message_string[i * 8 + j]);

    while (j < 8)
    {
        boolean_string += char_to_booleans(c);
        j++;
    }

    bitset<64> mess(boolean_string);
    vect->push_back(mess);

    bitset<64> nb_messages_bitset(nb_messages);
    return nb_messages_bitset;
}

bitset<64> keystring_to_bitset(string key)
{
    reverse(key.begin(), key.end());
    bitset<64> key_bitset(key);
    return (key_bitset);
}

bitset<64> encryption(bitset<64> key, bitset<64> block)
{
    bitset<48> *keys = (bitset<48> *)malloc(sizeof(bitset<48>) * 16);
    key_generation(key, keys);

    bitset<64> block_IP = permut.do_IP(block);

    bitset<32> block_R(block_IP.to_string().substr(0, 32));
    bitset<32> block_L(block_IP.to_string().substr(32, 32));

    for (size_t i = 0; i < 16; i++)
        fk(block_R, block_L, keys[i]);

    bitset<64> block_rounded(block_L.to_string() + block_R.to_string());

    return (permut.do_IP_R(block_rounded));
}

void fk(bitset<32> &block_R, bitset<32> &block_L, bitset<48> K)
{
    bitset<32> block_R_copy = block_R;

    bitset<48> block_R_EP = permut.do_EP(block_R);
    bitset<48> block_XOR_K = (block_R_EP ^= K);
    bitset<32> block_s_boxes = substitution(block_XOR_K);
    bitset<32> block_P = permut.do_P(block_s_boxes);
    block_R = (block_P ^= block_L);

    block_L = block_R_copy;
}

void key_generation(bitset<64> key, bitset<48> *keys)
{
    bitset<56> key_CP1(permut.do_CP1(key));
    bitset<56> *shifted_keys = (bitset<56> *)malloc(sizeof(bitset<56>) * 16);
    shifted_keys[0] = permut.circular_left_shift(key_CP1);
    keys[0] = permut.do_CP2(shifted_keys[0]);

    for (size_t n = 1; n < 16; n++)
    {
        shifted_keys[n] = permut.circular_left_shift(shifted_keys[n - 1]);

        if (LS[n] == 2)
            shifted_keys[n] = permut.circular_left_shift(shifted_keys[n]);

        keys[n] = permut.do_CP2(shifted_keys[n]);
    }
}

bitset<64> decryption(bitset<64> key, bitset<64> cipher)
{
    bitset<48> *keys = (bitset<48> *)malloc(sizeof(bitset<48>) * 16);
    key_generation(key, keys);

    bitset<64> block_IP = permut.do_IP(cipher);

    bitset<32> block_R(block_IP.to_string().substr(0, 32));
    bitset<32> block_L(block_IP.to_string().substr(32, 32));

    for (int i = 15; i >= 0; i--)
        fk(block_R, block_L, keys[i]);

    bitset<64> block_rounded(block_L.to_string() + block_R.to_string());

    return (permut.do_IP_R(block_rounded));
}

bitset<32> substitution(bitset<48> input)
{
    string sub;

    for (size_t i = 0; i < 8; i++)
    {
        string srow = to_string(input[i * 6]) + to_string(input[i * 6 + 5]);
        string scol = to_string(input[i * 6 + 1]) + to_string(input[i * 6 + 2]) + to_string(input[i * 6 + 3]) + to_string(input[i * 6 + 4]);

        int row = stoi(srow, nullptr, 2);
        int col = stoi(scol, nullptr, 2);

        bitset<4> S_OUTPUT(permut.s_box(i + 1, 16 * row + col));
        sub += S_OUTPUT.to_string();
    }

    reverse(sub.begin(), sub.end());
    bitset<32> res(sub);
    return (res);
}

string generate_DES_key()
{
    srand(time(NULL));
    string key = std::string();
    for (int i = 0; i < 64; i++)
        key += (char)(rand() % 2) + '0';

    return (key);
}
