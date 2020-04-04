#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>

#include "socket.h"

string exec_command(const char *);

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, pid, n;
    socklen_t clilen;
    unsigned char buffer[BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    string message;
    bitset<64> init_vector;

    // On vérifie que l'utilisateur a bien passé le numéro de port en paramètre
    if (argc < 2)
        error("Le programme nécessite 1 argument : port");

    // On créer un nouveau socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Impossible de créer le socket");

    // On configure le socket que l'on vient de créer
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Erreur binding");

    // On génère et on enregistre les clés RSA
    generate_key();
    RSA *rsa_private = load_private_key((char *)"private.pem");

    cout << "Serveur pret à communiquer" << endl;

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1)
    {
        // On attend une connexion au serveur
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd < 0)
            error("Erreur accept");

        // Pour chaque nouveau client, un nouveau processus qui se chargera des communication sera créé
        pid = fork();
        if (pid < 0)
            error("Erreur fork");
        if (pid == 0)
        {
            close(sockfd);

            char DES_key_char[64];
            cout << "Nouveau client connecté" << endl;

            // On récupère le message de début de session qui nous informe du mode de communication
            bzero(buffer, BUFFER_SIZE);
            socket_read(n, newsockfd, buffer, BUFFER_SIZE);
            mode_comm mode = get_mode_comm((char *)buffer);

            // On transmet la clé publique RSA au client
            bzero(buffer, BUFFER_SIZE);
            write_public_key_to_buffer((char *)buffer);
            socket_write(n, newsockfd, buffer, BUFFER_SIZE);

            // On récupère la clé DES et on la déchiffre avec la clé privée RSA
            bzero(buffer, BUFFER_SIZE);
            socket_read(n, newsockfd, buffer, BUFFER_SIZE);
            RSA_private_decrypt(256, (const unsigned char *)buffer, (unsigned char *)DES_key_char, rsa_private, PADDING);
            string DES_key_string(DES_key_char);
            DES_key_string = DES_key_string.substr(0, 64);
            bitset<64> DES_key = keystring_to_bitset(DES_key_string);

            // On acquitte (en ECB) la réception de la clé :
            // - via le vecteur d'initialisation pour le mode CBC
            // - via un simple message OK pour le mode ECB
            bzero(buffer, BUFFER_SIZE);
            if (mode == CBC)
            {
                string init_vector_string = generate_DES_key();
                init_vector = keystring_to_bitset(init_vector_string);
                send_ECB(init_vector_string, newsockfd, DES_key);
            }
            if (mode == ECB)
            {
                string ack = "OK";
                send_ECB(ack, newsockfd, DES_key);
            }

            while (1)
            {
                // On attends puis on récupère le message transmit par le client
                receive_encrypted(message, newsockfd, DES_key, mode);

                // Si le client souhaite terminer la session, on ferme la communication et on termine le processus (du client en question seulement)
                if (strcmp(message.c_str(), "quit") == 0)
                {
                    close(newsockfd);
                    printf("client déconnecté \n");
                    break;
                }

                cout << "commande reçu : " << message << endl;

                // On exécute la commande et on récupère sdtout et stderr
                message.insert(0, "2>&1 ");
                string output = exec_command(message.c_str());

                // On transmet la sortie de la commande
                send_encrypted(output, newsockfd, DES_key, mode);
            }

            exit(EXIT_SUCCESS);
        }
        else
            close(newsockfd);
    }

    close(sockfd);

    return EXIT_SUCCESS;
}

string exec_command(const char *cmd)
{
    char buffer[128];
    std::string result = "";
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        throw std::runtime_error("erreur popen() !");
    try
    {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL)
            result += buffer;
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}