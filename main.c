#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define ARTICLE1 "Pommes par cajot de 10kg" // 1er article
#define STOCKPOMME 20 // Le nombre de cajots de pomme en stock
#define PRIXPOMME 25 // Le prix unitaire d'un cajot de pomme
#define QTEPOMME 3 // Quantite voulue de pomme par l'acheteur
#define ARTICLE2 "Oranges par cajot de 10kg" // 2e article
#define STOCKORANGE 10 // Le nombre de cajots d'orange en stock
#define PRIXORANGE 20 // Le prix unitaire d'un cajot d'orange
#define QTEORANGE 2 // Quantite voulue d'orange par l'acheteur
#define ARTICLE3 "Bananes par caisse de 20kg" // 3e article
#define STOCKBANANE 5 // Le nombre de caisses de banane en stock
#define PRIXBANANE 20 // Le prix unitaire d'une caisse de banane
#define QTEBANANE 5 // Quantite voulue de banane par l'acheteur

//#define SW "sw1"
//#define SW "sw2"
#define SW "sw3"
//#define ACHETEUR "Laure"
#define ACHETEUR "Philippe"
//#define ACHETEUR "Paul"
//#define LIVREUR "Xavier"
//#define LIVREUR "Michel"
#define LIVREUR "France"
#define CB "4974 7345 8343 9317"
#define CRYPTOGRAMME "872"
#define MSGLENGTH 312

char* entree(int p[], char* buffer){
    int msg;
    while (1) {
        msg = read(p[0], buffer, MSGLENGTH);
        switch (msg) {
            case -1:
                perror("Erreur de lecture");
                exit(5);
            default :
                printf("%d : Reception du message : \"%s\".\n\n", getpid(), buffer);
                return buffer;
        }
    }
}

void sortie(int p[], char* msg){
    write(p[1], msg, MSGLENGTH);
    printf("%d : Envoi du message : \"%s\".\n\n", getpid(), msg);
}

void serveurWeb(int pipeServeurLivreur[],  int pipeAcheteurServeur[], int pipeServeurAcheteur[], char* buffer){
    // Commande des pommes
    
    // Lecture du nom de l'article
    char* nomArticle1 = entree(pipeAcheteurServeur, buffer);
    
    // Transmission de la quantite disponible en stock
    char qteStockPomme[MSGLENGTH];
    snprintf (qteStockPomme, MSGLENGTH, "Serveur web : Bonjour %s, il y a %d cajots de pommes en stock.", ACHETEUR, STOCKPOMME);
    sortie(pipeServeurAcheteur, qteStockPomme);
    
    // Lecture de la quantite voulue par l'acheteur
    char* quantitePomme = entree(pipeAcheteurServeur, buffer);
    
    // Transmission du panier de l'acheteur
    char panier[MSGLENGTH];
    snprintf (panier, MSGLENGTH, "Serveur web : Voici le panier de %s : %d cajots de pommes", ACHETEUR, QTEPOMME);
    sortie(pipeServeurAcheteur, panier);
    
    // Commande des oranges
    
    // Lecture du nom de l'article
    char* nomArticle2 = entree(pipeAcheteurServeur, buffer);
    
    // Transmission de la quantite disponible en stock
    char qteStockOrange[MSGLENGTH];
    snprintf (qteStockOrange, MSGLENGTH, "Serveur web : D'accord %s, il y a %d cajots d'oranges en stock.", ACHETEUR, STOCKORANGE);
    sortie(pipeServeurAcheteur, qteStockOrange);
    
    // Lecture de la quantite voulue par l'acheteur
    char* quantiteOrange = entree(pipeAcheteurServeur, buffer);
    
    // Actualisation et transmission du panier a l'acheteur
    char panierActualise[MSGLENGTH];
    snprintf (panierActualise, MSGLENGTH, "Serveur web : Voici le panier actualise de %s : %d cajots de pommes et %d cajots d'oranges", ACHETEUR, QTEPOMME, QTEORANGE);
    sortie(pipeServeurAcheteur, panierActualise);
    
    // Commande des bananes
    
    // Lecture du nom de l'article
    char* nomArticle3 = entree(pipeAcheteurServeur, buffer);
    
    // Transmission de la quantite disponible en stock
    char qteStockBanane[MSGLENGTH];
    snprintf (qteStockBanane, MSGLENGTH, "Serveur web : D'accord %s, il y a %d caisse de bananes en stock.", ACHETEUR, STOCKBANANE);
    sortie(pipeServeurAcheteur, qteStockBanane);
    
    // Lecture de la quantite voulue par l'acheteur
    char* quantiteBanane = entree(pipeAcheteurServeur, buffer);
    
    // Calcul du montant total de la commande
    int total = (QTEPOMME * PRIXPOMME) + (QTEORANGE * PRIXORANGE) + (QTEBANANE * PRIXBANANE);
    
    // Transmission de la facture presentant le montant total a l'acheteur 
    char facture[MSGLENGTH];
    snprintf (facture, MSGLENGTH, "Serveur web : Facture de %s : Vous avez commande %d cajots de pommes, %d cajots d'oranges et %d caisses de bananes, pour un total de %d€.", ACHETEUR, QTEPOMME, QTEORANGE, QTEBANANE, total);
    sortie(pipeServeurAcheteur, facture);
    
    // Lecture de la carte bancaire de l'acheteur
    char* cb = entree(pipeAcheteurServeur, buffer);
    
    // Transmission à l'acheteur d'un accuse de reception du paiement rappelant le montant total
    char accuseReceptionPaiement[MSGLENGTH];
    snprintf(accuseReceptionPaiement, MSGLENGTH, "Serveur web : Merci pour votre achat %s, voici votre accuse de reception du paiement.\nVous avez paye %d€ avec la carte bancaire n°%s associe au cryptogramme %s. Votre commande sera assuree par le livreur : %s.", ACHETEUR, total, CB, CRYPTOGRAMME, LIVREUR);
    sortie(pipeServeurAcheteur, accuseReceptionPaiement);
    
    // Transmission d'un bon de livraison en double exemplaire au livreur
    sleep(4);
    char bonLivreur[MSGLENGTH];
    snprintf(bonLivreur, MSGLENGTH, "Serveur web : Bon de livraison pour le livreur %s : Commande de %d cajots de pommes, %d cajots d'oranges et %d caisses de bananes pour l'acheteur %s livre par %s.", LIVREUR, QTEPOMME, QTEORANGE, QTEBANANE, ACHETEUR, LIVREUR);
    sortie(pipeServeurLivreur, bonLivreur);
    sleep(4);
    sortie(pipeServeurLivreur, bonLivreur);
}

