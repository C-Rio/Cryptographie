#ifndef SOCKET_H
#define SOCKET_H

#include "DES.h"

// Taille du buffer (multiple de 64)
#define BUFFER_SIZE 640

// Modes de communications
enum mode_comm
{
    ECB,
    CBC,
    autre
};

// Permet d'écrire dans le socket et de quitter en cas d'erreur
#define socket_write(n, sockfd, buffer, size) \
    {                                         \
        n = write(sockfd, buffer, size);      \
        if (n < 0)                            \
            error("Erreur écriture socket");  \
    }

// Permet de lire dans le socket et de quitter en cas d'erreur
#define socket_read(n, sockfd, buffer, size) \
    {                                        \
        n = read(sockfd, buffer, size);      \
        if (n < 0)                           \
            error("Erreur lecture socket");  \
    }

// Permet de chiffrer et d'envoyer la chaine de caractères "message" via le mode ECB
#define send_ECB(message, socket, DES_key)                                                                                               \
    {                                                                                                                                    \
        vector<bitset<64>> vec;                                                                                                          \
        bitset<64> nb_messages = message_to_bitsets(message, &vec);                                                                      \
                                                                                                                                         \
        bitset<64> enc_nb_messages = encryption(DES_key, nb_messages);                                                                   \
        bzero(buffer, BUFFER_SIZE);                                                                                                      \
        sprintf((char *)buffer, "%s", enc_nb_messages.to_string().c_str());                                                              \
        socket_write(n, socket, buffer, BUFFER_SIZE);                                                                                    \
                                                                                                                                         \
        string message_string = "";                                                                                                      \
                                                                                                                                         \
        for (unsigned long i = 0; i < nb_messages.to_ulong(); i++)                                                                       \
        {                                                                                                                                \
            bitset<64> enc_message = encryption(DES_key, vec[i]);                                                                        \
            message_string += enc_message.to_string();                                                                                   \
        }                                                                                                                                \
                                                                                                                                         \
        if (nb_messages.to_ulong() * 64 > BUFFER_SIZE)                                                                                   \
        {                                                                                                                                \
            unsigned long i = 0;                                                                                                         \
            while (i * BUFFER_SIZE < (nb_messages.to_ulong() * 64 - BUFFER_SIZE))                                                        \
            {                                                                                                                            \
                bzero(buffer, BUFFER_SIZE);                                                                                              \
                sprintf((char *)buffer, "%s", message_string.substr(i *BUFFER_SIZE, BUFFER_SIZE).c_str());                               \
                socket_write(n, socket, buffer, BUFFER_SIZE);                                                                            \
                i++;                                                                                                                     \
            }                                                                                                                            \
            if (i * BUFFER_SIZE != nb_messages.to_ulong() * 64)                                                                          \
            {                                                                                                                            \
                bzero(buffer, BUFFER_SIZE);                                                                                              \
                sprintf((char *)buffer, "%s", message_string.substr(i *BUFFER_SIZE, message_string.length() - i * BUFFER_SIZE).c_str()); \
                socket_write(n, socket, buffer, BUFFER_SIZE);                                                                            \
            }                                                                                                                            \
        }                                                                                                                                \
        else                                                                                                                             \
        {                                                                                                                                \
            bzero(buffer, BUFFER_SIZE);                                                                                                  \
            sprintf((char *)buffer, "%s", message_string.c_str());                                                                       \
            socket_write(n, socket, buffer, BUFFER_SIZE);                                                                                \
        }                                                                                                                                \
    }

