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
void charge_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font, SDL_Texture** texture, SDL_Rect *rectDest) {
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

void read_objects_v2(const char* nomfichier, int* nbobj, obj_t* tab_obj, SDL_Renderer *renderer){
  //this is the modified version of function read_objects where it will create a structure called obj_t for each object, the benfit of using this structure is that we can easily avoid using key color and hiding a huge amount od surface rendered as black. Instead the array tab_obj can contain the exact position of each cell and only that cell will be rendered and also it can be selected easier by their SDL_Rect values
  //Max nbobj = 15
  //first we must find how many objects are there in our file, keep in mind that the lines before the first return line is not an object of our interest in this function
  //we start number of object by 0 because the objects start after the first empty line
  int random;
  int i,j,t,lig,col,obj_counter;
  FILE *fi;
  *nbobj = -1;
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
    //printf("%c",ch);
    //do this instruction for only objects, not terrain not empty lines
    if(obj_counter>=0 && obj_counter<*nbobj && lig >=0 && lig <5 && col>=0 && col<5 && ch == '#'){
      random=(rand()%3);
      //printf("%d\n",random);
      tab_obj[obj_counter].tab_cell[lig][col]=random;
      //choosing random texture between 3 available for each object
    }
  }
  
  
  SDL_FreeSurface(sur_temp);
  SDL_FreeSurface(tile);
  SDL_FreeSurface(tile_hi);
  fclose(fi);
  free(tab_obj_rect_temp);
}


void mouse_trigger(SDL_Point* p,obj_t *tab_obj, int* GLOBAL_OBJ_SELECTED, int nbobj,int* dx, int* dy, int* GLOBAL_LAST_OBJ_SELECTED,SDL_Rect* cw_rect,SDL_Rect* ccw_rect, SDL_Rect* flip_rect, SDL_Rect* save_rect, long int* chronometer_counter, SDL_Window* win, SDL_Point* previous_position){
  //in this function we eill search if any object hast been selected
  int i,lig,col;
  for (i=0;i<nbobj; i++){
    for(lig=0; lig<5;lig++){
      for(col=0; col<5; col++){
        if(tab_obj[i].tab_cell[lig][col]>=0){
          if(myPointInRect(p,&tab_obj[i].tab_rect[lig][col])){
            //printf("Obj clicked on=%d,Obj clicked on=%d,Obj clicked on=%d,",i,i,i);
            *GLOBAL_OBJ_SELECTED = i;
            *GLOBAL_LAST_OBJ_SELECTED= i;
            //save the coords upper left corner of obj in previous_position so that if there is collision it returns to its previous place, keep in mind that mouse position is not acurate as an return point its much better to choose the upper left corner
            previous_position->x=tab_obj[i].tab_rect[0][0].x;
            previous_position->y=tab_obj[i].tab_rect[0][0].y;
            *dx=p->x-tab_obj[i].tab_rect[0][0].x;
            *dy=p->y-tab_obj[i].tab_rect[0][0].y;
            return;//to reduce calculations
          }  
        }
      }
    }
  }
  //if the area selected does not belong to any obj we check if any icon has been hit
  if(*GLOBAL_OBJ_SELECTED == -1){//if no obj is selected
    if(myPointInRect(p, cw_rect)){//if icon of cw is hit
      if (*GLOBAL_LAST_OBJ_SELECTED==-1){//if no obj has been selected up until now
        SDL_ShowSimpleMessageBox(0, "Rotate ClockWise", "No Object has been selected so far! please select an Object", win);
      }else{
        rotate_clockwise(&tab_obj[*GLOBAL_LAST_OBJ_SELECTED]);
      }
    }else if(myPointInRect(p, ccw_rect)){//if icon of ccw is hit
        
      if (*GLOBAL_LAST_OBJ_SELECTED==-1){//if no obj has been selected up until now
        SDL_ShowSimpleMessageBox(0, "Rotate Counter-ClockWise", "No Object has been selected so far! please select an Object", win);
      }else{
        rotate_counterclockwise(&tab_obj[*GLOBAL_LAST_OBJ_SELECTED]);
      }
    }else if(myPointInRect(p, flip_rect)){//if icon flip is hit
      
      if (*GLOBAL_LAST_OBJ_SELECTED==-1){//if no obj has been selected up until now
        SDL_ShowSimpleMessageBox(0, "Flip", "No Object has been selected so far! please select an Object", win);
      }else{
        flip(&tab_obj[*GLOBAL_LAST_OBJ_SELECTED]);
      }
    }else if(myPointInRect(p, save_rect)){//if icon save is hit
      save_button(tab_obj,*chronometer_counter,nbobj);
    }
  }
}

