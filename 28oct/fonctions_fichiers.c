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

void render_terrain(char** tab,int nblig, int nbcol,SDL_Point p, int size, SDL_Renderer* screen,SDL_Texture *tex, SDL_Rect* source){
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
          SDL_SaveBMP(sur,"kkkkkkkkkkkkkkkkkkkk");
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
      tile_temp.x =(rand()%3)*32;//choosing random square between 3 available
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

void read_objects_v2(const char* nomfichier, int* nbobj, obj_t* tab_obj, SDL_Renderer *renderer){
  //this is the modified version of function read_objects where it will create a structure called obj_t for each object, the benfit of using this structure is that we can easily avoid using key color and hiding a huge amount od surface rendered as black. Instead the array tab_obj can contain the exact position of each cell and only that cell will be rendered and also it can be selected easier by their SDL_Rect values
  //Max nbobj = 15
  //first we must find how many objects are there in our file, keep in mind that the lines before the first return line is not an object of our interest in this function
  //we start number of object by 0 because the objects start after the first empty line
  int random;
  int i,j,t,lig,col,obj_counter;
  FILE *fi;
  *nbobj = 0;
  fi = fopen(nomfichier,"r");
  char ch,chOld = ' ';
  //size that we have reserved for each cell is equal to 32pixels
  SDL_Surface* sur_temp = SDL_CreateRGBSurface(0,32,32,32,0,0,0,0);
  SDL_SetColorKey(sur_temp, SDL_TRUE, SDL_MapRGB(sur_temp->format,0,0,0));
  //rect for indication of the area of image tile that is going to be copied
  SDL_Rect tile_temp;
  tile_temp.w = 32;
  tile_temp.h = 32;
  //Loading the tile bitmap and tile highlighted
  SDL_Surface* tile = SDL_LoadBMP("tile32.bmp");
  SDL_Surface* tile_hi = SDL_LoadBMP("tile32hi.bmp");
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
  // defining initial position and size of each object and saving them into a temporary array of SDL_Rect called tab_obj_rect_temp
  SDL_Rect *tab_obj_rect_temp = (SDL_Rect *)malloc(15*sizeof(SDL_Rect));
  for(i = 0; i<15; i++){
    tab_obj_rect_temp[i].w = 32;
    tab_obj_rect_temp[i].h = 32;
  }
  tab_obj_rect_temp[0].x = 32 ;
  tab_obj_rect_temp[0].y = 40;
  tab_obj_rect_temp[1].x = 32;
  tab_obj_rect_temp[1].y = 160;
  tab_obj_rect_temp[2].x = 32;
  tab_obj_rect_temp[2].y = 320;
  tab_obj_rect_temp[3].x = 32;
  tab_obj_rect_temp[3].y = 480;
  tab_obj_rect_temp[4].x = 224;
  tab_obj_rect_temp[4].y = 160;
  tab_obj_rect_temp[5].x = 224;
  tab_obj_rect_temp[5].y = 320;
  tab_obj_rect_temp[6].x = 224;
  tab_obj_rect_temp[6].y = 480;
  tab_obj_rect_temp[7].x = 799;
  tab_obj_rect_temp[7].y = 32;
  tab_obj_rect_temp[8].x = 799;
  tab_obj_rect_temp[8].y = 160;
  tab_obj_rect_temp[9].x = 799;
  tab_obj_rect_temp[9].y = 320;
  tab_obj_rect_temp[10].x = 799;
  tab_obj_rect_temp[10].y = 480;
  tab_obj_rect_temp[11].x = 416;
  tab_obj_rect_temp[11].y = 480;
  tab_obj_rect_temp[12].x = 608;
  tab_obj_rect_temp[12].y = 480;
  tab_obj_rect_temp[13].x = 416;
  tab_obj_rect_temp[13].y = 320;
  tab_obj_rect_temp[14].x = 608;
  tab_obj_rect_temp[14].y = 320;
  //fill in the array containing the existance of cell by -1(default) i.e. empty also defining the sdl_rect for each cell
  for(t=0; t<15; t++){//for each obj
    for(i=0; i<5; i++){//for each line
      for(j=0 ; j<5; j++){//foreach cellin the line
        tab_obj[t].tab_cell[i][j]=-1;
        tab_obj[t].tab_rect[i][j].x=(tab_obj_rect_temp[t].x)+(j*32);
        tab_obj[t].tab_rect[i][j].y=(tab_obj_rect_temp[t].y)+(i*32);
        tab_obj[t].tab_rect[i][j].w=32;
        tab_obj[t].tab_rect[i][j].h=32;
      }
    }
  }
  //defining 3 different textures for each color of object
  for(t=0; t<15; t++){//for each obj
    for(i=0; i<6; i++){//for each of 3 textures
      if(i<3){//normal looking cells
        tile_temp.y = (t*32)%384;
        tile_temp.x = i*32;
        SDL_BlitSurface(tile,&tile_temp,sur_temp,NULL);
        tab_obj[t].tab_tex[i]= SDL_CreateTextureFromSurface(renderer,sur_temp);
        SDL_FillRect(sur_temp,NULL, SDL_MapRGB(sur_temp->format,0,0,0));
      }else{//i>=3highlighted cells
        tile_temp.y = (t*32)%384;
        tile_temp.x = (i-3)*32;
        SDL_BlitSurface(tile_hi,&tile_temp,sur_temp,NULL);
        tab_obj[t].tab_tex[i]= SDL_CreateTextureFromSurface(renderer,sur_temp);
        SDL_FillRect(sur_temp,NULL, SDL_MapRGB(sur_temp->format,0,0,0));
      }
    }
  }
  
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
          tab_obj[obj_counter].num_obj =obj_counter;
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
      random=(rand()%3);
      printf("%d\n",random);
      tab_obj[obj_counter].tab_cell[lig][col]=random;
      //choosing random texture between 3 available for each object
    }
  }
  
  
  SDL_FreeSurface(sur_temp);
  SDL_FreeSurface(tile);
  fclose(fi);
  free(tab_obj_rect_temp);
}


