#include "fonctions_fichiers.h"



char** allouer_tab_2D(int n, int m){
  int lig,col;
  char** res; 
  res = (char **)malloc(n*sizeof(char*));
  for(lig = 0; lig<n; lig++){
    res[lig] = (char *)malloc(m*sizeof(char));
  }
  for(lig = 0; lig<n; lig++){
    for(col = 0; col<m; col++){
      res[lig][col] = ' ';
    }
  }
  return res;
}



void desallouer_tab_2D(char** tab, int n){
  for(int i = 0; i < n; i++){
    free(tab[i]);
  }
  free(tab);
}

void afficher_tab_2D(char** tab, int n, int m){
  int lig, col;
  //char res[m];
  for(lig = 0; lig<n; lig++){
    for(col = 0; col<m; col++){
      printf("%c",tab[lig][col]);
      //res[col]= tab[lig][col];
    }
    //printf("%s",res);
    printf("\n");
  }
  printf("\n");
}

void taille_fichier(const char* nomFichier, int* nbLig, int* nbCol){
  int col,colmax;
  char ch;
  FILE *fi;
  fi = fopen(nomFichier,"r");
  colmax = 0;
  col = 0;
  *nbLig= 0;
  while(!feof(fi)) {
    ch = fgetc(fi);
    if(ch == '\n') {
      *nbLig= *nbLig +1;
      col = 0;
    } else {
      col++;
      if(colmax<col){
	colmax = col;
      }
    }
  }
  *nbCol = colmax;
  fclose(fi);
}

char** lire_fichier(const char* nomFichier){
  char** res;
  int nbLig, nbCol,lig,col;
  FILE *fi;
  char ch;
  taille_fichier(nomFichier,&nbLig,&nbCol);
  fi = fopen(nomFichier,"r");
  res = allouer_tab_2D(nbLig, nbCol);
  col = 0;
  lig = 0;
  while(!feof(fi)) {
    ch = fgetc(fi); 
    
    if(ch == '\n') {
      lig++;
      col = 0;
    } else {
      if(lig<nbLig && col<nbCol){
        res[lig][col] = ch;
      }
      col++;
    }
  }
  return res;
  fclose(fi);
}

void afficher_fichier(const char* nomFichier){
  int lig,col;
  taille_fichier(nomFichier, &lig, &col);
  afficher_tab_2D(lire_fichier(nomFichier), lig, col);
  
}
//this function will rplace the specified car ancien in array tab by char nouveau, if '*' used as ancien all the tab will be changed no matter what character
void modifier_caractere(char** tab, int n, int m, char ancien, char nouveau){
  //char** res;
  //res = allouer_tab_2D(n, m);
  int lig,col;
  for(lig = 0; lig<n; lig++){
    for(col = 0; col<m; col++){
      if(tab[lig][col]== ancien || ancien =='*'){
        if(tab[lig][col]== ' ' && ancien != ' '){
          continue;
        }
        tab[lig][col]=nouveau;
      } else{
        //res[lig][col]= tab[lig][col];
      }
    }
  }
  
  //return res;
}

void ecrire_fichier(const char* nomFichier, char** tab, int n, int m){
  int lig,col;
  FILE *fp;
  fp=fopen(nomFichier,"w+");
  for(lig=0 ;lig<n; lig++){
    if (lig!=0){
      fputs("\n",fp);
    }
    for(col=0; col<m; col++){
      fputc(tab[lig][col],fp);
    }
  }
  fclose(fp);
}

void print_file_without_array(const char* nomFichier){
  FILE *fp;
  char c;
  fp = fopen(nomFichier,"r+");
  if (fp == NULL){
    printf("cannot open file!");
    exit(0);
  }
  printf("\nThe content of file:\n");
  while((c = fgetc(fp))!= EOF){
    putchar(c);
  }
  fclose(fp);
}

SDL_Texture* charger_image (const char* nomfichier, SDL_Renderer* renderer){
  SDL_Surface* sur = SDL_LoadBMP(nomfichier);
  SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, sur) ;
  SDL_FreeSurface(sur);
  return tex;
}

SDL_Texture* charger_image_transparente(const char* nomfichier, SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b){
  int colorkey;
  SDL_Surface* sur = SDL_LoadBMP(nomfichier);
  
  colorkey= SDL_MapRGB(sur->format,r, g, b);
  // Définir la couleur (pixel transparent) dans une surface.
  SDL_SetColorKey(sur, SDL_TRUE, colorkey) ;
  SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, sur) ;
  SDL_FreeSurface(sur);
  return tex;
}