// Permet de récuperer et déchiffrer la chaine de caractères "message" via le mode ECB
#define receive_ECB(message, socket, DES_key)                                                \
    {                                                                                        \
        message = "";                                                                        \
        bzero(buffer, BUFFER_SIZE);                                                          \
        socket_read(n, socket, buffer, BUFFER_SIZE);                                         \
                                                                                             \
        string enc_nb_messages_string((const char *)buffer);                                 \
        bitset<64> enc_nb_messages_bitset(enc_nb_messages_string);                           \
        bitset<64> nb_messages_bitset = decryption(DES_key, enc_nb_messages_bitset);         \
        unsigned long nb_messages = nb_messages_bitset.to_ulong();                           \
                                                                                             \
        if (nb_messages * 64 > BUFFER_SIZE)                                                  \
        {                                                                                    \
            unsigned long i = 0;                                                             \
            while (i < nb_messages)                                                          \
            {                                                                                \
                bzero(buffer, BUFFER_SIZE);                                                  \
                socket_read(n, socket, buffer, BUFFER_SIZE);                                 \
                string enc_bool_message_string((const char *)buffer);                        \
                                                                                             \
                for (unsigned long j = 0; (j < BUFFER_SIZE / 64) && (i < nb_messages); j++)  \
                {                                                                            \
                    bitset<64> enc_bool_message(enc_bool_message_string.substr(j * 64, 64)); \
                    message += bitset_to_string(decryption(DES_key, enc_bool_message));      \
                    i++;                                                                     \
                }                                                                            \
            }                                                                                \
        }                                                                                    \
        else                                                                                 \
        {                                                                                    \
            bzero(buffer, BUFFER_SIZE);                                                      \
            socket_read(n, socket, buffer, BUFFER_SIZE);                                     \
                                                                                             \
            string enc_bool_message_string((const char *)buffer);                            \
                                                                                             \
            for (unsigned long i = 0; i < nb_messages; i++)                                  \
            {                                                                                \
                bitset<64> enc_bool_message(enc_bool_message_string.substr(i * 64, 64));     \
                message += bitset_to_string(decryption(DES_key, enc_bool_message));          \
            }                                                                                \
        }                                                                                    \
    }

// Permet de chiffrer et d'envoyer la chaine de caractères "message" via le mode CBC
#define send_CBC(message, socket, DES_key, init_vector)                                                                                  \
    {                                                                                                                                    \
        vector<bitset<64>> vec;                                                                                                          \
        bitset<64> nb_messages = message_to_bitsets(message, &vec);                                                                      \
                                                                                                                                         \
        bitset<64> enc_nb_messages = encryption(DES_key, nb_messages);                                                                   \
        bzero(buffer, BUFFER_SIZE);                                                                                                      \
        sprintf((char *)buffer, "%s", enc_nb_messages.to_string().c_str());                                                              \
        socket_write(n, socket, buffer, BUFFER_SIZE);                                                                                    \
                                                                                                                                         \
        string message_string = "";                                                                                                      \
                                                                                                                                         \
        bitset<64> enc_message = encryption(DES_key, (vec[0] ^= init_vector));                                                           \
        message_string += enc_message.to_string();                                                                                       \
                                                                                                                                         \
        for (unsigned long i = 1; i < nb_messages.to_ulong(); i++)                                                                       \
        {                                                                                                                                \
            enc_message = encryption(DES_key, (vec[i] ^= enc_message));                                                                  \
            message_string += enc_message.to_string();                                                                                   \
        }                                                                                                                                \
                                                                                                                                         \
        if (nb_messages.to_ulong() * 64 > BUFFER_SIZE)                                                                                   \
        {                                                                                                                                \
            unsigned long i = 0;                                                                                                         \
            while (i * BUFFER_SIZE < (nb_messages.to_ulong() * 64 - BUFFER_SIZE))                                                        \
            {                                                                                                                            \
                bzero(buffer, BUFFER_SIZE);                                                                                              \
                sprintf((char *)buffer, "%s", message_string.substr(i *BUFFER_SIZE, BUFFER_SIZE).c_str());                               \
                socket_write(n, socket, buffer, BUFFER_SIZE);                                                                            \
                i++;                                                                                                                     \
            }                                                                                                                            \
            if (i * BUFFER_SIZE != nb_messages.to_ulong() * 64)                                                                          \
            {                                                                                                                            \
                bzero(buffer, BUFFER_SIZE);                                                                                              \
                sprintf((char *)buffer, "%s", message_string.substr(i *BUFFER_SIZE, message_string.length() - i * BUFFER_SIZE).c_str()); \
                socket_write(n, socket, buffer, BUFFER_SIZE);                                                                            \
            }                                                                                                                            \
        }                                                                                                                                \
        else                                                                                                                             \
        {                                                                                                                                \
            bzero(buffer, BUFFER_SIZE);                                                                                                  \
            sprintf((char *)buffer, "%s", message_string.c_str());                                                                       \
            socket_write(n, socket, buffer, BUFFER_SIZE);                                                                                \
        }                                                                                                                                \
    }

