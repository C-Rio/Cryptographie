---
title: "TP2 - Sécurité et Cryptographie"
subtitle: "TP RSA"
author: Charles RIO & Romain AUGER
date: 16/02/2019
lang: fr
output: 
    pdf_document:
        toc: true
        number_sections: true
        template : eisvogel.tex
        pandoc_args: [
            "--listings"
         ]
---

# Génération d’une paire de clefs RSA

## Question 1 : Générez une bi-clé RSA de 2048 bits dans un fichier .pem

Commande :

    openssl genrsa -out cle.pem 2048

@import "./img/cle.png"

@import "./cle.pem"

\newpage

# Visualisation des clés RSA

## Question 2 : Afficher le contenu du fichier .pem avec la commande cat sous Linux, puis avec la commande rsa de openssl. Comparer les différences ?

    openssl rsa -in cle.epm -text -noout

@import "./img/visu_cat.png"

@import "./img/visu_rsa.png"

Avec la commande cat, le fichier est affiché tel quel. Par contre, avec rsa, la clé est affichés en hexadécimal. On peut distinguer les différents élements.


## Question 3 : Que vaut votre exposant de chiffrement ? comparez avec ceux de vos voisins.

La valeur de l'exposant est : 65537. Cette valeur est identique pour tout le monde, c'est l'exposant public par défaut. Il permet une valeur assez élevée pour être assez sécurisé et permet une meilleure compatibilité.

## Question 4 : Utilisez l’option -pubout pour exporter clé publique dans un fichier .pub.pem.

Commande : 

    openssl rsa -in cle.pem -pubout -out clepub.pub.pem

@import "./img/cle_pub.png"

@import "./img/visu_cat_pub.png"

# Chiffrement d’un fichier de clés RSA

## Question 5 : Chiffré votre clé RSA avec un algorithme symétrique ; Afficher le contenu du fichier .pem puis avec la commande rsa. Essayer différents algorithmes symétriques

### DES3

Commande :

    openssl rsa -in cle.pem -des3 -out cle.pem

@import "./img/chiffrement_des.png"

@import "./img/visu_cat_des.png"

@import "./img/des_rsa.JPG"

### AES256

Commande : 

    openssl rsa -in cle.pem -aes256 -out cle_enc.pem

@import "./img/chiffrement_aes.png"

@import "./img/visu_cat_aes.png"

@import "./img/aes_rsa.JPG"

# Chiffrement, déchiffrement avec RSA

## Question 6 : Echanger entre vous vos clés publiques et chiffrez de petit message. Envoyez-lez à vos collègues.

Fichier à chiffrer :

@import "./test.txt"

On utilise la commande suivante pour chiffer :

    openssl rsautl -encrypt -in test.txt -inkey cle.pem -out test_enc.txt

Résultat :

@import "./img/test_enc.png"

On utilise la commande suivante pour déchiffer :

    openssl rsautl -decrypt -in test_enc.txt -inkey cle.pem -out test_dec.txt

@import "./img/test_dec.png"


Chiffrement avec clé publique :

    openssl rsautl -encrypt -in test.txt -inkey clepub.pub.pem -out fic_enc_pub.txt -pubin

@import "./img/fic_enc_pub.png"

Déchiffrement :

    openssl rsautl -decrypt -in fic_enc_pub.txt -inkey cle.pem -out fic_dec_pub.txt

@import "./img/fic_dec_pub.png"


# Signature avec RSA

## Question 7 : Signez un petit fichier et échanger le avec vos collègues

Commande pour signer le fichier test.txt :

    openssl rsautl -sign -in test.txt -inkey cle.pem -out sign

@import "./img/signed.png"

On vérifie avec la commande suivante :

    openssl rsautl -verify -in sign -pubin -inkey clepub.pub.pem -out file

@import "./img/file.png"

# Empreinte d’un document

## Question 8 : Signez un gros fichier en utilisant son empreinte

Calcul de l'empreinte d'un document avec la fonction md5 :

    openssl dgst -md5 -out empreinte test.txt

@import "./img/md5.png"

Calcul de l'empreinte d'un document avec la fonction sha256 :

    openssl dgst -sha256 -out empreinte test.txt

@import "./img/sha256.png"

On signe l'empreinte calculée :

    openssl rsautl -sign -in empreinte -inkey cle.pem -out fic_sign

On vérifie avec la commande suivante :

    openssl rsautl -verify -in fic_sign -pubin -inkey clepub.pub.pem -out fic_unsign

@import "./img/fic_unsign.png"