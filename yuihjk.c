#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CLIENTS 100
#define MAX_CHAMBRES 50
#define MAX_RESERVATIONS 200

typedef struct {
    int id;
    char nom[50];
    char prenom[50];
    char email[100];
    char telephone[20];
} Client;

typedef struct {
    int numero;
    char type[20];
    float prixParNuit;
    int disponible;
} Chambre;

typedef struct {
    int idReservation;
    int idClient;
    int numeroChambre;
    char dateArrivee[11];
    char dateDepart[11];
    int nbNuits;
    float montantTotal;
    int statut;
} Reservation;

// Prototypes
void initialiserChambres(Chambre chambres[], int *nbChambres);
void sauvegarderClients(Client clients[], int nb);
int chargerClients(Client clients[]);
void sauvegarderChambres(Chambre chambres[], int nb);
int chargerChambres(Chambre chambres[]);
void sauvegarderReservations(Reservation reservations[], int nb);
int chargerReservations(Reservation reservations[]);
int calculerNuits(char *arrivee, char *depart);
Client* rechercherClient(Client clients[], int nb, int id);
float calculerFacture(Reservation r, Chambre chambres[], int nbChambres);
void genererFacture(Reservation r, Client c, Chambre ch);
void afficherEtatChambres(Chambre chambres[], int nb, Reservation reservations[], int nbResa);
void afficherReservationsEnCours(Reservation resa[], int nb, Client clients[], int nbClients);
void ajouterClient(Client clients[], int *nbClients);
void afficherClients(Client clients[], int nbClients);
void modifierClient(Client clients[], int nbClients);
void supprimerClient(Client clients[], int *nbClients, Reservation reservations[], int nbReservations);
void creerReservation(Client clients[], int nbClients, Chambre chambres[], int nbChambres, Reservation reservations[], int *nbReservations);
void enregistrerArrivee(Reservation reservations[], int nbReservations, Chambre chambres[], int nbChambres);
void gererDepart(Reservation reservations[], int nbReservations, Chambre chambres[], int nbChambres, Client clients[], int nbClients);
void facturerClient(Reservation reservations[], int nbReservations, Client clients[], int nbClients, Chambre chambres[], int nbChambres);
void vueEnsemble(Chambre chambres[], int nbChambres, Reservation reservations[], int nbReservations, Client clients[], int nbClients);
void viderBuffer();

// Implementation
void initialiserChambres(Chambre chambres[], int *nbChambres) {
    *nbChambres = 6;
    
    chambres[0].numero = 101;
    strcpy(chambres[0].type, "simple");
    chambres[0].prixParNuit = 50.0;
    chambres[0].disponible = 1;
    
    chambres[1].numero = 102;
    strcpy(chambres[1].type, "double");
    chambres[1].prixParNuit = 80.0;
    chambres[1].disponible = 1;
    
    chambres[2].numero = 103;
    strcpy(chambres[2].type, "double");
    chambres[2].prixParNuit = 80.0;
    chambres[2].disponible = 1;
    
    chambres[3].numero = 201;
    strcpy(chambres[3].type, "suite");
    chambres[3].prixParNuit = 150.0;
    chambres[3].disponible = 1;
    
    chambres[4].numero = 202;
    strcpy(chambres[4].type, "simple");
    chambres[4].prixParNuit = 55.0;
    chambres[4].disponible = 1;
    
    chambres[5].numero = 203;
    strcpy(chambres[5].type, "suite");
    chambres[5].prixParNuit = 200.0;
    chambres[5].disponible = 1;
}

void sauvegarderClients(Client clients[], int nb) {
    FILE *f = fopen("clients.dat", "wb");
    if (f) {
        fwrite(clients, sizeof(Client), nb, f);
        fclose(f);
    }
}

int chargerClients(Client clients[]) {
    FILE *f = fopen("clients.dat", "rb");
    if (!f) return 0;
    int nb = fread(clients, sizeof(Client), MAX_CLIENTS, f);
    fclose(f);
    return nb;
}

