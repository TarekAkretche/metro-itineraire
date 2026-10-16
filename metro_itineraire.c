#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 50
#define INF 999999

/*DONNÉES GLOBALES*/

char noms[MAX][64];              /* noms des stations */
int  nbStations = 0;

int  temps[MAX][MAX];            /* temps entre stations (0 si pas de lien) */
char ligne[MAX][MAX][8];         /* ligne de métro entre 2 stations */

/* Résultats de Dijkstra */
int  distance[MAX];
int  precedent[MAX];
int  visite[MAX];

/* TROUVER UNE STATION  → son numéro (-1 si absente)*/

int trouverStation(char *nom) {
    int i;
    for (i = 0; i < nbStations; i++) {
        if (strcmp(noms[i], nom) == 0) return i;
    }
    return -1;
}

/* AJOUTER UNE STATION */

int ajouterStation(char *nom) {
    int id = trouverStation(nom);
    if (id != -1) return id;

    strcpy(noms[nbStations], nom);
    return nbStations++;
}

/* CHARGER LE FICHIER
 *  Format : station1 ; station2 ; ligne ; temps */

void chargerFichier(char *fichier) {
    FILE *f = fopen(fichier, "r");
    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir %s\n", fichier);
        exit(1);
    }

    /* Initialiser */
    int i, j;
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            temps[i][j] = 0;
            ligne[i][j][0] = '\0';
        }
    }

    char ligneFichier[256];

    while (fgets(ligneFichier, sizeof(ligneFichier), f) != NULL) {

        if (ligneFichier[0] == '#' || ligneFichier[0] == '\n') continue;

        /* Découper sur ';' */
        char *s1  = strtok(ligneFichier, ";");
        char *s2  = strtok(NULL, ";");
        char *lig = strtok(NULL, ";");
        char *tps = strtok(NULL, ";");

        if (s1 == NULL || s2 == NULL ||
            lig == NULL || tps == NULL) continue;

        /* Enlever le '\n' à la fin de tps */
        int n = strlen(tps);
        if (n > 0 && tps[n-1] == '\n') tps[n-1] = '\0';

        int a = ajouterStation(s1);
        int b = ajouterStation(s2);
        int t = atoi(tps);

        /* Graphe non orienté */
        temps[a][b] = t;
        temps[b][a] = t;
        strcpy(ligne[a][b], lig);
        strcpy(ligne[b][a], lig);
    }

    fclose(f);
    printf("Graphe charge : %d stations\n", nbStations);
}

/* DIJKSTRA */

void dijkstra(int source) {
    int i, j;

    for (i = 0; i < nbStations; i++) {
        distance[i] = INF;
        precedent[i] = -1;
        visite[i] = 0;
    }
    distance[source] = 0;

    for (int etape = 0; etape < nbStations; etape++) {

        /* 1. Trouver la station non visitée la plus proche */
        int u = -1;
        int min = INF;
        for (i = 0; i < nbStations; i++) {
            if (visite[i] == 0 && distance[i] < min) {
                min = distance[i];
                u = i;
            }
        }

        if (u == -1) break;
        visite[u] = 1;

        /* 2. Mettre à jour les voisins */
        for (j = 0; j < nbStations; j++) {
            if (temps[u][j] > 0 && visite[j] == 0) {
                int nouvelle = distance[u] + temps[u][j];
                if (nouvelle < distance[j]) {
                    distance[j] = nouvelle;
                    precedent[j] = u;
                }
            }
        }
    }
}

/*  AFFICHER LE CHEMIN AVEC LES LIGNES */

void afficherChemin(int source, int arrivee) {
    if (distance[arrivee] == INF) {
        printf("Aucun chemin trouve.\n");
        return;
    }

    /* Reconstruire le chemin (de la source vers l'arrivée) */
    int chemin[MAX];
    int taille = 0;
    int courant = arrivee;

    while (courant != -1) {
        chemin[taille] = courant;
        taille++;
        if (courant == source) break;
        courant = precedent[courant];
    }

    /* Inverser pour avoir source → arrivée */
    int i, j;
    for (i = 0, j = taille - 1; i < j; i++, j--) {
        int tmp = chemin[i];
        chemin[i] = chemin[j];
        chemin[j] = tmp;
    }

    /* Affichage */
    printf("\n===== ITINERAIRE =====\n");
    printf("De    : %s\n", noms[source]);
    printf("Vers  : %s\n", noms[arrivee]);
    printf("Temps : %d minutes\n", distance[arrivee]);
    printf("Arrets: %d\n\n", taille);

    int correspondances = 0;
    char ligneActuelle[8] = "";
    char lignePrecedente[8] = "";

    for (i = 0; i < taille; i++) {
        printf("  . %s\n", noms[chemin[i]]);

        if (i < taille - 1) {
            int suivant = chemin[i + 1];
            strcpy(ligneActuelle, ligne[chemin[i]][suivant]);
            int t = temps[chemin[i]][suivant];

            /* Détection de correspondance */
            if (i > 0 && strcmp(ligneActuelle, lignePrecedente) != 0) {
                correspondances++;
                printf("      !! Correspondance => ligne %s\n", ligneActuelle);
            }

            printf("       ligne %s (%d min)\n", ligneActuelle, t);
            strcpy(lignePrecedente, ligneActuelle);
        }
    }

    printf("\n  Correspondances : %d\n", correspondances);
    printf("  Temps total     : %d minutes\n", distance[arrivee]);
    printf("======================\n");
}

/*PROGRAMME PRINCIPAL*/

int main() {
    chargerFichier("metro.txt");

    /* Afficher les stations */
    printf("\nStations disponibles :\n");
    for (int i = 0; i < nbStations; i++)
        printf("  %d. %s\n", i + 1, noms[i]);

    /* Demander les stations */
    char depart[64], arrivee[64];

    printf("\nDepart  : ");
    fgets(depart, 64, stdin);
    depart[strlen(depart) - 1] = '\0';

    printf("Arrivee : ");
    fgets(arrivee, 64, stdin);
    arrivee[strlen(arrivee) - 1] = '\0';

    int src = trouverStation(depart);
    int dst = trouverStation(arrivee);

    if (src == -1) { printf("Station inconnue : %s\n", depart);  return 1; }
    if (dst == -1) { printf("Station inconnue : %s\n", arrivee); return 1; }

    dijkstra(src);
    afficherChemin(src, dst);

    return 0;
}
