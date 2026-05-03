  #include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  STRUCTURES

typedef struct Client {
    int id;
    char nom[50];
    char prenom[50];
    struct Client *suivant;
} Client;

typedef struct Chambre {
    int numero;
    char type[20];
    float prix;
    int disponible; 
    struct Chambre *suivant;
} Chambre;

typedef struct Reservation {
    int id;
    int idClient;
    int numeroChambre;
    char arrivee[11];
    char depart[11];
    int nuits;
    float total;
    int statut; 
    struct Reservation *suivant;
} Reservation;

//  FONCTIONS 
 // Cette fonction vide le buffer d'entrée pour éviter les problèmes de saisie
void viderBuffer() {
   
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
 // Cette fonction initialise les 6 chambres de l'hôtel avec leurs caractéristiques (numéro, type, prix)
Chambre* initChambres() {
   
    int nums[] = {101, 102, 103, 201, 202, 203};
    char types[][20] = {"simple", "double", "double", "suite", "simple", "suite"};
    float prix[] = {50, 80, 80, 150, 55, 200};

    Chambre *tete = NULL, *courant = NULL;

    for(int i = 0; i < 6; i++) {
        Chambre *n = (Chambre*)malloc(sizeof(Chambre));
        n->numero = nums[i];
        strcpy(n->type, types[i]);
        n->prix = prix[i];
        n->disponible = 1;  
        n->suivant = NULL;

        if(!tete) {
            tete = courant = n;
        } else {
            courant->suivant = n;
            courant = n;
        }
    }
    return tete;
}
 // Cette fonction recherche un client par son ID dans la liste chaînée des clients
Client* chercherClient(Client *c, int id) {
   
    while(c) {
        if(c->id == id) return c;
        c = c->suivant;
    }
    return NULL;
}
 // Cette fonction recherche une chambre par son numéro dans la liste chaînée des chambres
Chambre* chercherChambre(Chambre *ch, int num) {
   
    while(ch) {
        if(ch->numero == num) return ch;
        ch = ch->suivant;
    }
    return NULL;
}
// Cette fonction permet d'ajouter un nouveau client avec un ID automatique
void ajouterClient(Client **tete) {
    
    Client *n = (Client*)malloc(sizeof(Client));

    int max = 0;
    Client *c = *tete;
    while(c) {
        if(c->id > max) max = c->id;
        c = c->suivant;
    }

    n->id = max + 1;

    printf("Nom: ");
    viderBuffer();
    fgets(n->nom, 50, stdin);
    n->nom[strcspn(n->nom, "\n")] = 0;

    printf("Prenom: ");
    fgets(n->prenom, 50, stdin);
    n->prenom[strcspn(n->prenom, "\n")] = 0;

    n->suivant = *tete;
    *tete = n;

    printf("Client ajouté ID=%d\n", n->id);
}
// Cette fonction affiche la liste complète de tous les clients enregistrés
void afficherClients(Client *tete) {
    
    if(!tete) {
        printf("Aucun client\n");
        return;
    }
    
    printf("\n LISTE DES CLIENTS\n");
    Client *c = tete;
    while(c) {
        printf("ID: %d | %s %s\n", c->id, c->prenom, c->nom);
        c = c->suivant;
    }}
}
 // Cette fonction calcule le nombre de nuits entre deux dates (arrivée et départ)