void sauvegarderChambres(Chambre chambres[], int nb) {
    FILE *f = fopen("chambres.dat", "wb");
    if (f) {
        fwrite(chambres, sizeof(Chambre), nb, f);
        fclose(f);
    }
}

int chargerChambres(Chambre chambres[]) {
    FILE *f = fopen("chambres.dat", "rb");
    if (!f) return 0;
    int nb = fread(chambres, sizeof(Chambre), MAX_CHAMBRES, f);
    fclose(f);
    return nb;
}

void sauvegarderReservations(Reservation reservations[], int nb) {
    FILE *f = fopen("reservations.dat", "wb");
    if (f) {
        fwrite(reservations, sizeof(Reservation), nb, f);
        fclose(f);
    }
}

int chargerReservations(Reservation reservations[]) {
    FILE *f = fopen("reservations.dat", "rb");
    if (!f) return 0;
    int nb = fread(reservations, sizeof(Reservation), MAX_RESERVATIONS, f);
    fclose(f);
    return nb;
}

int calculerNuits(char *arrivee, char *depart) {
    int j1, m1, a1, j2, m2, a2;
    sscanf(arrivee, "%d/%d/%d", &j1, &m1, &a1);
    sscanf(depart, "%d/%d/%d", &j2, &m2, &a2);
    return j2 - j1;
}

Client* rechercherClient(Client clients[], int nb, int id) {
    for (int i = 0; i < nb; i++)
        if (clients[i].id == id) return &clients[i];
    return NULL;
}

float calculerFacture(Reservation r, Chambre chambres[], int nbChambres) {
    float prixNuit = 0;
    for (int i = 0; i < nbChambres; i++)
        if (chambres[i].numero == r.numeroChambre)
            prixNuit = chambres[i].prixParNuit;
    return r.nbNuits * prixNuit;
}

void genererFacture(Reservation r, Client c, Chambre ch) {
    printf("\n========================================\n");
    printf("           FACTURE HOTEL\n");
    printf("========================================\n");
    printf("Client : %s %s\n", c.prenom, c.nom);
    printf("Chambre : %d - %s\n", ch.numero, ch.type);
    printf("Prix par nuit : %.2f €\n", ch.prixParNuit);
    printf("Date arrivee : %s\n", r.dateArrivee);
    printf("Date depart : %s\n", r.dateDepart);
    printf("Nombre de nuits : %d\n", r.nbNuits);
    printf("----------------------------------------\n");
    printf("Total a payer : %.2f €\n", r.montantTotal);
    printf("========================================\n");
}

void afficherEtatChambres(Chambre chambres[], int nb, Reservation reservations[], int nbResa) {
    printf("\n=== ETAT DES CHAMBRES ===\n");
    for (int i = 0; i < nb; i++) {
        int occupe = 0;
        for (int j = 0; j < nbResa; j++)
            if (reservations[j].numeroChambre == chambres[i].numero && reservations[j].statut == 0)
                occupe = 1;
        printf("Chambre %d (%s) : %s\n", chambres[i].numero, chambres[i].type, occupe ? "Occupee" : "Libre");
    }
}

void afficherReservationsEnCours(Reservation resa[], int nb, Client clients[], int nbClients) {
    printf("\n=== RESERVATIONS EN COURS ===\n");
    int trouve = 0;
    for (int i = 0; i < nb; i++)
        if (resa[i].statut == 0) {
            Client *c = rechercherClient(clients, nbClients, resa[i].idClient);
            if (c) {
                printf("Resa %d - %s %s - Chambre %d - Arrivee %s - Depart %s\n",
                       resa[i].idReservation, c->prenom, c->nom,
                       resa[i].numeroChambre, resa[i].dateArrivee, resa[i].dateDepart);
                trouve = 1;
            }
        }
    if (!trouve) printf("Aucune reservation en cours.\n");
}