void mouse_rollover(SDL_Point* p,obj_t *tab_obj,int* GLOBAL_OBJ_SELECTED, int* GLOBAL_OBJ_HIGHLIGHTED,int* dx, int* dy, int* nbobj, SDL_Rect* cw_rect,SDL_Rect* ccw_rect, SDL_Rect* flip_rect, SDL_Rect* save_rect, SDL_Rect* load_rect, int* GLOBAL_ICON_ANIM, double* GLOBAL_ICON_ANGLE){
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
        //all the rects in obj should move no matter if full or empty
        tab_obj[*GLOBAL_OBJ_SELECTED].tab_rect[lig][col].x=p->x - *dx +(col*32);
        tab_obj[*GLOBAL_OBJ_SELECTED].tab_rect[lig][col].y=p->y - *dy +(lig*32);
        
      }
    }
  }else{//none obj is selected
    for (i=0;i<*nbobj; i++){
      for(lig=0; lig<5;lig++){
        for(col=0; col<5; col++){
          if(tab_obj[i].tab_cell[lig][col]>=0){
            if(myPointInRect(p,&tab_obj[i].tab_rect[lig][col])){
              *GLOBAL_OBJ_HIGHLIGHTED = i;
              return;
            }else{
              *GLOBAL_OBJ_HIGHLIGHTED = -1;
            }
          }  
        }
      }
    }
    //while none obj is selected we test to see if mouse is over icons
    if(myPointInRect(p,cw_rect)){
      *GLOBAL_ICON_ANIM = 0;
    }else if(myPointInRect(p,ccw_rect)){
      *GLOBAL_ICON_ANIM = 1;
    }else if(myPointInRect(p,flip_rect)){
      *GLOBAL_ICON_ANIM = 2;
    }else if(myPointInRect(p,save_rect)){
      *GLOBAL_ICON_ANIM = 3;
    }else if(myPointInRect(p,load_rect)){
      *GLOBAL_ICON_ANIM = 4;
    }else{//no icon
      *GLOBAL_ICON_ANIM = -1;
      *GLOBAL_ICON_ANGLE = 0;
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

void mouse_release(obj_t* obj, int* GLOBAL_OBJ_SELECTED, SDL_Rect* terrain, obj_t *tab_obj, int nbobj, SDL_Point* previous_position){
  //in this function we will place each object in the appropriate case if the urface area of intersection of terrain area and object is more than 70 percent of total surface of object
  if(*GLOBAL_OBJ_SELECTED<0){//if no object is selected to reduce the calculation we can skip the rest of func
    printf("mouse button released without selection\n");
    return;
  }
  
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
  if(((double)surface_intersection/surface_obj)>percent){//that means more than 70 percent of the are of the object is within the terrain
    for(lig=0; lig<5; lig++){
      for(col=0; col<5; col++){
        obj->tab_rect[lig][col].x=round_up(obj->tab_rect[lig][col].x ,32);
        obj->tab_rect[lig][col].y=round_up(obj->tab_rect[lig][col].y ,32);
      }
    }
  }
  if(selected_obj_collision(tab_obj, GLOBAL_OBJ_SELECTED, nbobj)){//test to verify if the selected object has collision with other objects in the gamme
    //reposition the selected obj to where it was before selection
    for(lig=0; lig<5; lig++){
      for(col=0; col<5; col++){
        obj->tab_rect[lig][col].x=previous_position->x+(col*32);
        obj->tab_rect[lig][col].y=previous_position->y+(lig*32);
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

bool myPointInRect(SDL_Point* p, SDL_Rect* rect)
{
  bool res = false;
  if(p->x >= rect->x && p->x <= (rect->x+rect->w) && p->y >= rect->y && p->y <= (rect->y + rect->h)){
    res = true;
  }
  return res;
}

void rotate_clockwise(obj_t* obj)
{
  int lig,col;
  int tab_temp[5][5];
  //copy the tab cell to a temporary array which we will use later
  for(lig=0; lig<5; lig++){
    for(col=0; col<5; col++){
      tab_temp[lig][col]= obj->tab_cell[lig][col];
    }
  }
  //rotating algorithm
  for(lig=0; lig<5; lig++){
    for(col=0; col<5; col++){
      obj->tab_cell[lig][col] = tab_temp[4-col][lig];
    }
  }
}

void rotate_counterclockwise(obj_t* obj)
{
  int lig,col;
  int tab_temp[5][5];
  //copy the tab cell to a temporary array which we will use later
  for(lig=0; lig<5; lig++){
    for(col=0; col<5; col++){
      tab_temp[lig][col]= obj->tab_cell[lig][col];
    }
  }
  //rotating algorithm
  for(lig=0; lig<5; lig++){
    for(col=0; col<5; col++){
      obj->tab_cell[lig][col] = tab_temp[col][4-lig];
    }
  }
}

void render_icons(SDL_Renderer* renderer, SDL_Texture* cw, SDL_Texture* ccw, SDL_Texture* flip_tex,SDL_Texture* save_tex, SDL_Texture* load_tex, SDL_Rect* rect_cw, SDL_Rect* rect_ccw, SDL_Rect* rect_flip, SDL_Rect* rect_save, SDL_Rect* rect_load, SDL_RendererFlip* flip_none, int* GLOBAL_ICON_ANIM, double* GLOBAL_ICON_ANGLE)
{
  //icon animation int value refers to the icons which the cursor is pointing at,-1=none, 0=cw and 1=ccw, 2=flip_icon, 3=save_icon, 4 =load icon
  //we render every item and repeat it in every case except the icon that is being touched by cursor then the rendering of that icon is different
  SDL_Rect src ={.x= 0, .y = 0, .h =64, .w =64};
  
  SDL_Point center={.x= 32, .y=31};
  if(*GLOBAL_ICON_ANIM==-1){//render normally as if no button has been touched
    SDL_RenderCopy(renderer, cw ,&src, rect_cw);
    SDL_RenderCopy(renderer, ccw ,&src, rect_ccw);
    SDL_RenderCopy(renderer, flip_tex ,&src, rect_flip);
    SDL_RenderCopy(renderer, save_tex ,&src, rect_save);
    SDL_RenderCopy(renderer, load_tex ,&src, rect_load);
    
  }else if(*GLOBAL_ICON_ANIM==0){//rotation cw
      SDL_RenderCopy(renderer, ccw ,&src, rect_ccw);
      SDL_RenderCopy(renderer, flip_tex ,&src, rect_flip);
      SDL_RenderCopy(renderer, save_tex ,&src, rect_save);
      SDL_RenderCopy(renderer, load_tex ,&src, rect_load);
      //different rendering for cw
      *GLOBAL_ICON_ANGLE = *GLOBAL_ICON_ANGLE+0.5;//increment angle degree by 1 
      SDL_RenderCopyEx(renderer,cw, &src, rect_cw, *GLOBAL_ICON_ANGLE, &center, *flip_none);

  }else if(*GLOBAL_ICON_ANIM==1){//*GLOBAL_ICON_ANIM=1 means rotation ccw touched
      SDL_RenderCopy(renderer, cw ,&src, rect_cw);
      SDL_RenderCopy(renderer, flip_tex ,&src, rect_flip);
      SDL_RenderCopy(renderer, save_tex ,&src, rect_save);
      SDL_RenderCopy(renderer, load_tex ,&src, rect_load);
      //different rendering for ccw
      *GLOBAL_ICON_ANGLE = *GLOBAL_ICON_ANGLE-0.5;//increment angle degree by 1 
      SDL_RenderCopyEx(renderer,ccw, &src, rect_ccw, *GLOBAL_ICON_ANGLE, &center, *flip_none);
  }else if(*GLOBAL_ICON_ANIM==2){//flip change color touched
    SDL_RenderCopy(renderer, cw ,&src, rect_cw);
    SDL_RenderCopy(renderer, ccw ,&src, rect_ccw);
    SDL_RenderCopy(renderer, save_tex ,&src, rect_save);
    SDL_RenderCopy(renderer, load_tex ,&src, rect_load);
    //darker rendering for flip icon
    SDL_SetTextureColorMod(flip_tex,100,100,100);
    SDL_RenderCopy(renderer, flip_tex ,&src, rect_flip);
    SDL_SetTextureColorMod(flip_tex,255,255,255);
  }else if(*GLOBAL_ICON_ANIM==3){//save icon touched
    SDL_RenderCopy(renderer, cw ,&src, rect_cw);
    SDL_RenderCopy(renderer, ccw ,&src, rect_ccw);
    SDL_RenderCopy(renderer, flip_tex ,&src, rect_flip);
    SDL_RenderCopy(renderer, load_tex ,&src, rect_load);
    //darker rendering for save icon
    SDL_SetTextureColorMod(save_tex,100,100,100);
    SDL_RenderCopy(renderer, save_tex ,&src, rect_save);
    SDL_SetTextureColorMod(save_tex,255,255,255);
  }else if(*GLOBAL_ICON_ANIM==4){//load icon touched
    SDL_RenderCopy(renderer, cw ,&src, rect_cw);
    SDL_RenderCopy(renderer, ccw ,&src, rect_ccw);
    SDL_RenderCopy(renderer, flip_tex ,&src, rect_flip);
    SDL_RenderCopy(renderer, save_tex ,&src, rect_save);
    //darker rendering for load icon
    SDL_SetTextureColorMod(load_tex,100,100,100);
    SDL_RenderCopy(renderer, load_tex ,&src, rect_load);
    SDL_SetTextureColorMod(load_tex,255,255,255);
  }
}

void flip(obj_t* obj)
{
  int lig,col;
  int tab_temp[5][5];
  //copy the tab cell to a temporary array which we will use later
  for(lig=0; lig<5; lig++){
    for(col=0; col<5; col++){
      tab_temp[lig][col]= obj->tab_cell[lig][col];
    }
  }
  //rotating algorithm
  for(lig=0; lig<5; lig++){
    for(col=0; col<5; col++){
      obj->tab_cell[lig][col] = tab_temp[lig][4-col];
    }
  }
}

bool selected_obj_collision(obj_t *tab_obj, int* GLOBAL_OBJ_SELECTED, int nbobj)
{
  //this function will determine if the selected is being place over another object or not
  //algorithme checks every filled rect of the selected obj with all fill rects present
  //for (int i=0;i<*nbobj; i++){//for each obj
  int i, lig_selected,col_selected, lig, col;
  for(lig_selected=0; lig_selected<5;lig_selected++){//for each line 0 to 4 of the selected obj
    for(col_selected=0; col_selected<5; col_selected++){//for each col 0 to 4 i.e. for each cell in our reserved box of our selected obj
      if(tab_obj[*GLOBAL_OBJ_SELECTED].tab_cell[lig_selected][col_selected]>=0){//the rects of selected object that are filled with texture not empty ones
        for (i=0;i<nbobj; i++){//for every object present in our game
          if(i==*GLOBAL_OBJ_SELECTED){
            //the possibility of collision of selected object and it self in iteration is 100 percent thus it should be avoided by this if statement
            continue;
          }
          for(lig=0; lig<5;lig++){//for every line of that object
            for(col=0; col<5; col++){//for every col of that obj
              if(tab_obj[i].tab_cell[lig][col]>=0){//if it is filled with texture
                if(SDL_HasIntersection(&tab_obj[*GLOBAL_OBJ_SELECTED].tab_rect[lig_selected][col_selected], &tab_obj[i].tab_rect[lig][col])){
                  //if the filled rect of the selected object has intersection with other rects in the game
                  printf("collision\n");
                  return true;
                  
                }
              }
            }
          }
        }
      }
    }
  }
  return false;
}

void chronometer(SDL_Renderer* renderer, long int* chronometer_counter, int* seconds_passed, SDL_Texture** time_tex, SDL_Rect* time_rect, TTF_Font* font)
{ 
  //this function will convert the milliseconds passed into seconds
  //it doesnt update the texture every millisecond instead it counts the number of seconds if it differs from last call of the function it will update the texture
  //this function return a SDL_texture and SDL_Rect correspondant
  int digit1,digit2,digit3,digit4;
  int text_w;
  int text_h;
  int temp= *chronometer_counter/1000;
  char* str_time=(char*)malloc(7*sizeof(char));
  if (*chronometer_counter == 0){//the first call of the function>>>it has to create texture
    //charge_text(renderer, 500, 500, str_time, font, &time_tex, time_rect);
    str_time = "00:00";
    SDL_Surface *sur;
    SDL_Color Color = {100, 200, 100, 0};
    sur = TTF_RenderText_Solid(font, str_time, Color); 
    *time_tex = SDL_CreateTextureFromSurface(renderer, sur);
    text_w = sur->w;
    text_h = sur->h;
    SDL_FreeSurface(sur);
    time_rect->x = 900;
    time_rect->y = 20;
    time_rect->w = text_w;
    time_rect->h = text_h;
  }
  
   if (temp > *seconds_passed){//it will update the texture only if number of seconds is incremented
    *seconds_passed = *chronometer_counter/1000;// chronometer counter count time in milli second and from that we extract each digit of chronometre up to 99:59
    printf("seconds passed=%d,\n", *seconds_passed);
    digit1 = (*chronometer_counter/1000)%10;
    digit2 = (*chronometer_counter/10000)%6;
    digit3 = (*chronometer_counter/60000)%10;
    digit4 = (*chronometer_counter/600000)%10;
    str_time[5] = ' ';
    str_time[4] = '0'+digit1;
    str_time[3] = '0'+digit2;
    str_time[2] = ':';
    str_time[1] = '0'+digit3;
    str_time[0] = '0'+digit4;
    //updating texture
    SDL_Surface *sur;
    SDL_Color Color = {100, 200, 100, 0};//light green
    sur = TTF_RenderText_Solid(font, str_time, Color); 
    *time_tex = SDL_CreateTextureFromSurface(renderer, sur);
    text_w = sur->w;
    text_h = sur->h;
    SDL_FreeSurface(sur);
    time_rect->x = 900;
    time_rect->y = 20;
    time_rect->w = text_w;
    time_rect->h = text_h;
  }
  
}

void render_objects(SDL_Renderer* renderer, obj_t *tab_obj,SDL_Rect* obj_src_rect , int* nbobj, int* GLOBAL_OBJ_SELECTED, int* GLOBAL_OBJ_HIGHLIGHTED)
{
  //this function reads the array containing objects and renders each ones
  int obj,lig,col,i;
  //loop for rendering objects
  for(obj = 0; obj<*nbobj;obj++){
    if(obj==*GLOBAL_OBJ_SELECTED){
      continue;//we have to render the selected object on top of other objects for this reason we skip its render and will do it at the end
    }
    //two loops for reading the array that has the information of existance of each cell of each object
    for(lig=0; lig<5; lig++){
      for(col=0; col<5; col++){
        i=tab_obj[obj].tab_cell[lig][col];
        if(i>=0){
          //if the value of array for that case is positive then a cell exists
          if(*GLOBAL_OBJ_HIGHLIGHTED != obj){
            SDL_RenderCopy(renderer, tab_obj[obj].tab_tex[i] ,obj_src_rect, &tab_obj[obj].tab_rect[lig][col]);
          }else{
            SDL_RenderCopy(renderer, tab_obj[obj].tab_tex[i+3] ,obj_src_rect, &tab_obj[obj].tab_rect[lig][col]);
          }
        }
      }
    }
  }
  //rendering the selected object on top
  if(*GLOBAL_OBJ_SELECTED >= 0){//make sure that we dont do it for -1 where no obj is selected at that moment
    for(lig=0; lig<5; lig++){
      for(col=0; col<5; col++){
        i=tab_obj[*GLOBAL_OBJ_SELECTED].tab_cell[lig][col];
        if(i>=0){
          //if the value of array for that case is positive then a cell exists
          SDL_RenderCopy(renderer, tab_obj[*GLOBAL_OBJ_SELECTED].tab_tex[i+3] ,obj_src_rect, &tab_obj[*GLOBAL_OBJ_SELECTED].tab_rect[lig][col]);
        }
      }
    }
  }
}

void save_button(obj_t *tab_obj,int chronometer_counter, int nbobj)
{
  //in this function we will save the upper left position of every object and their orientation(cells that are occupied) and time in a file
  //calculating second from milliseconds
  int obj,lig,col;
  FILE *fi;
  fi = fopen("saved_parameters.txt","w");
  int second= chronometer_counter/1000;
  fprintf(fi,"%04d",second);//the first line time is saved
  fputs("\n",fi);
  fprintf(fi,"%02d",nbobj);//the second line number of objects is saved
  fputs("\n",fi);
  for(obj=0; obj<nbobj; obj++){
    fprintf(fi,"%02d",obj);//next line print the obj index
    fputs(" ",fi);//separate with space
    fprintf(fi,"%03d ",tab_obj[obj].tab_rect[0][0].x);
    fprintf(fi,"%03d",tab_obj[obj].tab_rect[0][0].y);
    fputs("\n",fi);
  }
  fputs("/\n",fi);
  for(obj=0; obj<nbobj; obj++){
    for(lig=0; lig<5; lig++){
      for(col=0; col<5 ;col++){
        if(tab_obj[obj].tab_cell[lig][col]==-1){
          fputs("x ",fi);
        }else{
        fprintf(fi,"%01d ",tab_obj[obj].tab_cell[lig][col]);
        }
      }
    }
    fputs("\n",fi);
  }
  fclose(fi);
}