void mouse_trigger(SDL_Point* p,obj_t *tab_obj, int* GLOBAL_OBJ_SELECTED, int nbobj,int* dx, int* dy){
  //in this function we eill search if any object hast been selected
  int i,lig,col;
  for (i=0;i<nbobj; i++){
    for(lig=0; lig<5;lig++){
      for(col=0; col<5; col++){
        if(tab_obj[i].tab_cell[lig][col]>=0){
          if(SDL_PointInRect(p,&tab_obj[i].tab_rect[lig][col])){
            printf("Obj clicked on=%d,Obj clicked on=%d,Obj clicked on=%d,",i,i,i);
            *GLOBAL_OBJ_SELECTED = i;
            *dx=p->x-tab_obj[i].tab_rect[0][0].x;
            *dy=p->y-tab_obj[i].tab_rect[0][0].y;
            return;
          }  
        }
      }
    }
  }
}

void mouse_rollover(SDL_Point* p,obj_t *tab_obj,int* GLOBAL_OBJ_SELECTED, int* GLOBAL_OBJ_HIGHLIGHTED,int* dx, int* dy, int* nbobj){
  //this function first checks that if any object has been selected or not and performs the action required for each condition
  int i,lig,col;
  SDL_Point point_temp;
  if(*GLOBAL_OBJ_SELECTED>=0){//at leat an object is selected
    //using the function overpass we will know if it surpasses the borders or not
    point_temp.x = p->x - *dx;
    point_temp.y = p->y - *dy;
    if (overpass(tab_obj[*GLOBAL_OBJ_SELECTED], &point_temp)){
      return;
    }
    for(lig=0; lig<5; lig++){
      for(col=0; col<5 ;col++){
        if(tab_obj[*GLOBAL_OBJ_SELECTED].tab_cell[lig][col]>=0){
          tab_obj[*GLOBAL_OBJ_SELECTED].tab_rect[lig][col].x=p->x - *dx +(col*32);
          tab_obj[*GLOBAL_OBJ_SELECTED].tab_rect[lig][col].y=p->y - *dy +(lig*32);
        }
      }
    }
  }else{//none obj is selected
    for (i=0;i<*nbobj; i++){
      for(lig=0; lig<5;lig++){
        for(col=0; col<5; col++){
          if(tab_obj[i].tab_cell[lig][col]>=0){
            if(SDL_PointInRect(p,&tab_obj[i].tab_rect[lig][col])){
              *GLOBAL_OBJ_HIGHLIGHTED = i;
              return;
            }else{
              *GLOBAL_OBJ_HIGHLIGHTED = -1;
            }
          }  
        }
      }
    }
  }
}

