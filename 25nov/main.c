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
  int nblig,nbcol,i,j;
  int size_gray_block= 32;
  char** tabTerrain;
  tabTerrain=read_terrain("pentomino.txt", &nblig, &nbcol);
  afficher_tab_2D(tabTerrain,nblig,nbcol);
  SDL_Point start_terrain;
  start_terrain.x=416;
  start_terrain.y=64;
  //creating a SDL rect of terrain for its use in mouse release function
  SDL_Rect terrain;
  terrain.x = start_terrain.x;
  terrain.y = start_terrain.y;
  terrain.w = 32*(nbcol);
  terrain.h = 32*(nbcol);
  //----------------------------------------
  //----------------------------------------
  //rendering a text texture ttf
  /* Inint TTF. */
    TTF_Init(); 
    TTF_Font *font = TTF_OpenFont("Ubuntu-R.ttf", 20);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
    SDL_Texture* texture_Text;
    SDL_Rect text_Rect;
    charge_text(screen, 0, 0, "Project done by MIRMOHAMMADI Ali & WANG Jinniu (Icon made by Freepik from www.flaticon.com)", font, &texture_Text, &text_Rect);
    int alpha_text =0; 
    int cnt1=0; 
  //----------------------------------------
  //chronometer counter
    long int chronometer_counter = 0;
    int seconds_passed=0;
    SDL_Texture* time_text;
    SDL_Rect time_Rect;
  //----------------------------------------
  //testing the read_objects_v2 func
  int nbobj; 
  obj_t* tab_obj;
  //reserve enough space on memory RAM for arrays containing the typedef obj_t
  tab_obj= (obj_t*)malloc(15*sizeof(obj_t));
  read_objects_v2("pentomino.txt", &nbobj, tab_obj, screen);
  printf("nbobj=%d\n",nbobj);
  SDL_Rect obj_src_rect;
  obj_src_rect.x = 0;
  obj_src_rect.y = 0;
  obj_src_rect.w = 32 ;
  obj_src_rect.h = 32 ;
  //charging the icons clockwise cw and counter clockwise ccw
  SDL_RendererFlip icon_flip_none = SDL_FLIP_NONE;
  double GLOBAL_ICON_ANGLE = 0.0;
    //icon animation int value refers to the icons which the cursor is pointing at,-1=none, 0=cw and 1=ccw
  int GLOBAL_ICON_ANIM = -1;
  SDL_Texture *cw= charger_image_transparente("cw.bmp", screen ,0,0,0);
  SDL_Texture *ccw= charger_image_transparente("ccw.bmp", screen ,0,0,0);
  SDL_Texture *flip_tex= charger_image_transparente("flip.bmp", screen ,0,0,0);
  SDL_Texture *save_tex= charger_image_transparente("save.bmp", screen ,0,0,0);
  SDL_Texture *load_tex= charger_image_transparente("load.bmp", screen ,0,0,0);
  SDL_Rect rect_cw ={.x= 320, .y = 32, .h =64, .w =64};
  SDL_Rect rect_ccw ={.x= 320, .y = 128, .h =64, .w =64};
  SDL_Rect rect_flip ={.x= 220, .y = 32, .h =64, .w =64};
  SDL_Rect rect_save ={.x= 760, .y = 560, .h =64, .w =64};
  SDL_Rect rect_load ={.x= 860, .y = 560, .h =64, .w =64};
  
  //handling mouse events
  int dx,dy,GLOBAL_OBJ_SELECTED, GLOBAL_OBJ_HIGHLIGHTED, GLOBAL_LAST_OBJ_SELECTED;
  //-1 MEANS THAT NO OBJECT IS SELECTED
  bool key_down= false;
  GLOBAL_OBJ_SELECTED=-1;
  GLOBAL_LAST_OBJ_SELECTED= -1;
  //SDL_Point will save the coordinates of cursor
  SDL_Point mouse_point;
  mouse_point.x=0;
  mouse_point.y=0;
  SDL_Point previous_position ={.x=0,.y=0};//we use this variable to save the previous position of objects which are being selected
  // Boucle principale
  //testing save_button
  save_button(tab_obj,chronometer_counter, nbobj);
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
            case SDLK_RIGHT:
              if(!key_down && GLOBAL_LAST_OBJ_SELECTED!=-1){ rotate_clockwise(&tab_obj[GLOBAL_LAST_OBJ_SELECTED]);}
              key_down = true;
              GLOBAL_ICON_ANIM=0;
              break;
            case SDLK_LEFT:
              if(!key_down && GLOBAL_LAST_OBJ_SELECTED!=-1){ rotate_counterclockwise(&tab_obj[GLOBAL_LAST_OBJ_SELECTED]);}
              key_down = true;
              GLOBAL_ICON_ANIM=1;break;
            case SDLK_DOWN:
            case SDLK_UP:
              if(!key_down && GLOBAL_LAST_OBJ_SELECTED!=-1){ flip(&tab_obj[GLOBAL_LAST_OBJ_SELECTED]);}
              key_down = true;
              GLOBAL_ICON_ANIM=2;break;
            default: 
              break;
          }break;
        case SDL_KEYUP:
          GLOBAL_ICON_ANGLE=0.0;
          GLOBAL_ICON_ANIM=-1;
          key_down = false; break;
	case SDL_MOUSEBUTTONDOWN:
          printf("mouse button down\n");
          switch (evenements.button.button)
          {
            case SDL_BUTTON_LEFT:
              mouse_point.x = evenements.motion.x;
              mouse_point.y = evenements.motion.y;
              mouse_trigger(&mouse_point, tab_obj, &GLOBAL_OBJ_SELECTED,nbobj,&dx, &dy, &GLOBAL_LAST_OBJ_SELECTED, &rect_cw, &rect_ccw, &rect_flip, &rect_save, &chronometer_counter, win, &previous_position);
              printf("xmouse = %d, ymouse = %d,nb obj=%d\n",mouse_point.x,mouse_point
              .y,nbobj); break;
              
            case SDL_BUTTON_RIGHT:
              SDL_ShowSimpleMessageBox(0, "Mouse", "Right button was pressed!", win); break;
          } break;
        case SDL_MOUSEBUTTONUP:
          printf("%d\n",GLOBAL_OBJ_SELECTED);
          mouse_release(&tab_obj[GLOBAL_OBJ_SELECTED], &GLOBAL_OBJ_SELECTED, &terrain, tab_obj, nbobj, &previous_position);
          printf("mouse button up\n");
          break;
        case SDL_MOUSEMOTION:
          mouse_point.x = evenements.motion.x;
          mouse_point.y = evenements.motion.y;
          mouse_rollover(&mouse_point,tab_obj, &GLOBAL_OBJ_SELECTED, &GLOBAL_OBJ_HIGHLIGHTED, &dx, &dy,&nbobj,&rect_cw, &rect_ccw, &rect_flip, &rect_save,&rect_load, &GLOBAL_ICON_ANIM, &GLOBAL_ICON_ANGLE);
          //printf("xmouse = %d, ymouse = %d\n",mouse_point.x,mouse_point.y); break;
        default:
          break;
      }
    }
    
    SDL_RenderClear(screen);
    SDL_RenderCopy(screen, fond, NULL, NULL);
    render_terrain(tabTerrain,nblig, nbcol,start_terrain, size_gray_block, screen, gray_block, &SrcRectGray);
    render_objects(screen, tab_obj, &obj_src_rect , &nbobj, &GLOBAL_OBJ_SELECTED, &GLOBAL_OBJ_HIGHLIGHTED);
    render_icons(screen, cw, ccw, flip_tex,save_tex,load_tex, &rect_cw, &rect_ccw, &rect_flip, &rect_save, &rect_load, &icon_flip_none, &GLOBAL_ICON_ANIM, &GLOBAL_ICON_ANGLE);
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
      SDL_RenderCopy(screen, texture_Text, NULL, &text_Rect);
    }    
    //passing parameters to function chronometer and rendering time
    chronometer(screen, &chronometer_counter, &seconds_passed, &time_text, &time_Rect, font);
    chronometer_counter= SDL_GetTicks();
    SDL_RenderCopy(screen, time_text, NULL, &time_Rect); 
    SDL_RenderPresent(screen);
  }
  // Libération de l’écran (renderer) 
  SDL_DestroyRenderer(screen);
  SDL_DestroyTexture(fond);
  SDL_DestroyTexture(gray_block);
  SDL_DestroyTexture(cw);
  SDL_DestroyTexture(ccw);
  SDL_DestroyTexture(flip_tex);
  SDL_DestroyTexture(save_tex);
  SDL_DestroyTexture(load_tex);
  SDL_DestroyTexture(texture_Text);
  SDL_DestroyTexture(time_text);
  TTF_CloseFont(font);
  desallouer_tab_2D(tabTerrain,nblig);
  
  for(i=0;i<15;i++){
    for(j=0;j<6;j++){
      SDL_DestroyTexture(tab_obj[i].tab_tex[j]);
    }
  }
  free(tab_obj);
  
  //Quitter SDL
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
//valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all ./main
//to check memory leak