char** read_terrain(const char* nomfichier, int *nblig, int *nbcol){
  char** out;
  int col,colmax,lig;
  char ch,chOld = ' ';
  FILE *fi;
  fi = fopen(nomfichier,"r");
  colmax = 0;
  col = 0;
  *nblig= 0;
  while(!feof(fi)) {
    //if there are two consecutive returns or next lines
    if((chOld == '\n' && ch=='\r') || (chOld == '\n' && ch=='\n') || (chOld == '\r' && ch=='\r')){
      *nblig = *nblig-1;
      break;
    }
    chOld = ch;
    ch = fgetc(fi);
    if(ch == '\n') {
      *nblig= *nblig +1;
      
      col = 0;
    } else {
      col++;
      if(colmax<col){
	colmax = col;
      }
    }
  }
  *nbcol = colmax;
  printf("nblig de terrain=%d, nbcol de terrain=%d\n",*nblig,*nbcol);
  *nbcol = colmax;
  out = allouer_tab_2D(*nblig,*nbcol);
  //start from the beginning of the file and registering each car of terrain part to tab out
  rewind(fi);
  col=0;
  lig=0;
  while(!feof(fi)) {
    ch = fgetc(fi);
    if(ch == '\n') {
      lig++;
      col = 0;
    } else {
      if(lig<*nblig && col<*nbcol){
        out[lig][col] = ch;
        
      }else{
        break;
      }
      col++;
    }
  }
  fclose(fi);
  //replacing every char by '*'
  printf("After replacing all the characters (i.e:#) in source file with 'x' the final Terrain array looks like:\n");
  modifier_caractere(out,*nblig,*nbcol,'*','x');
  return out;
}

void render_terrain(char** tab,int nblig, int nbcol,Point p, int size, SDL_Renderer* screen,SDL_Texture *tex, SDL_Rect* source){
  //point p is a structure which contains the upper left corner of the empty terrain
  //opacity must be between 0 and 255
  //tab is the array containing the x value which has to be presented by a gray transparent square
  //tex is the SDL_texture of each gray square
  int lig,col; 
  SDL_Rect dest;
  dest.x = p.x;
  dest.y = p.y;
  dest.w = size;
  dest.h = size;
  
  for(lig=0; lig<nblig; lig++){
    for(col=0; col<nbcol; col++){
      if(tab[lig][col]=='x'){
        dest.x = p.x + col*size;
        dest.y = p.y + lig*size;
        dest.w = size;
        dest.h = size;
        SDL_RenderCopy(screen, tex, source, &dest);
      }
    }
  }
}
 
//function charge_text will convert a text line to sdl texture and rect
void charge_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rectDest) {
  //height and width  
  int text_w;
  int text_h;
  SDL_Surface *sur;
  SDL_Color Color = {73, 142, 255, 0};
  sur = TTF_RenderText_Solid(font, text, Color);
  *texture = SDL_CreateTextureFromSurface(renderer, sur);
  text_w = sur->w;
  text_h = sur->h;
  SDL_FreeSurface(sur);
  rectDest->x = x;
  rectDest->y = y;
  rectDest->w = text_w;
  rectDest->h = text_h;
}

