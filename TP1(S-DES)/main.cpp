#include "permutation.h"

bitset<8> fk(bitset<8>, bitset<8>);
bitset<8> encryption(bitset<10>, bitset<8>);
bitset<8> decryption(bitset<10>, bitset<8>);
void key_generation(bitset<10>, bitset<8> &, bitset<8> &);

// Permet d'afficher les bits d'un <bitset>
#define print_bits(text, bits)              \
    {                                       \
        string x = bits.to_string();        \
        reverse(x.begin(), x.end());        \
        cout << text << " : " << x << "\n"; \
    }

#define GET_ROW_BITS(bits) ((bits[3] + bits[0]))
#define GET_COL_BITS(bits) ((bits[2] + bits[1]))

// Permet l'accès à S0 et S1 de façon matricielle
#define S_ACCESS(b, row, col) ((b == 1) ? (S1[4 * row + col]) : (S0[4 * row + col]))

// On déclare un objet permutation, celui-ci nous permettra de réaliser l'ensemble des permutations nécessaires au S-DES (P8, P10, ...)
permutation permut;

int main()
{
    string key_string;
    string block_string;

    // On demande à l'utilisateur d'entrer la clé et le block
    // On inverse la chaine car bitset accède aux éléments en commençant par le bit de poids faible
    cout << "Key : ";
    getline(cin, key_string);
    reverse(key_string.begin(), key_string.end());
    bitset<10> key(key_string);

    cout << "Block : ";
    getline(cin, block_string);
    reverse(block_string.begin(), block_string.end());
    bitset<8> block(block_string);

    // On réalise le chiffrement et on affiche le block chiffré
    bitset<8> ENC = encryption(key, block);
    print_bits("Encrypted", ENC);

    // On déchiffre le bloc que l'on vient de calculer et on affiche le résultat
    bitset<8> DEC = decryption(key, ENC);
    print_bits("Decrypted", DEC);

    return 0;
}

// Fonction réalisant le chiffrement du block "block" avec la clé "key"
bitset<8> encryption(bitset<10> key, bitset<8> block)
{
    bitset<8> K1, K2;
    key_generation(key, K1, K2);
    bitset<8> block_IP = permut.do_IP(block);
    bitset<8> RES_K1 = fk(block_IP, K1);
    bitset<8> RES_SWITCH(RES_K1.to_string().substr(4, 4) + RES_K1.to_string().substr(0, 4));
    bitset<8> RES_K2 = fk(RES_SWITCH, K2);
    return (permut.do_IP_R(RES_K2));
}

// Réalise fk
bitset<8> fk(bitset<8> input, bitset<8> K)
{
    bitset<4> block_R(input.to_string().substr(0, 4));
    bitset<4> block_L(input.to_string().substr(4, 4));

    bitset<8> block_R_EP = permut.do_EP(block_R);

    bitset<8> block_XOR_K = (block_R_EP ^= K);

    bitset<4> block_XOR_K_R(block_XOR_K.to_string().substr(0, 4));
    bitset<4> block_XOR_K_L(block_XOR_K.to_string().substr(4, 4));

    bitset<2> S0_ROW(GET_ROW_BITS(block_XOR_K_L));
    unsigned int row_S0 = S0_ROW.to_ulong();

    bitset<2> S0_COL(GET_COL_BITS(block_XOR_K_L));
    unsigned int col_S0 = S0_COL.to_ulong();

    bitset<2> S1_ROW(GET_ROW_BITS(block_XOR_K_R));
    unsigned int row_S1 = S1_ROW.to_ulong();

    bitset<2> S1_COL(GET_COL_BITS(block_XOR_K_R));
    unsigned int col_S1 = S1_COL.to_ulong();

    bitset<2> S0_OUTPUT(S_ACCESS(0, row_S0, col_S0));
    bitset<2> S1_OUTPUT(S_ACCESS(1, row_S1, col_S1));

    string S0_S1_bits = S0_OUTPUT.to_string() + S1_OUTPUT.to_string();
    bitset<4> S_OUTPUT(S0_S1_bits);

    bitset<4> block_P4(permut.do_P4(S_OUTPUT));

    bitset<4> block_XOR_P4 = (block_P4 ^= block_L);

    string XOR_P4_block_R = block_R.to_string() + block_XOR_P4.to_string();
    bitset<8> RES(XOR_P4_block_R);

    return (RES);
}

// Calcul K1 et K2 à partir de la clé "key"
void key_generation(bitset<10> key, bitset<8> &K1, bitset<8> &K2)
{
    bitset<10> key_P10 = permut.do_P10(key);
    permut.circular_left_shift(key_P10);
    K1 = permut.do_P8(key_P10);

    permut.circular_left_shift(key_P10);
    permut.circular_left_shift(key_P10);
    K2 = permut.do_P8(key_P10);
}

// Fonction réalisant le déchiffrement du block chiffré "cipher" avec la clé "key"
bitset<8> decryption(bitset<10> key, bitset<8> cipher)
{
    bitset<8> K1, K2;
    key_generation(key, K1, K2);

    bitset<8> cipher_IP = permut.do_IP(cipher);
    bitset<8> RES_K2 = fk(cipher_IP, K2);
    bitset<8> RES_SWITCH(RES_K2.to_string().substr(4, 4) + RES_K2.to_string().substr(0, 4));
    bitset<8> RES_K1 = fk(RES_SWITCH, K1);
    return (permut.do_IP_R(RES_K1));
}