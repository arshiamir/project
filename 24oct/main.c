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
  Uint8 r = 252, g = 25, b = 25, a=100;
  SDL_Texture* gray_block= charger_image_transparente("gray_block1.bmp", screen ,r,g,b);
  //setting the opacity of texture
  SDL_SetTextureAlphaMod(gray_block,a);
  SDL_Rect SrcRectGray;
  SrcRectGray.x = 0;
  SrcRectGray.y = 0;
  SrcRectGray.w = 32;
  SrcRectGray.h = 32;
  int q,w;
  int size_gray_block= 40;
  char** tabTerrain;
  tabTerrain=read_terrain("pentomino.txt", &q, &w);
  afficher_tab_2D(tabTerrain,q,w);
  //calculation of the point x where the empty terrain aligns exactly in the center of screen
  Point start_terrain ={.x=((SCREEN_WIDTH - size_gray_block*w)/2), .y=200};  
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
    
  //----------------------------------------
  /*
  int nblig,nbcol,lig,col,i,seg_x,seg_y;

  seg_x=960/nbcol;
  seg_y=640/nblig;
  SDL_Rect blocks_src_rect_array[10];
  for (i=0; i<10; i++){
    blocks_src_rect_array[i].x= BLOCK_SIZE*i;
    blocks_src_rect_array[i].y=0;
    blocks_src_rect_array[i].w =BLOCK_SIZE;
    blocks_src_rect_array[i].h =BLOCK_SIZE;
  }
  SDL_Rect blocks_dest_rect_array[nblig][nbcol];
  for(lig=0; lig<nblig; lig++){
    for(col=0; col<nbcol; col++){
      blocks_dest_rect_array[lig][col].x=col*seg_x;
      blocks_dest_rect_array[lig][col].y=lig*seg_y;
      blocks_dest_rect_array[lig][col].w= seg_x;
      blocks_dest_rect_array[lig][col].h= seg_y;
    }
  } 
  */
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
    SDL_RenderCopy(screen, texture_Text, NULL, &text_Rect);
    SDL_RenderPresent(screen);
  }
  // Libération de l’écran (renderer) 
  SDL_DestroyRenderer(screen);
  SDL_DestroyTexture(texture_Text);
  SDL_DestroyTexture(fond);
  SDL_DestroyTexture(gray_block);

  //Quitter SDL
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}