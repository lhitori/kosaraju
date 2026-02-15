#include "graphe.h"

struct sommet {
  int pos_x;
  int pos_y;
  int couleur;
  char etiquette[32];
};

struct graphe {
  int nb_sommet;
  sommet* sommet;
  bool** matrice;

  int** couleur_arc;
};

graphe* initialiser_graphe(int nb_sommet) {
  graphe* g = malloc(sizeof(graphe));

  g->nb_sommet = nb_sommet;
  g->sommet = malloc(nb_sommet * sizeof(sommet));

  for (int i = 0; i < nb_sommet; i += 1) {

    g->sommet[i].pos_x   = 0;
    g->sommet[i].pos_y   = 0;
    g->sommet[i].couleur = 0;

    strcpy(g->sommet[i].etiquette, "");
  }

  g->matrice     = malloc(nb_sommet * sizeof(bool*));
  g->couleur_arc = malloc(nb_sommet * sizeof(int*));

  for (int i = 0; i < nb_sommet; i += 1) {
    g->matrice[i]     = malloc(nb_sommet * sizeof(bool));
    g->couleur_arc[i] = malloc(nb_sommet * sizeof(int));

    for (int j = 0; j < nb_sommet; j += 1) {
      g->matrice[i][j] = false;
      g->couleur_arc[i][j] = 0;
    }
  }

  return g;
}

void liberer_graphe(graphe* g) {
  for (int i = 0; i < g->nb_sommet; i += 1) {
    free(g->matrice[i]);
    free(g->couleur_arc[i]);
  }

  free(g->matrice);
  free(g->couleur_arc);
  free(g->sommet);
  free(g);
}

int nb_sommet(graphe* g) {
  return g->nb_sommet;
}

void ajouter_arc(graphe* g, int s, int t) {
  g->matrice[s][t] = true;
}

void supprimer_arc(graphe* g, int s, int t) {
  g->matrice[s][t] = false;
}

bool tester_arc(graphe* g, int s, int t) {
  return g->matrice[s][t];
}

void modifier_position_sommet(graphe* g, int s, int x, int y) {
  g->sommet[s].pos_x = x;
  g->sommet[s].pos_y = y;
}

void modifier_couleur_sommet(graphe* g, int s, int couleur) {
  g->sommet[s].couleur = couleur;
}

void modifier_etiquette_sommet(graphe* g, int s, char etiquette[32]) {
  strcpy(g->sommet[s].etiquette, etiquette);
}

void graphviz(graphe* g, char* nom_fichier) {
  (void)g; (void)nom_fichier; // "Hack" pour supprimer les avertissement. Vous pouvez supprimer cette ligne.

  FILE*  file = fopen(nom_fichier, "w");
  if (file == NULL) {
    printf("L'ouverture a échouchée");
    return;
  }

  fprintf(file, "digraph G {\n");
  fprintf(file, "  node [shape=circle style=filled colorscheme=paired12 width=0.5 fixedsize=true fontsize=6 fontcolor =white];\n");
  fprintf(file, "  edge [colorscheme=paired12 penwidth=2];\n");

  for (int i = 0; i < g->nb_sommet; i += 1) {
    fprintf(
        file,
        "  %d[pos=\"%d,%d!\" color=\"%d\" label=\"%s\"];\n",
        i, g->sommet[i].pos_x, g->sommet[i].pos_y, g->sommet[i].couleur, g->sommet[i].etiquette
    );

    for (int j = 0; j < g->nb_sommet; j += 1) {
      if (g->matrice[i][j])
        fprintf(
            file,
            "  %d -> %d[color=\"%d\"];\n",
            i, j, g->couleur_arc[i][j]
        );
    }
  }


  fprintf(file, "}");
  fclose(file);
}

void transpose(graphe* g) {
  for (int i = 0; i < g->nb_sommet; i += 1) {
    for (int j = i + 1; j < g->nb_sommet; j += 1) {
      if (tester_arc(g, i, j)) {
        ajouter_arc(g, j, i);
        supprimer_arc(g, i, j);
      } else if (tester_arc(g, j, i)) {
        ajouter_arc(g, i, j);
        supprimer_arc(g, j, i);
      }
    }
  }
}

void modifier_couleur_arc(graphe* g, int s, int t, int couleur) {
  g->couleur_arc[s][t] = couleur;
}

