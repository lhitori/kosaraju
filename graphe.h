#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct sommet sommet;
typedef struct graphe graphe;

graphe* initialiser_graphe(int nb_sommet);
void liberer_graphe(graphe* g);

int nb_sommet(graphe* g);

void ajouter_arc(graphe* g, int s, int t);
void supprimer_arc(graphe* g, int s, int t);
bool tester_arc(graphe* g, int s, int t);

void modifier_position_sommet(graphe* g, int s, int x, int y);
void modifier_couleur_sommet(graphe* g, int s, int couleur);
void modifier_etiquette_sommet(graphe* g, int s, char etiquette[32]);

void graphviz(graphe* g, char* nom_fichier);

void transpose(graphe* g);

void modifier_couleur_arc(graphe* g, int s, int t, int couleur);