bool overpass(obj_t obj, SDL_Point* p){
  //this functions will check if an object overpasses the borders of the window pre defined at starting point given(upperleft corner) send 0 if false send 1 if true
  int lig,col,xtemp,ytemp;
  //x temp is the x value of upper left corner of object
  xtemp = p->x;
  ytemp = p->y;
  // we suppose at the beginning that there is no collision between obj and borders
  //left border
  int xl1=0;
  int yl1=0;
  int xl2=0;
  int yl2=640;
  //right border
  int xr1=960;
  int yr1=0;
  int xr2=960;
  int yr2=640;
  //top border
  int xt1=0;
  int yt1=0;
  int xt2=960;
  int yt2=0;
  //boottom border
  int xb1=0;
  int yb1=640;
  int xb2=960;
  int yb2=640;
  //writing a switch case where the object is close to any border it will only calculate the collision with that border not 4 borders at the same time to decrese calculations
  //first case lef border
  if(xtemp<=200){
    for(lig=0; lig<5; lig++){
      for(col=0; col<5 ;col++){
        if(obj.tab_cell[lig][col]>=0){
          if (SDL_IntersectRectAndLine(&obj.tab_rect[lig][col], &xl1, &yl1, &xl2, &yl2)){
            return true;
          }
        }
      }
    }    
  }else{
    if(xtemp>=750){
      for(lig=0; lig<5; lig++){
        for(col=0; col<5 ;col++){
          if(obj.tab_cell[lig][col]>=0){
            if (SDL_IntersectRectAndLine(&obj.tab_rect[lig][col], &xr1, &yr1, &xr2, &yr2)){
              return true;
            }
          }
        }
      }
    }    
  }
  
  if(ytemp<=160){
    for(lig=0; lig<5; lig++){
      for(col=0; col<5 ;col++){
        if(obj.tab_cell[lig][col]>=0){
          if (SDL_IntersectRectAndLine(&obj.tab_rect[lig][col], &xt1, &yt1, &xt2, &yt2)){
            return true;
          }
        }
      }
    }    
  }else{
    if(xtemp>=430){
      for(lig=0; lig<5; lig++){
        for(col=0; col<5 ;col++){
          if(obj.tab_cell[lig][col]>=0){
            if (SDL_IntersectRectAndLine(&obj.tab_rect[lig][col], &xb1, &yb1, &xb2, &yb2)){
              return true;
            }
          }
        }
      }
    }    
  }
  return false;
}

void mouse_release(obj_t* obj, int* GLOBAL_OBJ_SELECTED, SDL_Rect* terrain){
  //in this function we will place each object in the appropriate case if the urface area of intersection of terrain area and object is more than 70 percent of total surface of object
  
  SDL_Rect rect_temp={.x=0, .y=0, .w=0, .h=0};
  int surface_intersection,surface_obj,lig,col;
  double percent=0.7;
  surface_obj=0;
  surface_intersection=0;
  for(lig=0; lig<5; lig++){
    for(col=0; col<5; col++){
      if(obj->tab_cell[lig][col]>=0){
        surface_obj+=1024;//each cell has surface are of 1024 pixels
        if(SDL_IntersectRect(terrain, &obj->tab_rect[lig][col],&rect_temp)){
          surface_intersection+=(rect_temp.w*rect_temp.h);
        }
      }
    }
  }  
  printf("surface intersection=%d\n",surface_intersection);
  printf("surface obj=%d\n",surface_obj);
  if(((double)surface_intersection/surface_obj)>percent){
    for(lig=0; lig<5; lig++){
      for(col=0; col<5; col++){
        if(obj->tab_cell[lig][col]>=0){
          printf("gone!!!!!!!");
          obj->tab_rect[lig][col].x=round_up(obj->tab_rect[lig][col].x ,32);
          obj->tab_rect[lig][col].y=round_up(obj->tab_rect[lig][col].y ,32);
        }
      }
    }
  }
  *GLOBAL_OBJ_SELECTED=-1;
}

int round_up(int numToRound, int multiple)
{
  if(multiple == 0){
    return numToRound;
  }
  int remainder = numToRound % multiple;
  if(remainder == 0){
    return numToRound;
  }
  if(remainder > (multiple/2)){
    return numToRound + multiple - remainder;
  }else{
    return numToRound - remainder;
  }
  
}














