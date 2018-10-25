#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <string.h>
#include <ctype.h>

typedef struct point_structure{
    int x;
    int y;
}Point; 

char** allouer_tab_2D(int n, int m);
void desallouer_tab_2D(char** tab, int n);
void afficher_tab_2D(char** tab, int n, int m);
void taille_fichier(const char* nomFichier,int* nbLig, int* nbCol);
char** lire_fichier(const char* nomFichier);
void afficher_fichier(const char* nomFichier);
void modifier_caractere(char** tab, int n, int m, char ancien, char nouveau);
void ecrire_fichier(const char* nomFichier, char** tab, int n, int m);
void print_file_without_array(const char* nomFichier);
void test(const char* nomFichier);
SDL_Texture* charger_image (const char* nomfichier, SDL_Renderer* renderer);
SDL_Texture* charger_image_transparente(const char* nomfichier, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b);
//SDL_Texture* charger_image_part_trans(const char* nomfichier, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b,Uint8 alpha);
char** read_terrain(const char* nomfichier, int *nblig, int *nbcol);
void render_terrain(char** tab,int nblig, int nbcol,Point p, int size, SDL_Renderer* screen,SDL_Texture *tex, SDL_Rect* source);
void charge_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect); 