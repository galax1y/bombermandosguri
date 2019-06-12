#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#define APPC 22 //define Altura de pixels por char da matriz
#define LPPC 22 //define Largura de pixels por char da matriz
#define MAXLINHA 25
#define MAXCOLUNA 60
#define A_TELA 550//define altura da tela
#define L_TELA 1320//define largura tela
#define FPS 30

void mainMenu();
void gamePause();
void loadGame();
void saveGame();
void renderMap();
//void updatePos();
void updateGame();
void comandoJogador();
void renderback();

/*int main(){
    mainMenu();
    while(1){
        while(!kbhit()){
            if (fps % sla == valido){
                updateGame();
            }
        }
        comandoJogador();
    }
}*/

void renderback(char gameMap[][MAXCOLUNA])//funcao le todas as paredes indstrutiveis e espaços em branco e salva em um png para salvar processamento
{
    int i,j;
    ALLEGRO_BITMAP *wall,*blank,*background = al_create_bitmap(L_TELA,A_TELA);
    al_set_target_bitmap(background);
    for(i=0; i<MAXLINHA; i++)
    {
        for(j=0; j<MAXCOLUNA; j++)
        {
            if(gameMap[i][j] == 'W')
            {
                wall = al_load_bitmap("paredeindes.png");
                al_draw_bitmap(wall, LPPC*j, APPC*i, 0);
            }
            else if(gameMap[i][j] == ' ' || gameMap[i][j] == 'Q')
            {
                blank = al_load_bitmap("blank.png");
                al_draw_bitmap(blank, LPPC*j, APPC*i, 0);
            }
        }
    }

    al_save_bitmap("background.png", background);
}

 void render(char gameMap[][MAXCOLUNA], ALLEGRO_DISPLAY* janela, int* playerposX, int* playerposY){
    int i,j;

     al_clear_to_color(al_map_rgb(195,195,195));

     ALLEGRO_BITMAP *back,*playerbmp,*box,*key,*enemy;
     back = al_load_bitmap("background.png");
     al_draw_bitmap(back,0,0,0);

    for(i=0;i<MAXLINHA;i++){
        for(j=0;j<MAXCOLUNA;j++){
            printf("%c",gameMap[i][j]);

            if (gameMap[i][j] == 's'){
                *playerposX = j;
                *playerposY = i;
                playerbmp = al_load_bitmap("dino.png");
                al_convert_mask_to_alpha(playerbmp, al_map_rgb(255,0,255));

                al_draw_bitmap(playerbmp, LPPC*j, APPC*i, 0);
                //al_flip_display();
            }
            else if(gameMap[i][j] == 'D'){
                box = al_load_bitmap("nuvem.png");
                al_draw_bitmap(box, LPPC*j, APPC*i, 0);
                //al_flip_display();
            }
            else if(gameMap[i][j] == 'E'){
                enemy = al_load_bitmap("enemy.png");
                al_convert_mask_to_alpha(enemy, al_map_rgb(255,0,255));
                al_draw_bitmap(enemy, LPPC*j, APPC*i, 0);
                //al_flip_display();
            }
            else if(gameMap[i][j] == 'a'){
                *playerposX = j;
                *playerposY = i;
                playerbmp = al_load_bitmap("dino.png");
                al_convert_mask_to_alpha(playerbmp, al_map_rgb(255,0,255));

                al_draw_bitmap(playerbmp, LPPC*j, APPC*i,ALLEGRO_FLIP_HORIZONTAL);
                //al_flip_display();
            }
            else if(gameMap[i][j] == 'd'){
                *playerposX = j;
                *playerposY = i;
                playerbmp = al_load_bitmap("dino.png");
                al_convert_mask_to_alpha(playerbmp, al_map_rgb(255,0,255));

                al_draw_bitmap(playerbmp, LPPC*j, APPC*i,0);
                //al_flip_display();
            }
        }
        printf("\n");
    }
    al_flip_display();
}

   void updatePos(char gameMap[][MAXCOLUNA], ALLEGRO_DISPLAY* janela, ALLEGRO_BITMAP* imagem, char keyPressed, int* playerposX, int* playerposY){
       imagem = al_load_bitmap("dino.png");
       printf("entrei no updatepos\n");
       switch(keyPressed){
           case 'w':
               gameMap[*playerposY][*playerposX] = ' ';
               playerposY--;
               gameMap[*playerposY][*playerposX] = 'w';
               break;
           case 'a':
               gameMap[*playerposY][*playerposX] = ' ';
               playerposX--;
               gameMap[*playerposY][*playerposX] = 'a';
               break;
           case 's':
               gameMap[*playerposY][*playerposX] = ' ';
               playerposY++;
               gameMap[*playerposY][*playerposX] = 'a';
               break;
           case 'd':
               gameMap[*playerposY][*playerposX] = ' ';
               playerposX++;
               gameMap[*playerposY][*playerposX] = 'd';
               break;
       }
       al_flip_display();

       printf("paraaaaa");
       system("pause");
   }

