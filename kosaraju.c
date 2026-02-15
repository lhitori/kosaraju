#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "graphe.h"

graphe* creer_grille(int taille) {
    graphe* g = initialiser_graphe(taille * taille);

    for (int i = 0; i < taille; i += 1) {
        for (int j = 0; j < taille; j += 1) {
            modifier_position_sommet(g, i*taille + j, i, j);
        }
    }

    for (int i = 0; i < taille; i += 1) {
        for (int k = 0; k < taille-1; k += 1) {
            if (rand() % 2 == 0) {
                ajouter_arc(g, i*taille + k, i*taille + (k+1));
            } else {
                ajouter_arc(g, i*taille + (k+1), i*taille + k);
            }
            if (rand() % 2 == 0) {
                ajouter_arc(g, k*taille + i, (k+1)*taille + i);
            } else {
                ajouter_arc(g, (k+1)*taille + i, k*taille + i);
            }
        }
    }

    return g;
}

typedef enum {
    VU,
    EN_EXPLORATION,
    NON_VU
} etat;

typedef struct {
    etat etat;
    int debut;
    int fin;
    int parent;
} info_sommet;

void visiter(graphe* g, int s, int* temps, info_sommet* infos) {
    *temps += 1;
    infos[s].debut = *temps;
    infos[s].etat = EN_EXPLORATION;

    for (int t = 0; t < nb_sommet(g); t += 1) {
        if (tester_arc(g, s, t)) {
            if (infos[t].etat == NON_VU) {
                infos[t].parent = s;
                visiter(g, t, temps, infos);
            }
        }
    }

    *temps += 1;
    infos[s].fin = *temps;
    infos[s].etat = VU;
}

info_sommet* parcours_profondeur(graphe* g, int* ordre) {
    info_sommet* infos = malloc(nb_sommet(g) * sizeof(info_sommet));

    for (int s = 0; s < nb_sommet(g); s += 1) {
        infos[s].etat = NON_VU;
        infos[s].parent = s;
    }

    int temps = 0;

    for (int i = 0; i < nb_sommet(g); i += 1) {
        int s = i;

        if (ordre != NULL) s = ordre[i];

        if (infos[s].etat == NON_VU) visiter(g, s, &temps, infos);
    }

    return infos;
}

void echanger(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(info_sommet* infos, int* ordre, int min, int max) {
    int pivot = ordre[min];
    int i = min;

    for (int j = min + 1; j <= max; j += 1) {
        if (infos[ordre[j]].fin > infos[pivot].fin) {
            i += 1;
            echanger(&ordre[i], &ordre[j]);
        }
    }

    echanger(&ordre[min], &ordre[i]);
    return i;
}

void tri_rapide(info_sommet* infos, int* ordre, int min, int max) {
    if (min < max) {
        int p = partition(infos, ordre, min, max);

        tri_rapide(infos, ordre, min, p - 1);
        tri_rapide(infos, ordre, p + 1, max);
    }
}

info_sommet* kosaraju(graphe* g) {
    int* ordre = malloc(sizeof(int) * nb_sommet(g));
    for (int i = 0; i < nb_sommet(g); i += 1) {
        ordre[i] = i;
    }
    info_sommet* infos1 = parcours_profondeur(g, ordre);
    tri_rapide(infos1, ordre, 0, nb_sommet(g)-1);
    free(infos1);

    transpose(g);
    info_sommet* infos2 = parcours_profondeur(g, ordre);
    transpose(g);
    free(ordre);

    return infos2;
}

int representant(info_sommet* infos, int i) {
    if (infos[i].parent != i) {
        return representant(infos, infos[i].parent);
    } else {
        return i;
    }
}

void colorer_cfc(graphe* g, info_sommet* infos) {
    for (int s = 0; s < nb_sommet(g); s += 1) {
        int rs = representant(infos, s);
        modifier_couleur_sommet(g, s, rs % 12 + 1);
        for (int t = 0; t < nb_sommet(g); t += 1) {
            int rt = representant(infos, t);
            if (rs == rt) {
                modifier_couleur_arc(g, s, t, rs % 12 + 1);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    graphe* test = initialiser_graphe(4);

    modifier_position_sommet(test, 0, 0, 0);
    modifier_position_sommet(test, 1, 0, 3);
    modifier_position_sommet(test, 2, 3, 3);
    modifier_position_sommet(test, 3, 3, 0);

    ajouter_arc(test, 0, 1);
    ajouter_arc(test, 1, 2);
    ajouter_arc(test, 2, 3);
    ajouter_arc(test, 3, 0);

    graphviz(test, "test.dot");

    liberer_graphe(test);

    if (argc <= 2) {
        printf("Usage : %s <TAILLE> <FICHIER.dot>", argv[0]);
        return -1;
    }

    int taille = atoi(argv[1]);
    char* nom_fichier = argv[2];
    graphe* grille = creer_grille(taille);
    info_sommet* pf1 = parcours_profondeur(grille, NULL);

    for (int i = 0; i < nb_sommet(grille); i += 1) {
        char etiquette[32] = "";
        sprintf(etiquette, "%d/%d", pf1[i].debut, pf1[i].fin);
        modifier_etiquette_sommet(grille, i, etiquette);
    }

    info_sommet* pf2 = kosaraju(grille);
    colorer_cfc(grille, pf2);

    graphviz(grille, nom_fichier);

    free(pf1);
    free(pf2);
    liberer_graphe(grille);
    return 0;
}
