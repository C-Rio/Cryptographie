#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "socket.h"

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    RSA *rsa_public_server;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    unsigned char buffer[BUFFER_SIZE] = "";
    string message;
    bitset<64> init_vector;

    // On vérifie que l'utilisateur a bien passé le hostname et le numéro de port en paramètre
    if (argc < 4)
        error("Le programme nécessite 3 arguments : hostname du serveur, port, mode (ECB ou CBC)");

    mode_comm mode = get_mode_comm(argv[3]);

    // On créer un nouveau socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Impossible d'ouvrir le socket");

    // On récupère des informations sur l'hôte (notamment l'IP)
    server = gethostbyname(argv[1]);
    if (server == NULL)
        error("Serveur introuvable");

    // On configure le socket que l'on vient de créer
    portno = atoi(argv[2]);
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Erreur de connection");

    // Message de début de session
    sprintf((char *)buffer, "%s", argv[3]);
    socket_write(n, sockfd, buffer, BUFFER_SIZE);

    // On récupère et on enregistre la clé publique du serveur
    bzero(buffer, BUFFER_SIZE);
    socket_read(n, sockfd, buffer, BUFFER_SIZE);
    write_public_key_to_file((char *)buffer);

    // On génère la clé du DES
    string DES_key_string = generate_DES_key();
    bitset<64> DES_key = keystring_to_bitset(DES_key_string);

    // On charge la clé publique du serveur
    rsa_public_server = load_public_key((char *)"public_key_server.pem");

    // On chiffre la clé DES avec la clé publique RSA du serveur puis on l'envoie
    bzero(buffer, sizeof(buffer));
    RSA_public_encrypt(64, (const unsigned char *)DES_key_string.c_str(), (unsigned char *)buffer, rsa_public_server, PADDING);

    socket_write(n, sockfd, buffer, BUFFER_SIZE);

    // On récupère l'acquittement du serveur
    receive_ECB(message, sockfd, DES_key);

    if (mode == ECB && strcmp(message.c_str(), "OK") != 0)
        error("Erreur d'acquittement du serveur");

    if (mode == CBC)
        init_vector = keystring_to_bitset(message);

    cout << "Début de session terminée" << endl;

    while (1)
    {
        cout << "> ";
        getline(cin, message);

        if (strcmp(message.c_str(), "") == 0)
            continue;

        send_encrypted(message, sockfd, DES_key, mode);

        if (strcmp(message.c_str(), "quit") == 0)
        {
            cout << "Suppression de la clé publique du serveur et de la clé DES" << endl;
            if (remove("public_key_server.pem") != 0)
                error("Erreur de suppression du fichier");
            DES_key.reset();
            DES_key_string.clear();

            break;
        }

        receive_encrypted(message, sockfd, DES_key, mode);

        cout << message << endl;
    }

    cout << "Fin de la connexion" << endl;
    close(sockfd);

    return EXIT_SUCCESS;
}