int main(){
    int i,j;
    char gameMap[MAXLINHA][MAXCOLUNA] = {{"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"},{"QWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWQ"},{"QWW                                                     EWWQ"},{"QWWWWWW  WW  WWWWWWWW  WW  WW  WW  WW  WWWWWWWWDDWW  WWWWWWQ"},{"QWW      WW                                      WW      WWQ"},{"QWW  WW  WW  WW  WW    WWWWWW  WWWWWW    WW  WW  WW  WW  WWQ"},{"QWW  WW  WW    WWB     WW          WW      WW    WW  WW  WWQ"},{"QWW  WW  WW  WW  WW    WW  WW  WW  WW    WW KWW  WW  WW  WWQ"},{"QWW  WW  WW                WW  WW  DD            WW  WW  WWQ"},{"QWW  WW  WW                WW  WW  DD            WW  WW  WWQ"},{"QWW  WW  WWWWWWWWWW    WW      B   WW    WWWWWWWWWW  WW  WWQ"},{"QWWK     WW                WW  WW                WW     KWWQ"},{"QWWWW    DD   EWWWWWW      WW  WW      WWWWWWE         WWWWQ"},{"QWW      WW                WW  WW               BWW      WWQ"},{"QWW  WW  WWWWWWWWWW    WW   B      WW    WWWWWWWWWW  WW  WWQ"},{"QWW  WW  WWB           DD  WW  WW        DD     BWW  WW  WWQ"},{"QWW  WW  WW            DD  WW  WW        DD      WW  WW  WWQ"},{"QWW  WW  WW  WW  WW    WW  WW  WW  WW    WW  WW  WW  WW  WWQ"},{"QWW  WW  WW   BWW      WW          WW      WW    WW  WW  WWQ"},{"QWW  WW  WW  WW  WW    WWWWWW  WWWWWW   KWWB WW  WW  WW  WWQ"},{"QWW      WW                                      WW      WWQ"},{"QWWWWWW  WW  WWWWWWWW  WW  WW  WW  WW  WWWWWWWW  WW  WWWWWWQ"},{"QWWE    sK                                      DD      EWWQ"},{"QWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWQ"},{"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"}};
    int playerposX;
    int playerposY;

    char keyPressed;

    al_init();
    al_init_image_addon(); // inicializa

    renderback(gameMap);

    ALLEGRO_BITMAP *imagem = NULL;
    ALLEGRO_DISPLAY *janela = NULL; // "abre" o display chamado janela
    janela = al_create_display(L_TELA,A_TELA); // definições do display onde vamos colocar imagens

   // al_flip_display() // atualiza a tela
   // imagem = al_load_bitmap("dino.png"); // carrega a imagem que vai ser imprimida quando pedirmor pra desenhar

   render(gameMap, janela, &playerposX, &playerposY);

   /*while(1){
        keyPressed = getch();
        printf("coletei o caracter\n");
        updatePos(gameMap, janela,imagem, keyPressed, &playerposX, &playerposY);
   }
   system("PAUSE");
    */
    system("PAUSE");
    al_destroy_display(janela);
    al_destroy_bitmap(imagem);
}