void acheteur(int pipeAcheteurLivreur[], int pipeAcheteurServeur[], int pipeServeurAcheteur[], int pipeLivreurAcheteur[], char* buffer){
    // Commande des pommes
    
    // Transmission au serveur du nom de l'article
    char nomArticle1[MSGLENGTH];
    snprintf (nomArticle1, MSGLENGTH, "Acheteur %s : Bonjour %s ! Je vais acheter des %s.", ACHETEUR, SW, ARTICLE1);
    sortie(pipeAcheteurServeur, nomArticle1);
    
    // Lecture de la quantite disponible en stock
    char* qteStockPomme = entree(pipeServeurAcheteur, buffer);
    
    // Transmission de la quantite voulue par l'acheteur
    char quantitePomme[MSGLENGTH];
    snprintf (quantitePomme, MSGLENGTH, "Acheteur %s : Il me faut %d cajots de pomme.", ACHETEUR, QTEPOMME);
    sortie(pipeAcheteurServeur, quantitePomme);
    
    // Lecture du panier de l'acheteur
    char* panier = entree(pipeServeurAcheteur, buffer);
    
    // Commande des oranges
    
    // Transmission au serveur du nom de l'article
    char nomArticle2[MSGLENGTH];
    snprintf (nomArticle2, MSGLENGTH, "Acheteur %s : J'aimerais aussi des %s.", ACHETEUR, ARTICLE2);
    sortie(pipeAcheteurServeur, nomArticle2);
    
    // Lecture de la quantite disponible en stock
    char* qteStockOrange = entree(pipeServeurAcheteur, buffer);
    
    // Transmission de la quantite voulue par l'acheteur
    char quantiteOrange[MSGLENGTH];
    snprintf (quantiteOrange, MSGLENGTH, "Acheteur %s : Il me faut %d cajots d'oranges.", ACHETEUR, QTEORANGE);
    sortie(pipeAcheteurServeur, quantiteOrange);
    
    // Lecture du panier actualise de l'acheteur
    char* panierActualise = entree(pipeServeurAcheteur, buffer);
    
    // Commande des bananes
    
    // Transmission au serveur du nom de l'article
    char nomArticle3[MSGLENGTH];
    snprintf (nomArticle3, MSGLENGTH, "Acheteur %s : J'aimerais aussi des %s.", ACHETEUR, ARTICLE3);
    sortie(pipeAcheteurServeur, nomArticle3);
    
    // Lecture de la quantite disponible en stock
    char* qteStockBanane = entree(pipeServeurAcheteur, buffer);
    
    // Transmission de la quantite voulue par l'acheteur
    char quantiteBanane[MSGLENGTH];
    snprintf (quantiteBanane, MSGLENGTH, "Acheteur %s : Il me faut %d caisses de bananes.", ACHETEUR, QTEBANANE);
    sortie(pipeAcheteurServeur, quantiteBanane);
    
    // Lecture de la facture presentant le montant total a l'acheteur
    char* facture = entree(pipeServeurAcheteur, buffer);
    
    // Transmission au serveur de la carte bancaire de l'acheteur
    char cb[MSGLENGTH];
    snprintf (cb, MSGLENGTH, "Acheteur %s : D'accord pour la facture. Je paie par carte : mon numero de carte est %s et mon cryptogramme est %s.", ACHETEUR, CB, CRYPTOGRAMME);
    sortie(pipeAcheteurServeur, cb);
    
    // Lecture de l'accuse de reception du paiement rappelant le montant total transmis par le serveur
    char* accuseReceptionPaiement = entree(pipeServeurAcheteur, buffer);
    
    // Reception de la commande
    entree(pipeLivreurAcheteur, buffer);
    
    // Lecture des 2 bons transmis par le livreur
    char* bonLivraison1 = entree(pipeLivreurAcheteur, buffer);
    char* bonLivraison2 = entree(pipeLivreurAcheteur, buffer);
    
    // Signature d'un bon et transmission de ce bon au livreur
    char bonLivraisonSigne[MSGLENGTH];
    snprintf(bonLivraisonSigne, MSGLENGTH, "Acheteur %s : Merci pour la livraison %s, je vous rends votre bon signe : Je soussigne %s avoir recu la %s", ACHETEUR, LIVREUR, ACHETEUR, bonLivraison1);
    sortie(pipeAcheteurLivreur, bonLivraisonSigne);
}

