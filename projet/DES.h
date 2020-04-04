#ifndef DES_H
#define DES_H

#include "permutation.h"
#include "time.h"
#include "rsa.h"

// Converti un char en une chaine de caractères de booléens correspondant à sa valeur
string char_to_booleans(char);

// Transforme un tableau de 64 booléens en chaine de 8 caractères correspondants
string bitset_to_string(bitset<64>);

// Convertie une chaîne de caractères en un vecteur de tableaux de booléens correspondant
bitset<64> message_to_bitsets(string, vector<bitset<64>> *);

// Transforme la clé DES depuis une chaine de caractères de booléens en <bitset>
bitset<64> keystring_to_bitset(string);

// Fonction réalisant le chiffrement d'un block block avec une clé
bitset<64> encryption(bitset<64>, bitset<64>);

// Fonction réalisant le déchiffrement d'un block avec une clé
bitset<64> decryption(bitset<64>, bitset<64>);

// Création des différentes clés de DES
void key_generation(bitset<64>, bitset<48> *);

bitset<32> substitution(bitset<48>);
void fk(bitset<32> &, bitset<32> &, bitset<48>);

// Génère 64 booléen aléatoirement pour servir de clé
string generate_DES_key();

// Permet d'afficher les bits d'un <bitset>
#define print_bits(text, bits)              \
    {                                       \
        string x = bits.to_string();        \
        reverse(x.begin(), x.end());        \
        cout << text << " : " << x << "\n"; \
    }

#endif