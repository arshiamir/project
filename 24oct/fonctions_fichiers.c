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

void test(const char* nomFichier){
  //char** in;
  char** out;
  int lig,col;
  taille_fichier(nomFichier,&lig,&col);
  //in = allouer_tab_2D(lig,col);
  out = allouer_tab_2D(lig,col);
  afficher_fichier(nomFichier);
  //in = lire_fichier(nomFichier);
  //out= modifier_caractere(in,lig,col,'*','x');
  afficher_tab_2D(out,lig,col);
  ecrire_fichier("output.txt",out,lig,col);
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
  printf("After replacing all the characters (i.e:#) in source file with '*' the final Terrain array looks like:\n");
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
  SDL_Color Color = {255, 255, 255, 0};
  sur = TTF_RenderText_Solid(font, text, Color);
  *texture = SDL_CreateTextureFromSurface(renderer, sur);
  text_w = sur->w;
  text_h = sur->h;
  SDL_FreeSurface(sur);
  rectDest->x = x;
  rectDest->y = y;
  rectDest->w = text_w;
  rectDest->h = text_h;
  printf("gone");
}










