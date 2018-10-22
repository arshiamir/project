
#include "fonctions_fichiers.h"
#define BLOCK_SIZE 32

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
  win = SDL_CreateWindow("Fenetre SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 670, 469, SDL_WINDOW_RESIZABLE);
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
  SDL_Texture* fond = charger_image( "blue_gradient.bmp", screen );
//-------------------------------------------------------------------------
  
  // Charger l’image avec la transparence
  Uint8 r = 255, g = 255, b = 255;
  SDL_Texture* block = charger_image_transparente("pavage.bmp", screen ,r,g,b);

//----------------------------------------------------------------------------
  
  int nblig,nbcol,lig,col,i,seg_x,seg_y;
  char** array_terrain;
  SDL_Rect SrcTemp, DestTemp;
  taille_fichier("terrain.txt",&nblig,&nbcol);
  array_terrain = lire_fichier("terrain.txt");
  seg_x=670/nbcol;
  seg_y=469/nblig;
  afficher_tab_2D(array_terrain,nblig,nbcol);
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
      i=array_terrain[lig][col];
      blocks_dest_rect_array[lig][col].x=col*seg_x;
      blocks_dest_rect_array[lig][col].y=lig*seg_y;
      blocks_dest_rect_array[lig][col].w= seg_x;
      blocks_dest_rect_array[lig][col].h= seg_y;
    }
  }
  
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
    for(lig=0; lig<nblig; lig++){
      for(col=0; col<nbcol; col++){
        i=(array_terrain[lig][col])-'0';
        
        if(i!=0){
          SrcTemp = blocks_src_rect_array[i];
          DestTemp = blocks_dest_rect_array[lig][col];
          SDL_RenderCopy(screen, block, &SrcTemp, &DestTemp);
        }
      }
    }
    
    SDL_RenderPresent(screen);
  }
  // Libération de l’écran (renderer)
  SDL_DestroyRenderer(screen);

  //Quitter SDL
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}