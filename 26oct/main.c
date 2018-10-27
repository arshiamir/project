#include "fonctions_fichiers.h"
#define BLOCK_SIZE 32
#define SCREEN_HEIGHT 640
#define SCREEN_WIDTH 960

int main(int argc, char *argv[])
{
  SDL_Window* win;//Déclaration de la fenêtre
  SDL_Event evenements;//Événement lié à la fermeture de la fenêtre
  bool terminer=false;
  if(SDL_Init(SDL_INIT_VIDEO) < 0)//initialisation de la SDL
  {
    printf("Erreur d’initialisation de la SDL: %s",SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }
  //Créer la fenêtre
  win = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
  if(win == NULL)//En cas d’erreur
  {
    printf("Erreur de la creation d’une win: %s",SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }
  // Mettre en place un contexte de rendu de l’écran
  SDL_Renderer* screen;
  screen = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  // Charger l’image
  SDL_Texture* fond = charger_image( "wood.bmp", screen );
  //-------------------------------------------------------------------------
  
  // Charger l’image avec la transparence
  Uint8 r = 252, g = 255, b = 255, a=100;
  SDL_Texture* gray_block= charger_image_transparente("gray_block1.bmp", screen ,r,g,b);
  //setting the opacity of texture
  SDL_SetTextureAlphaMod(gray_block,a);
  SDL_Rect SrcRectGray;
  SrcRectGray.x = 0;
  SrcRectGray.y = 0;
  SrcRectGray.w = 32;
  SrcRectGray.h = 32;
  int q,w;
  int size_gray_block= 32;
  char** tabTerrain;
  tabTerrain=read_terrain("pentomino.txt", &q, &w);
  afficher_tab_2D(tabTerrain,q,w);
  
  Point start_terrain ={.x=416, .y=40}; 
  //----------------------------------------
  //----------------------------------------
  //rendering a text texture ttf
  /* Inint TTF. */
    TTF_Init(); 
    TTF_Font *font = TTF_OpenFont("Ubuntu-R.ttf", 24);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
    SDL_Texture *texture_Text;
    SDL_Rect text_Rect;
    charge_text(screen, 0, 0, "Project done by MIRMOHAMMADI Ali & WANG Jinniu(oct24)", font, &texture_Text, &text_Rect);
    int alpha_text =0; 
    int cnt1=0;
  //----------------------------------------
  //testing the read_obj func
  int nbobj,obj;
  SDL_Rect* tab_obj_rect;
  //reserve enough space on memory RAM for two arrays containing the rectangular information and texture of the objects
  tab_obj_rect = (SDL_Rect *)malloc(15*sizeof(SDL_Rect));
  SDL_Texture** tab_obj_tex;
  tab_obj_tex = (SDL_Texture**)malloc(15*sizeof(SDL_Texture*));
  read_objects("pentomino.txt", &nbobj, tab_obj_tex, tab_obj_rect, screen);
  SDL_Rect obj_src_rect;
  obj_src_rect.x = 0;
  obj_src_rect.y = 0;
  obj_src_rect.w = 160;
  obj_src_rect.h = 160;

  // Boucle principale
  while(!terminer)
  {
    while( SDL_PollEvent( &evenements ) )
    {
      switch(evenements.type)
      {
	case SDL_QUIT:
	  terminer = true; break;
	case SDL_KEYDOWN:
	switch(evenements.key.keysym.sym)
	{
	  case SDLK_ESCAPE:
	  case SDLK_q:
	    terminer = true;  break;
	}
      }
    }
    
    SDL_RenderClear(screen);
    SDL_RenderCopy(screen, fond, NULL, NULL);
    render_terrain(tabTerrain,q, w,start_terrain, size_gray_block, screen, gray_block, &SrcRectGray);
    //loop for rendering objects
    for(obj = 0; obj<nbobj;obj++){
      SDL_RenderCopy(screen, tab_obj_tex[obj] ,&obj_src_rect, &tab_obj_rect[obj]);
    }
    //text fade in and fade out
    if(cnt1<10200){
      cnt1++;
      if(cnt1<5100){
        alpha_text = cnt1/20;
        SDL_SetTextureAlphaMod(texture_Text,alpha_text);
      }else{
        alpha_text = 255-((cnt1-5100)/20);
        SDL_SetTextureAlphaMod(texture_Text,alpha_text);
      }
    }    
    SDL_RenderCopy(screen, texture_Text, NULL, &text_Rect);
    SDL_RenderPresent(screen);
  }
  // Libération de l’écran (renderer) 
  SDL_DestroyRenderer(screen);
  SDL_DestroyTexture(texture_Text);
  SDL_DestroyTexture(fond);
  SDL_DestroyTexture(gray_block);
  desallouer_tab_2D(tabTerrain,q);


  //Quitter SDL
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}