void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void ajouterClient(Client clients[], int *nbClients) {
    if (*nbClients >= MAX_CLIENTS) {
        printf("Capacite maximale atteinte!\n");
        return;
    }
    
    Client nouveau;
    nouveau.id = *nbClients + 1;
    
    printf("\n=== AJOUTER CLIENT ===\n");
    printf("Nom: ");
    viderBuffer();
    fgets(nouveau.nom, 50, stdin);
    nouveau.nom[strcspn(nouveau.nom, "\n")] = 0;
    
    printf("Prenom: ");
    fgets(nouveau.prenom, 50, stdin);
    nouveau.prenom[strcspn(nouveau.prenom, "\n")] = 0;
    
    printf("Email: ");
    fgets(nouveau.email, 100, stdin);
    nouveau.email[strcspn(nouveau.email, "\n")] = 0;
    
    printf("Telephone: ");
    fgets(nouveau.telephone, 20, stdin);
    nouveau.telephone[strcspn(nouveau.telephone, "\n")] = 0;
    
    clients[*nbClients] = nouveau;
    (*nbClients)++;
    
    printf("Client ajoute avec succes! ID: %d\n", nouveau.id);
}

void afficherClients(Client clients[], int nbClients) {
    printf("\n=== LISTE DES CLIENTS ===\n");
    if (nbClients == 0) {
        printf("Aucun client.\n");
        return;
    }
    
    printf("%-5s %-20s %-20s %-30s %-15s\n", "ID", "Nom", "Prenom", "Email", "Telephone");
    printf("--------------------------------------------------------------------------------\n");
    for (int i = 0; i < nbClients; i++) {
        printf("%-5d %-20s %-20s %-30s %-15s\n", 
               clients[i].id, clients[i].nom, clients[i].prenom, 
               clients[i].email, clients[i].telephone);
    }
}

void modifierClient(Client clients[], int nbClients) {
    int id;
    printf("\n=== MODIFIER CLIENT ===\n");
    printf("ID du client: ");
    scanf("%d", &id);
    
    Client *c = rechercherClient(clients, nbClients, id);
    if (!c) {
        printf("Client non trouve!\n");
        return;
    }
    
    printf("Client trouve: %s %s\n", c->prenom, c->nom);
    printf("Laissez vide pour ne pas modifier\n\n");
    
    char buffer[100];
    printf("Nouveau nom (%s): ", c->nom);
    viderBuffer();
    fgets(buffer, 50, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) strcpy(c->nom, buffer);
    
    printf("Nouveau prenom (%s): ", c->prenom);
    fgets(buffer, 50, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) strcpy(c->prenom, buffer);
    
    printf("Nouvel email (%s): ", c->email);
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) strcpy(c->email, buffer);
    
    printf("Nouveau telephone (%s): ", c->telephone);
    fgets(buffer, 20, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strlen(buffer) > 0) strcpy(c->telephone, buffer);
    
    printf("Client modifie!\n");
}

void supprimerClient(Client clients[], int *nbClients, Reservation reservations[], int nbReservations) {
    int id;
    printf("\n=== SUPPRIMER CLIENT ===\n");
    printf("ID du client: ");
    scanf("%d", &id);
    
    for (int i = 0; i < nbReservations; i++) {
        if (reservations[i].idClient == id && reservations[i].statut == 0) {
            printf("Impossible: Client a des reservations en cours!\n");
            return;
        }
    }
    
    int trouve = 0;
    for (int i = 0; i < *nbClients; i++) {
        if (clients[i].id == id) {
            for (int j = i; j < *nbClients - 1; j++) {
                clients[j] = clients[j + 1];
            }
            (*nbClients)--;
            trouve = 1;
            break;
        }
    }
    
    if (trouve) printf("Client supprime!\n");
    else printf("Client non trouve!\n");
}