// Permet de récuperer et déchiffrer la chaine de caractères "message" via le mode CBC
#define receive_CBC(message, socket, DES_key, init_vector)                                          \
    {                                                                                               \
        message = "";                                                                               \
        bzero(buffer, BUFFER_SIZE);                                                                 \
        socket_read(n, socket, buffer, BUFFER_SIZE);                                                \
                                                                                                    \
        string enc_nb_messages_string((const char *)buffer);                                        \
        bitset<64> enc_nb_messages_bitset(enc_nb_messages_string);                                  \
        bitset<64> nb_messages_bitset = decryption(DES_key, enc_nb_messages_bitset);                \
        unsigned long nb_messages = nb_messages_bitset.to_ulong();                                  \
                                                                                                    \
        if (nb_messages * 64 > BUFFER_SIZE)                                                         \
        {                                                                                           \
            unsigned long i = 0;                                                                    \
            bitset<64> old_cipher;                                                                  \
            while (i < nb_messages)                                                                 \
            {                                                                                       \
                bzero(buffer, BUFFER_SIZE);                                                         \
                socket_read(n, socket, buffer, BUFFER_SIZE);                                        \
                string enc_bool_message_string((const char *)buffer);                               \
                                                                                                    \
                for (unsigned long j = 0; (j < BUFFER_SIZE / 64) && (i < nb_messages); j++)         \
                {                                                                                   \
                    if (i == 0 && j == 0)                                                           \
                    {                                                                               \
                        bitset<64> enc_bool_message(enc_bool_message_string.substr(0, 64));         \
                        old_cipher = enc_bool_message;                                              \
                        bitset<64> XOR = decryption(DES_key, enc_bool_message) ^= init_vector;      \
                        message += bitset_to_string(XOR);                                           \
                    }                                                                               \
                    else                                                                            \
                    {                                                                               \
                        bitset<64> enc_bool_message(enc_bool_message_string.substr(j * 64, 64));    \
                        bitset<64> XOR = decryption(DES_key, enc_bool_message) ^= old_cipher;       \
                        message += bitset_to_string(XOR);                                           \
                        old_cipher = enc_bool_message;                                              \
                    }                                                                               \
                    i++;                                                                            \
                }                                                                                   \
            }                                                                                       \
        }                                                                                           \
        else                                                                                        \
        {                                                                                           \
            bzero(buffer, BUFFER_SIZE);                                                             \
            socket_read(n, socket, buffer, BUFFER_SIZE);                                            \
                                                                                                    \
            string enc_bool_message_string((const char *)buffer);                                   \
                                                                                                    \
            bitset<64> enc_bool_message(enc_bool_message_string.substr(0, 64));                     \
            message += bitset_to_string((decryption(DES_key, enc_bool_message) ^= init_vector));    \
                                                                                                    \
            for (unsigned long i = 1; i < nb_messages; i++)                                         \
            {                                                                                       \
                bitset<64> enc_bool_message(enc_bool_message_string.substr(i * 64, 64));            \
                bitset<64> old_cipher(enc_bool_message_string.substr((i - 1) * 64, 64));            \
                message += bitset_to_string((decryption(DES_key, enc_bool_message) ^= old_cipher)); \
            }                                                                                       \
        }                                                                                           \
    }

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// Retourne le mode_comm correspondant à la chaîne de caractères passée en paramètre
mode_comm get_mode_comm(char *string)
{
    mode_comm mode = autre;
    if (strcmp(string, "ECB") == 0)
        mode = ECB;
    if (strcmp(string, "CBC") == 0)
        mode = CBC;

    if (mode == autre)
        error("Mode de communication invalide");

    return mode;
}

// Permet de chiffrer et d'envoyer la chaine de caractères "message" via le mode "mode" (ECB ou CBC)
#define send_encrypted(message, socket, DES_key, mode)       \
    {                                                        \
        if (mode == ECB)                                     \
        {                                                    \
            send_ECB(message, socket, DES_key);              \
        }                                                    \
        else if (mode == CBC)                                \
        {                                                    \
            send_CBC(message, socket, DES_key, init_vector); \
        }                                                    \
        else                                                 \
        {                                                    \
            error("Mode inconnu");                           \
        }                                                    \
    }

// Permet de récuperer et déchiffrer la chaine de caractères "message" via le mode "mode" (ECB ou CBC)
#define receive_encrypted(message, socket, DES_key, mode)       \
    {                                                           \
        if (mode == ECB)                                        \
        {                                                       \
            receive_ECB(message, socket, DES_key);              \
        }                                                       \
        else if (mode == CBC)                                   \
        {                                                       \
            receive_CBC(message, socket, DES_key, init_vector); \
        }                                                       \
        else                                                    \
        {                                                       \
            error("Mode inconnu");                              \
        }                                                       \
    }

#endif