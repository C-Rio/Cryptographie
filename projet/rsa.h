#ifndef RSA_H
#define RSA_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <exception>

#define PADDING RSA_PKCS1_PADDING

// Génère les clés RSA de 2048 bits et les enregistre dans public.pem et private.pem
void generate_key();

// Retourne la clé publique contenue dans le fichier dont le nom est passé en paramètre
RSA *load_public_key(char *);

// Retourne la clé privée contenue dans le fichier dont le nom est passé en paramètre
RSA *load_private_key(char *);

// Copie la clé publique dans le buffer passé en paramètre
void write_public_key_to_buffer(char *);

// Enregistre dans un fichier la clé publique passsé en paramètre
void write_public_key_to_file(char *);

#endif