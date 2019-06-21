
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>

#define TAM_MENU_P 5
#define MAXLINHA 25
#define MAXCOLUNA 61
#define MAXBOMBAS 30
#define FPS 30.0
#define LPPC 64
#define APPC 64

#define MAIN_MENU 0
#define SINGLEPLAYER 1
#define MULTIPLAYER 2
#define SUBMENU_CHAR 3
#define SUBMENU_SETTINGS 4
#define SAIR_JOGO 5
#define MENU_PAUSE 6

typedef struct displaydejogo
{
    float largura,altura;
} tamanhoJogo;

typedef struct entidades
{
    int posX,posY;
} posEntidade;

typedef struct informacoes{
    int level;
	int vidas;
	int numBombas;
	int numChaves;
	int score;
	int posXBomba[MAXBOMBAS]; // esse numero dentro limita quantas bombas pode ter no mapa, se tiver mais q isso, buga
	int posYBomba[MAXBOMBAS];
	int tempoBomba[MAXBOMBAS];
}playerInfo;

ALLEGRO_BITMAP *jogador, *paredeDES, *paredeIND, *caixa, *bomba, *blank, *inimigo, *icon, *key = NULL,*life=NULL;
ALLEGRO_DISPLAY *telaJogo = NULL;
ALLEGRO_BITMAP *gamescreen=NULL;
ALLEGRO_SAMPLE *efeitoSonoro = NULL;
ALLEGRO_TIMER *tempo = NULL;
ALLEGRO_AUDIO_STREAM *musicaFundo = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;


ALLEGRO_BITMAP *menubmp=NULL;
ALLEGRO_BITMAP *wall=NULL;
ALLEGRO_BITMAP *deswall=NULL;
ALLEGRO_BITMAP *logo=NULL;
ALLEGRO_BITMAP *c_player=NULL;
ALLEGRO_BITMAP *player1=NULL;
ALLEGRO_BITMAP *player2=NULL;
ALLEGRO_BITMAP *player3=NULL;
ALLEGRO_BITMAP *player4=NULL;
ALLEGRO_BITMAP *player5=NULL;
ALLEGRO_BITMAP *player6=NULL;
ALLEGRO_BITMAP *player8=NULL;


ALLEGRO_BITMAP *mapa=NULL;
ALLEGRO_BITMAP *level1=NULL;
ALLEGRO_BITMAP *level2=NULL;
ALLEGRO_BITMAP *level3=NULL;
ALLEGRO_BITMAP *level4=NULL;
ALLEGRO_BITMAP *level5=NULL;
ALLEGRO_BITMAP *level6=NULL;
ALLEGRO_BITMAP *level8=NULL;

ALLEGRO_FONT *fonte_menu,*fonte_menu_small;
ALLEGRO_COLOR color;

void gotoxy(int x, int y)
{
    COORD coord = {0,0};
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// prototipos
void submenuchar();
void submenulevels();
void mainmenu();
void submenusettings();

void executaJogo        (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA,tamanhoJogo *tela, int gameMode,char arquivo[], int* estado_menu);
void punch              (char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA);
int avalia_punch        (char obstaculo, char lado);
void moveEnemy          (char gameMap[][MAXCOLUNA], posEntidade enemy[], int direcao, int i, playerInfo* PlayerA);
void updateEnemies      (char gameMap[][MAXCOLUNA], posEntidade enemy[], char instrucao, playerInfo* PlayerA);
//void updatePosAllegro   (char gamemap[][MAXCOLUNA], posEntidade* player, char keyPressed);
void renderAllegro      (char gameMap[][MAXCOLUNA], char inst);
void mainMenu		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void comandoJogador     (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], char keyPressed, playerInfo* PlayerA);
void renderMap		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[]);
void updatePos		    (char gameMap[][MAXCOLUNA], posEntidade* player, char keyPressed, playerInfo* PlayerA);
//void loadGame		    (char gameMap[][MAXCOLUNA], char keyPressed, posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void loadGame		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int* level,char arquivo[]);
int gamePause		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void saveGame		    (char gameMap[][MAXCOLUNA], playerInfo* PlayerA);
void updateGame		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int* cont_tempo);
void setBomb		    (char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA, char keyPressed);
void showPos            (posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void explodeBomb        (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int indice);
/*
void multiplayer();
*/

void game_mode_zoom(posEntidade* player, tamanhoJogo*tela)
{
    if((*player).posY<6)
    {
        if((*player).posX<15)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,0,(*tela).largura/30,(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>=15 &&(*player).posX<=30)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,-((*player).posX-14)*((*tela).largura/30),((*tela).altura-((*tela).largura)*25/60)/2,(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>30 &&(*player).posX<=45)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,-((*player).posX-15)*((*tela).largura/30),((*tela).altura-((*tela).largura)*25/60)/2,(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>45)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,(-(*tela).largura),((*tela).altura-((*tela).largura)*25/60)/2,(*tela).largura*2,((*tela).largura)*25/30,0);
        }
    }
    else if((*player).posY>=6&&(*player).posY<=18)
    {
        if((*player).posX<15)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,0,((((*tela).altura-((*tela).largura)*25/60)/2)-(((*player).posY-4)*((*tela).altura/12.5)))+16*(*player).posY,(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>=15 &&(*player).posX<=30)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,-((*player).posX-14)*((*tela).largura/30),((((*tela).altura-((*tela).largura)*25/60)/2)-(((*player).posY-5)*((*tela).altura/12.5))),(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>30 &&(*player).posX<=45)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,-((*player).posX-15)*((*tela).largura/30),((((*tela).altura-((*tela).largura)*25/60)/2)-(((*player).posY-5)*((*tela).altura/12.5))),(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>45)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,(-(*tela).largura),((((*tela).altura-((*tela).largura)*25/60)/2)-(((*player).posY-5)*((*tela).altura/12.5))),(*tela).largura*2,((*tela).largura)*25/30,0);
        }
    }
    else if((*player).posY>18)
    {
        if((*player).posX<15)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,0,((((*tela).altura-((*tela).largura)*25/60)/2)-(*tela).largura*25/60),(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>=15 &&(*player).posX<=30)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,-((*player).posX-14)*((*tela).largura/30),((((*tela).altura-((*tela).largura)*25/60)/2)-(*tela).largura*25/60),(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>30 &&(*player).posX<=45)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,-((*player).posX-15)*((*tela).largura/30),((((*tela).altura-((*tela).largura)*25/60)/2)-(*tela).largura*25/60),(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>45)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,(-(*tela).largura),((((*tela).altura-((*tela).largura)*25/60)/2)-(*tela).largura*25/60),(*tela).largura*2,((*tela).largura)*25/30,0);
        }
    }
}
void definescreensize(tamanhoJogo *tela)
{
    int largura_monitor = al_get_display_width(telaJogo);
    int altura_monitor = al_get_display_height(telaJogo);

    printf("%dx%d\n",largura_monitor,altura_monitor);
    if((largura_monitor/altura_monitor)<=(16/9))
    {
        (*tela).largura=largura_monitor;
        (*tela).altura=(float)(*tela).largura*9/16;
    }
    else if((largura_monitor/altura_monitor)>(16/9))
    {
        (*tela).altura=altura_monitor;
        (*tela).largura=(float)(*tela).altura*16/9;
    }
    //printf("%.0fx%.0f\n",(*tela).largura,(*tela).altura);
    gamescreen=al_create_bitmap(3840,1600);
}