void livreur(int pipeServeurLivreur[], int pipeAcheteurLivreur[], int pipeLivreurAcheteur[], char* buffer){
    // Lecture des bons de livraisons tranmis par le serveur
    char* bonLivraison1 = entree(pipeServeurLivreur, buffer);
    char* bonLivraison2 = entree(pipeServeurLivreur, buffer);
    char* bonLivraison3 = &bonLivraison1[56];
    
    // Livraison de la commande a l'acheteur
    char livraison[MSGLENGTH];
    snprintf(livraison, MSGLENGTH, "Livreur %s : Bonjour %s, voici votre commande. Je vous remets les 2 bons de livraison, vous pouvez en garder un et signer l'autre pour me le redonner", LIVREUR, ACHETEUR);
    sortie(pipeLivreurAcheteur, livraison);
    
    // Transmission des deux bons a l'acheteur
    sortie(pipeLivreurAcheteur, bonLivraison3);
    sortie(pipeLivreurAcheteur, bonLivraison3);
    
    // Lecture du bon signe par l'acheteur
    entree(pipeAcheteurLivreur, buffer);
}


int main() {
    // Scenario de vente : Un scénario met en scène 1 serveur web, 1 acheteur, 1 livreur et 1 liste d'articles et leurs quantités, le stock en cajots ou caisses et les prix unitaires
    printf("Scenario de vente :\nL'acheteur est : %s.\nLe serveur web utilise est : %s\nLe livreur est : %s.\nListe d'articles :\n\t%s, stock en cajot : %d, prix unitaire : %d€, quantite voulue par l'acheteur : %d\n\t%s, stock en cajot : %d, prix unitaire : %d€, quantite voulue par l'acheteur : %d\n\t%s, stock en caisse : %d, prix unitaire : %d€, quantite voulue par l'acheteur : %d\n\n", ACHETEUR, SW, LIVREUR, ARTICLE1, STOCKPOMME, PRIXPOMME, QTEPOMME, ARTICLE2, STOCKORANGE, PRIXORANGE, QTEORANGE, ARTICLE3, STOCKBANANE, PRIXBANANE, QTEBANANE);

    char* buffer = malloc(sizeof(char) * MSGLENGTH);

    int pipeServeurLivreur[2];
    int pipeAcheteurLivreur[2];
    int pipeAcheteurServeur[2];
    int pipeServeurAcheteur[2];
    int pipeLivreurAcheteur[2];

    if ((pipe(pipeServeurLivreur) < 0) || (pipe(pipeAcheteurLivreur) < 0) || (pipe(pipeAcheteurServeur) < 0) || (pipe(pipeServeurAcheteur) < 0) || (pipe(pipeLivreurAcheteur) < 0)) {
        printf("Erreur de pipe\n");
        exit(1);
    }

    // Le processus pere represente le serveur
    // Un processus fils represente l'acheteur
    // L'autre fils represente le livreur
    
    pid_t pidServeur = getpid();
    printf("pidServeur = %d\n\n", pidServeur);

    switch (fork()) {
        case -1:
            printf("Erreur de fork\n");
            exit(3);
        case 0:;
            pid_t pidAcheteur = getpid();
            printf("pidAcheteur = %d\n\n", pidAcheteur);
            acheteur(pipeAcheteurLivreur, pipeAcheteurServeur, pipeServeurAcheteur, pipeLivreurAcheteur, buffer);
            break;
        default:
            switch(fork()){
                 case -1:
                    printf("Erreur de fork\n");
                    exit(4);
                 case 0:;
                    pid_t pidLivreur = getpid();
                    printf("pidLivreur = %d\n\n", pidLivreur);
                    livreur(pipeServeurLivreur, pipeAcheteurLivreur, pipeLivreurAcheteur, buffer);
                    break;
                 default:
                    serveurWeb(pipeServeurLivreur,  pipeAcheteurServeur, pipeServeurAcheteur, buffer);
                    free(buffer);
                    break;
            }
            break;
    }
    sleep(3);
    return 0;
}