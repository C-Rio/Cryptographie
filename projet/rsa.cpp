#include "rsa.h"

void generate_key()
{
    RSA *r = NULL;
    BIGNUM *bne = NULL;
    BIO *bp_public = NULL, *bp_private = NULL;
    int bits = 2048;
    unsigned long e = RSA_F4;

    bne = BN_new();
    BN_set_word(bne, e);

    r = RSA_new();
    RSA_generate_key_ex(r, bits, bne, NULL);

    bp_public = BIO_new_file("public.pem", "w+");
    PEM_write_bio_RSAPublicKey(bp_public, r);

    bp_private = BIO_new_file("private.pem", "w+");
    PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);

    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    RSA_free(r);
    BN_free(bne);
}

RSA *load_public_key(char *filename)
{
    RSA *rsa;
    rsa = RSA_new();

    BIO *bp_public = NULL;

    bp_public = BIO_new_file(filename, "r");
    rsa = PEM_read_bio_RSAPublicKey(bp_public, &rsa, NULL, NULL);

    return rsa;
}

RSA *load_private_key(char *filename)
{
    RSA *rsa;
    rsa = RSA_new();

    BIO *bp_private = NULL;

    bp_private = BIO_new_file(filename, "r");
    rsa = PEM_read_bio_RSAPrivateKey(bp_private, &rsa, NULL, NULL);

    return rsa;
}

void write_public_key_to_buffer(char *buffer)
{
    FILE *key = fopen("./public.pem", "rb");
    fseek(key, 0, SEEK_END);
    long fsize = ftell(key);
    fseek(key, 0, SEEK_SET);
    fread(buffer, fsize, 1, key);
    fclose(key);
}

void write_public_key_to_file(char *key)
{
    FILE *fp = fopen("./public_key_server.pem", "wb");
    if (fp != NULL)
    {
        fputs(key, fp);
        fclose(fp);
    }
}
