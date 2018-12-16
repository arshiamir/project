#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

typedef struct object_structure{
    int num_obj;
    int tab_cell[5][5];
    SDL_Rect tab_rect[5][5];
    SDL_Texture* tab_tex[6];//because for each color exist 3 different texture real color and 3 highlighted
}obj_t;

char** allouer_tab_2D(int n, int m);
void desallouer_tab_2D(char** tab, int n);
void afficher_tab_2D(char** tab, int n, int m);
void taille_fichier(const char* nomFichier,int* nbLig, int* nbCol);
char** lire_fichier(const char* nomFichier);
void afficher_fichier(const char* nomFichier);
void modifier_caractere(char** tab, int n, int m, char ancien, char nouveau);
void ecrire_fichier(const char* nomFichier, char** tab, int n, int m);
SDL_Texture* charger_image (const char* nomfichier, SDL_Renderer* renderer);
SDL_Texture* charger_image_transparente(const char* nomfichier, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b);
char** read_terrain(const char* nomfichier, int *nblig, int *nbcol);
void render_terrain(char** tab,int nblig, int nbcol,SDL_Point p, int size, SDL_Renderer* screen,SDL_Texture *tex, SDL_Rect* source);
void charge_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rectDest); 
void read_objects_v2(const char* nomfichier, int* nbobj, obj_t* tab_obj, SDL_Renderer *renderer);
void mouse_trigger(SDL_Point* p,obj_t *tab_obj, int* GLOBAL_OBJ_SELECTED, int* nbobj,int* dx, int* dy, int* GLOBAL_LAST_OBJ_SELECTED,SDL_Rect* cw_rect,SDL_Rect* ccw_rect, SDL_Rect* flip_rect, SDL_Rect* save_rect, SDL_Rect* load_rect,int* time_shift, long int* chronometer_counter, SDL_Window* win, SDL_Point* previous_position);
void mouse_rollover(SDL_Point* p,obj_t *tab_obj,int* GLOBAL_OBJ_SELECTED, int* GLOBAL_OBJ_HIGHLIGHTED,int* dx, int* dy, int* nbobj, SDL_Rect* cw_rect,SDL_Rect* ccw_rect, SDL_Rect* flip_rect, SDL_Rect* save_rect, SDL_Rect* load_rect, int* GLOBAL_ICON_ANIM, double* GLOBAL_ICON_ANGLE);
bool overpass(obj_t obj, SDL_Point* p);
void mouse_release(obj_t* obj, int* GLOBAL_OBJ_SELECTED, SDL_Rect* terrain, obj_t *tab_obj, int nbobj, SDL_Point* previous_position);
int round_up(int numToRound, int multiple);
bool myPointInRect(SDL_Point* p, SDL_Rect* rect);
void rotate_clockwise(obj_t *obj);
void rotate_counterclockwise(obj_t *obj);
void render_icons(SDL_Renderer* renderer, SDL_Texture* cw, SDL_Texture* ccw, SDL_Texture* flip_tex,SDL_Texture* save_tex, SDL_Texture* load_tex, SDL_Rect* rect_cw, SDL_Rect* rect_ccw, SDL_Rect* rect_flip, SDL_Rect* rect_save, SDL_Rect* rect_load, SDL_RendererFlip* flip_none, int* GLOBAL_ICON_ANIM, double* GLOBAL_ICON_ANGLE);
void flip(obj_t* obj);
bool selected_obj_collision(obj_t *tab_obj, int* GLOBAL_OBJ_SELECTED, int nbobj);
void chronometer(SDL_Renderer* renderer, long int* chronometer_counter, int* seconds_passed, SDL_Texture** time_tex, SDL_Rect* time_rect, TTF_Font* font);

// void chronometer_v2(SDL_Renderer* renderer, int* chronometer_counter_v2, int* seconds_passed_v2, SDL_Texture** time_tex_v2, SDL_Rect* time_rect_v2, TTF_Font* font);

void render_objects(SDL_Renderer* renderer, obj_t *tab_obj,SDL_Rect* obj_src_rect , int* nbobj, int* GLOBAL_OBJ_SELECTED, int* GLOBAL_OBJ_HIGHLIGHTED);
void save_button(obj_t *tab_obj,int chronometer_counter, int nbobj);
void load_button (obj_t *tab_obj, int* time_shift, int* nbobj);
void gameover(long int* chronometer_counter,int* won,bool* GLOBAL_MENU , SDL_Window* window);
void menu_trigger(SDL_Point* p, SDL_Rect* play_rect, SDL_Rect* resume_rect, SDL_Rect* quit_rect,bool* GLOBAL_MENU, bool* terminer, obj_t *tab_obj, int* time_shift, int* nbobj,SDL_Renderer* renderer);
void calulate_win(char** tab,int nblig, int nbcol, SDL_Point* p, obj_t *tab_obj, bool* GLOBAL_MENU, int* won, int nbobj, int* seconds_passed, int* score);
void menu_mouse_rollover(SDL_Point* p,SDL_Rect* play_rect, SDL_Rect* resume_rect, SDL_Rect* quit_rect,int* GLOBAL_MENU_HIGHLIGHT);
void render_score(SDL_Renderer* renderer, SDL_Texture* cup, TTF_Font* font_score, int* score);