void creerReservation(Client clients[], int nbClients, Chambre chambres[], int nbChambres, 
                      Reservation reservations[], int *nbReservations) {
    if (*nbReservations >= MAX_RESERVATIONS) {
        printf("Capacite maximale de reservations atteinte!\n");
        return;
    }
    
    Reservation nouvelle;
    nouvelle.idReservation = *nbReservations + 1;
    
    printf("\n=== NOUVELLE RESERVATION ===\n");
    
    int choix;
    printf("1. Client existant\n2. Nouveau client\nChoix: ");
    scanf("%d", &choix);
    
    if (choix == 1) {
        afficherClients(clients, nbClients);
        printf("ID client: ");
        scanf("%d", &nouvelle.idClient);
        if (!rechercherClient(clients, nbClients, nouvelle.idClient)) {
            printf("Client non trouve!\n");
            return;
        }
    } else {
        ajouterClient(clients, &nbClients);
        nouvelle.idClient = nbClients;
    }
    
    printf("\nChambres disponibles:\n");
    for (int i = 0; i < nbChambres; i++) {
        if (chambres[i].disponible) {
            printf("Chambre %d - %s - %.2f €/nuit\n", 
                   chambres[i].numero, chambres[i].type, chambres[i].prixParNuit);
        }
    }
    
    printf("Numero de chambre: ");
    scanf("%d", &nouvelle.numeroChambre);
    
    printf("Date arrivee (JJ/MM/AAAA): ");
    scanf("%s", nouvelle.dateArrivee);
    printf("Date depart (JJ/MM/AAAA): ");
    scanf("%s", nouvelle.dateDepart);
    
    nouvelle.nbNuits = calculerNuits(nouvelle.dateArrivee, nouvelle.dateDepart);
    if (nouvelle.nbNuits <= 0) {
        printf("Dates invalides!\n");
        return;
    }
    
    nouvelle.montantTotal = calculerFacture(nouvelle, chambres, nbChambres);
    nouvelle.statut = 0;
    
    reservations[*nbReservations] = nouvelle;
    (*nbReservations)++;
    
    printf("Reservation creee! ID: %d | Total: %.2f €\n", 
           nouvelle.idReservation, nouvelle.montantTotal);
}

void enregistrerArrivee(Reservation reservations[], int nbReservations, Chambre chambres[], int nbChambres) {
    int idResa;
    printf("\n=== ENREGISTRER ARRIVEE ===\n");
    printf("ID reservation: ");
    scanf("%d", &idResa);
    
    for (int i = 0; i < nbReservations; i++) {
        if (reservations[i].idReservation == idResa && reservations[i].statut == 0) {
            for (int j = 0; j < nbChambres; j++) {
                if (chambres[j].numero == reservations[i].numeroChambre) {
                    chambres[j].disponible = 0;
                    break;
                }
            }
            printf("Arrivee enregistree!\n");
            return;
        }
    }
    printf("Reservation non trouvee!\n");
}

void gererDepart(Reservation reservations[], int nbReservations, Chambre chambres[], int nbChambres, 
                 Client clients[], int nbClients) {
    int idResa;
    printf("\n=== GERER DEPART ===\n");
    printf("ID reservation: ");
    scanf("%d", &idResa);
    
    for (int i = 0; i < nbReservations; i++) {
        if (reservations[i].idReservation == idResa && reservations[i].statut == 0) {
            for (int j = 0; j < nbChambres; j++) {
                if (chambres[j].numero == reservations[i].numeroChambre) {
                    chambres[j].disponible = 1;
                    Chambre ch = chambres[j];
                    Client *c = rechercherClient(clients, nbClients, reservations[i].idClient);
                    if (c) {
                        genererFacture(reservations[i], *c, ch);
                    }
                    break;
                }
            }
            reservations[i].statut = 1;
            printf("Depart enregistre!\n");
            return;
        }
    }
    printf("Reservation non trouvee!\n");
}