int initializeAllegro()
{
    if (!al_init())
    {
        printf("Falha ao inicializar a Allegro");
        return 0;
    }
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW); //seta tela para fullscreen
    telaJogo = al_create_display(0,0); //cria display. o tamanho vai ser automatico
    if(!telaJogo)
    {
        printf("Falha ao criar Tela");
        return 0;
    }

    if (!al_init_image_addon())
    {
        printf("Falha ao inicializar addon de imagens");
        return 0;
    }

    if (!al_install_keyboard())
    {
        printf("Falha ao inicializar o teclado");
        return 0;
    }
    tempo = al_create_timer(1.0 / FPS);
    //al_get_display_mode(al_get_num_display_modes() - 1, &janela_info);

    fila_eventos = al_create_event_queue();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(64);
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    jogador = al_create_bitmap(256,192);
    bomba = al_load_bitmap("imagens/bomb.png");
    caixa = al_load_bitmap("imagens/box.png");
    paredeDES = al_load_bitmap("imagens/nuvem.png");
    paredeIND = al_load_bitmap("imagens/paredeindes.png");
    blank = al_load_bitmap("imagens/blank.png");
    inimigo = al_load_bitmap("imagens/enemynew.png");
    icon = al_load_bitmap("imagens/dinoicon.png");
    menubmp = al_load_bitmap("imagens/gamemenu.png");
    wall = al_load_bitmap("imagens/wall1.png");
    deswall = al_load_bitmap("imagens/wall.png");
    key = al_load_bitmap("imagens/key.png");
    life= al_load_bitmap("imagens/life.png");
    player1 = al_load_bitmap("imagens/spritesheetbertotto.png");
    player2 = al_load_bitmap("imagens/spritesheetwermann.png");
    player3 = al_load_bitmap("imagens/spritesheetspidey.png");
    player4 = al_load_bitmap("imagens/spritesheetbat.png");
    player5 = al_load_bitmap("imagens/spritesheetskywalker.png");
    player6 = al_load_bitmap("imagens/spritesheetrobocop.png");
    player8 = al_load_bitmap("imagens/dino.png");
    logo=al_load_bitmap("imagens/logo.png");
    if(!bomba||!caixa||!paredeDES) //colocar outros casos
    {
        printf("Falha ao carregar imagens");
        return 0;
    }

    al_convert_mask_to_alpha(player1, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player2, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player3, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player4, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player5, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player6, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player8, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(jogador, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(bomba, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(inimigo, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(key, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(life, al_map_rgb(255, 0, 255));

    fonte_menu_small = al_load_font("ka1.ttf", 36, 0);
    fonte_menu = al_load_font("ka1.ttf", 48, 0);

    musicaFundo = al_load_audio_stream("abc.wav",4,1024);
    al_attach_audio_stream_to_mixer(musicaFundo, al_get_default_mixer());
    al_set_audio_stream_playmode(musicaFundo, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_gain(musicaFundo, 0.2);

    al_register_event_source(fila_eventos, al_get_timer_event_source(tempo));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(telaJogo));
    al_start_timer(tempo);
    // definescreensize();
    return 1;
}

void choose_your_fighter(int *char_escolhido){
        if (*char_escolhido == 1)
        {
            jogador=player1;
        }
        else if (*char_escolhido == 2)
        {
            jogador=player2;
        }
}


void renderAllegro(char gameMap[][MAXCOLUNA],char inst)
{
    al_set_target_bitmap(gamescreen);
    int i,j;
    switch(inst)
    {
    case 'l':
        al_clear_to_color(al_map_rgb(195,195,195));
        for (i=0; i<MAXLINHA; i++)
        {
            for(j=0; j<MAXCOLUNA; j++)
            {
                switch(gameMap[i][j])
                {
                case 'W':
                    al_draw_scaled_bitmap(wall,0,0,32,32, j*LPPC, i*APPC,64,64, 0);
                    break;
                case 'D':
                    al_draw_scaled_bitmap(deswall,0,0,32,32, j*LPPC, i*APPC,64,64, 0);
                    break;
                case 'K':
                    al_draw_scaled_bitmap(caixa,0,0,14,14, j*LPPC, i*APPC,64,64, 0);
                    break;
                case 'B':
                    al_draw_scaled_bitmap(bomba,0,0,22,22, j*LPPC, i*APPC,64,64, 0);
                    break;
                case 'E':
                    al_draw_scaled_bitmap(inimigo,0,0,32,32, j*LPPC, i*APPC,64,64, 0);
                    break;
                case 'J':
                    al_draw_scaled_bitmap(jogador, 0,0,32,32,j*LPPC, i*APPC,64,64, 0);
                    break;
                case 'w':
                    al_draw_scaled_bitmap(jogador, 128,32,32,32,j*LPPC, i*APPC,64,64, 0);
                    break;
                case 'a':
                    al_draw_scaled_bitmap(jogador, 0,0,32,32,j*LPPC, i*APPC,APPC,APPC, ALLEGRO_FLIP_HORIZONTAL);
                    break;
                case 's':
                    al_draw_scaled_bitmap(jogador, 0,32,32,32,j*LPPC, i*APPC,APPC,APPC, 0);
                    break;
                case 'd':
                    al_draw_scaled_bitmap(jogador, 0,0,32,32,j*LPPC, i*APPC,64,64, 0);
                    break;
                default:
                    al_draw_scaled_bitmap(blank,0,0,22,22, j*LPPC, i*APPC,64,64, 0);
                    break;
                }
            }
        }
        case 'r':
            for (i=0; i<MAXLINHA; i++)
            {
                for(j=0; j<MAXCOLUNA; j++)
                {
                    switch(gameMap[i][j])
                    {
                    case 'W':
                        al_draw_scaled_bitmap(wall,0,0,32,32, j*LPPC, i*APPC,64,64, 0);
                        break;
                    case 'D':
                        al_draw_scaled_bitmap(deswall,0,0,32,32, j*LPPC, i*APPC,64,64, 0);
                        break;
                    case 'K':
                        al_draw_scaled_bitmap(caixa,0,0,14,14, j*LPPC, i*APPC,64,64, 0);
                        break;
                    case 'B':
                        al_draw_scaled_bitmap(bomba,0,0,22,22, j*LPPC, i*APPC,64,64, 0);
                        break;
                    case 'E':
                        al_draw_scaled_bitmap(inimigo,0,0,32,32, j*LPPC, i*APPC,64,64, 0);
                        break;
                    case 'J':
                        al_draw_scaled_bitmap(jogador, 0,0,32,32,j*LPPC, i*APPC,64,64, 0);
                        break;
                    case 'w':
                        al_draw_scaled_bitmap(jogador, 128,32,32,32,j*LPPC, i*APPC,64,64, 0);
                        break;
                    case 'a':
                        al_draw_scaled_bitmap(jogador, 0,0,32,32,j*LPPC, i*APPC,APPC,APPC, ALLEGRO_FLIP_HORIZONTAL);
                        break;
                    case 's':
                        al_draw_scaled_bitmap(jogador, 0,32,32,32,j*LPPC, i*APPC,APPC,APPC, 0);
                        break;
                    case 'd':
                        al_draw_scaled_bitmap(jogador, 0,0,32,32,j*LPPC, i*APPC,64,64, 0);
                        break;
                    case 'C':
                        al_draw_scaled_bitmap(blank,0,0,22,22, j*LPPC, i*APPC,64,64, 0);
                        al_draw_scaled_bitmap(key, 0,0,32,32,j*LPPC, i*APPC,64,64, 0);
                        break;
                    default:
                        al_draw_scaled_bitmap(blank,0,0,22,22, j*LPPC, i*APPC,64,64, 0);
                        break;
                    }
                }

            }
        }
    // al_save_bitmap("background.png", gamescreen);
    al_set_target_bitmap(al_get_backbuffer(telaJogo));
    //al_flip_display();
}
void renderInfoAllegro(playerInfo* PlayerA, tamanhoJogo*tela)
{
    int i;
    al_set_target_bitmap(al_get_backbuffer(telaJogo));
    color=al_map_rgb(195,195,195);
    // (*PlayerA).vidas=3;
    printf("\n%d\n",(*PlayerA).vidas);
    al_draw_filled_rectangle(0,((*tela).altura/2)+((*tela).largura*25/120),(*tela).largura,(*tela).altura,color);
    if((*PlayerA).vidas==3)
    {
        al_draw_scaled_bitmap(life,0,0,11,10,140,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
        al_draw_scaled_bitmap(life,0,0,11,10,44,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
        al_draw_scaled_bitmap(life,0,0,11,10,92,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
    }
    else if((*PlayerA).vidas==2)
    {
        al_draw_scaled_bitmap(life,0,0,11,10,0,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
        al_draw_scaled_bitmap(life,0,0,11,10,44,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
        al_draw_scaled_bitmap(life,11,0,11,10,88,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
    }
    else if((*PlayerA).vidas==1)
    {
        al_draw_scaled_bitmap(life,0,0,11,10,0,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
        al_draw_scaled_bitmap(life,11,0,11,10,44,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
        al_draw_scaled_bitmap(life,11,0,11,10,88,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
    }
    else if((*PlayerA).vidas==0)
    {
        al_draw_scaled_bitmap(life,11,0,11,10,44,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
        al_draw_scaled_bitmap(life,11,0,11,10,132,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
        al_draw_scaled_bitmap(life,11,0,11,10,88,(((*tela).altura-((*tela).largura)*25/60)/4)-20,44,40,0);
    }
    if((*PlayerA).numChaves==5)
    {
        for(i=0; i<5; i++)
        {
            al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*i),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);
        }
    }
    else if((*PlayerA).numChaves==4)
    {
        for(i=0; i<4; i++)
        {
            al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*i),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);
        }
        al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*4),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);
    }
    else if((*PlayerA).numChaves==3)
    {
        for(i=0; i<3; i++)
        {
            al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*i),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);
        }
        al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*3),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);
        al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*4),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);
    }
    else if((*PlayerA).numChaves==2)
    {
        for(i=0; i<3; i++)
        {
            al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*(i+2)),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);
        }
        al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*0),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);
        al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*1),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);
    }
    else if((*PlayerA).numChaves==1)
    {
        for(i=0; i<4; i++)
        {
            al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*(i+1)),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);
        }
        al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*0),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);

    }
    else if((*PlayerA).numChaves==0)
    {
        for(i=0; i<5; i++)
        {
            al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*i),(((*tela).altura-((*tela).largura)*25/60)/4)-32,64,64,0);
        }


    }



}

void renderMap(char gameMap[MAXLINHA][MAXCOLUNA], posEntidade* player, posEntidade enemy[])   //
{
    int i, j, k, l;
    //k = 0;
    system("cls");
    for (i = 0; i < MAXLINHA; i++)
    {
        for (j = 0; j < MAXCOLUNA; j++)
        {
            if (gameMap[i][j] == 'W')
            {
                printf("#");
            }
            else if (gameMap[i][j] == 'D')
            {
                printf("&");
            }
            else if (gameMap[i][j] == 'K')
            {
                printf("K");
            }
            else if (gameMap[i][j] == 'B')
            {
                printf("B");
            }
            else
                printf("%c",gameMap[i][j]);
        }
    }
}


void loadGame(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int* level,char arquivo[]){
	FILE *fp;
	int i,j,k = 0;
	int indice = 0;
//printf("\n%s", arquivo);
	//char arquivo[50] = {0};
	char extensao[5] = {".txt"};
	char info[60] = {"info"};

	//gets(arquivo);

	strcat(arquivo,extensao);
	fp = fopen(arquivo, "r");
    //printf("\n%s", arquivo);
    //system("pause");
	for(i=0; i<MAXLINHA; i++){
        for(j=0; j<MAXCOLUNA; j++){
            fscanf(fp, "%c", &gameMap[i][j]);
            //printf("%c",gameMap[i][j]);
            if (gameMap[i][j] == 'w' || gameMap[i][j] == 'a' || gameMap[i][j] == 's' || gameMap[i][j] == 'd' || gameMap[i][j] == 'J'){
                (*player).posX = j;
                (*player).posY = i;
            }
            else if (gameMap[i][j] == 'B'){
                (*PlayerA).posXBomba[indice] = j;
                (*PlayerA).posYBomba[indice] = i;
                indice++;
            }
            else if (gameMap[i][j] == 'E'){
                enemy[k].posX = j;
                enemy[k].posY = i;
                k++;
            }
        }
    }
    fclose(fp);

    // se numero de inimigos for menor que 5, destroi tudo por algum motivo, logo, invalidar a posicao dos que ja 'morreram'
    while (k<5){
        enemy[k].posX = -1;
        enemy[k].posY = -1;
        k++;
    }

    strcat(info,arquivo);
    fp = fopen(info,"r");
    if (fp == NULL){
        (*PlayerA).level = 0;
        (*PlayerA).numBombas = 3;
        (*PlayerA).numChaves = 0;
        (*PlayerA).score = 0;
        (*PlayerA).vidas = 3;
    }else{
        fscanf(fp, "%d\n%d\n%d\n%d\n%d", &(*PlayerA).level, &(*PlayerA).vidas, &(*PlayerA).numBombas,&(*PlayerA).numChaves , &(*PlayerA).score);
    }
    // setup de bombas e inimigos
	setBomb(gameMap, player, PlayerA, 'l');
	updateEnemies(gameMap, enemy, 'l', PlayerA);
	// renderiza o que acabou de receber
	renderAllegro(gameMap, 'l');
	fclose(fp);

}