int calculerNuits(char *a, char *d) {
   
    int j1, m1, a1, j2, m2, a2;
    sscanf(a, "%d/%d/%d", &j1, &m1, &a1);
    sscanf(d, "%d/%d/%d", &j2, &m2, &a2);

    return (a2 - a1) * 365 + (m2 - m1) * 30 + (j2 - j1);
}
// Cette fonction affiche uniquement les chambres libres et non réservées
void afficherChambresDispo(Chambre *ch, Reservation *r) {
    
    printf("\n    CHAMBRES DISPONIBLES  \n");
    
    int trouve = 0;
    
    while(ch) {
        int estReservee = 0;
        Reservation *tmp = r;
        while(tmp) {
            if(tmp->numeroChambre == ch->numero && tmp->statut == 0) {
                estReservee = 1;
                break;
            }
            tmp = tmp->suivant;
        }
        
        if(ch->disponible == 1 && !estReservee) {
            printf("Chambre %d | %s | %.2f dt/nuit\n",
                   ch->numero, ch->type, ch->prix);
            trouve = 1;
        }
        
        ch = ch->suivant;
    }
    
    if(!trouve) {
        printf("Aucune chambre disponible pour le moment.\n");
    }
}
// Cette fonction crée une nouvelle réservation pour un client existant ou nouveau
void creerReservation(Client **clients, Chambre *ch, Reservation **res) {
    
    Reservation *n = (Reservation*)malloc(sizeof(Reservation));

    int max = 0;
    Reservation *r = *res;
    while(r) {
        if(r->id > max) max = r->id;
        r = r->suivant;
    }
    n->id = max + 1;

    int choix;
    printf("\n NOUVELLE RESERVATION \n");
    printf("1. Client existant\n2. Nouveau client\nChoix: ");
    scanf("%d", &choix);

    if(choix == 1) {
        afficherClients(*clients);
        printf("ID client: ");
        scanf("%d", &n->idClient);
        if(!chercherClient(*clients, n->idClient)) {
            printf("Client introuvable\n");
            free(n);
            return;
        }
    } else {
        ajouterClient(clients);
       
        Client *temp = *clients;
        int maxId = temp->id;
        while(temp) {
            if(temp->id > maxId) maxId = temp->id;
            temp = temp->suivant;
        }
        n->idClient = maxId;
    }
 afficherChambresDispo(ch, *res);

    printf("Numero chambre: ");
    scanf("%d", &n->numeroChambre);

    Chambre *c = chercherChambre(ch, n->numeroChambre);
    if(!c) {
        printf("Chambre inexistante\n");
        free(n);
        return;
    }

    int dispo = c->disponible;
    r = *res;
    while(r) {
        if(r->numeroChambre == n->numeroChambre && r->statut == 0) {
            dispo = 0;
            break;
        }
        r = r->suivant;
    }

    if(!dispo) {
        printf("Chambre non disponible\n");
        free(n);
        return;
    }

    printf("Date arrivee (JJ/MM/AAAA): ");
    scanf("%s", n->arrivee);
    printf("Date depart (JJ/MM/AAAA): ");
    scanf("%s", n->depart);

    n->nuits = calculerNuits(n->arrivee, n->depart);

    if(n->nuits <= 0) {
        printf("Dates invalides\n");
        free(n);
        return;
    }

    n->total = n->nuits * c->prix;
    n->statut = 0;
    n->suivant = *res;
    *res = n;
    
    c->disponible = 0;

    printf("\n Reservation créée avec succès !\n");
    printf("ID Reservation: %d\n", n->id);
    printf("Client: %d\n", n->idClient);
    printf("Chambre: %d\n", n->numeroChambre);
    printf("Total à payer: %.2f €\n", n->total);
}
// Cette fonction enregistre le départ du client, calcule la facture et libère la chambre
void depart(Reservation **res, Client *clients, Chambre *chambres) {
    
    int id;
    printf("\n DEPART CLIENT \n");
    printf("ID reservation: ");
    scanf("%d", &id);

    Reservation *r = *res;
    while(r) {
        if(r->id == id && r->statut == 0) {
            Client *cl = chercherClient(clients, r->idClient);
            Chambre *chb = chercherChambre(chambres, r->numeroChambre);

            
            printf("            FACTURE\n");
           
            printf("Client : %s %s\n", cl->prenom, cl->nom);
            printf("Chambre : %d (%s)\n", r->numeroChambre, chb->type);
            printf("Arrivee : %s\n", r->arrivee);
            printf("Depart : %s\n", r->depart);
            printf("Nuits : %d\n", r->nuits);
            printf("Prix par nuit : %.2f €\n", chb->prix);
           
            printf("TOTAL A PAYER : %.2f €\n", r->total);
            
            r->statut = 1;
            chb->disponible = 1;

            printf("\n Depart enregistre. Merci !\n");
            return;
        }
        r = r->suivant;
    }
    printf(" Reservation introuvable ou deja terminee\n");
}
// Cette fonction affiche l'état détaillé de toutes les chambres (libre/occupée/indisponible)
void afficherEtatComplet(Chambre *ch, Reservation *r) {
    
    printf("    ETAT COMPLET DES CHAMBRES\n");
    
    while(ch) {
        int occupee = 0;
        Reservation *tmp = r;
        while(tmp) {
            if(tmp->numeroChambre == ch->numero && tmp->statut == 0) {
                occupee = 1;
                break;
            }
            tmp = tmp->suivant;
        }
        
        printf("Chambre %d (%s) : ", ch->numero, ch->type);
        if(occupee) {
            printf(" OCCUPEE\n");
        } else if(ch->disponible) {
            printf(" LIBRE\n");
        } else {
            printf(" INDISPONIBLE\n");
        }
        
        ch = ch->suivant;
    }}
}

//  PROGRAMME PRINCIPAL 

int main() {
    // Le programme principal affiche le menu et gère les choix de l'utilisateur
    system("chcp 65001 > nul");
    
    Client *clients = NULL;
    Chambre *chambres = initChambres();
    Reservation *reservations = NULL;
    
    int choix;
    
    do {
       
        printf("         MENU PRINCIPAL             \n");
        
        printf(" 1. Nouvelle reservation            \n");
        printf(" 2. Depart / Facturation            \n");
        printf(" 3. Voir chambres disponibles       \n");
        printf(" 4. Voir etat complet des chambres  \n");
        printf(" 5. Gestion clients                 \n");
        printf(" 0. Quitter                         \n");
       
        printf("Choix: ");
        scanf("%d", &choix);
        
        switch(choix) {
            case 1: 
                creerReservation(&clients, chambres, &reservations); 
                break;
            case 2: 
                depart(&reservations, clients, chambres); 
                break;
            case 3: 
                afficherChambresDispo(chambres, reservations); 
                break;
            case 4: 
                afficherEtatComplet(chambres, reservations); 
                break;
            case 5:
                printf("\n1. Ajouter client\n2. Afficher clients\nChoix: ");
                int choixClient;
                scanf("%d", &choixClient);
                if(choixClient == 1) ajouterClient(&clients);
                else if(choixClient == 2) afficherClients(clients);
                break;
            case 0: 
                printf("\nAu revoir !\n"); 
                break;
            default: 
                printf("Choix invalide\n");
        }
        
        if(choix != 0) {
            printf("\nAppuyez sur Entree pour continuer...");
            viderBuffer();
            getchar();
        }
        
    } while(choix != 0);
    
    return 0;
}
}     