void facturerClient(Reservation reservations[], int nbReservations, Client clients[], 
                    int nbClients, Chambre chambres[], int nbChambres) {
    int idResa;
    printf("\n=== FACTURER CLIENT ===\n");
    printf("ID reservation: ");
    scanf("%d", &idResa);
    
    for (int i = 0; i < nbReservations; i++) {
        if (reservations[i].idReservation == idResa) {
            Client *c = rechercherClient(clients, nbClients, reservations[i].idClient);
            if (!c) {
                printf("Client non trouve!\n");
                return;
            }
            
            Chambre ch;
            int trouve = 0;
            for (int j = 0; j < nbChambres; j++) {
                if (chambres[j].numero == reservations[i].numeroChambre) {
                    ch = chambres[j];
                    trouve = 1;
                    break;
                }
            }
            
            if (trouve) {
                genererFacture(reservations[i], *c, ch);
            }
            return;
        }
    }
    printf("Reservation non trouvee!\n");
}

void vueEnsemble(Chambre chambres[], int nbChambres, Reservation reservations[], 
                 int nbReservations, Client clients[], int nbClients) {
    printf("\n=== VUE D'ENSEMBLE DE L'HOTEL ===\n");
    afficherEtatChambres(chambres, nbChambres, reservations, nbReservations);
    afficherReservationsEnCours(reservations, nbReservations, clients, nbClients);
}

int main() {
    // Pour Windows - Forcer l'encodage ANSI
    system("chcp 65001 > nul");
    
    Client clients[MAX_CLIENTS];
    Chambre chambres[MAX_CHAMBRES];
    Reservation reservations[MAX_RESERVATIONS];
    
    int nbClients = chargerClients(clients);
    int nbChambres = chargerChambres(chambres);
    int nbReservations = chargerReservations(reservations);
    
    if (nbChambres == 0) {
        initialiserChambres(chambres, &nbChambres);
    }
    
    int choix;
    do {
        printf("\n+------------------------------------+\n");
        printf("|       MENU PRINCIPAL               |\n");
        printf("+------------------------------------+\n");
        printf("| 1. Nouvelle reservation            |\n");
        printf("| 2. Enregistrer arrivee             |\n");
        printf("| 3. Gerer depart                    |\n");
        printf("| 4. Gestion clients                 |\n");
        printf("| 5. Facturation                     |\n");
        printf("| 6. Vue d'ensemble                  |\n");
        printf("| 0. Quitter                         |\n");
        printf("+------------------------------------+\n");
        printf("Choix: ");
        scanf("%d", &choix);
        
        switch(choix) {
            case 1:
                creerReservation(clients, nbClients, chambres, nbChambres, reservations, &nbReservations);
                break;
            case 2:
                enregistrerArrivee(reservations, nbReservations, chambres, nbChambres);
                break;
            case 3:
                gererDepart(reservations, nbReservations, chambres, nbChambres, clients, nbClients);
                break;
            case 4: {
                int choixClient;
                printf("\n1. Ajouter client\n2. Afficher clients\n3. Modifier client\n4. Supprimer client\nChoix: ");
                scanf("%d", &choixClient);
                if (choixClient == 1) ajouterClient(clients, &nbClients);
                else if (choixClient == 2) afficherClients(clients, nbClients);
                else if (choixClient == 3) modifierClient(clients, nbClients);
                else if (choixClient == 4) supprimerClient(clients, &nbClients, reservations, nbReservations);
                break;
            }
            case 5:
                facturerClient(reservations, nbReservations, clients, nbClients, chambres, nbChambres);
                break;
            case 6:
                vueEnsemble(chambres, nbChambres, reservations, nbReservations, clients, nbClients);
                break;
            case 0:
                printf("\nAu revoir!\n");
                break;
            default:
                printf("Choix invalide!\n");
        }
        
        if (choix != 0) {
            printf("\nAppuyez sur Entree pour continuer...");
            viderBuffer();
            getchar();
        }
        
    } while(choix != 0);
    
    sauvegarderClients(clients, nbClients);
    sauvegarderChambres(chambres, nbChambres);
    sauvegarderReservations(reservations, nbReservations);
    
    return 0;
}
