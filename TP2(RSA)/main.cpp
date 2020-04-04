#include <stdio.h>
#include <iostream>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <exception>

#define PADDING RSA_PKCS1_PADDING

void generate_key()
{
    size_t pri_len;
    size_t pub_len;
    char *pri_key;
    char *pub_key;

    int ret = 0;
    RSA *r = NULL;
    BIGNUM *bne = NULL;
    BIO *bp_public = NULL, *bp_private = NULL;
    int bits = 2048;
    unsigned long e = RSA_F4;

    RSA *pub_rsa = NULL;
    RSA *p_rsa = NULL;

    BIO *pbkeybio = NULL;
    BIO *pkeybio = NULL;

    bne = BN_new();
    ret = BN_set_word(bne, e);

    r = RSA_new();
    ret = RSA_generate_key_ex(r, bits, bne, NULL);

    bp_public = BIO_new_file("public.pem", "w+");
    ret = PEM_write_bio_RSAPublicKey(bp_public, r);

    bp_private = BIO_new_file("private.pem", "w+");
    ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);

    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    RSA_free(r);
    BN_free(bne);
}

RSA *load_public_key()
{
    RSA *rsa;
    rsa = RSA_new();

    BIO *bp_public = NULL;

    bp_public = BIO_new_file("public.pem", "r");
    rsa = PEM_read_bio_RSAPublicKey(bp_public, &rsa, NULL, NULL);

    // RSA_print_fp(stdout, rsa, 3);
    return rsa;
}

RSA *load_private_key()
{
    RSA *rsa;
    rsa = RSA_new();

    BIO *bp_private = NULL;

    bp_private = BIO_new_file("private.pem", "r");
    rsa = PEM_read_bio_RSAPrivateKey(bp_private, &rsa, NULL, NULL);

    // RSA_print_fp(stdout, rsa, 3);
    return rsa;
}

int main(int argc, char *argv[])
{
    char text[200] = "1110011101100110010001101101000111000111001010011001110000110100";
    unsigned char encrypted[4098] = {};
    unsigned char decrypted[4098] = {};

    generate_key();
	printf("Clés RSA générées et enregistrées\n\n");
	
    RSA *rsa_private, *rsa_public;
    rsa_public = load_public_key();
    rsa_private = load_private_key();
	printf("Clés RSA lues depuis les fichiers public.pem et private.pem \n\n");
    
    printf("\nClé publique : \n");
    RSA_print_fp(stdout, rsa_public, 3);

    printf("\nClé privé : \n");
    RSA_print_fp(stdout, rsa_private, 3);

    int res = RSA_public_encrypt(strlen(text), (const unsigned char *)text, encrypted, rsa_public, PADDING);

    printf("\nTexte chiffré : \n");
	for(int i =0; i < res; i++)
	{
			printf("%d",encrypted[i]);
	}
	printf("\n");


    RSA_private_decrypt(res, encrypted, decrypted, rsa_private, PADDING);

    printf("\nTexte déchiffré :\n\n%s\n", decrypted);

    return 0;
}