int gamePause(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA)
{
    char opcao;
    system("cls");
    printf("Jogo pausado.\n\nContinuar jogo (ESC)\nCarregar jogo (l)\nSalvar jogo (s)\nVoltar ao menu principal (e)\n");
    do
    {
        opcao = getch();
    }
    while (opcao != 27 && opcao != 'l' && opcao != 's' && opcao != 'e');

    switch (opcao)
    {
    case 27: // funcional
        break;
    case 'l': // funcional
        //loadGame(gameMap, player, enemy, PlayerA, &(*PlayerA).level);
        break;
    case 's': // funcional
        saveGame(gameMap, PlayerA);
        renderMap(gameMap,player,enemy);
        break;
    case 'e': // funcional
        return 1;
        break;

    }
    renderMap(gameMap,player,enemy);
}

void mainMenu(char gameMap[][MAXCOLUNA], posEntidade *player, posEntidade enemy[], playerInfo* PlayerA)
{
    menubmp = al_load_bitmap("mainmenu.png");
    //al_draw_bitmap(menubmp, 0, 0, 0);
    //al_flip_display();
    system("cls");
    printf("BOMBERMAN MAIS **DA DA GALAXIA\n\nNew game (n)\nLoad Game(l)\nMultiplayer (m)\nExit Game(ESC)\n");
    char keyPressed = getch();
    switch (keyPressed)
    {
    case 'n':
        //loadGame(gameMap, player, enemy, PlayerA, (*PlayerA).level);
        break;
    case 'l':
        //loadGame(gameMap, player, enemy, PlayerA, (*PlayerA).level);
        break;
    case 'm':
        //multiplayer();
        break;
    case 27:
        exit(0);
    }
}

void comandoJogador(char gameMap[MAXLINHA][MAXCOLUNA], posEntidade* player, posEntidade enemy[], char keyPressed, playerInfo* PlayerA)
{
    switch(keyPressed)
    {
    case 27:
        if(gamePause(gameMap, player, enemy, PlayerA) == 1)
            mainMenu(gameMap, player, enemy, PlayerA);
        break;
    case 'w':
        updatePos(gameMap,player, keyPressed, PlayerA);
        break;
    case 'a':
        updatePos(gameMap,player, keyPressed, PlayerA);
        break;
    case 's':
        updatePos(gameMap,player, keyPressed, PlayerA);
        break;
    case 'd':
        updatePos(gameMap,player, keyPressed, PlayerA);
        break;
    case 'b':
        setBomb(gameMap,player,PlayerA, 'b');
        break;
    case 'k':
        explodeBomb(gameMap, player, enemy, PlayerA, 0);
        al_flip_display();
        break;
    case 'p':
        punch(gameMap, player, PlayerA);
        //renderMap(gameMap, player, enemy);
        break;
    default:
        return;
    }
}

