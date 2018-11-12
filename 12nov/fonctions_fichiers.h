#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

typedef struct point_structure{
    int x;
    int y;
}Point;
typedef struct object_structure{
    int num_obj;
    int tab_cell[5][5];
    SDL_Rect tab_rect[5][5];
    SDL_Texture* tab_tex[6];//because for each color exist 3 different textureli
}obj_t;

char** allouer_tab_2D(int n, int m);
void desallouer_tab_2D(char** tab, int n);
void afficher_tab_2D(char** tab, int n, int m);
void taille_fichier(const char* nomFichier,int* nbLig, int* nbCol);
char** lire_fichier(const char* nomFichier);
void afficher_fichier(const char* nomFichier);
void modifier_caractere(char** tab, int n, int m, char ancien, char nouveau);
void ecrire_fichier(const char* nomFichier, char** tab, int n, int m);
void print_file_without_array(const char* nomFichier);
SDL_Texture* charger_image (const char* nomfichier, SDL_Renderer* renderer);
SDL_Texture* charger_image_transparente(const char* nomfichier, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b);
//SDL_Texture* charger_image_part_trans(const char* nomfichier, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b,Uint8 alpha);
char** read_terrain(const char* nomfichier, int *nblig, int *nbcol);
void render_terrain(char** tab,int nblig, int nbcol,SDL_Point p, int size, SDL_Renderer* screen,SDL_Texture *tex, SDL_Rect* source);
void charge_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect); 
void read_objects(const char* nomfichier, int* nbobj,  SDL_Texture** tab_obj_tex, SDL_Rect* tab_obj_rect, SDL_Renderer *renderer);

void read_objects_v2(const char* nomfichier, int* nbobj, obj_t* tab_obj, SDL_Renderer *renderer);
void mouse_trigger(SDL_Point* p,obj_t *tab_obj, int* GLOBAL_OBJ_SELECTED, int nbobj,int* dx, int* dy, int* GLOBAL_LAST_OBJ_SELECTED,SDL_Rect* cw_rect,SDL_Rect* ccw_rect, SDL_Rect* flip_rect, SDL_Window* win);
void mouse_rollover(SDL_Point* p,obj_t *tab_obj,int* GLOBAL_OBJ_SELECTED, int* GLOBAL_OBJ_HIGHLIGHTED,int* dx, int* dy, int* nbobj, SDL_Rect* cw_rect,SDL_Rect* ccw_rect, SDL_Rect* flip_rect, int* GLOBAL_ICON_ANIM, double* GLOBAL_ICON_ANGLE);
bool overpass(obj_t obj, SDL_Point* p);
void mouse_release(obj_t *obj, int* GLOBAL_OBJ_SELECTED, SDL_Rect* terrain);
int round_up(int numToRound, int multiple);
bool myPointInRect(SDL_Point* p, SDL_Rect* rect);
void rotate_clockwise(obj_t *obj);
void rotate_counterclockwise(obj_t *obj);
void render_icons(SDL_Renderer* renderer, SDL_Texture* cw, SDL_Texture* ccw, SDL_Texture* flip_tex, SDL_Rect* rect_cw, SDL_Rect* rect_ccw, SDL_Rect* rect_flip, SDL_RendererFlip* flip_none, int* GLOBAL_ICON_ANIM, double* GLOBAL_ICON_ANGLE);
void flip(obj_t* obj);