void read_objects(const char* nomfichier, int* nbobj, SDL_Texture** tab_obj_tex, SDL_Rect* tab_obj_rect, SDL_Renderer *renderer){
  //Max nbobj = 15
  //first we must find how many objects are there in our file, keep in mind that the lines before the first return line is not an object of our interest in this function
  //we start number of object by 0 because the objects start after the first empty line
  int i,lig,col,obj_counter;
  FILE *fi;
  *nbobj = 0;
  fi = fopen(nomfichier,"r");
  char ch,chOld = ' ';
  //size that we have reserved for each object is equal to 5 * 32 =160 so we need a surface of 160x160 pixels
  SDL_Surface* sur = SDL_CreateRGBSurface(0,160,160,32,0,0,0,0);
  //Loading the tile bitmap
  SDL_Surface* tile = SDL_LoadBMP("tile32.bmp");
  // we need two temporary SDL_Rect: one for indication of the area of image tile that is going to be copied and one rect indicating the area which the tile surface is going to be copied to
  SDL_Rect tile_temp, paste_rect;
  tile_temp.w = 32;
  tile_temp.h = 32;
  paste_rect.w = 32;
  paste_rect.h = 32;
  //the first step is to count the objects presented in the file
  while(!feof(fi)) {
    //if there are two consecutive returns or next lines
    if((chOld == '\n' && ch=='\r') || (chOld == '\n' && ch=='\n') || (chOld == '\r' && ch=='\r')){
      *nbobj = *nbobj + 1;
    }
    chOld = ch;
    ch = fgetc(fi);
  }
  assert(*nbobj<=15);
  // defining initial position and size of each object and saving them into the array tab_obj_rect
  for(i = 0; i<15; i++){
    tab_obj_rect[i].w = 160;
    tab_obj_rect[i].h = 160;
  }
  tab_obj_rect[0].x = 32 ;
  tab_obj_rect[0].y = 40;
  tab_obj_rect[1].x = 32;
  tab_obj_rect[1].y = 160;
  tab_obj_rect[2].x = 32;
  tab_obj_rect[2].y = 320;
  tab_obj_rect[3].x = 32;
  tab_obj_rect[3].y = 480;
  tab_obj_rect[4].x = 224;
  tab_obj_rect[4].y = 160;
  tab_obj_rect[5].x = 224;
  tab_obj_rect[5].y = 320;
  tab_obj_rect[6].x = 224;
  tab_obj_rect[6].y = 480;
  tab_obj_rect[7].x = 799;
  tab_obj_rect[7].y = 0;
  tab_obj_rect[8].x = 799;
  tab_obj_rect[8].y = 160;
  tab_obj_rect[9].x = 799;
  tab_obj_rect[9].y = 320;
  tab_obj_rect[10].x = 799;
  tab_obj_rect[10].y = 480;
  tab_obj_rect[11].x = 416;
  tab_obj_rect[11].y = 480;
  tab_obj_rect[12].x = 608;
  tab_obj_rect[12].y = 480;
  tab_obj_rect[13].x = 416;
  tab_obj_rect[13].y = 320;
  tab_obj_rect[14].x = 608;
  tab_obj_rect[14].y = 320;
  
  //going back to the first char of the file
  rewind(fi);
  chOld = ' ';
  ch = ' ';
  lig = 0;
  col = 0;
  obj_counter=-1;//we set to -1 because the first object is the emtpy plan
  srand(time(0));//thats for initiating the random number generator
  while(!feof(fi)) {
    //in this part we are going to count the number of lines and columns of the object so that afterward we multiply them by block size and paste them in the right position
    chOld = ch;
    ch = fgetc(fi);
    if (ch=='\n' || ch == '\r'){ //a new line started
      //if there are two consecutive returns or next lines which implies an empty line
      if((chOld == '\n' && ch=='\r') || (chOld == '\n' && ch=='\n') || (chOld == '\r' && ch=='\r')){
        lig = -1;
        if (obj_counter>=0){
          SDL_SetColorKey(sur, SDL_TRUE, SDL_MapRGB(sur->format,0,0,0));
          tab_obj_tex[obj_counter] = SDL_CreateTextureFromSurface(renderer,sur);
          SDL_FillRect(sur,NULL, SDL_MapRGB(sur->format,0,0,0));
        }
        obj_counter++;
      }
      col = -1;
      lig++;
    }else{
      col++;
    }
    printf("%c",ch);
    //do this instruction for only objects, not terrain not empty lines
    if(obj_counter>=0 && obj_counter<*nbobj && lig >=0 && lig <5 && col>=0 && col<5 && ch == '#'){
      tile_temp.y = (obj_counter*32)%384;
      tile_temp.x =(rand()%3)*32;//choosing random cube between 3 available
      paste_rect.x = col*32;
      paste_rect.y = lig*32;
      SDL_BlitSurface(tile,&tile_temp,sur,&paste_rect);
    }
  }
  SDL_SetColorKey(sur, SDL_TRUE, SDL_MapRGB(sur->format,0,0,0));
  tab_obj_tex[obj_counter] = SDL_CreateTextureFromSurface(renderer,sur);
  
  SDL_FreeSurface(sur);
  SDL_FreeSurface(tile);
  fclose(fi);
}