void punch(char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA){
    char lado = gameMap[(*player).posY][(*player).posX];
    switch(lado){
        case 'w':
            if (avalia_punch(gameMap[(*player).posY - 1][(*player).posX], lado) == 1){
                gameMap[(*player).posY - 1][(*player).posX] = ' ';
                //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY - 1) * 22, 0);
            }
            else if (avalia_punch(gameMap[(*player).posY - 1][(*player).posX], lado) == 2){
                gameMap[(*player).posY - 1][(*player).posX] = 'C';
                //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY - 1) * 22, 0);
                //al_draw_bitmap(key, ((*player).posX)*22, ((*player).posY - 1) * 22, 0);
            }
            else if (avalia_punch(gameMap[(*player).posY - 1][(*player).posX], lado) == 3){
                gameMap[(*player).posY - 1][(*player).posX] = ' ';
                //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY - 1)*22, 0);
                (*PlayerA).numChaves++;
            }
            break;
        case 'a':
            if (avalia_punch(gameMap[(*player).posY][(*player).posX - 1], lado) == 1){
                gameMap[(*player).posY][(*player).posX - 1] = ' ';
                //al_draw_bitmap(blank, ((*player).posX - 1)*22, ((*player).posY) * 22, 0);
            }
            else if (avalia_punch(gameMap[(*player).posY][(*player).posX - 1], lado) == 2){
                gameMap[(*player).posY][(*player).posX - 1] = 'C';
                //al_draw_bitmap(blank, ((*player).posX - 1)*22, ((*player).posY) * 22, 0);
                //al_draw_bitmap(key, ((*player).posX - 1)*22, ((*player).posY) * 22, 0);
            }
            else if (avalia_punch(gameMap[(*player).posY][(*player).posX - 1], lado) == 3){
                gameMap[(*player).posY][(*player).posX - 1] = ' ';
                //al_draw_bitmap(blank, ((*player).posX - 1)*22, (*player).posY*22, 0);
                (*PlayerA).numChaves++;
            }
            break;
        case 's':
            if (avalia_punch(gameMap[(*player).posY + 1][(*player).posX], lado) == 1){
                gameMap[(*player).posY + 1][(*player).posX] = ' ';
                //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY + 1) * 22, 0);
            }
            else if (avalia_punch(gameMap[(*player).posY + 1][(*player).posX], lado) == 2){
                gameMap[(*player).posY + 1][(*player).posX] = 'C';
                //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY + 1) * 22, 0);
                //al_draw_bitmap(key, ((*player).posX)*22, ((*player).posY + 1) * 22, 0);
            }
            else if (avalia_punch(gameMap[(*player).posY + 1][(*player).posX], lado) == 3){
                gameMap[(*player).posY + 1][(*player).posX] = ' ';
                //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY + 1)*22, 0);
                (*PlayerA).numChaves++;
            }
            break;
        case 'd':
            if (avalia_punch(gameMap[(*player).posY][(*player).posX + 1], lado) == 1){
                gameMap[(*player).posY][(*player).posX + 1] = ' ';
                //al_draw_bitmap(blank, ((*player).posX+1)*22, (*player).posY * 22, 0);
            }
            else if (avalia_punch(gameMap[(*player).posY][(*player).posX + 1], lado) == 2){
                gameMap[(*player).posY][(*player).posX + 1] = 'C';
                //al_draw_bitmap(blank, ((*player).posX+1)*22, (*player).posY * 22, 0);
                //al_draw_bitmap(key, ((*player).posX+1)*22, (*player).posY * 22, 0);
            }
            else if (avalia_punch(gameMap[(*player).posY][(*player).posX + 1], lado) == 3){
                gameMap[(*player).posY][(*player).posX + 1] = ' ';
                //al_draw_bitmap(blank, ((*player).posX + 1)*22, (*player).posY*22, 0);
                (*PlayerA).numChaves++;
            }
            break;
        default:
            return;
    }
    al_flip_display();
    al_play_sample(efeitoSonoro, 0.4, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

int avalia_punch(char obstaculo, char lado){
    switch(obstaculo){
    case 'W':
        efeitoSonoro = al_load_sample("punchwall.wav");
        return 0;
    case 'E':
        efeitoSonoro = al_load_sample("oof.wav");
        return 0;
    case ' ':
        efeitoSonoro = al_load_sample("failpunch.wav");
        return 0;
    case 'K':
        efeitoSonoro = al_load_sample("punch.wav");
        return 2;
    case 'C':
        efeitoSonoro = al_load_sample("dlingdling.wav");
        return 3;
    default:
        efeitoSonoro = al_load_sample("soco.wav");
        return 1;
    }
}

void takeDmg(playerInfo* PlayerA)
{
    efeitoSonoro = al_load_sample("classic_hurt.wav");
    (*PlayerA).vidas--;
    (*PlayerA).score = (*PlayerA).score - 100;
    if ((*PlayerA).score < 0)
    {
        (*PlayerA).score = 0;
    }
    if((*PlayerA).vidas >= 1)
    {
        al_play_sample(efeitoSonoro, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}

void updatePos(char gameMap[][MAXCOLUNA], posEntidade* player, char keyPressed, playerInfo* PlayerA)  // A e D prontos
{
    gotoxy((*player).posX,(*player).posY);
    switch(keyPressed)
    {
    case 'w':
        if (gameMap[(*player).posY - 1][(*player).posX] == ' ')
        {

            // matriz de caracteres
            gameMap[(*player).posY][(*player).posX] = ' ';
            gotoxy((*player).posX,(*player).posY);
            printf(" ");

            // display allegro

            //al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);

            (*player).posY--;

            //al_draw_bitmap(jogador, (*player).posX * 22, (*player).posY * 22, 0);
            gotoxy((*player).posX,(*player).posY);
            printf("w");
        }
        else if (gameMap[(*player).posY - 1][(*player).posX] == 'E')
        {
            takeDmg(PlayerA); // andar em cima de inimigo
            gotoxy((*player).posX,(*player).posY);
            printf("w");
            //al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
        }
        else
        {
            gotoxy((*player).posX,(*player).posY);
            printf("w");
            //    al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
        }
        //al_draw_scaled_bitmap(jogador, 128,32,32,32,(*player).posX * 22,  (*player).posY * 22,APPC,APPC, 0);
        break;

    case 'a':
        if (gameMap[(*player).posY][(*player).posX - 1] == ' ')
        {

            gameMap[(*player).posY][(*player).posX] = ' ';
            gotoxy((*player).posX,(*player).posY);
            printf(" ");


            //al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
            (*player).posX--;

            //      al_draw_scaled_bitmap(jogador, 0,0,32,32,(*player).posX * 22,  (*player).posY * 22,APPC,APPC, ALLEGRO_FLIP_HORIZONTAL);

            gotoxy((*player).posX,(*player).posY);
            printf("a");
        }
        else if (gameMap[(*player).posY][(*player).posX - 1] == 'E')
        {
            takeDmg(PlayerA);
            gotoxy((*player).posX,(*player).posY);
            printf("a");

            //al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
        }

        else
        {
            gotoxy((*player).posX,(*player).posY);
            printf("a");

            //  al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
        }
        //al_draw_scaled_bitmap(jogador, 0,0,32,32,(*player).posX * 22,  (*player).posY * 22,APPC,APPC, ALLEGRO_FLIP_HORIZONTAL);
        break;

    case 's':
        if (gameMap[(*player).posY + 1][(*player).posX] == ' ')
        {
            gameMap[(*player).posY][(*player).posX] = ' ';
            gotoxy((*player).posX,(*player).posY);
            printf(" ");

            //  al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);

            (*player).posY++;


            gotoxy((*player).posX,(*player).posY);
            printf("s");
        }
        else if (gameMap[(*player).posY + 1][(*player).posX] == 'E')
        {
            takeDmg(PlayerA);
            gotoxy((*player).posX,(*player).posY);
            printf("s");
            //al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
        }
        else
        {
            gotoxy((*player).posX,(*player).posY);
            printf("s");
            //al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
        }
        //al_draw_scaled_bitmap(jogador, 0,32,32,32,(*player).posX * 22,  (*player).posY * 22,APPC,APPC, 0);
        break;

    case 'd':
        if(gameMap[(*player).posY][(*player).posX + 1] == ' ')
        {
            gameMap[(*player).posY][(*player).posX] = ' ';
            gotoxy((*player).posX,(*player).posY);
            printf(" ");

            //  al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);

            (*player).posX++;



            gotoxy((*player).posX,(*player).posY);
            printf("d");
        }
        else if (gameMap[(*player).posY][(*player).posX + 1] == 'E')
        {
            takeDmg(PlayerA);
            gotoxy((*player).posX,(*player).posY);
            printf("d");

            //al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
        }
        else
        {
            gotoxy((*player).posX,(*player).posY);
            printf("d");
            //al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
        }
        //al_draw_scaled_bitmap(jogador, 0,0,32,32,(*player).posX * 22,  (*player).posY * 22,APPC,APPC, 0);
        break;
    }
    gameMap[(*player).posY][(*player).posX] = keyPressed;
    //al_flip_display();
}
void saveGame(char gameMap[][MAXCOLUNA], playerInfo* PlayerA){
    FILE * fp;
    int i,j;

    char arquivo[50] = {0};
    char info[60]={"info"};
    char extensao[5] = {".txt"};

    printf("Digite o nome do arquivo onde quer salvar:\n");

    gets(arquivo);
    strcat(arquivo, extensao);

    system("cls");
    fp = fopen(arquivo, "w");
    for(i=0; i<MAXLINHA; i++){
        for(j=0; j<MAXCOLUNA; j++){
            fprintf(fp, "%c", gameMap[i][j]);
            printf("%c",gameMap[i][j]);
        }
    }
    system("pause");
    fclose(fp);

    strcat(info,arquivo);
    fp = fopen(info, "w");
    fprintf(fp,"%d\n%d\n%d\n%d\n%d\n",(*PlayerA).level, (*PlayerA).vidas, (*PlayerA).numBombas, (*PlayerA).numChaves, (*PlayerA).score);
    fclose(fp);
	/*printf("Digite o save que voce quer sobrescrever: (1,2,3)\n");
	char save = getch();
	switch(save){
		case '1':
			fp = fopen("save1.txt","w");
			for(i=0;i<MAXLINHA;i++){
				for(j=0;j<MAXCOLUNA;j++){
					fprintf(fp,"%c",gameMap[i][j]);
				}
			}
			fflush(fp);
			fp = fopen("infoSave1.txt","w");
			fprintf(fp,"%d%d",(*PlayerA).vidas,(*PlayerA).numBombas);
			break;
			fflush(fp);
		case '2':
			fp = fopen("save2.txt","w");
			for(i=0;i<MAXLINHA;i++){
				for(j=0;j<MAXCOLUNA;j++){
					fprintf(fp,"%c",gameMap[i][j]);
				}
			}
			fflush(fp);
			fp = fopen("infoSave2.txt","w");
			fprintf(fp,"%d%d",(*PlayerA).vidas,(*PlayerA).numBombas);
			fflush(fp);
			break;
		case '3':
			fp = fopen("save3.txt","w");
			for(i=0;i<MAXLINHA;i++){
				for(j=0;j<MAXCOLUNA;j++){
					fprintf(fp,"%c",gameMap[i][j]);
				}
			}
			fflush(fp);
			fp = fopen("infoSave3.txt","w");
			fprintf(fp,"%d%d",(*PlayerA).vidas,(*PlayerA).numBombas);
			fflush(fp);
			break;
	}
	fclose(fp);*/
}

void moveEnemy(char gameMap[][MAXCOLUNA], posEntidade enemy[], int direcao, int i, playerInfo* PlayerA)
{
    switch(direcao)
    {
    case 0: // w
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        // al_draw_bitmap(blank,(enemy[i].posX) * 22,enemy[i].posY * 22,0);

        enemy[i].posY--;

        //al_draw_bitmap(inimigo,(enemy[i].posX) * 22, enemy[i].posY * 22,0);
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");
        break;
    case 1: // a
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        //al_draw_bitmap(blank,(enemy[i].posX) * 22,enemy[i].posY * 22,0);

        enemy[i].posX--;

        //al_draw_bitmap(inimigo,(enemy[i].posX) * 22, enemy[i].posY * 22,0);
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");
        break;
    case 2: // s
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        //al_draw_bitmap(blank,(enemy[i].posX) * 22,enemy[i].posY * 22,0);

        enemy[i].posY++;

        //al_draw_bitmap(inimigo,(enemy[i].posX) * 22, enemy[i].posY * 22,0);
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");

        break;
    case 3: // d
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        //al_draw_bitmap(blank,(enemy[i].posX) * 22, enemy[i].posY * 22,0);

        enemy[i].posX++;

        //al_draw_bitmap(inimigo,(enemy[i].posX) * 22, enemy[i].posY * 22,0);
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");
        break;
    }
    //al_flip_display(); // provavelmente nao e o lugar ideal
}

void updateEnemies(char gameMap[][MAXCOLUNA], posEntidade enemy[], char instrucao, playerInfo* PlayerA)
{
    int i;
    static int direcao[5];
    static int contador;

    if (instrucao == 'u')  // se a funcao for chamada por update de fps (na main)
    {
        contador++;
        for(i=0; i<5; i++)
        {
            if (enemy[i].posX == -1 || enemy[i].posY == -1) continue;
            switch(direcao[i])
            {
            case 0: // w y-1
                if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' '){
                    direcao[i] = 3;
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                else if (gameMap[enemy[i].posY - 1][enemy[i].posX] != ' '){
                    if (gameMap[enemy[i].posY - 1][enemy[i].posX] == 'w' || gameMap[enemy[i].posY - 1][enemy[i].posX] == 'a' || gameMap[enemy[i].posY - 1][enemy[i].posX] == 's' || gameMap[enemy[i].posY - 1][enemy[i].posX] == 'd'){
                        takeDmg(PlayerA);
                    }
                    direcao[i] = 1;
                    break;
                }
                else {
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }

                /*if (gameMap[enemy[i].posY - 1][enemy[i].posX] != ' '){
                    if (gameMap[enemy[i].posY][enemy[i].posX + 1] != ' ' && gameMap[enemy[i].posY][enemy[i].posX - 1] != ' '){
                        direcao[i] = 2;
                    }
                    else if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ' && gameMap[enemy[i].posY][enemy[i].posX - 1] == ' '){
                        if (rand() % 2 == 1){
                            direcao[i] = 1;
                        }else direcao[i] = 3;
                    }
                    else if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ' && gameMap[enemy[i].posY][enemy[i].posX - 1] != ' '){
                        direcao[i] = 3;
                    }
                    else if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ' && gameMap[enemy[i].posY][enemy[i].posX + 1] != ' '){
                        direcao[i] = 1;
                    }
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                else{
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                */
            case 1: // a x-1
                if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' '){
                    direcao[i] = 0;
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                else if (gameMap[enemy[i].posY][enemy[i].posX - 1] != ' '){
                    if (gameMap[enemy[i].posY][enemy[i].posX - 1] == 'w' || gameMap[enemy[i].posY][enemy[i].posX - 1] == 'a' || gameMap[enemy[i].posY][enemy[i].posX - 1] == 's' || gameMap[enemy[i].posY][enemy[i].posX - 1] == 'd'){
                        takeDmg(PlayerA);
                    }
                    direcao[i] = 2;
                    break;
                }
                else {
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                /*if (gameMap[enemy[i].posY][enemy[i].posX - 1] != ' '){
                    if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] != ' '){
                        direcao[i] = 3;
                    }
                    else if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] == ' '){
                        if (rand() % 2 == 1){
                            direcao[i] = 0;
                        }else direcao[i] = 3;
                    }
                    else if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ' && gameMap[enemy[i].posY + 1][enemy[i].posX] != ' '){
                        direcao[i] = 0;
                    }
                    else if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] != ' '){
                        direcao[i] = 3;
                    }
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                else{
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }*/
            case 2: // s y+1
                if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' '){
                    direcao[i] = 1;
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                else if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' '){
                    if (gameMap[enemy[i].posY + 1][enemy[i].posX] == 'w' || gameMap[enemy[i].posY + 1][enemy[i].posX] == 'a' || gameMap[enemy[i].posY + 1][enemy[i].posX] == 's' || gameMap[enemy[i].posY + 1][enemy[i].posX] == 'd'){
                        takeDmg(PlayerA);
                    }
                    direcao[i] = 3;
                    break;
                }
                else {
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                /*if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' '){
                    if (gameMap[enemy[i].posY][enemy[i].posX - 1] != ' ' && gameMap[enemy[i].posY][enemy[i].posX + 1] != ' '){
                        direcao[i] = 0;
                    }
                    else if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ' && gameMap[enemy[i].posY][enemy[i].posX + 1] == ' '){
                        if (rand() % 2 == 1){
                            direcao[i] = 1;
                        }else direcao[i] = 3;
                    }
                    else if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ' && gameMap[enemy[i].posY][enemy[i].posX + 1] != ' '){
                        direcao[i] = 1;
                    }
                    else if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ' && gameMap[enemy[i].posY][enemy[i].posX - 1] != ' '){
                        direcao[i] = 3;
                    }
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                else{
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }*/
            case 3: // d x+1
                if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' '){
                    direcao[i] = 2;
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                else if (gameMap[enemy[i].posY][enemy[i].posX + 1] != ' '){
                    if (gameMap[enemy[i].posY][enemy[i].posX + 1] == 'w' || gameMap[enemy[i].posY][enemy[i].posX + 1] == 'a' || gameMap[enemy[i].posY][enemy[i].posX + 1] == 's' || gameMap[enemy[i].posY][enemy[i].posX + 1] == 'd'){
                        takeDmg(PlayerA);
                    }
                    direcao[i] = 0;
                    break;
                }
                else{
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                /*if (gameMap[enemy[i].posY][enemy[i].posX + 1] != ' '){
                    if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] != ' '){
                        direcao[i] = 1;
                    }
                    else if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] == ' '){
                        if (rand() % 2 == 1){
                            direcao[i] = 0;
                        }else direcao[i] = 3;
                    }
                    else if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ' && gameMap[enemy[i].posY + 1][enemy[i].posX] != ' '){
                        direcao[i] = 0;
                    }
                    else if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] != ' '){
                        direcao[i] = 3;
                    }
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                else{
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                */
                //al_flip_display(); aqui fica esquisito
            }
        }
    }
    else if (instrucao == 'l')  // se a funcao for chamada pelo loadgame, escolhe as direcoes que os inimigos vao andar (NAO ANDA AINDA)
    {
        contador = 0;
        for(i=0; i<5; i++)
        {
            direcao[i] = 8;
        }  // inicializa todas as direcoes a -1 (que é o invalido)
        for (i=0; i<5; i++)
        {
            do
            {
                if (enemy[i].posX + 1 != ' ' && enemy[i].posX - 1 != ' ' && enemy[i].posY + 1 != ' ' && enemy[i].posY - 1 != ' ')
                {
                    direcao[i] = 2;
                    continue;
                }
                else
                {
                    direcao[i] = rand() % 4; // aleatoriza uma direcao
                    switch(direcao[i])  // 0 = norte/w          1 = oeste/a       2 = sul/s      3 = leste/d
                    {
                    case 0: // case W
                        if (gameMap[(enemy[i].posY) - 1][enemy[i].posX] == ' ')  // se o proximo espaço for vazio
                        {
                            direcao[i] = 0; // a direcao que vai começar a andar se torna aquela
                        }
                        else
                        {
                            direcao[i] = 8;
                        }
                        break;
                    case 1: // case A
                        if (gameMap[enemy[i].posY][(enemy[i].posX) - 1] == ' ')
                        {
                            direcao[i] = 1;
                        }
                        else
                        {
                            direcao[i] = 8;
                        }
                        break;
                    case 2: // case S
                        if (gameMap[(enemy[i].posY) + 1][enemy[i].posX] == ' ')
                        {
                            direcao[i] = 2;
                        }
                        else
                        {
                            direcao[i] = 8;
                        }
                        break;
                    case 3: // case D
                        if (gameMap[enemy[i].posY][(enemy[i].posX) + 1] == ' ')
                        {
                            direcao[i] = 3;
                        }
                        else
                        {
                            direcao[i] = 8;
                        }
                        break;
                    }
                }
            }
            while(direcao[i] == 8);  // se direcao continuar sendo invalida, repete a parte de coletar aleatorio
        }
    }
}

void showPos(posEntidade* player, posEntidade enemy[], playerInfo* PlayerA){
	int i;
	gotoxy(0,26);
	printf("pos player = [%d][%d]\n",(*player).posX,(*player).posY);
	for(i=0; i<5; i++){
		printf("pos enemy[%d] = [%d][%d]\n",i,enemy[i].posX,enemy[i].posY);
	}
	for(i=0; i<MAXBOMBAS; i++ ){
        if ((*PlayerA).posXBomba[i] >= 0 && (*PlayerA).posXBomba[i] <= 59 && (*PlayerA).posYBomba[i] >= 0 && (*PlayerA).posYBomba[i] <= 24){
            printf("posBomba[%d] X = %d   Y = %d\n", i, (*PlayerA).posXBomba[i], (*PlayerA).posYBomba[i]);
        }
	}
	gotoxy (20, 25);
    printf("Level = %d  Vidas = %d  NumBombas = %d  NumChaves = %d  Score = %d\n",(*PlayerA).level,(*PlayerA).vidas,(*PlayerA).numBombas, (*PlayerA).numChaves,(*PlayerA).score);
}

void updateGame(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int* cont_tempo)
{
    int i;
    //gotoxy (0,33);
    //printf("tempo no updategame = %d",tempo);

    for(i=0; i<MAXBOMBAS; i++){
        if((*PlayerA).posXBomba[i] == -1) continue;
        if (clock() >= (*PlayerA).tempoBomba[i] + 2500){
            explodeBomb(gameMap, player, enemy, PlayerA, i);
        }
    }

    if ((*PlayerA).score < 0){
        (*PlayerA).score = 0;
    }

    if((*PlayerA).vidas <= 0)
    {
        system("cls");
        printf("vose morel otareo\n");
        efeitoSonoro = al_load_sample("morri.wav");
        al_clear_to_color(al_map_rgb(0,0,0));
        al_flip_display();
        al_play_sample(efeitoSonoro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        system("PAUSE");
        mainMenu(gameMap,player,enemy,PlayerA);
    }

    if ((*PlayerA).numChaves == 5){
	    al_draw_text(fonte_menu, al_map_rgb(255, 0, 255), 250, 250, 0, "PASSOU DE FASE");
        al_flip_display();
        al_rest(5);
        al_clear_to_color(al_map_rgb(195,195,195));
        al_flip_display();
        // level ++
        //loadgame(~~~~~~~,level)
	}

    if (*cont_tempo >=  1 / ( (*PlayerA).level ) *10) // controla a velocidade de movimento dos inimigos
    {
        updateEnemies(gameMap, enemy, 'u', PlayerA);
        *cont_tempo = 0;
//	if (PASSOU X FRAMES)
        //updateEnemies();
    }
    gotoxy(0, 25);
    printf("cont_tempo = %d",*cont_tempo);
}

void setBomb(char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA, char keyPressed){
	int i,j,k;
	efeitoSonoro = al_load_sample("setbomb2.wav");
	if((*PlayerA).numBombas <= 0){
		return;
	}
	if (keyPressed == 'b'){
		switch(gameMap[(*player).posY][(*player).posX]) {
			case 'w':
				if (gameMap[(*player).posY - 1][(*player).posX] == ' '){
                    // display allegro
					//al_draw_bitmap(bomba,((*player).posX) * 22,((*player).posY - 1) * 22,0);

					(*PlayerA).numBombas--;
					for (i=0; i<MAXBOMBAS; i++){
                        if ((*PlayerA).posXBomba[i] == -1){
                            (*PlayerA).posXBomba[i] = (*player).posX;
                            (*PlayerA).posYBomba[i] = (*player).posY - 1;
                            (*PlayerA).tempoBomba[i] = clock();
                            break;
                        }
					}
					// matriz caracteres
					gameMap[(*player).posY - 1][(*player).posX] = 'B';
					gotoxy((*player).posX, (*player).posY - 1);
					printf("B");
					al_play_sample(efeitoSonoro, 1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				break;
			case 'a':
				if (gameMap[(*player).posY][(*player).posX - 1] == ' '){
                    //al_draw_bitmap(bomba,((*player).posX - 1) * 22,((*player).posY) * 22,0);

					(*PlayerA).numBombas--;
					for (i=0; i<MAXBOMBAS; i++){
                        if ((*PlayerA).posXBomba[i] == -1){
                            (*PlayerA).posXBomba[i] = (*player).posX - 1;
                            (*PlayerA).posYBomba[i] = (*player).posY;
                            (*PlayerA).tempoBomba[i] = clock();
                            break;
                        }
					}

					gameMap[(*player).posY][(*player).posX - 1] = 'B';
					gotoxy((*player).posX - 1, (*player).posY);
					printf("B");
					al_play_sample(efeitoSonoro, 1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				break;
			case 's':
				if (gameMap[(*player).posY + 1][(*player).posX] == ' '){
                    //al_draw_bitmap(bomba,((*player).posX) * 22,((*player).posY + 1) * 22,0);

					(*PlayerA).numBombas--;
					for (i=0; i<MAXBOMBAS; i++){
                        if ((*PlayerA).posXBomba[i] == -1){
                            (*PlayerA).posXBomba[i] = (*player).posX;
                            (*PlayerA).posYBomba[i] = (*player).posY + 1;
                            (*PlayerA).tempoBomba[i] = clock();
                            break;
                        }
					}
					gameMap[(*player).posY + 1][(*player).posX] = 'B';
					gotoxy((*player).posX, (*player).posY + 1);
					printf("B");
					al_play_sample(efeitoSonoro, 1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				break;
			case 'd':
				if (gameMap[(*player).posY][(*player).posX + 1] == ' '){
                    //al_draw_bitmap(bomba,((*player).posX + 1) * 22,((*player).posY) * 22,0);

					(*PlayerA).numBombas--;
					for (i=0; i<MAXBOMBAS; i++){
                        if ((*PlayerA).posXBomba[i] == -1){
                            (*PlayerA).posXBomba[i] = (*player).posX + 1;
                            (*PlayerA).posYBomba[i] = (*player).posY;
                            (*PlayerA).tempoBomba[i] = clock();
                            break;
                        }
					}
					gameMap[(*player).posY][(*player).posX + 1] = 'B';
					gotoxy((*player).posX + 1, (*player).posY);
					printf("B");
					al_play_sample(efeitoSonoro, 1, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
				}
				break;
			default:
				return;
		}
		al_flip_display();
	}
	else if (keyPressed == 'l'){
		k = 0;
		int x = 0;

		for(i=0;i<MAXBOMBAS;i++){
            (*PlayerA).posYBomba[i] = -1;
            (*PlayerA).posXBomba[i] = -1;
		}

		for(i=0;i<MAXLINHA;i++){
			for(j=0;j<MAXCOLUNA;j++){
				if (gameMap[i][j] == 'B'){
					(*PlayerA).posXBomba[k] = j;
					(*PlayerA).posYBomba[k] = i;
					(*PlayerA).tempoBomba[k] = clock();
					k++;
				}
			}
		}
	}
}

void explodeBomb(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int indice){
    int i,j,k;
    int posxbomba = (*PlayerA).posXBomba[indice];
    int posybomba = (*PlayerA).posYBomba[indice];
    /*for(i= - 2; i<3; i++){
        if (gameMap [(*PlayerA).posYBomba[indice] + i] [(*PlayerA).posXBomba[indice]] != 'W'){
            gameMap [(*PlayerA).posYBomba[indice] + i] [(*PlayerA).posXBomba[indice]] = ' ';
            al_draw_bitmap(blank,((*PlayerA).posXBomba[indice] + i)*22 , ((*PlayerA).posYBomba[indice])*22, 0);
        }
        if (gameMap [(*PlayerA).posYBomba[indice]]   [(*PlayerA).posXBomba[indice] + i] != 'W'){
            gameMap [(*PlayerA).posYBomba[indice]]   [(*PlayerA).posXBomba[indice] + i] = ' ';
            al_draw_bitmap(blank,((*PlayerA).posXBomba[indice])*22 , ((*PlayerA).posYBomba[indice] + i)*22, 0);
        }
    }*/

    if (posxbomba == -1 || posybomba == -1) return; // importante
    for (i=1; i<4; i++){
        if (gameMap[posybomba - i][posxbomba] == 'W') break;
        else if (gameMap[posybomba - i][posxbomba] == gameMap[(*player).posY][(*player).posX]){
            takeDmg(PlayerA);
        }
        else if (gameMap[posybomba - i][posxbomba] == 'K'){
            gameMap[posybomba - i][posxbomba] = 'C';
            //(blank, posxbomba * 22, (posybomba - i) * 22, 0);
            //al_draw_bitmap(key, posxbomba * 22, (posybomba - i) * 22, 0);
            efeitoSonoro = al_load_sample("dlingdling.wav");
            al_play_sample(efeitoSonoro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        else if (gameMap[posybomba - i][posxbomba] == 'C'){
            continue;
        }
        else if (gameMap[posybomba - i][posxbomba] == 'E'){
            for(k = 0; k<5; k++){
                if (enemy[k].posY == posybomba - i && enemy[k].posX == posxbomba){
                    //al_draw_bitmap(blank, enemy[k].posX*22, enemy[k].posY*22, 0);
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerA).score = (*PlayerA).score + 20;
                    efeitoSonoro = al_load_sample("wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else{
            gameMap[posybomba - i][posxbomba] = ' ';
            //al_draw_bitmap(blank, posxbomba*22, (posybomba - i)*22, 0);
            continue;
        }
    }

    for (i=1; i<4; i++){
        if (gameMap[posybomba][posxbomba - i] == 'W') break;
        else if (gameMap[posybomba][posxbomba - i] == gameMap[(*player).posY][(*player).posX]){
            takeDmg(PlayerA);
        }
        else if (gameMap[posybomba][posxbomba - i] == 'K'){
            gameMap[posybomba][posxbomba - i] = 'C';
            //al_draw_bitmap(blank, (posxbomba - i)*22, posybomba*22, 0);
            //al_draw_bitmap(key, (posxbomba - i)*22, posybomba*22, 0);
        }
        else if (gameMap[posybomba][posxbomba - i] == 'E'){
            for(k = 0; k<5; k++){
                if (enemy[k].posY == posybomba && enemy[k].posX == posxbomba - i){
              //      al_draw_bitmap(blank, enemy[k].posX*22, enemy[k].posY*22, 0);
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerA).score = (*PlayerA).score + 20;
                    efeitoSonoro = al_load_sample("wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else if (gameMap[posybomba][posxbomba - i] == 'C'){
            continue;
        }
        else{
            gameMap[posybomba][posxbomba - i] = ' ';
            //al_draw_bitmap(blank, (posxbomba - i)*22, posybomba*22, 0);
            continue;
        }
    }

    for (i=1; i<4; i++){
        if (gameMap[posybomba + i][posxbomba] == 'W') break;
        else if (gameMap[posybomba + i][posxbomba] == gameMap[(*player).posY][(*player).posX]){
            takeDmg(PlayerA);
        }
        else if (gameMap[posybomba + i][posxbomba] == 'K'){
            gameMap[posybomba + i][posxbomba] = 'C';
            //al_draw_bitmap(blank, posxbomba*22, (posybomba + i)*22, 0);
            //al_draw_bitmap(key, posxbomba*22, (posybomba + i)*22, 0);
        }
        else if (gameMap[posybomba + i][posxbomba] == 'C'){
            continue;
        }
        else if (gameMap[posybomba + i][posxbomba] == 'E'){
            for(k = 0; k<5; k++){
                if (enemy[k].posY == posybomba + i && enemy[k].posX == posxbomba){
                    //al_draw_bitmap(blank, enemy[k].posX*22, enemy[k].posY*22, 0);
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerA).score = (*PlayerA).score + 20;
                    efeitoSonoro = al_load_sample("wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else{
            gameMap[posybomba + i][posxbomba] = ' ';
            //al_draw_bitmap(blank, posxbomba*22, (posybomba + i)*22, 0);
            continue;
        }
    }

    for (i=1; i<4; i++){
        if (gameMap[posybomba][posxbomba + i] == 'W') break;
        else if (gameMap[posybomba][posxbomba + i] == gameMap[(*player).posY][(*player).posX]){
            takeDmg(PlayerA);
        }
        else if (gameMap[posybomba][posxbomba + i] == 'K'){
            gameMap[posybomba][posxbomba + i] = 'C';
            //al_draw_bitmap(blank, (posxbomba + i)*22, posybomba*22, 0);
            //al_draw_bitmap(key, (posxbomba + i)*22, posybomba*22, 0);
        }
        else if (gameMap[posybomba][posxbomba + i] == 'C'){
            continue;
        }
        else if (gameMap[posybomba][posxbomba + i] == 'E'){
            for(k = 0; k<5; k++){
                if (enemy[k].posY == posybomba && enemy[k].posX == posxbomba + i){
                    //al_draw_bitmap(blank, enemy[k].posX*22, enemy[k].posY*22, 0);
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerA).score = (*PlayerA).score + 20;
                    efeitoSonoro = al_load_sample("wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else{
            gameMap[posybomba][posxbomba + i] = ' ';
            //al_draw_bitmap(blank, (posxbomba + i)*22, posybomba*22, 0);
            continue;
        }
    }
    //gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice]] = ' ';
    //al_draw_bitmap(blank,((*PlayerA).posXBomba[indice] + i)*22 , ((*PlayerA).posYBomba[indice] + i)*22, 0);
    (*PlayerA).numBombas++;
    //al_draw_bitmap(blank, posxbomba*22, posybomba*22, 0);
    efeitoSonoro = al_load_sample("explosion.wav");
    al_play_sample(efeitoSonoro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    gameMap[posybomba][posxbomba] = ' ';
    //renderMap(gameMap,player,enemy);
    (*PlayerA).posXBomba[indice] = -1; // invalida a bomba, permite sobrescrever em outro setBomb()

}

int main()
{
    char gameMap[MAXLINHA][MAXCOLUNA];
    char arquivo[50] = {0};
    int estado_menu = 0;
    tamanhoJogo tela;// struct de tamanho de tela
    int char_escolhido=0;
    posEntidade player, enemy[5];
    playerInfo PlayerA;
    srand(time(NULL));
    char_escolhido=1+(rand()%2);
    initializeAllegro();
    choose_your_fighter(&char_escolhido);
    definescreensize(&tela);
    printf("%.0fx%.0f\n",tela.largura,tela.altura);
    //system("pause");
    //mainMenu(gameMap, &player, enemy, &PlayerA);


    int level_escolhido=0;
    int load_level=0;
    int gameMode=0;
    while(estado_menu != SAIR_JOGO)
    {
        if (estado_menu == MAIN_MENU)//condicao padrao
        {
            mainmenu(&estado_menu,&tela);
        }
        else if (estado_menu  == SINGLEPLAYER)//se no menu for selecionado jogar entra nesta condiçao
        {

            submenulevels(&estado_menu,&level_escolhido,&load_level,arquivo,&tela);
            if(estado_menu==SINGLEPLAYER)
            executaJogo(gameMap, &player, enemy, &PlayerA,&tela,gameMode,arquivo,&estado_menu);//codigo do jogo

        }
        else if (estado_menu == MULTIPLAYER)//se no menu for selecionado submenu de escolher levels entra nesta condiçao
        {
            estado_menu=0;
        }
        else if (estado_menu == SUBMENU_CHAR)//se no menu for selecionado submenu de escolher personagem entra nesta condiçao
        {
            submenuchar(&estado_menu,&char_escolhido);
        }
        else if (estado_menu  == SUBMENU_SETTINGS)//se no menu for selecionado submenu de configuraçoes entra nesta condiçao
        {
            submenusettings(&estado_menu,&gameMode);
        }
    }


    al_destroy_bitmap(menubmp);
    al_destroy_display(telaJogo);
    al_destroy_sample(efeitoSonoro);
    al_destroy_audio_stream(musicaFundo);

}

void executaJogo(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA,tamanhoJogo *tela,int gameMode,char arquivo[],int*estado_menu)
{
    char keyPressed;
    int cont_tempo = 0;
    int desenha = 0;
    int itemmenu=0;
    char menu_pause[5][50]= {"RETURN","SAVE GAME","CHOOSE CHARACTER","BACK TO MAIN MENU"};
    int i;
    loadGame(gameMap, player, enemy, PlayerA, &(*PlayerA).level,arquivo);
    renderMap(gameMap,player, enemy);
    while(*estado_menu==SINGLEPLAYER)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_TIMER)
        {
            cont_tempo++;
            desenha = 1;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_W:
                keyPressed = 'w';
                break;
            case ALLEGRO_KEY_A:
                keyPressed = 'a';
                break;
            case ALLEGRO_KEY_S:
                keyPressed = 's';
                break;
            case ALLEGRO_KEY_D:
                keyPressed = 'd';
                break;
            case ALLEGRO_KEY_ESCAPE:
                *estado_menu=MENU_PAUSE;
                al_flush_event_queue(fila_eventos);
                while(*estado_menu==MENU_PAUSE)
                {
                    al_wait_for_event(fila_eventos, &evento);
                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
                    {
                        if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE||evento.keyboard.keycode == ALLEGRO_KEY_ENTER)// se tecla pressionada foi espaço ou enter
                        {
                            printf("space||enter");
                            if (itemmenu == 0)
                            {
                                *estado_menu=SINGLEPLAYER;
                                break;
                            }
                            else if (itemmenu == 1)
                            {
                                //save game
                                break;
                            }
                            else if (itemmenu == 2)
                            {

                                break;
                            }
                            else if (itemmenu == 3)
                            {
                                *estado_menu=0;
                                break;
                            }
                        }
                        else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W)//se tecla pressionada foi w ou seta
                        {
                            itemmenu--;
                            if (itemmenu<0) //se apertar para cima no primeiro item, seleciona o ultimo
                                itemmenu =3;
                        }
                        else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)//se tecla pressionada foi s ou seta
                        {
                            itemmenu++;
                            if (itemmenu==4) //se apertar para cima no primeiro item, seleciona o ultimo
                                itemmenu=0;
                        }
                    }
                    if(al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
                    {
                        for(i=0; i<4; i++)
                        {
                            if(i==itemmenu)
                            {
                                color=al_map_rgb(255,255,255);
                            }
                            else
                            {
                                color=al_map_rgb(0,0,0);
                            }
                            al_draw_textf(fonte_menu, color, 100,((50*i)+200), ALLEGRO_ALIGN_LEFT, menu_pause[i]);

                        }
                        al_flip_display();
                    }

                }

                al_flush_event_queue(fila_eventos);
                al_clear_to_color(al_map_rgb(0,0,0));

                al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,0,105,(*tela).largura,(*tela).largura*25/60,0);
                if(gameMode)
                {
                    game_mode_zoom(player, tela);
                }
                else
                {
                    al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,0,105,(*tela).largura,(*tela).largura*25/60,0);
                }

                break;
            case ALLEGRO_KEY_B:
                keyPressed = 'b';
                break;
            case ALLEGRO_KEY_K:
                keyPressed = 'k';
                break;
            case ALLEGRO_KEY_P:
                keyPressed = 'p';
                break;
            }
            comandoJogador(gameMap, player, enemy, keyPressed, PlayerA);
        }
        if(desenha && al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            updateGame(gameMap, player, enemy, PlayerA, &cont_tempo);
            al_clear_to_color(al_map_rgb(0,0,0));
            renderAllegro(gameMap,'r');
            showPos(player, enemy, PlayerA);
            if(gameMode)
            {
                game_mode_zoom(player, tela);
            }
            else
            {
                al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,0,105,(*tela).largura,(*tela).largura*25/60,0);
            }

            renderInfoAllegro(PlayerA, tela);
            // al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,(*tela).largura*5/6,(*tela).altura*5/6,(*tela).largura/6,(*tela).altura/6,0);

            desenha = 0;

            al_flip_display();
        }
    }
}

void mainmenu(int *estado_menu,tamanhoJogo* tela)
{
    ALLEGRO_EVENT evento;
    int i;
    int cont_tempo=0;
    int desenha=0;
    char menu_p[5][50] = {"SINGLEPLAYER","MULTIPLAYER","CHOOSE CHARACTER","SETTINGS","EXIT GAME"};
    int imenu=0;
    while(*estado_menu ==0)
    {
        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_TIMER)
        {
            cont_tempo++;
            desenha = 1;

        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) //se tecla foi pressionada
        {

            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE||evento.keyboard.keycode == ALLEGRO_KEY_ENTER)// se tecla pressionada foi espaço ou enter
            {
                printf("space||enter\n");
                if (imenu == 0)  //inicia jogo com level 1
                {
                    *estado_menu=1;
                    break;
                }
                else if (imenu == 1)  //submenu mapas
                {
                    *estado_menu=2;
                    break;
                }
                else if (imenu == 2)//submenu personagens
                {
                    *estado_menu=3;
                    break;
                }
                else if (imenu == 3)  //configuraçoes
                {
                    *estado_menu=4;
                    break;
                }
                else if (imenu == 4)  //sair
                {
                    *estado_menu=5;
                    break;
                }
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W)//se tecla pressionada foi w ou seta
            {
                imenu--;
                printf("key up||w\n");
                if (imenu<0) //se apertar para cima no primeiro item, seleciona o ultimo
                    imenu =TAM_MENU_P-1;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)//se tecla pressionada foi s ou seta
            {
                printf("key down||s\n");
                imenu++;
                if (imenu==(TAM_MENU_P)) //se apertar para cima no primeiro item, seleciona o ultimo
                    imenu=0;
            }
        }
        //desenha=1;
        if(desenha && al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_bitmap(menubmp,0,0,0);
            al_draw_scaled_bitmap(logo,0,0,3200,1000,((*tela).largura-al_get_bitmap_width(logo))/2,100,500,250,0);

            for(i=0; i<TAM_MENU_P; i++)
            {
                if(i==imenu)
                {
                    color=al_map_rgb(255,255,255);
                }
                else
                {
                    color=al_map_rgb(0,0,0);
                }
                al_draw_textf(fonte_menu, color, ((*tela).largura/2)-10 , 50*(i+1)+300, ALLEGRO_ALIGN_CENTRE, menu_p[i]);

            }
            desenha = 0;
            al_flip_display();
        }

        //al_flip_display();
    }
}

void submenuchar(int *estado_menu,int *char_escolhido)
{
    ALLEGRO_EVENT evento;
    char submenu_characters[9][50]= {"BERTOTTO","WERMANN","SPIDER MAN","BATMAN","LUKE SKYWALKER","ROBOCOP","LOAD CHARACTER","BACK TO MAIN MENU","EASTER EGG"}; //matriz menu de personagens
    int i;
    int cont_tempo=0;
    int desenha=0;
    int imenu=0;
    int coluna_sprite=0;
    int coluna_atual=0;
    int linha_sprite=0;
    *char_escolhido=0;

    printf("\n%d\n",*char_escolhido);
    while(*estado_menu ==3)
    {
        if (imenu == 0)
        {
            jogador=player1;
        }
        else if (imenu == 1)
        {
            jogador=player2;
        }
        else if (imenu == 2)
        {
            jogador=player3;
        }
        else if (imenu == 3)
        {
            jogador=player4;
        }
        else if (imenu == 4)
        {
            jogador=player5;
        }
        else if (imenu == 5)
        {
            jogador=player6;
        }
        else if (imenu == 6)
        {
            jogador=player6;
        }
        else if (imenu == 8)
        {
            jogador=player8;
        }
        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_TIMER)
        {

            cont_tempo++;
            desenha = 1;

            if (cont_tempo >= 15 && *char_escolhido==0)
            {
                linha_sprite=0;
                cont_tempo=0;
                coluna_atual++;
                if (coluna_atual >= 2)
                {
                    coluna_atual=0;
                }
            }

            if (cont_tempo >= 5 && *char_escolhido!=0)
            {
                linha_sprite=64;
                cont_tempo=0;
                coluna_atual++;
                if (coluna_atual >= 4)
                {

                    *estado_menu=0;
                    break;

                }
            }
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)//se tecla foi pressionada
        {
            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE||evento.keyboard.keycode == ALLEGRO_KEY_ENTER)// se tecla pressionada foi espaço ou enter
            {
                printf("space||enter");
                if (imenu == 0)
                {
                    *char_escolhido=1;
                    coluna_atual=0;
                }
                else if (imenu == 1)
                {
                    *char_escolhido=2;
                    coluna_atual=0;
                }
                else if (imenu == 2)
                {
                    *char_escolhido=3;
                    coluna_atual=0;
                }
                else if (imenu == 3)
                {
                    *char_escolhido=4;


                    coluna_atual=0;
                }
                else if (imenu == 4)
                {
                    *char_escolhido=5;
                }
                else if (imenu == 5)
                {
                    *char_escolhido=6;
                }
                else if (imenu == 6)
                {
                    //load_char=1;
                }
                else if (imenu == 7)
                {
                    *estado_menu=0;
                    break;
                }
                else if (imenu == 8)
                {
                    *char_escolhido=8;
                    coluna_atual=0;
                }
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W)//se tecla pressionada foi w ou seta
            {
                imenu--;
                printf("key up||w\n");
                if (imenu<0) //se apertar para cima no primeiro item, seleciona o ultimo
                    imenu =7;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)//se tecla pressionada foi s ou seta
            {
                printf("key down||s\n");
                imenu++;
                if (imenu==9) //se apertar para cima no primeiro item, seleciona o ultimo
                    imenu=0;
            }
        }
        if(desenha && al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            coluna_sprite=coluna_atual*32;
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_scaled_bitmap(blank,0,0,22,22,110,110,500,500,0);
            al_draw_scaled_bitmap(blank,0,0,22,22,-390,110,500,500,0);
            al_draw_scaled_bitmap(deswall,0,0,32,32,563,110,500,500,0);
            al_draw_scaled_bitmap(wall,0,0,32,32,563,-390,500,500,0);
            al_draw_scaled_bitmap(wall,0,0,32,32,63,-390,500,500,0);
            al_draw_scaled_bitmap(wall,0,0,32,32,63,610,500,500,0);
            al_draw_scaled_bitmap(wall,0,0,32,32,-437,-390,500,500,0);
            al_draw_scaled_bitmap(wall,0,0,32,32,1063,-390,500,500,0);
            al_draw_scaled_bitmap(wall,0,0,32,32,-437,610,500,500,0);
            al_draw_scaled_bitmap(wall,0,0,32,32,563,610,500,500,0);
            al_draw_scaled_bitmap(deswall,0,0,32,32,1063,110,500,500,0);
            al_draw_scaled_bitmap(wall,0,0,32,32,1063,610,500,500,0);
            for(i=0; i<8; i++)
            {
                if(i!=imenu)
                {
                    color=al_map_rgb(0,0,0);
                }
                else
                {
                    color=al_map_rgb(255,255,255);
                }
                al_draw_textf(fonte_menu, color, 900,(50*i+150), ALLEGRO_ALIGN_CENTRE, submenu_characters[i]);

            }
            if(imenu==8)
            {
                color=al_map_rgb(255,255,255);
                al_draw_textf(fonte_menu, color, 900,550, ALLEGRO_ALIGN_CENTRE, submenu_characters[8]);
            }
            if(estado_menu!=0)
            {
                al_draw_scaled_bitmap(jogador,coluna_sprite,linha_sprite,32,32,110,110,500,500,0);
            }
            else if(char_escolhido!=0)
            {
                al_draw_scaled_bitmap(jogador,coluna_sprite,linha_sprite,32,32,110,110,500,500,0);
            }
            al_flip_display();
            desenha = 0;
        }
    }
}

void submenulevels(int *estado_menu, int *level_escolhido, int *load_level,char arquivo[],tamanhoJogo* tela)
{
    char submenu_levels[13][20]= {"LEVEL 01","LEVEL 02","LEVEL 03","LEVEL 04","LEVEL 05","LEVEL 06","LEVEL 07","LEVEL 08","LEVEL 09","LEVEL 10","LOAD MAP","LOAD SAVE","BACK TO MAIN MENU"}; //matriz menu de niveis
    ALLEGRO_EVENT evento;
    int i;
    int cont_tempo=0;
    int desenha=0;
    int imenu=0;
    char level[50]={"mapa0"};

    while(*estado_menu ==1)
    {
        al_wait_for_event(fila_eventos, &evento);
        if (imenu == 0)
                {
                    mapa=level1;
                }
                else if (imenu == 1)
                {
                    mapa=level2;
                }
        if(evento.type == ALLEGRO_EVENT_TIMER)
        {
            al_draw_bitmap(menubmp,0,0,0);
            cont_tempo++;
            desenha = 1;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)//se tecla foi pressionada
        {
            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE||evento.keyboard.keycode == ALLEGRO_KEY_ENTER)// se tecla pressionada foi espaço ou enter
            {
                printf("space||enter");
                if (imenu == 0)
                {
                    *level_escolhido=1;
                    strcat(arquivo,level);
                    //arquivo = "mapa0";
                    //printf("\n%s", arquivo);
                    break;
                }
                else if (imenu == 1)
                {
                    *level_escolhido=2;
                    break;
                }
                else if (imenu == 2)
                {
                    *level_escolhido=3;
                    break;
                }
                else if (imenu == 3)
                {
                    *level_escolhido=4;
                    break;
                }
                else if (imenu == 4)
                {
                    *level_escolhido=5;
                    break;
                }
                else if (imenu == 5)
                {
                    *level_escolhido=6;
                    break;
                }
                else if (imenu == 6)
                {
                    *level_escolhido=7;
                    break;
                }
                else if (imenu == 7)
                {
                    *level_escolhido=8;
                    break;
                }
                else if (imenu == 8)
                {
                    *level_escolhido=9;
                    break;
                }
                else if (imenu == 9)
                {
                    *level_escolhido=10;
                    break;
                }
                else if (imenu == 10)
                {
                    *load_level=1;
                    break;
                }
                else if (imenu == 11)
                {
                    *load_level=2;
                    break;
                }
                else if (imenu == 12)
                {
                    *estado_menu=0;
                    break;
                }
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W)//se tecla pressionada foi w ou seta
            {
                imenu--;
                printf("key up||w\n");
                if (imenu<0) //se apertar para cima no primeiro item, seleciona o ultimo
                    imenu =12;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)//se tecla pressionada foi s ou seta
            {
                printf("key down||s\n");
                imenu++;
                if (imenu==13) //se apertar para cima no primeiro item, seleciona o ultimo
                    imenu=0;
            }
        }
        if(desenha && al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            for(i=0; i<13; i++)
            {
                if(i==imenu)
                {
                    color=al_map_rgb(255,255,255);
                    al_draw_textf(fonte_menu, color,(*tela).largura*49/50,50*(i+1), ALLEGRO_ALIGN_RIGHT, submenu_levels[i]);
                }
                else
                {
                    color=al_map_rgb(0,0,0);
                    al_draw_textf(fonte_menu_small, color, (*tela).largura*49/50,50*(i+1)+5, ALLEGRO_ALIGN_RIGHT, submenu_levels[i]);
                }

            }
            al_flip_display();
            desenha = 0;
        }
    }
}
void submenusettings(int *estado_menu,int* gameMode)
{
    char submenu_settings[6][50]= {"EFEITOS SONOROS","MUSICA DE FUNDO","TAMANHO TELA DE JOGO","MODO DE JOGO","CREDITS","BACK TO MAIN MENU"};

    char percent[11][6]= {"(100%)","(90%)","(80%)","(70%)","(60%)","(50%)","(40%)","(30%)","(20%)","(10%)","(0%)"};
    char game_screen_mode[2][50]= {"FULLSCREEN","ZOOM"};
    int i;
    int cont_tempo=0;
    int desenha=0;
    int settings=0;
    int item_menu=0;
    int item_menu1=0;
    int item_menu2=0;
    int item_menu4=0;
    while(*estado_menu ==4)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_TIMER)
        {
            al_draw_bitmap(menubmp,0,0,0);
            cont_tempo++;
            desenha = 1;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)//se tecla foi pressionada
        {

            if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)// se tecla pressionada foi espaço ou enter
            {
                if(item_menu==3){
                    settings=4;
                }
                else if (item_menu == 4)//submenu personagens
                {
                    settings=5;//display credits
                }
                else if (item_menu == 5)  //configuraçoes
                {
                    *estado_menu=0;
                    break;
                }

            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_LEFT || evento.keyboard.keycode == ALLEGRO_KEY_A)// se tecla pressionada foi espaço ou enter
            {
                printf("space||enter");

                if (item_menu == 0)
                {
                    item_menu1++;
                    if (item_menu1>=11)
                        item_menu1=11;

                }
                else if (item_menu == 2)
                {
                    item_menu2--;
                    if (item_menu2<0)
                        item_menu2=3;
                }
                else if (item_menu == 3)
                {
                    item_menu4=0;
                    *gameMode=0;

                }

            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT || evento.keyboard.keycode == ALLEGRO_KEY_D)// se tecla pressionada foi espaço ou enter
            {
                printf("space||enter");

                if (item_menu == 0)
                {
                    item_menu1--;
                    if (item_menu1<0)
                        item_menu1=10;
                }
                else if (item_menu == 2)
                {
                    item_menu2++;
                    if (item_menu2>=4)
                        item_menu2=0;
                }
                else if (item_menu == 3)
                {
                    item_menu4=1;
                    *gameMode=1;
                }

            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W)//se tecla pressionada foi w ou seta
            {
                item_menu--;
                printf("key up||w\n");
                if (item_menu<0) //se apertar para cima no primeiro item, seleciona o ultimo
                    item_menu =5;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)//se tecla pressionada foi s ou seta
            {
                printf("key down||s\n");
                item_menu++;
                if (item_menu==6) //se apertar para cima no primeiro item, seleciona o ultimo
                    item_menu=0;



            }
        }
        //desenha=1;
        if(desenha && al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            for(i=0; i<6; i++)
            {
                if(i==item_menu)
                {
                    color=al_map_rgb(255,255,255);
                    al_draw_textf(fonte_menu, color, 100,((50*i)+200), ALLEGRO_ALIGN_LEFT, submenu_settings[i]);
                    if(item_menu==3)
                    al_draw_textf(fonte_menu, color, 1000,350, ALLEGRO_ALIGN_CENTER, game_screen_mode[item_menu4]);

                }
                else
                {
                    color=al_map_rgb(0,0,0);
                    al_draw_textf(fonte_menu_small, color, 100,((50*i)+200), ALLEGRO_ALIGN_LEFT, submenu_settings[i]);
                    al_draw_textf(fonte_menu, color, 1000,350, ALLEGRO_ALIGN_CENTER, game_screen_mode[item_menu4]);

                }


            }
            color=al_map_rgb(0,0,0);
            //al_draw_text(fonte_menu, color, 1000,200, ALLEGRO_ALIGN_RIGHT, percent[item_menu1]);
            //al_draw_textf(fonte_menu, color, 1000,250, ALLEGRO_ALIGN_RIGHT, percent[item_menu1]);
            //al_draw_textf(fonte_menu, color, 1000,300, ALLEGRO_ALIGN_CENTER, screen_sizes[item_menu2]);
            //al_draw_textf(fonte_menu, color, 1000,350, ALLEGRO_ALIGN_CENTER, game_screen_mode[item_menu4]);
            al_flip_display();
            desenha = 0;
        }
    }
}

// parametros para jogar posEntidade em funcao >> &player/&enemy
// receber como *posEntidade player


//showPos(&player, enemy, &PlayerA);
//al_wait_for_event(fila_eventos, &evento);
/* while(!kbhit()){// (evento.type == ALLEGRO_EVENT_TIMER){
         Sleep(100);
         updateGame(gameMap, &player, enemy, &PlayerA);
         showPos(&player, enemy, &PlayerA);
 }*/

