#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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

#define TAM_MENU_P 6
#define MAXLINHA 25
#define MAXCOLUNA 61
#define MAXBOMBAS 30
#define FPS 30.0
#define PPC 64
#define MAIN_MENU 0
#define SINGLEPLAYER 1
#define MULTIPLAYER 2
#define SUBMENU_CHAR 3
#define SUBMENU_SETTINGS 4
#define SAIR_JOGO 5
#define MENU_PAUSE 6
#define MENU_DEATH 7
#define HIGHSCORES 8

typedef struct displaydejogo
{
    float largura,altura;
} tamanhoJogo;

typedef struct entidades
{
    int posX,posY;
} posEntidade;

typedef struct informacoes
{
    int level;
    int vidas;
    int numBombas;
    int numChaves;
    int score;
    int posXBomba[MAXBOMBAS]; // esse numero dentro limita quantas bombas pode ter no mapa, se tiver mais q isso, buga
    int posYBomba[MAXBOMBAS];
    int tempoBomba[MAXBOMBAS];
} playerInfo;

typedef struct informacoessettings
{
    int map,gameMode,bombMode;
    float volm,vols;
} settings;

typedef struct infohighscores
{
    char names[11][25];
    int scores[11];

} highscores;

ALLEGRO_BITMAP *jogador = NULL,*jogador1 = NULL,*jogador2 = NULL,*caixa = NULL, *bomba = NULL, *blank = NULL, *inimigo = NULL, *icon = NULL, *key = NULL,*life=NULL;
ALLEGRO_BITMAP *explosion=NULL;
ALLEGRO_BITMAP *menubmp=NULL;
ALLEGRO_BITMAP *wall=NULL;
ALLEGRO_BITMAP *wall2=NULL;
ALLEGRO_BITMAP *deswall=NULL;
ALLEGRO_BITMAP *deswall2=NULL;
ALLEGRO_BITMAP *logo=NULL;
ALLEGRO_BITMAP *logo2=NULL;
ALLEGRO_BITMAP *c_player=NULL;
ALLEGRO_BITMAP *player1=NULL;
ALLEGRO_BITMAP *player2=NULL;
ALLEGRO_BITMAP *player3=NULL;
ALLEGRO_BITMAP *player4=NULL;
ALLEGRO_BITMAP *player5=NULL;
ALLEGRO_BITMAP *player6=NULL;
ALLEGRO_BITMAP *player8=NULL;
ALLEGRO_BITMAP *gamescreen=NULL;
ALLEGRO_DISPLAY *telaJogo = NULL;
ALLEGRO_SAMPLE *efeitoSonoro = NULL;
ALLEGRO_TIMER *tempo = NULL;
ALLEGRO_AUDIO_STREAM *musicaFundo = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos2 = NULL;
ALLEGRO_FONT *fonte_menu = NULL,*fonte_menu_small = NULL;
ALLEGRO_COLOR color;

void gotoxy(int x, int y)
{
    COORD coord = {0,0};
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// prototipos de funcoes
int  avalia_punch       (char obstaculo, char lado);
void check_highscore    (tamanhoJogo *tela, playerInfo* PlayerA,highscores* scores);
void choose_your_fighter(int *char_escolhido);
void comandoJogador     (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], char keyPressed, playerInfo* PlayerA, settings config);
void comandoJogador2    (char gameMap[][MAXCOLUNA], posEntidade* playerb, posEntidade enemy[], char keyPressed, playerInfo* PlayerB, settings config);
void credits            ();
void definescreensize   (tamanhoJogo *tela);
void executaJogo        (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA,tamanhoJogo *tela, int gameMode,char arquivo[], int* estado_menu,posEntidade* playerb,int map, settings config,highscores* scores);
void explodeBomb        (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int indice, settings config);
void game_mode_zoom     (posEntidade* player, tamanhoJogo*tela);
int  initializeAllegro  ();
void ler_entrada_allegro(tamanhoJogo* tela,char string[],char instrucao);
void load_config        (settings* config);
int  load_highscores    (highscores* scores);
void loadGame		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA,char arquivo[],posEntidade* playerb, settings config);
void mainmenu           ();
void menu_death         (int* estado_menu);
void menu_pause         (char gameMap[][MAXCOLUNA], playerInfo* PlayerA, int* estado_menu);
void moveEnemy          (char gameMap[][MAXCOLUNA], posEntidade enemy[], int direcao, int i, playerInfo* PlayerA);
void multiplayer        (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade* playerb, posEntidade enemy[], playerInfo* PlayerA,tamanhoJogo *tela, char arquivo[], int*estado_menu,playerInfo* PlayerB,int map, settings config);
void punch              (char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA, settings config);
void renderAllegro      (char gameMap[][MAXCOLUNA], char inst);
void renderInfoAllegro  (playerInfo* PlayerA, tamanhoJogo*tela,int map);
void renderMap		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[]);
void save_highscores    (highscores* scores);
void saveGame		    (char gameMap[][MAXCOLUNA], playerInfo* PlayerA);
void setBomb		    (char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA, char keyPressed, settings config);
void setBomb2           (char gameMap[][MAXCOLUNA], posEntidade* playerb, playerInfo* PlayerB, char keyPressed, settings config);
void show_highscores    (tamanhoJogo* tela,highscores* scores,int* estado_menu);
void showPos            (posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void submenuchar        ();
void submenulevels      ();
void submenusettings    ();
void takeDmg            (playerInfo* PlayerA, settings config);
void takeDmg2           (playerInfo* PlayerB, settings config);
void updateEnemies      (char gameMap[][MAXCOLUNA],posEntidade* player, posEntidade enemy[], char instrucao, playerInfo* PlayerA, settings config);
void updateGame		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int* cont_tempo, settings config);
void updateGame2        (char gameMap[][MAXCOLUNA], posEntidade* playerb, posEntidade enemy[], playerInfo* PlayerB, settings config);
void updatePos		    (char gameMap[][MAXCOLUNA], posEntidade* player, char keyPressed, playerInfo* PlayerA, settings config);
void updatePos2         (char gameMap[][MAXCOLUNA], posEntidade* playerb, char keyPressed, playerInfo* PlayerA, settings config);
//jogo
int main()
{
    srand(time(NULL));

    char gameMap[MAXLINHA][MAXCOLUNA];
    char arquivo[50] = {0};
    int estado_menu = 0;
    int level_escolhido=0;
    int load_level=0;
    int char_escolhido = 1 + (rand() % 2);
    settings config;
    highscores scores;
    tamanhoJogo tela;// struct de tamanho de tela
    posEntidade player,playerb, enemy[5];
    playerInfo PlayerA,PlayerB;

    initializeAllegro();

    choose_your_fighter(&char_escolhido);

    definescreensize(&tela);

    load_config(&config);

    load_highscores(&scores);

    while(estado_menu != SAIR_JOGO)//enquanto nao usar EXIT GAME
    {
        if (estado_menu == MAIN_MENU)//condicao padrao
        {
            mainmenu(&estado_menu,&tela);
        }
        else if (estado_menu  == SINGLEPLAYER)//modo singleplayer selecionado
        {
            submenulevels(&estado_menu,&level_escolhido,&load_level,arquivo,&tela);

            if(estado_menu==SINGLEPLAYER)
            {
                executaJogo(gameMap, &player, enemy, &PlayerA,&tela,config.gameMode,arquivo,&estado_menu,&playerb,config.map,config,&scores);//codigo do jogo
            }
            strcpy(arquivo,"\0");
        }
        else if (estado_menu == MULTIPLAYER)//se no menu for selecionado submenu de escolher levels entra nesta condiçao
        {
            char mapa[20]= {"mapa0m"};
            strcat(arquivo,mapa);
            multiplayer(gameMap, &player,&playerb, enemy, &PlayerA,&tela,arquivo,&estado_menu,&PlayerB,config.map,config);
        }
        else if (estado_menu == SUBMENU_CHAR)//se no menu for selecionado submenu de escolher personagem entra nesta condiçao
        {
            submenuchar(&estado_menu,&char_escolhido,&tela);
        }
        else if (estado_menu  == SUBMENU_SETTINGS)//se no menu for selecionado submenu de configuraçoes entra nesta condiçao
        {
            submenusettings(&estado_menu,&tela,&config);
        }
        else if (estado_menu  == HIGHSCORES)//se no menu for selecionado display de highscores entra nesta condiçao
        {
            show_highscores(&tela,&scores,&estado_menu);
        }
    }
    save_highscores(&scores);//salva highscores

    al_destroy_bitmap(menubmp);//destruindo allegro
    al_destroy_display(telaJogo);
    al_destroy_sample(efeitoSonoro);
    al_destroy_audio_stream(musicaFundo);

}

int avalia_punch(char obstaculo, char lado) // funcao que escolhe o som a ser reproduzido ao acionar o botão de interação de acordo com o obstaculo
{
    switch(obstaculo)
    {
    case 'W':
        efeitoSonoro = al_load_sample("sons/punchwall.wav");
        return 0;
    case 'E':
        efeitoSonoro = al_load_sample("sons/oof.wav");
        return 0;
    case ' ':
        efeitoSonoro = al_load_sample("sons/failpunch.wav");
        return 0;
    case 'K':
        efeitoSonoro = al_load_sample("sons/punch.wav");
        return 2;
    case 'C':
        efeitoSonoro = al_load_sample("sons/dlingdling.wav");
        return 3;
    default:
        efeitoSonoro = al_load_sample("sons/soco.wav");
        return 1;
    }
}

void check_highscore(tamanhoJogo *tela, playerInfo* PlayerA,highscores* scores)
{
    char string[50]= {0};
    int i,j,pos=-1;
    for(i=9; i>=0; i--)
    {
        if((*PlayerA).score>=(*scores).scores[i])
        {
            pos=i;
        }
    }
    printf("%d",pos);
    printf("%d",(*scores).scores[0]);
    if(pos>=0)
    {
        for(j=9; pos<=j; j--)
        {
            (*scores).scores[j+1]=(*scores).scores[j];
            strcpy((*scores).names[j+1],(*scores).names[j]);
        }
        (*scores).scores[pos]=(*PlayerA).score;
        ler_entrada_allegro(tela,string,'h');
        strcpy((*scores).names[pos],string);

    }
}

void choose_your_fighter(int *char_escolhido)
{
    //funcao recebe o parametro de char escolhido pelo rand da main, e define os bitmaps para player 1 e 2
    if (*char_escolhido == 1)
    {
        jogador=player1;
        jogador1=player2;
    }
    else if (*char_escolhido == 2)
    {
        jogador=player2;
        jogador1=player1;
    }
}

void comandoJogador(char gameMap[MAXLINHA][MAXCOLUNA], posEntidade* player, posEntidade enemy[], char keyPressed, playerInfo* PlayerA, settings config)
{
    // funcao recebe a tecla pressionada dentro do display da allegro e ativa a função correspondente ao comando
    switch(keyPressed)
    {
    case 'w':
        updatePos(gameMap,player, keyPressed, PlayerA,config);
        break;
    case 'a':
        updatePos(gameMap,player, keyPressed, PlayerA,config);
        break;
    case 's':
        updatePos(gameMap,player, keyPressed, PlayerA,config);
        break;
    case 'd':
        updatePos(gameMap,player, keyPressed, PlayerA,config);
        break;
    case 'b':
        setBomb(gameMap,player,PlayerA, 'b',config);
        break;
    case 'p':
        punch(gameMap, player, PlayerA,config);
        break;
    default:
        return;
    }
}

void comandoJogador2(char gameMap[MAXLINHA][MAXCOLUNA], posEntidade* playerb, posEntidade enemy[], char keyPressed, playerInfo* PlayerB, settings config)
{
    switch(keyPressed)
    {
    case 't':
        updatePos2(gameMap,playerb, keyPressed, PlayerB,config);
        break;
    case 'f':
        updatePos2(gameMap,playerb, keyPressed, PlayerB,config);
        break;
    case 'g':
        updatePos2(gameMap,playerb, keyPressed, PlayerB,config);
        break;
    case 'h':
        updatePos2(gameMap,playerb, keyPressed, PlayerB,config);
        break;
    case 'm':
        setBomb2(gameMap,playerb,PlayerB, 'b',config);
        break;
    case 'l':
        punch(gameMap, playerb, PlayerB,config);
        break;
    default:
        return;
    }
}

void credits(tamanhoJogo* tela)
{
    int i;
    while(true)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
            break;
        if(al_is_event_queue_empty(fila_eventos))
        {
            al_clear_to_color(al_map_rgb(195,195,195));
            for(i=0; i<4; i++)
            {
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,0,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(deswall,0,0,32,32,(i*(*tela).largura/3)-(*tela).largura/6,(*tela).altura/5+(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
            }

            al_draw_textf(fonte_menu, al_map_rgb(0,0,0), (*tela).largura/2,(*tela).altura/10, ALLEGRO_ALIGN_CENTER, "DEVELOPED BY");
            al_draw_textf(fonte_menu, al_map_rgb(0,0,0), (*tela).largura/2,(*tela).altura*2/10, ALLEGRO_ALIGN_CENTER, "JOAO BERTOTTO E LUCAS WERMANN");
            al_draw_scaled_bitmap(player1,0,0,32,32,(*tela).largura/6,(*tela).altura/5,(*tela).largura/3,(*tela).largura/3,0);
            al_draw_scaled_bitmap(player2,0,0,32,32,(*tela).largura/2,(*tela).altura/5,(*tela).largura/3,(*tela).largura/3,ALLEGRO_FLIP_HORIZONTAL);
            al_draw_filled_rectangle(0,(*tela).altura,al_get_display_width(telaJogo),al_get_display_height(telaJogo),al_map_rgb(0,0,0));
            al_flip_display();
        }
    }
}

void definescreensize(tamanhoJogo *tela)
{
    int largura_monitor = al_get_display_width(telaJogo);
    int altura_monitor = al_get_display_height(telaJogo);

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
}

void executaJogo(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA,tamanhoJogo *tela, int gameMode, char arquivo[], int*estado_menu,posEntidade* playerb, int map, settings config,highscores* scores)
{
    // é onde o jogo acontece, é carregado um mapa, começa a contagem de tempo para realizar as renderizações e espera por comandos do jogador dentro do display
    char keyPressed;
    int cont_tempo = 0;
    int desenha = 0;
    int i;
    char inst;
    loadGame(gameMap, player, enemy, PlayerA, arquivo,playerb,config);
    renderMap(gameMap, player, enemy);
    while(*estado_menu == SINGLEPLAYER) // while (true) pra realizar o gameloop, dependendo do comando, essa condição não é mais cumprida e o jogo volta pro menu principal
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_TIMER)
        {
            cont_tempo++;
            desenha = 1;

        }
        else if(evento.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            // coleta a tecla pressionada
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
            }
            comandoJogador(gameMap, player, enemy, keyPressed, PlayerA,config); // passa a tecla pressionada pra funcao que vai levar aos comandos correspondentes
            keyPressed = 'z';
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE: // se a tecla pressionada for ESC, entra no menu de pausa
                *estado_menu = MENU_PAUSE;
                al_flush_event_queue(fila_eventos);
                menu_pause(gameMap, PlayerA, estado_menu);

                al_flush_event_queue(fila_eventos);
                al_clear_to_color(al_map_rgb(0,0,0));

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
            comandoJogador(gameMap, player, enemy, keyPressed, PlayerA,config);
            keyPressed = 'z';
        }
        if ((*PlayerA).vidas <=0) // checando o numero de vidas
        {
            *estado_menu=MENU_DEATH;
            //efeitoSonoro = al_load_sample("sons/morri.wav");
            menu_death(estado_menu);
            break;
        }
        if ((*PlayerA).numChaves == 5) // checa o numero de chaves que o usuario coletou
        {
            check_highscore(tela,PlayerA,scores);
            arquivo[4] = arquivo[4] + 1;
            arquivo[5] = '\0';
            (*PlayerA).level++;
            showPos(player, enemy, PlayerA);
            if(gameMode)
            {
                game_mode_zoom(player, tela);
            }
            else
            {
                al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,0,(*tela).altura/15,(*tela).largura,(*tela).largura*25/60,0);
            }
            renderInfoAllegro(PlayerA, tela,map);
            al_draw_textf(fonte_menu, al_map_rgb(0,0,0),(*tela).largura/2,(*tela).altura/2, ALLEGRO_ALIGN_CENTRE, "NEXT LEVEL");
            al_flip_display();
            while((*PlayerA).numChaves == 5)
            {
                al_wait_for_event(fila_eventos, &evento);

                if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
                    loadGame(gameMap, player, enemy, PlayerA, arquivo,playerb,config);
            }
        }
        if(desenha && al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            updateGame(gameMap, player, enemy, PlayerA, &cont_tempo,config);
            al_clear_to_color(al_map_rgb(0,0,0));
            renderAllegro(gameMap,'r');
            showPos(player, enemy, PlayerA);
            if(gameMode)
            {
                game_mode_zoom(player, tela);
            }
            else
            {
                al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,0,(*tela).altura/15,(*tela).largura,(*tela).largura*25/60,0);
            }
            renderInfoAllegro(PlayerA, tela,map);
            desenha = 0;
            al_flip_display();
        }
    }
}
void explodeBomb(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int indice, settings config)
{
    // nome auto-explicativo, a funcao recebe o indice da bomba a ser explodida e assim o faz
    int i,j,k;
    int posxbomba = (*PlayerA).posXBomba[indice];
    int posybomba = (*PlayerA).posYBomba[indice];

    if (posxbomba == -1 || posybomba == -1) // se o indice da bomba for invalido, não faz nada
        return; // importante
    for (i=1; i<4; i++)
    {
        if (gameMap[posybomba - i][posxbomba] == 'W')
            break;
        else if (gameMap[posybomba - i][posxbomba] == gameMap[(*player).posY][(*player).posX])
        {
            takeDmg(PlayerA,config);
        }
        else if (gameMap[posybomba - i][posxbomba] == 'K')
        {
            gameMap[posybomba - i][posxbomba] = 'C';
            (*PlayerA).score = (*PlayerA).score + 10;
            efeitoSonoro = al_load_sample("sons/dlingdling.wav");
            al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        else if (gameMap[posybomba - i][posxbomba] == 'C')
        {
            continue;
        }
        else if (gameMap[posybomba - i][posxbomba] == 'E')
        {
            for(k = 0; k<5; k++)
            {
                if (enemy[k].posY == posybomba - i && enemy[k].posX == posxbomba)
                {
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerA).score = (*PlayerA).score + 20;
                    efeitoSonoro = al_load_sample("sons/wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else if (gameMap[posybomba - i][posxbomba] == 'b')
            continue;
        else if (gameMap[posybomba - i][posxbomba] == ' ')
            continue;
        else
        {
            gameMap[posybomba - i][posxbomba] = ' ';
            (*PlayerA).score = (*PlayerA).score + 10;
            continue;
        }
    }
    for (i=1; i<4; i++)
    {
        if (gameMap[posybomba][posxbomba - i] == 'W')
            break;
        else if (gameMap[posybomba][posxbomba - i] == gameMap[(*player).posY][(*player).posX])
        {
            takeDmg(PlayerA,config);
        }
        else if (gameMap[posybomba][posxbomba - i] == 'K')
        {
            gameMap[posybomba][posxbomba - i] = 'C';
            (*PlayerA).score = (*PlayerA).score + 10;
        }
        else if (gameMap[posybomba][posxbomba - i] == 'E')
        {
            for(k = 0; k<5; k++)
            {
                if (enemy[k].posY == posybomba && enemy[k].posX == posxbomba - i)
                {
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerA).score = (*PlayerA).score + 20;
                    efeitoSonoro = al_load_sample("sons/wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else if (gameMap[posybomba][posxbomba - i] == 'C')
        {
            continue;
        }
        else if (gameMap[posybomba][posxbomba - i] == 'b')
            continue;
        else if (gameMap[posybomba][posxbomba - i] == ' ')
            continue;
        else
        {
            gameMap[posybomba][posxbomba - i] = ' ';
            (*PlayerA).score = (*PlayerA).score + 10;
            continue;
        }
    }
    for (i=1; i<4; i++)
    {
        if (gameMap[posybomba + i][posxbomba] == 'W')
            break;
        else if (gameMap[posybomba + i][posxbomba] == gameMap[(*player).posY][(*player).posX])
        {
            takeDmg(PlayerA,config);
        }
        else if (gameMap[posybomba + i][posxbomba] == 'K')
        {
            gameMap[posybomba + i][posxbomba] = 'C';
            (*PlayerA).score = (*PlayerA).score + 10;
        }
        else if (gameMap[posybomba + i][posxbomba] == 'C')
        {
            continue;
        }
        else if (gameMap[posybomba + i][posxbomba] == 'E')
        {
            for(k = 0; k<5; k++)
            {
                if (enemy[k].posY == posybomba + i && enemy[k].posX == posxbomba)
                {
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerA).score = (*PlayerA).score + 20;
                    efeitoSonoro = al_load_sample("sons/wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else if (gameMap[posybomba + i][posxbomba] == 'b')
            continue;
        else if (gameMap[posybomba + i][posxbomba] == ' ')
            continue;
        else
        {
            gameMap[posybomba + i][posxbomba] = ' ';
            (*PlayerA).score = (*PlayerA).score + 10;
            continue;
        }
    }
    for (i=1; i<4; i++)
    {
        if (gameMap[posybomba][posxbomba + i] == 'W')
            break;
        else if (gameMap[posybomba][posxbomba + i] == gameMap[(*player).posY][(*player).posX])
        {
            takeDmg(PlayerA,config);
        }
        else if (gameMap[posybomba][posxbomba + i] == 'K')
        {
            gameMap[posybomba][posxbomba + i] = 'C';
            (*PlayerA).score = (*PlayerA).score + 10;
        }
        else if (gameMap[posybomba][posxbomba + i] == 'C')
        {
            continue;
        }
        else if (gameMap[posybomba][posxbomba + i] == 'E')
        {
            for(k = 0; k<5; k++)
            {
                if (enemy[k].posY == posybomba && enemy[k].posX == posxbomba + i)
                {
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerA).score = (*PlayerA).score + 20;
                    efeitoSonoro = al_load_sample("sons/wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else if (gameMap[posybomba][posxbomba + i] == 'b')
            continue;
        else if (gameMap[posybomba][posxbomba + i] == ' ')
            continue;
        else
        {
            gameMap[posybomba][posxbomba + i] = ' ';
            (*PlayerA).score = (*PlayerA).score + 10;
            continue;
        }
    }
    if(config.bombMode)
    {
        (*PlayerA).numBombas++;
    }
    efeitoSonoro = al_load_sample("sons/explosion.wav");
    al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    gameMap[posybomba][posxbomba] = ' ';
    (*PlayerA).posXBomba[indice] = -1; // invalida a bomba, permite sobrescrever em outro setBomb()
}

void explodeBomb2(char gameMap[][MAXCOLUNA], posEntidade* playerb, posEntidade enemy[], playerInfo* PlayerB, int indice, settings config)
{
    int i,j,k;
    int posxbomba = (*PlayerB).posXBomba[indice];
    int posybomba = (*PlayerB).posYBomba[indice];

    if (posxbomba == -1 || posybomba == -1)
        return; // importante
    for (i=1; i<4; i++)
    {
        if (gameMap[posybomba - i][posxbomba] == 'W')
            break;
        else if (gameMap[posybomba - i][posxbomba] == gameMap[(*playerb).posY][(*playerb).posX])
        {
            takeDmg(PlayerB,config);
        }
        else if (gameMap[posybomba - i][posxbomba] == 'K')
        {
            gameMap[posybomba - i][posxbomba] = 'C';
            (*PlayerB).score = (*PlayerB).score + 10;
            efeitoSonoro = al_load_sample("sons/dlingdling.wav");
            al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        else if (gameMap[posybomba - i][posxbomba] == 'C')
        {
            continue;
        }
        else if (gameMap[posybomba - i][posxbomba] == 'E')
        {
            for(k = 0; k<5; k++)
            {
                if (enemy[k].posY == posybomba - i && enemy[k].posX == posxbomba)
                {
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerB).score = (*PlayerB).score + 20;
                    efeitoSonoro = al_load_sample("sons/wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else if (gameMap[posybomba - i][posxbomba] == 'b')
            continue;
        else if (gameMap[posybomba - i][posxbomba] == ' ')
            continue;
        else
        {
            gameMap[posybomba - i][posxbomba] = ' ';
            (*PlayerB).score = (*PlayerB).score + 10;
            continue;
        }
    }
    for (i=1; i<4; i++)
    {
        if (gameMap[posybomba][posxbomba - i] == 'W')
            break;
        else if (gameMap[posybomba][posxbomba - i] == gameMap[(*playerb).posY][(*playerb).posX])
        {
            takeDmg(PlayerB,config);
        }
        else if (gameMap[posybomba][posxbomba - i] == 'K')
        {
            gameMap[posybomba][posxbomba - i] = 'C';
            (*PlayerB).score = (*PlayerB).score + 10;
        }
        else if (gameMap[posybomba][posxbomba - i] == 'E')
        {
            for(k = 0; k<5; k++)
            {
                if (enemy[k].posY == posybomba && enemy[k].posX == posxbomba - i)
                {
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerB).score = (*PlayerB).score + 20;
                    efeitoSonoro = al_load_sample("sons/wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else if (gameMap[posybomba][posxbomba - i] == 'C')
        {
            continue;
        }
        else if (gameMap[posybomba][posxbomba - i] == 'b')
            continue;
        else if (gameMap[posybomba][posxbomba - i] == ' ')
            continue;
        else
        {
            gameMap[posybomba][posxbomba - i] = ' ';
            (*PlayerB).score = (*PlayerB).score + 10;
            continue;
        }
    }
    for (i=1; i<4; i++)
    {
        if (gameMap[posybomba + i][posxbomba] == 'W')
            break;
        else if (gameMap[posybomba + i][posxbomba] == gameMap[(*playerb).posY][(*playerb).posX])
        {
            takeDmg(PlayerB,config);
        }
        else if (gameMap[posybomba + i][posxbomba] == 'K')
        {
            gameMap[posybomba + i][posxbomba] = 'C';
            (*PlayerB).score = (*PlayerB).score + 10;
        }
        else if (gameMap[posybomba + i][posxbomba] == 'C')
        {
            continue;
        }
        else if (gameMap[posybomba + i][posxbomba] == 'E')
        {
            for(k = 0; k<5; k++)
            {
                if (enemy[k].posY == posybomba + i && enemy[k].posX == posxbomba)
                {
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerB).score = (*PlayerB).score + 20;
                    efeitoSonoro = al_load_sample("sons/wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else if (gameMap[posybomba + i][posxbomba] == 'b')
            continue;
        else if (gameMap[posybomba + i][posxbomba] == ' ')
            continue;
        else
        {
            gameMap[posybomba + i][posxbomba] = ' ';
            (*PlayerB).score = (*PlayerB).score + 10;
            continue;
        }
    }
    for (i=1; i<4; i++)
    {
        if (gameMap[posybomba][posxbomba + i] == 'W')
            break;
        else if (gameMap[posybomba][posxbomba + i] == gameMap[(*playerb).posY][(*playerb).posX])
        {
            takeDmg(PlayerB,config);
        }
        else if (gameMap[posybomba][posxbomba + i] == 'K')
        {
            gameMap[posybomba][posxbomba + i] = 'C';
            (*PlayerB).score = (*PlayerB).score + 10;
        }
        else if (gameMap[posybomba][posxbomba + i] == 'C')
        {
            continue;
        }
        else if (gameMap[posybomba][posxbomba + i] == 'E')
        {
            for(k = 0; k<5; k++)
            {
                if (enemy[k].posY == posybomba && enemy[k].posX == posxbomba + i)
                {
                    gameMap[enemy[k].posY][enemy[k].posX] = ' ';
                    enemy[k].posX = -1;
                    enemy[k].posY = -1;
                    (*PlayerB).score = (*PlayerB).score + 20;
                    efeitoSonoro = al_load_sample("sons/wilhelmscream.wav");
                    al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }
        else if (gameMap[posybomba][posxbomba + i] == 'b')
            continue;
        else if (gameMap[posybomba][posxbomba + i] == ' ')
            continue;
        else
        {
            gameMap[posybomba][posxbomba + i] = ' ';
            (*PlayerB).score = (*PlayerB).score + 10;
            continue;
        }
    }
    if(config.bombMode)
    {
        (*PlayerB).numBombas++;
    }
    efeitoSonoro = al_load_sample("sons/explosion.wav");
    al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    gameMap[posybomba][posxbomba] = ' ';
    (*PlayerB).posXBomba[indice] = -1; // invalida a bomba, permite sobrescrever em outro setBomb()
}

void game_mode_zoom(posEntidade* player, tamanhoJogo*tela)
{
    if((*player).posY<7)
    {
        if((*player).posX<15)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,0,(*tela).altura/15,(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>=15 &&(*player).posX<=30)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,-((*player).posX-14)*((*tela).largura/30),(*tela).altura/15,(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>30 &&(*player).posX<=45)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,-((*player).posX-15)*((*tela).largura/30),(*tela).altura/15,(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>45)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,(-(*tela).largura),(*tela).altura/15,(*tela).largura*2,((*tela).largura)*25/30,0);
        }
    }
    else if((*player).posY>=7&&(*player).posY<=17)
    {
        if((*player).posX<15)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,0,((*tela).altura/15)-(((*player).posY-6)*((*tela).largura/30)),(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>=15 &&(*player).posX<=30)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,-((*player).posX-14)*((*tela).largura/30),((*tela).altura/15)-(((*player).posY-6)*((*tela).largura/30)),(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>30 &&(*player).posX<=45)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,-((*player).posX-15)*((*tela).largura/30),((*tela).altura/15)-(((*player).posY-6)*((*tela).largura/30)),(*tela).largura*2,((*tela).largura)*25/30,0);
        }
        else if((*player).posX>45)
        {
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,(-(*tela).largura),((*tela).altura/15)-(((*player).posY-6)*((*tela).largura/30)),(*tela).largura*2,((*tela).largura)*25/30,0);
        }
    }
    else if((*player).posY>17)
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

    fila_eventos = al_create_event_queue();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(64);
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();


    gamescreen=al_create_bitmap(3840,1600);//buffer tela de jogo
    jogador = al_create_bitmap(256,192);
    jogador1 = al_create_bitmap(256,192);
    bomba = al_load_bitmap("imagens/bomb.png");
    caixa = al_load_bitmap("imagens/box.png");
    blank = al_load_bitmap("imagens/blank.png");
    inimigo = al_load_bitmap("imagens/enemynew.png");
    icon = al_load_bitmap("imagens/dinoicon.png");
    menubmp = al_load_bitmap("imagens/gamemenu.png");
    wall = al_load_bitmap("imagens/wall2.png");
    wall2 = al_load_bitmap("imagens/wall3.png");
    deswall = al_load_bitmap("imagens/wall.png");
    deswall2 = al_load_bitmap("imagens/wall4.png");
    key = al_load_bitmap("imagens/key.png");
    life= al_load_bitmap("imagens/life.png");
    explosion= al_load_bitmap("imagens/explosion.png");
    player1 = al_load_bitmap("imagens/spritesheetbertotto.png");
    player2 = al_load_bitmap("imagens/spritesheetwermann.png");
    player3 = al_load_bitmap("imagens/spritesheetspidey.png");
    player4 = al_load_bitmap("imagens/spritesheetbat.png");
    player5 = al_load_bitmap("imagens/spritesheetskywalker.png");
    player6 = al_load_bitmap("imagens/spritesheetrobocop.png");
    player8 = al_load_bitmap("imagens/dino.png");
    logo=al_load_bitmap("imagens/logo.png");
    logo2=al_load_bitmap("imagens/ufrgs.png");
    if(!bomba||!caixa) //colocar outros casos
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
    al_convert_mask_to_alpha(caixa, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(explosion, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(logo2, al_map_rgb(255, 0, 255));

    fonte_menu_small = al_load_font("fontes/ka1.ttf", 36, 0);
    fonte_menu = al_load_font("fontes/ka1.ttf", 48, 0);

    musicaFundo = al_load_audio_stream("sons/abc.wav",4,1024);
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

void ler_entrada_allegro(tamanhoJogo* tela,char string[],char instrucao)
{
    int i;
    ALLEGRO_EVENT evento;
    al_flush_event_queue(fila_eventos);

    int sair=0;
    while(!sair)
    {
        al_wait_for_event(fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            char temporario[] = {evento.keyboard.unichar, '\0'};
            if (evento.keyboard.unichar == ' '||evento.keyboard.unichar == '-')
            {
                strcat(string, temporario);
            }
            else if (evento.keyboard.unichar >= '0' && evento.keyboard.unichar <= '9')
            {
                strcat(string, temporario);
            }
            else if (evento.keyboard.unichar >= 'A' && evento.keyboard.unichar <= 'Z')
            {
                strcat(string, temporario);
            }
            else if (evento.keyboard.unichar >= 'a' && evento.keyboard.unichar <= 'z')
            {

                strcat(string, temporario);
            }
            if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(string) != 0)
            {
                string[strlen(string) - 1] = '\0';
            }
        }
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)//se tecla foi pressionada
        {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)
            {
                sair=1;
            }
        }
        if(al_is_event_queue_empty(fila_eventos))
        {
            for(i=0; i<4; i++)
            {
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,0,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(deswall,0,0,32,32,i*(*tela).largura/3,((*tela).altura/3.5)+(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
            }
            if(instrucao=='a')
            {
                al_draw_textf(fonte_menu, al_map_rgb(255,255,255), (*tela).largura/2,(*tela).altura/5, ALLEGRO_ALIGN_CENTER, "Digite o nome do arquivo que");
                al_draw_textf(fonte_menu, al_map_rgb(255,255,255), (*tela).largura/2,(*tela).altura/3.5, ALLEGRO_ALIGN_CENTER, "deseja carregar (sem formato)");
                al_draw_scaled_bitmap(jogador,0,0,32,32,(*tela).largura/30,(*tela).altura/3.5,(*tela).largura/3,(*tela).largura/3,0);
            }
            else if(instrucao=='h')
            {
                al_draw_textf(fonte_menu, al_map_rgb(255,255,255), (*tela).largura/2,(*tela).altura/5, ALLEGRO_ALIGN_CENTER, "you entered the highscores");
                al_draw_textf(fonte_menu, al_map_rgb(255,255,255), (*tela).largura/2,(*tela).altura/3.5, ALLEGRO_ALIGN_CENTER, "enter your name");
                al_draw_scaled_bitmap(jogador,96,128,32,32,(*tela).largura/30,(*tela).altura/3.5,(*tela).largura/3,(*tela).largura/3,0);
            }
            al_draw_textf(fonte_menu, al_map_rgb(255,255,255), (*tela).largura/2,(*tela).altura/2, ALLEGRO_ALIGN_CENTER, string);
            al_flip_display();

        }
    }
}

void load_config(settings* config)
{
    FILE *fp;
    fp = fopen("config.txt","r");
    if (fp == NULL)
    {
        (*config).vols=1;
        (*config).volm=0.6;
        (*config).map=0;
        (*config).gameMode=0;
        (*config).bombMode=1;
    }
    else
    {
        fscanf(fp, "%f\n%f\n%d\n%d\n%d",&(*config).vols,&(*config).volm,&(*config).map, &(*config).gameMode,&(*config).bombMode);
    }
    fclose(fp);
}

int load_highscores(highscores* scores)
{
    FILE *fp;
    int i;
    fp = fopen("highscores.txt","r");
    if (fp == NULL)
    {
        return 0;
    }
    else
    {
        for(i=0; i<10; i++)
        {
            fscanf(fp, "%s\n%d",&(*scores).names[i],&(*scores).scores[i]);
        }
    }
    fclose(fp);
}

void loadGame(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA,char arquivo[], posEntidade* playerb, settings config)
{
    // funcao que pega as informacoes do arquivo e escreve nas variaveis dentro do programa
    FILE *fp;
    int i,j,k = 0;
    int indice = 0;
    char pasta[50]= {"mapas/"};
    char extensao[5] = {".txt"};
    char info[60] = {"mapas/info"};
    strcat(arquivo,extensao);
    strcat(pasta,arquivo);
    fp = fopen(pasta, "r");
    if(!fp) // se a abertura for invalida, abre um mapa padrão
    {
        fp = fopen("mapas/mapa0.txt", "r");
    }
    for(i=0; i<MAXLINHA; i++) // percorre toda a matriz, coletando as informacoes e armazenando o que for necessário
    {
        for(j=0; j<MAXCOLUNA; j++)
        {
            fscanf(fp, "%c", &gameMap[i][j]);
            if (gameMap[i][j] == 'w' || gameMap[i][j] == 'a' || gameMap[i][j] == 's' || gameMap[i][j] == 'd' || gameMap[i][j] == 'J')
            {
                (*player).posX = j;
                (*player).posY = i;
            }
            else if (gameMap[i][j] == 'b')
            {
                (*PlayerA).posXBomba[indice] = j;
                (*PlayerA).posYBomba[indice] = i;
                indice++;
            }
            else if (gameMap[i][j] == 'E')
            {
                enemy[k].posX = j;
                enemy[k].posY = i;
                k++;
            }
            else if (gameMap[i][j] == 't' || gameMap[i][j] == 'f' || gameMap[i][j] == 'g' || gameMap[i][j] == 'h' || gameMap[i][j] == 'Y')
            {
                (*playerb).posX = j;
                (*playerb).posY = i;
            }
        }
    }
    fclose(fp);
    while (k<5)// se numero de inimigos no mapa lido for menor que 5, é necessario invalidar a posicao dos que ja 'morreram'
    {
        enemy[k].posX = -1;
        enemy[k].posY = -1;
        k++;
    }
    strcat(info,arquivo);
    fp = fopen(info,"r");
    if (fp == NULL)
    {
        (*PlayerA).level = 1;
        (*PlayerA).numBombas = 3;
        (*PlayerA).numChaves = 0;
        (*PlayerA).score = 0;
        (*PlayerA).vidas = 3;
    }
    else // le arquivo de informacoes do player
    {
        fscanf(fp, "%d\n%d\n%d\n%d\n%d", &(*PlayerA).level, &(*PlayerA).vidas, &(*PlayerA).numBombas,&(*PlayerA).numChaves, &(*PlayerA).score);
    }
    setBomb(gameMap, player, PlayerA, 'l',config);// setup de bombas quando carrega um mapa
    updateEnemies(gameMap,player, enemy, 'l', PlayerA,config); // prepara os inimigos para andar, escolhendo a direcao
    renderAllegro(gameMap, 'l');  // primeiro render
    fclose(fp);
    arquivo = 0;
}

void mainmenu(int *estado_menu,tamanhoJogo* tela)
{
    ALLEGRO_EVENT evento;
    int i;
    int cont_tempo=0;
    char menu_p[TAM_MENU_P][50] = {"SINGLEPLAYER","MULTIPLAYER","CHOOSE CHARACTER","HIGHSCORES","SETTINGS","EXIT GAME"};
    int imenu=0;
    while(*estado_menu ==0)
    {
        al_wait_for_event(fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN) //se tecla foi pressionada
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
                    *estado_menu=HIGHSCORES;
                    break;
                }
                else if (imenu == 4)  //configuraçoes
                {
                    *estado_menu=4;
                    break;
                }
                else if (imenu == 5)  //sair
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
        if(al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            for(i=0; i<4; i++)
            {
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,0,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(deswall,0,0,32,32,i*(*tela).largura/3,((*tela).altura/3.5)+(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
            }
            al_draw_scaled_bitmap(logo,0,0,al_get_bitmap_width(logo),al_get_bitmap_height(logo),(*tela).largura/4,(*tela).altura/15,(*tela).largura/2,(*tela).largura*al_get_bitmap_height(logo)/(2*al_get_bitmap_width(logo)),0);
            al_draw_scaled_bitmap(logo2,0,0,al_get_bitmap_width(logo2),al_get_bitmap_height(logo2),((*tela).largura-(*tela).largura/4)/2,(*tela).altura/4,(*tela).largura/4,(*tela).largura*al_get_bitmap_height(logo2)/(4*al_get_bitmap_width(logo2)),0);
            al_draw_filled_rectangle(0,(*tela).altura,al_get_display_width(telaJogo),al_get_display_height(telaJogo),al_map_rgb(0,0,0));
            al_draw_filled_rectangle((*tela).largura,0,al_get_display_width(telaJogo),al_get_display_height(telaJogo),al_map_rgb(0,0,0));

            al_draw_scaled_bitmap(jogador,0,0,32,32,(*tela).largura/30,(*tela).altura/3.5,(*tela).largura/3,(*tela).largura/3,0);
            al_draw_scaled_bitmap(jogador1,0,0,32,32,(*tela).largura-(*tela).largura/3-(*tela).largura/30,(*tela).altura/3.5,(*tela).largura/3,(*tela).largura/3,ALLEGRO_FLIP_HORIZONTAL);
            for(i=0; i<TAM_MENU_P; i++)
            {
                if(i==imenu)
                {
                    color=al_map_rgb(255,255,255);
                    al_draw_textf(fonte_menu, color,(*tela).largura/2,50*i+400, ALLEGRO_ALIGN_CENTRE, menu_p[i]);
                }
                else
                {
                    color=al_map_rgb(0,0,0);
                    al_draw_textf(fonte_menu_small, color,(*tela).largura/2,50*i+405, ALLEGRO_ALIGN_CENTRE, menu_p[i]);
                }

            }
            al_flip_display();
        }
    }
}

void menu_death(int* estado_menu)
{
    ALLEGRO_EVENT evento;
    int itemmenu=1;
    int i;
    char menu_pause[3][50] = {"YOU HAVE DIED","RELOAD LEVEL","BACK TO MAIN MENU"};
    while(*estado_menu==MENU_DEATH)
    {
        al_wait_for_event(fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE||evento.keyboard.keycode == ALLEGRO_KEY_ENTER)// se tecla pressionada foi espaço ou enter
            {
                printf("space||enter");
                if (itemmenu == 1)
                {
                    //reload level
                    break;
                }
                else if (itemmenu == 2)
                {
                    *estado_menu=0;
                    break;
                }

            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W)//se tecla pressionada foi w ou seta
            {
                itemmenu--;
                if (itemmenu<1) //se apertar para cima no primeiro item, seleciona o ultimo
                    itemmenu =2;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)//se tecla pressionada foi s ou seta
            {
                itemmenu++;
                if (itemmenu==3) //se apertar para cima no primeiro item, seleciona o ultimo
                    itemmenu=1;
            }
        }
        if(al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            for(i=0; i<3; i++)
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
}

void menu_pause(char gameMap[][MAXCOLUNA], playerInfo* PlayerA, int* estado_menu)
{
    ALLEGRO_EVENT evento;
    int itemmenu=0;
    int i;
    char menu_pause[3][50] = {"RETURN","SAVE GAME","BACK TO MAIN MENU"};
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
                    *estado_menu = SINGLEPLAYER;
                    break;
                }
                else if (itemmenu == 1)
                {
                    //save game
                    saveGame(gameMap, PlayerA);
                    *estado_menu = SINGLEPLAYER;
                    break;
                }
                else if (itemmenu == 2)
                {
                    *estado_menu=0;
                    break;
                }

            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W)//se tecla pressionada foi w ou seta
            {
                itemmenu--;
                if (itemmenu<0) //se apertar para cima no primeiro item, seleciona o ultimo
                    itemmenu =2;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)//se tecla pressionada foi s ou seta
            {
                itemmenu++;
                if (itemmenu==3) //se apertar para cima no primeiro item, seleciona o ultimo
                    itemmenu=0;
            }
        }
        if(al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            for(i=0; i<3; i++)
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
}

void moveEnemy(char gameMap[][MAXCOLUNA], posEntidade enemy[], int direcao, int i, playerInfo* PlayerA)
{
    // funcao que recebe o indice de um inimigo e o movimenta em uma direcao definida
    switch(direcao)
    {
    case 0: // w
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        enemy[i].posY--;
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");
        break;
    case 1: // a
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        enemy[i].posX--;
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");
        break;
    case 2: // s
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        enemy[i].posY++;
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");
        break;
    case 3: // d
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        enemy[i].posX++;
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");
        break;
    }
}

void multiplayer(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade* playerb, posEntidade enemy[], playerInfo* PlayerA,tamanhoJogo *tela, char arquivo[], int*estado_menu,playerInfo* PlayerB,int map, settings config)
{
    char keyPressed;
    int cont_tempo = 0;
    int desenha = 0;
    loadGame(gameMap, player, enemy, PlayerA, arquivo,playerb,config);
    renderMap(gameMap, player, enemy);
    (*PlayerB).level = 1;
    (*PlayerB).numBombas = 3;
    (*PlayerB).numChaves = 0;
    (*PlayerB).score = 0;
    (*PlayerB).vidas = 3;
    while(*estado_menu == MULTIPLAYER)
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
            case ALLEGRO_KEY_B:
                keyPressed = 'b';
                break;
            case ALLEGRO_KEY_E:
                keyPressed = 'p';
                break;
            }
            comandoJogador(gameMap, player, enemy, keyPressed, PlayerA,config);
            switch(evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
                keyPressed = 't';
                break;
            case ALLEGRO_KEY_LEFT:
                keyPressed = 'f';
                break;
            case ALLEGRO_KEY_DOWN:
                keyPressed = 'g';
                break;
            case ALLEGRO_KEY_RIGHT:
                keyPressed = 'h';
                break;
            case ALLEGRO_KEY_M:
                keyPressed = 'm';
                break;
            case ALLEGRO_KEY_L:
                keyPressed = 'p';
                break;
            }
            comandoJogador2(gameMap, playerb, enemy, keyPressed, PlayerB,config);
        }
        if(desenha && al_is_event_queue_empty(fila_eventos))
        {
          //updateGame2(gameMap, playerb, enemy, PlayerB,config);
            updateGame(gameMap, player, enemy, PlayerA, &cont_tempo,config);
            al_clear_to_color(al_map_rgb(0,0,0));
            renderAllegro(gameMap,'r');
            showPos(player, enemy, PlayerA);
            al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,0,(*tela).altura/15,(*tela).largura,(*tela).largura*25/60,0);
            renderInfoAllegro(PlayerA, tela, map);
            desenha = 0;
            al_flip_display();
        }
    }
}

void punch(char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA, settings config)
{
    // comando de 'soco' ou interacao, auto-explicativo
    char lado = gameMap[(*player).posY][(*player).posX];
    switch(lado) // a interacao soco depende do lado que o personagem está virado
    {
    case 'w':
        if (avalia_punch(gameMap[(*player).posY - 1][(*player).posX], lado) == 1) // a funcao avalia punch tambem retorna como o soco afeta o caracter com que esta interagindo
        {
            gameMap[(*player).posY - 1][(*player).posX] = ' '; // limpa, se for necessario
            //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY - 1) * 22, 0);
        }
        else if (avalia_punch(gameMap[(*player).posY - 1][(*player).posX], lado) == 2)
        {
            gameMap[(*player).posY - 1][(*player).posX] = 'C'; // se for uma caixa com chave, substitui o caracter por 'C' referente à chave
            //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY - 1) * 22, 0);
            //al_draw_bitmap(key, ((*player).posX)*22, ((*player).posY - 1) * 22, 0);
        }
        else if (avalia_punch(gameMap[(*player).posY - 1][(*player).posX], lado) == 3)
        {
            gameMap[(*player).posY - 1][(*player).posX] = ' '; // se interagir com uma chave, limpa o tile e coleta a chave
            //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY - 1)*22, 0);
            (*PlayerA).numChaves++;
        }
        break;
    case 'a':
        if (avalia_punch(gameMap[(*player).posY][(*player).posX - 1], lado) == 1)
        {
            gameMap[(*player).posY][(*player).posX - 1] = ' ';
            //al_draw_bitmap(blank, ((*player).posX - 1)*22, ((*player).posY) * 22, 0);
        }
        else if (avalia_punch(gameMap[(*player).posY][(*player).posX - 1], lado) == 2)
        {
            gameMap[(*player).posY][(*player).posX - 1] = 'C';
            //al_draw_bitmap(blank, ((*player).posX - 1)*22, ((*player).posY) * 22, 0);
            //al_draw_bitmap(key, ((*player).posX - 1)*22, ((*player).posY) * 22, 0);
        }
        else if (avalia_punch(gameMap[(*player).posY][(*player).posX - 1], lado) == 3)
        {
            gameMap[(*player).posY][(*player).posX - 1] = ' ';
            //al_draw_bitmap(blank, ((*player).posX - 1)*22, (*player).posY*22, 0);
            (*PlayerA).numChaves++;
        }
        break;
    case 's':
        if (avalia_punch(gameMap[(*player).posY + 1][(*player).posX], lado) == 1)
        {
            gameMap[(*player).posY + 1][(*player).posX] = ' ';
            //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY + 1) * 22, 0);
        }
        else if (avalia_punch(gameMap[(*player).posY + 1][(*player).posX], lado) == 2)
        {
            gameMap[(*player).posY + 1][(*player).posX] = 'C';
            //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY + 1) * 22, 0);
            //al_draw_bitmap(key, ((*player).posX)*22, ((*player).posY + 1) * 22, 0);
        }
        else if (avalia_punch(gameMap[(*player).posY + 1][(*player).posX], lado) == 3)
        {
            gameMap[(*player).posY + 1][(*player).posX] = ' ';
            //al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY + 1)*22, 0);
            (*PlayerA).numChaves++;
        }
        break;
    case 'd':
        if (avalia_punch(gameMap[(*player).posY][(*player).posX + 1], lado) == 1)
        {
            gameMap[(*player).posY][(*player).posX + 1] = ' ';
            //al_draw_bitmap(blank, ((*player).posX+1)*22, (*player).posY * 22, 0);
        }
        else if (avalia_punch(gameMap[(*player).posY][(*player).posX + 1], lado) == 2)
        {
            gameMap[(*player).posY][(*player).posX + 1] = 'C';
            //al_draw_bitmap(blank, ((*player).posX+1)*22, (*player).posY * 22, 0);
            //al_draw_bitmap(key, ((*player).posX+1)*22, (*player).posY * 22, 0);
        }
        else if (avalia_punch(gameMap[(*player).posY][(*player).posX + 1], lado) == 3)
        {
            gameMap[(*player).posY][(*player).posX + 1] = ' ';
            //al_draw_bitmap(blank, ((*player).posX + 1)*22, (*player).posY*22, 0);
            (*PlayerA).numChaves++;
        }
        break;
    default:
        return;
    }
    al_flip_display();
    al_play_sample(efeitoSonoro, (config).vols*0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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
                al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                switch(gameMap[i][j])
                {
                case 'W':
                    if(gameMap[i+1][j]=='W'||gameMap[i+1][j]=='D')
                    {
                        al_draw_scaled_bitmap(wall2,0,0,32,48, j*PPC, i*PPC-32,64,96, 0);
                    }
                    else
                    {
                        al_draw_scaled_bitmap(wall,0,0,32,48, j*PPC, i*PPC-32,64,96, 0);
                    }
                    break;
                case 'D':
                    if(gameMap[i+1][j]=='W'||gameMap[i+1][j]=='D')
                    {
                        al_draw_scaled_bitmap(deswall2,0,0,32,48, j*PPC, i*PPC-32,64,96, 0);
                    }
                    else
                    {
                        al_draw_scaled_bitmap(deswall,0,0,32,48, j*PPC, i*PPC-32,64,96, 0);
                    }
                    break;
                case 'K':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(caixa,0,0,64,64, j*PPC, i*PPC,64,64, 0);
                    break;
                case 'B':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(caixa,0,0,64,64, j*PPC, i*PPC,64,64, 0);
                    break;
                case 'b':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(bomba,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    break;
                case 'E':
                    al_draw_scaled_bitmap(inimigo,0,0,32,32, j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'J':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(jogador, 0,0,32,32,j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'w':
                    al_draw_scaled_bitmap(jogador, 128,32,32,32,j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'a':
                    al_draw_scaled_bitmap(jogador, 0,0,32,32,j*PPC, i*PPC-32,PPC,PPC, ALLEGRO_FLIP_HORIZONTAL);
                    break;
                case 's':
                    al_draw_scaled_bitmap(jogador, 0,32,32,32,j*PPC, i*PPC-32,PPC,PPC, 0);
                    break;
                case 'd':
                    al_draw_scaled_bitmap(jogador, 0,0,32,32,j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'Y':
                    al_draw_scaled_bitmap(jogador1, 0,0,32,32,j*PPC, i*PPC,64,64, 0);
                    break;
                case 't':
                    al_draw_scaled_bitmap(jogador1, 128,32,32,32,j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'f':
                    al_draw_scaled_bitmap(jogador1, 0,0,32,32,j*PPC, i*PPC-32,PPC,PPC, ALLEGRO_FLIP_HORIZONTAL);
                    break;
                case 'g':
                    al_draw_scaled_bitmap(jogador1, 0,32,32,32,j*PPC, i*PPC-32,PPC,PPC, 0);
                    break;
                case 'h':
                    al_draw_scaled_bitmap(jogador1, 0,0,32,32,j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'C':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(key,0,0,32,32,j*PPC, i*PPC,64,64, 0);
                    break;
                }
            }
        }
        al_save_bitmap("imagens/background.png", gamescreen);
    case 'r':
        for (i=0; i<MAXLINHA; i++)
        {
            for(j=0; j<MAXCOLUNA; j++)
            {
                al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                switch(gameMap[i][j])
                {
                case 'W':
                    if(gameMap[i+1][j]=='W')
                    {
                        al_draw_scaled_bitmap(wall2,0,0,32,48, j*PPC, i*PPC-32,64,96, 0);
                    }
                    else
                    {
                        al_draw_scaled_bitmap(wall,0,0,32,48, j*PPC, i*PPC-32,64,96, 0);
                    }
                    break;
                case ' ':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    break;
                case 'D':
                    if(gameMap[i-1][j]=='W'|| gameMap[i-1][j]=='D')
                    {
                        if(gameMap[i+1][j]=='W'|| gameMap[i+1][j]=='D')
                        {
                            al_draw_scaled_bitmap(deswall2,0,0,32,48, j*PPC, i*PPC-64,64,96, 0);
                        }
                        al_draw_scaled_bitmap(deswall,0,0,32,48, j*PPC, i*PPC,64,96, 0);
                    }
                    else if(gameMap[i+1][j]=='W'|| gameMap[i+1][j]=='D')
                    {
                        al_draw_scaled_bitmap(deswall2,0,0,32,48, j*PPC, i*PPC-32,64,96, 0);
                    }
                    else
                    {
                        al_draw_scaled_bitmap(deswall,0,0,32,48, j*PPC, i*PPC-32,64,96, 0);
                    }
                    break;
                case 'K':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(caixa,0,0,32,48, j*PPC+16, i*PPC-32,32,48, 0);
                    break;
                case 'B':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(caixa,0,0,32,48, j*PPC+16, i*PPC-32,32,48, 0);
                    break;
                case 'b':
                    al_draw_scaled_bitmap(bomba,0,0,32,32, j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'E':
                    al_draw_scaled_bitmap(inimigo,0,0,32,32, j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'J':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(jogador, 0,0,32,32,j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'w':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(jogador, 128,32,32,32,j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'a':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(jogador, 0,0,32,32,j*PPC, i*PPC-32,PPC,PPC, ALLEGRO_FLIP_HORIZONTAL);
                    break;
                case 's':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(jogador, 0,32,32,32,j*PPC, i*PPC-32,PPC,PPC, 0);
                    break;
                case 'd':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(jogador, 0,0,32,32,j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'C':
                    al_draw_scaled_bitmap(blank,0,0,32,32, j*PPC, i*PPC,64,64, 0);
                    al_draw_scaled_bitmap(key,0,0,32,32,j*PPC, i*PPC-32,64,64, 0);
                    break;
                case 'Y':
                    al_draw_scaled_bitmap(jogador1, 0,0,32,32,j*PPC, i*PPC,64,64, 0);
                    break;
                case 't':
                    al_draw_scaled_bitmap(jogador1, 128,32,32,32,j*PPC, i*PPC,64,64, 0);
                    break;
                case 'f':
                    al_draw_scaled_bitmap(jogador1, 0,0,32,32,j*PPC, i*PPC,PPC,PPC, ALLEGRO_FLIP_HORIZONTAL);
                    break;
                case 'g':
                    al_draw_scaled_bitmap(jogador1, 0,32,32,32,j*PPC, i*PPC,PPC,PPC, 0);
                    break;
                case 'h':
                    al_draw_scaled_bitmap(jogador1, 0,0,32,32,j*PPC, i*PPC,64,64, 0);
                    break;
                case 'e':
                    al_draw_scaled_bitmap(explosion, 0,0,35,35,j*PPC-96, i*PPC-96,64*3,64*3, 0);
                    //gameMap[posybomba][posxbomba] = ' ';
                    break;
                }
            }
        }

    }
    al_set_target_bitmap(al_get_backbuffer(telaJogo));
}

void renderInfoAllegro(playerInfo* PlayerA, tamanhoJogo*tela,int map)
{
    // coloca informacoes no display
    int i;
    al_set_target_bitmap(al_get_backbuffer(telaJogo));
    color=al_map_rgb(195,195,195);
    al_draw_filled_rectangle(0,0,(*tela).largura,(*tela).altura/15,color);
    al_draw_filled_rectangle(0,(*tela).largura*25/60+(*tela).altura/15,(*tela).largura,(*tela).altura,color);
    al_draw_textf(fonte_menu_small,al_map_rgb(0,0,0),(*tela).largura/2,((*tela).altura/30)-(al_get_font_line_height(fonte_menu_small)/2), ALLEGRO_ALIGN_CENTRE, "Score:%d",(*PlayerA).score);
    if((*PlayerA).vidas>=3)
    {
        al_draw_scaled_bitmap(life,0,0,11,10,140,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
        al_draw_scaled_bitmap(life,0,0,11,10,44,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
        al_draw_scaled_bitmap(life,0,0,11,10,92,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
    }
    else if((*PlayerA).vidas==2)
    {
        al_draw_scaled_bitmap(life,0,0,11,10,92,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
        al_draw_scaled_bitmap(life,0,0,11,10,44,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
        al_draw_scaled_bitmap(life,11,0,11,10,140,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
    }
    else if((*PlayerA).vidas==1)
    {
        al_draw_scaled_bitmap(life,0,0,11,10,44,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
        al_draw_scaled_bitmap(life,11,0,11,10,140,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
        al_draw_scaled_bitmap(life,11,0,11,10,92,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
    }
    else if((*PlayerA).vidas==0)
    {
        al_draw_scaled_bitmap(life,11,0,11,10,44,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
        al_draw_scaled_bitmap(life,11,0,11,10,140,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
        al_draw_scaled_bitmap(life,11,0,11,10,92,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,44,40,0);
    }

    if(!map)
    {
        if((*PlayerA).numBombas>=3)
        {
            al_draw_scaled_bitmap(bomba,0,0,32,32,(*tela).largura-96,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,0,0,32,32,(*tela).largura-148,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,0,0,32,32,(*tela).largura-200,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
        }
        else if((*PlayerA).numBombas==2)
        {
            al_draw_scaled_bitmap(bomba,0,0,32,32,(*tela).largura-200,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,0,0,32,32,(*tela).largura-149,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,32,0,32,32,(*tela).largura-96,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
        }
        else if((*PlayerA).numBombas==1)
        {
            al_draw_scaled_bitmap(bomba,0,0,32,32,(*tela).largura-200,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,32,0,32,32,(*tela).largura-148,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,32,0,32,32,(*tela).largura-96,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
        }
        else if((*PlayerA).numBombas==0)
        {
            al_draw_scaled_bitmap(bomba,32,0,32,32,(*tela).largura-200,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,48,48,0);
            al_draw_scaled_bitmap(bomba,32,0,32,32,(*tela).largura-148,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,48,48,0);
            al_draw_scaled_bitmap(bomba,32,0,32,32,(*tela).largura-96,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,48,48,0);
        }
    }
    else
    {
        if((*PlayerA).numBombas>=3)
        {
            al_draw_scaled_bitmap(bomba,0,0,32,32,276,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,0,0,32,32,328,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,0,0,32,32,380,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
        }
        else if((*PlayerA).numBombas==2)
        {
            al_draw_scaled_bitmap(bomba,0,0,32,32,328,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,0,0,32,32,276,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,32,0,32,32,380,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
        }
        else if((*PlayerA).numBombas==1)
        {
            al_draw_scaled_bitmap(bomba,0,0,32,32,276,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,32,0,32,32,328,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
            al_draw_scaled_bitmap(bomba,32,0,32,32,380,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-24,48,48,0);
        }
        else if((*PlayerA).numBombas==0)
        {
            al_draw_scaled_bitmap(bomba,32,0,32,32,276,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,48,48,0);
            al_draw_scaled_bitmap(bomba,32,0,32,32,328,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,48,48,0);
            al_draw_scaled_bitmap(bomba,32,0,32,32,380,(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-20,48,48,0);
        }
        al_draw_scaled_bitmap(gamescreen,0,0,3840,1600,(*tela).largura-(((*tela).altura-((*tela).largura*25/60+(*tela).altura/15))*60/25)-48,((*tela).largura*25/60+(*tela).altura/15),((*tela).altura-((*tela).largura*25/60+(*tela).altura/15))*60/25,(*tela).altura-((*tela).largura*25/60+(*tela).altura/15),0);
    }
    if((*PlayerA).numChaves==5)
    {
        for(i=0; i<5; i++)
        {
            al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*i),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
        }
    }
    else if((*PlayerA).numChaves==4)
    {
        for(i=0; i<4; i++)
        {
            al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*i),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
        }
        al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*4),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
    }
    else if((*PlayerA).numChaves==3)
    {
        for(i=0; i<3; i++)
        {
            al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*i),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
        }
        al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*3),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
        al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*4),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
    }
    else if((*PlayerA).numChaves==2)
    {
        for(i=0; i<3; i++)
        {
            al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*(i+2)),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
        }
        al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*0),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
        al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*1),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
    }
    else if((*PlayerA).numChaves==1)
    {
        for(i=0; i<4; i++)
        {
            al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*(i+1)),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
        }
        al_draw_scaled_bitmap(key,0,0,32,32,(((*tela).largura)/2)-160+(64*0),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
    }
    else if((*PlayerA).numChaves==0)
    {
        for(i=0; i<5; i++)
        {
            al_draw_scaled_bitmap(key,32,0,32,32,(((*tela).largura)/2)-160+(64*i),(((*tela).largura*25/60+(*tela).altura/15)+(*tela).altura)/2-32,64,64,0);
        }
    }
    al_draw_filled_rectangle(0,(*tela).altura,al_get_display_width(telaJogo),al_get_display_height(telaJogo),al_map_rgb(0,0,0));
}

void renderMap(char gameMap[MAXLINHA][MAXCOLUNA], posEntidade* player, posEntidade enemy[])   //
{
    // renderiza mapa no cmd, é a renderizacao do jogo sem a interface gráfica
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
            else if (gameMap[i][j] == 'b')
            {
                printf("b");
            }
            else
                printf("%c",gameMap[i][j]);
        }
    }
}

void save_highscores(highscores* scores)
{
    FILE *fp;
    int i;
    fp = fopen("highscores.txt","w");

    for(i=0; i<10; i++)
    {
        fprintf(fp, "%s\n%d\n",(*scores).names[i],(*scores).scores[i]);
    }
    fclose(fp);
}

void saveGame(char gameMap[][MAXCOLUNA], playerInfo* PlayerA)
{
    // salva todas as informacoes convenientes, ou seja, mapa atual e a struct de informacoes do player
    FILE * fp;
    int i,j;
    char arquivo[50] = {"save1"};
    char pasta[50] = {"mapas/"};
    char info[60]= {"mapas/info"};
    char extensao[5] = {".txt"};

    strcat(arquivo, extensao);
    strcat(pasta,arquivo);
    system("cls");
    fp = fopen(pasta, "w");
    for(i=0; i<MAXLINHA; i++)
    {
        for(j=0; j<MAXCOLUNA; j++)
        {
            fprintf(fp, "%c", gameMap[i][j]);
            printf("%c",gameMap[i][j]);
        }
    }
    fclose(fp);
    strcat(info,arquivo);
    fflush(fp);
    fp = fopen(info, "w");
    fprintf(fp,"%d\n%d\n%d\n%d\n%d\n",(*PlayerA).level, (*PlayerA).vidas, (*PlayerA).numBombas, (*PlayerA).numChaves, (*PlayerA).score);
    fclose(fp);
}

void setBomb(char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA, char keyPressed, settings config)
{
    // funcao setBomb tem 2 propósitos: 1) preparar o mapa quando é carregado um novo jogo 2) posicionar uma bomba quando o caracter 'b' for pressionado, verificando também se é uma posição válida e armazenando informacoes necessarias
    int i,j,k;
    efeitoSonoro = al_load_sample("sons/setbomb2.wav");
    if((*PlayerA).numBombas <= 0) // se nao tiver bombas disponiveis, nada que se possa fazer, retorna
    {
        return;
    }
    if (keyPressed == 'b') // instrucao para verifcar se é possivel instalar nova bomba
    {
        switch(gameMap[(*player).posY][(*player).posX]) // para colocar a bomba, precisa avaliar se o square é valido, decrementar o numero de bombas total, verificar indice invalidos e dar um índice válido à nova bomba
        {
        case 'w':
            if (gameMap[(*player).posY - 1][(*player).posX] == ' ')
            {
                (*PlayerA).numBombas--;
                for (i=0; i<MAXBOMBAS; i++)
                {
                    if ((*PlayerA).posXBomba[i] == -1)
                    {
                        (*PlayerA).posXBomba[i] = (*player).posX;
                        (*PlayerA).posYBomba[i] = (*player).posY - 1;
                        (*PlayerA).tempoBomba[i] = clock();
                        break;
                    }
                }
                // matriz caracteres
                gameMap[(*player).posY - 1][(*player).posX] = 'b'; // e tambem coloca a informacao na matriz de caracteres
                gotoxy((*player).posX, (*player).posY - 1);
                printf("b");
                al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            break;
        case 'a':
            if (gameMap[(*player).posY][(*player).posX - 1] == ' ')
            {
                (*PlayerA).numBombas--;
                for (i=0; i<MAXBOMBAS; i++)
                {
                    if ((*PlayerA).posXBomba[i] == -1)
                    {
                        (*PlayerA).posXBomba[i] = (*player).posX - 1;
                        (*PlayerA).posYBomba[i] = (*player).posY;
                        (*PlayerA).tempoBomba[i] = clock();
                        break;
                    }
                }
                gameMap[(*player).posY][(*player).posX - 1] = 'b';
                gotoxy((*player).posX - 1, (*player).posY);
                printf("b");
                al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            break;
        case 's':
            if (gameMap[(*player).posY + 1][(*player).posX] == ' ')
            {
                (*PlayerA).numBombas--;
                for (i=0; i<MAXBOMBAS; i++)
                {
                    if ((*PlayerA).posXBomba[i] == -1)
                    {
                        (*PlayerA).posXBomba[i] = (*player).posX;
                        (*PlayerA).posYBomba[i] = (*player).posY + 1;
                        (*PlayerA).tempoBomba[i] = clock();
                        break;
                    }
                }
                gameMap[(*player).posY + 1][(*player).posX] = 'b';
                gotoxy((*player).posX, (*player).posY + 1);
                printf("b");
                al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            break;
        case 'd':
            if (gameMap[(*player).posY][(*player).posX + 1] == ' ')
            {
                (*PlayerA).numBombas--;
                for (i=0; i<MAXBOMBAS; i++)
                {
                    if ((*PlayerA).posXBomba[i] == -1)
                    {
                        (*PlayerA).posXBomba[i] = (*player).posX + 1;
                        (*PlayerA).posYBomba[i] = (*player).posY;
                        (*PlayerA).tempoBomba[i] = clock();
                        break;
                    }
                }
                gameMap[(*player).posY][(*player).posX + 1] = 'b';
                gotoxy((*player).posX + 1, (*player).posY);
                printf("b");
                al_play_sample(efeitoSonoro,(config).vols, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            break;
        default:
            return;
        }
        al_flip_display();
    }
    else if (keyPressed == 'l') // instrucao de validar bombas quando carregar um novo mapa
    {
        k = 0;
        int x = 0;
        for(i=0; i<MAXBOMBAS; i++) // invalida todas as bombas
        {
            (*PlayerA).posYBomba[i] = -1;
            (*PlayerA).posXBomba[i] = -1;
        }
        for(i=0; i<MAXLINHA; i++) // analisa toda a matriz de caracteres, se houver um caracter 'b', atribui indice, posicao e inicia o clock
        {
            for(j=0; j<MAXCOLUNA; j++)
            {
                if (gameMap[i][j] == 'b')
                {
                    (*PlayerA).posXBomba[k] = j;
                    (*PlayerA).posYBomba[k] = i;
                    (*PlayerA).tempoBomba[k] = clock();
                    k++;
                }
            }
        }
    }
}

void setBomb2(char gameMap[][MAXCOLUNA], posEntidade* playerb, playerInfo* PlayerB, char keyPressed, settings config)
{
    int i,j,k;
    efeitoSonoro = al_load_sample("sons/setbomb2.wav");
    if((*PlayerB).numBombas <= 0)
    {
        return;
    }
    if (keyPressed == 'b')
    {
        switch(gameMap[(*playerb).posY][(*playerb).posX])
        {
        case 't':
            if (gameMap[(*playerb).posY - 1][(*playerb).posX] == ' ')
            {
                (*PlayerB).numBombas--;
                for (i=0; i<MAXBOMBAS; i++)
                {
                    if ((*PlayerB).posXBomba[i] == -1)
                    {
                        (*PlayerB).posXBomba[i] = (*playerb).posX;
                        (*PlayerB).posYBomba[i] = (*playerb).posY - 1;
                        (*PlayerB).tempoBomba[i] = clock();
                        break;
                    }
                }
                // matriz caracteres
                gameMap[(*playerb).posY - 1][(*playerb).posX] = 'b';
                gotoxy((*playerb).posX, (*playerb).posY - 1);
                printf("b");
                al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            break;
        case 'f':
            if (gameMap[(*playerb).posY][(*playerb).posX - 1] == ' ')
            {
                (*PlayerB).numBombas--;
                for (i=0; i<MAXBOMBAS; i++)
                {
                    if ((*PlayerB).posXBomba[i] == -1)
                    {
                        (*PlayerB).posXBomba[i] = (*playerb).posX - 1;
                        (*PlayerB).posYBomba[i] = (*playerb).posY;
                        (*PlayerB).tempoBomba[i] = clock();
                        break;
                    }
                }
                gameMap[(*playerb).posY][(*playerb).posX - 1] = 'b';
                gotoxy((*playerb).posX - 1, (*playerb).posY);
                printf("b");
                al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            break;
        case 'g':
            if (gameMap[(*playerb).posY + 1][(*playerb).posX] == ' ')
            {
                (*PlayerB).numBombas--;
                for (i=0; i<MAXBOMBAS; i++)
                {
                    if ((*PlayerB).posXBomba[i] == -1)
                    {
                        (*PlayerB).posXBomba[i] = (*playerb).posX;
                        (*PlayerB).posYBomba[i] = (*playerb).posY + 1;
                        (*PlayerB).tempoBomba[i] = clock();
                        break;
                    }
                }
                gameMap[(*playerb).posY + 1][(*playerb).posX] = 'b';
                gotoxy((*playerb).posX, (*playerb).posY + 1);
                printf("b");
                al_play_sample(efeitoSonoro, (config).vols, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            break;
        case 'h':
            if (gameMap[(*playerb).posY][(*playerb).posX + 1] == ' ')
            {
                (*PlayerB).numBombas--;
                for (i=0; i<MAXBOMBAS; i++)
                {
                    if ((*PlayerB).posXBomba[i] == -1)
                    {
                        (*PlayerB).posXBomba[i] = (*playerb).posX + 1;
                        (*PlayerB).posYBomba[i] = (*playerb).posY;
                        (*PlayerB).tempoBomba[i] = clock();
                        break;
                    }
                }
                gameMap[(*playerb).posY][(*playerb).posX + 1] = 'b';
                gotoxy((*playerb).posX + 1, (*playerb).posY);
                printf("b");
                al_play_sample(efeitoSonoro,(config).vols, 0.0, 1.2, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
            break;
        default:
            return;
        }
        al_flip_display();
    }
    else if (keyPressed == 'l')
    {
        k = 0;
        int x = 0;
        for(i=0; i<MAXBOMBAS; i++)
        {
            (*PlayerB).posYBomba[i] = -1;
            (*PlayerB).posXBomba[i] = -1;
        }
        for(i=0; i<MAXLINHA; i++)
        {
            for(j=0; j<MAXCOLUNA; j++)
            {
                if (gameMap[i][j] == 'b')
                {
                    (*PlayerB).posXBomba[k] = j;
                    (*PlayerB).posYBomba[k] = i;
                    (*PlayerB).tempoBomba[k] = clock();
                    k++;
                }
            }
        }
    }
}

void show_highscores(tamanhoJogo* tela,highscores* scores,int* estado_menu)
{
    int i;
    while(true)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            *estado_menu=MAIN_MENU;
            break;
        }
        if(al_is_event_queue_empty(fila_eventos))
        {
            for(i=0; i<4; i++)
            {
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,0,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(deswall,0,0,32,32,i*(*tela).largura/3,((*tela).altura/3.5)+(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
            }
            al_draw_textf(fonte_menu, al_map_rgb(0,0,0), (*tela).largura/2,50, ALLEGRO_ALIGN_CENTER, "HIGHSCORES");
            al_draw_textf(fonte_menu, al_map_rgb(0,0,0), 200,(*tela).altura/8, ALLEGRO_ALIGN_LEFT, "NAME");
            al_draw_textf(fonte_menu, al_map_rgb(0,0,0), (*tela).largura-200,(*tela).altura/8, ALLEGRO_ALIGN_RIGHT, "score");
            for(i=0; i<10; i++)
            {
                al_draw_textf(fonte_menu, al_map_rgb(0,0,0), 200,((50*i)+205), ALLEGRO_ALIGN_LEFT, (*scores).names[i]);
                al_draw_textf(fonte_menu, al_map_rgb(0,0,0),  (*tela).largura-200,((50*i)+205), ALLEGRO_ALIGN_RIGHT, "%d",(*scores).scores[i]);
            }

            al_draw_filled_rectangle(0,(*tela).altura,al_get_display_width(telaJogo),al_get_display_height(telaJogo),al_map_rgb(0,0,0));
            al_flip_display();
        }
    }
}

void showPos(posEntidade* player, posEntidade enemy[], playerInfo* PlayerA)
{
    // mostra informacoes do jogador no cmd
    int i;
    gotoxy(0,26);
    printf("pos player = [%d][%d]\n",(*player).posX,(*player).posY);
    for(i=0; i<5; i++)
    {
        printf("pos enemy[%d] = [%d][%d]\n",i,enemy[i].posX,enemy[i].posY);
    }
    for(i=0; i<MAXBOMBAS; i++ )
    {
        if ((*PlayerA).posXBomba[i] >= 0 && (*PlayerA).posXBomba[i] <= 59 && (*PlayerA).posYBomba[i] >= 0 && (*PlayerA).posYBomba[i] <= 24)
        {
            printf("posBomba[%d] X = %d   Y = %d\n", i, (*PlayerA).posXBomba[i], (*PlayerA).posYBomba[i]);
        }
    }
    gotoxy (20, 25);
    printf("Level = %d  Vidas = %d  NumBombas = %d  NumChaves = %d  Score = %d\n",(*PlayerA).level,(*PlayerA).vidas,(*PlayerA).numBombas, (*PlayerA).numChaves,(*PlayerA).score);
}

void submenuchar(int *estado_menu,int *char_escolhido,tamanhoJogo* tela )
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
    while(*estado_menu ==3)
    {
        if (*char_escolhido == 1)
        {
            jogador=player1;
        }
        else if (*char_escolhido == 2)
        {
            jogador=player2;
        }
        else if (*char_escolhido == 3)
        {
            jogador=player3;
        }
        else if (*char_escolhido == 4)
        {
            jogador=player4;
        }
        else if (*char_escolhido == 5)
        {
            jogador=player5;
        }
        else if (*char_escolhido == 6)
        {
            jogador=player6;
        }
        else if (*char_escolhido == 7)
        {
            jogador=player6;
        }
        else if (*char_escolhido == 9)
        {
            jogador=player8;
        }
        if (imenu == 0)
        {
            jogador2=player1;
        }
        else if (imenu == 1)
        {
            jogador2=player2;
        }
        else if (imenu == 2)
        {
            jogador2=player3;
        }
        else if (imenu == 3)
        {
            jogador2=player4;
        }
        else if (imenu == 4)
        {
            jogador2=player5;
        }
        else if (imenu == 5)
        {
            jogador2=player6;
        }
        else if (imenu == 6)
        {
            jogador2=jogador;
        }
        else if (imenu == 7)
        {
            jogador2=jogador;
        }
        else if (imenu == 8)
        {
            jogador2=player8;
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
            for(i=0; i<4; i++)
            {
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,0,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(deswall,0,0,32,32,i*(*tela).largura/3,(*tela).altura/2+(*tela).largura/6,(*tela).largura/3,(*tela).largura/3,0);
            }
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
                al_draw_textf(fonte_menu, color,(*tela).largura*4/6,(50*i+150), ALLEGRO_ALIGN_CENTRE, submenu_characters[i]);

            }
            if(imenu==8)
            {
                color=al_map_rgb(255,255,255);
                al_draw_textf(fonte_menu, color, 900,550, ALLEGRO_ALIGN_CENTRE, submenu_characters[8]);
            }
            if(estado_menu!=0)
            {
                al_draw_scaled_bitmap(jogador2,coluna_sprite,linha_sprite,32,32,(*tela).largura/6,(*tela).altura/2-(*tela).largura/6,(*tela).largura/3,(*tela).largura/3,0);
            }
            else if(char_escolhido!=0)
            {
                al_draw_scaled_bitmap(jogador2,coluna_sprite,linha_sprite,32,32,(*tela).largura/6,(*tela).altura/2-(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
            }
            al_draw_filled_rectangle(0,(*tela).altura,al_get_display_width(telaJogo),al_get_display_height(telaJogo),al_map_rgb(0,0,0));
            al_draw_filled_rectangle((*tela).largura,0,al_get_display_width(telaJogo),al_get_display_height(telaJogo),al_map_rgb(0,0,0));
            al_flip_display();
            desenha = 0;
        }
    }
}

void submenulevels(int *estado_menu, int *level_escolhido, int *load_level,char arquivo[],tamanhoJogo* tela)
{
    char submenu_levels[13][20]= {"LEVEL 01","LEVEL 02","LEVEL 03","LEVEL 04","LEVEL 05","LEVEL 06","LEVEL 07","LEVEL 08","LEVEL 09","LEVEL 10","LOAD MAP","LOAD LAST SAVE","BACK TO MAIN MENU"}; //matriz menu de niveis
    ALLEGRO_EVENT evento;
    int i;
    int cont_tempo=0;
    int desenha=0;
    int imenu=0;
    char level0[50]= {"mapa0"};
    char level1[50]= {"mapa1"};
    char level2[50]= {"mapa2"};
    char level3[50]= {"mapa3"};
    char level4[50]= {"mapa4"};
    char level5[50]= {"mapa5"};
    char level6[50]= {"mapa6"};
    char level7[50]= {"mapa7"};
    char level8[50]= {"mapa8"};
    char level9[50]= {"mapa9"};
    char save1[50]= {"save1"};
    char load[50]= {0};


    while(*estado_menu ==1)
    {
        al_wait_for_event(fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)//se tecla foi pressionada
        {
            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE||evento.keyboard.keycode == ALLEGRO_KEY_ENTER)// se tecla pressionada foi espaço ou enter
            {
                printf("space||enter");
                if (imenu == 0)//level 1
                {
                    *level_escolhido=1;
                    strcat(arquivo,level0);
                    break;
                }
                else if (imenu == 1)
                {
                    *level_escolhido=2;
                    strcat(arquivo,level1);
                    break;
                }
                else if (imenu == 2)
                {
                    *level_escolhido=3;
                    strcat(arquivo,level2);
                    break;
                }
                else if (imenu == 3)
                {
                    *level_escolhido=4;
                    strcat(arquivo,level3);
                    break;
                }
                else if (imenu == 4)
                {
                    *level_escolhido=5;
                    strcat(arquivo,level4);
                    break;
                }
                else if (imenu == 5)
                {
                    *level_escolhido=6;
                    strcat(arquivo,level5);
                    break;
                }
                else if (imenu == 6)
                {
                    *level_escolhido=7;
                    strcat(arquivo,level6);
                    break;
                }
                else if (imenu == 7)
                {
                    *level_escolhido=8;
                    strcat(arquivo,level7);
                    break;
                }
                else if (imenu == 8)
                {
                    *level_escolhido=9;
                    strcat(arquivo,level8);
                    break;
                }
                else if (imenu == 9)
                {
                    *level_escolhido=10;
                    strcat(arquivo,level9);
                    break;
                }
                else if (imenu == 10)
                {
                    *load_level=1;
                    ler_entrada_allegro(tela,load,'a');
                    strcat(arquivo,load);
                    break;
                }
                else if (imenu == 11)
                {
                    *load_level=2;
                    strcat(arquivo,save1);
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
        if(al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            for(i=0; i<4; i++)
            {
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,0,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(deswall,0,0,32,32,i*(*tela).largura/3,((*tela).altura/3.5)+(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
            }
            al_draw_scaled_bitmap(jogador,0,0,32,32,(*tela).largura/4,(*tela).altura/3.5,(*tela).largura/3,(*tela).largura/3,0);

            al_draw_filled_rectangle(0,(*tela).altura,al_get_display_width(telaJogo),al_get_display_height(telaJogo),al_map_rgb(0,0,0));
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

void submenusettings(int *estado_menu,tamanhoJogo* tela,settings* config)
{
    FILE *fp;

    char submenu_settings[8][50]= {"SOUNDS","MUSIC","MINI MAP","GAME MODE","INFINITE BOMBS","CREDITS","SAVE SETTINGS","BACK TO MAIN MENU"};
    char on_off[2][5]= {"OFF","ON"};
    char percent[11][5]= {"off","x0.1","x0.2","x0.3","x0.4","x0.5","x0.6","x0.7","x0.8","x0.9","MAX"};
    char game_screen_mode[2][50]= {"FULLWINDOW","ZOOM 2x"};
    int i;
    int cont_tempo=0;
    int desenha=0;
    int settings=0;
    int item_menu=0;
    int item_menu1=((*config).vols*10);
    int item_menu2=((*config).volm*10);
    while(*estado_menu ==4)
    {

        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)//se tecla foi pressionada
        {
            if (evento.keyboard.keycode == ALLEGRO_KEY_ENTER)// se tecla pressionada foi espaço ou enter
            {
                if(item_menu==3)
                {
                    settings=4;
                }
                else if (item_menu == 5)
                {
                    credits(tela);
                }
                else if (item_menu == 6)
                {
                    (*config).vols=((float)item_menu1/10);
                    (*config).volm=((float)item_menu2/10);
                    fp = fopen("config.txt","w");
                    fprintf(fp, "%f\n%f\n%d\n%d\n%d",(*config).vols,(*config).volm,(*config).map,(*config).gameMode,(*config).bombMode);
                    fclose(fp);
                }
                else if (item_menu == 7)
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
                    item_menu1--;
                    if (item_menu1<0)
                        item_menu1=0;

                }
                else if (item_menu == 1)
                {
                    item_menu2--;
                    if (item_menu2<0)
                        item_menu2=0;
                }
                else if (item_menu == 2)
                {
                    (*config).map=0;
                }
                else if (item_menu == 3)
                {

                    (*config).gameMode=0;

                }
                else if (item_menu == 4)
                {

                    (*config).bombMode=0;

                }

            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT || evento.keyboard.keycode == ALLEGRO_KEY_D)// se tecla pressionada foi espaço ou enter
            {
                printf("space||enter");

                if (item_menu == 0)
                {
                    item_menu1++;
                    if (item_menu1>=10)
                        item_menu1=10;
                }
                else if (item_menu == 1)
                {
                    item_menu2++;
                    if (item_menu2>=10)
                        item_menu2=10;
                }
                else if (item_menu == 2)
                {
                    (*config).map=1;
                }
                else if (item_menu == 3)
                {

                    (*config).gameMode=1;
                }
                else if (item_menu == 4)
                {

                    (*config).bombMode=1;

                }

            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W)//se tecla pressionada foi w ou seta
            {
                item_menu--;
                printf("key up||w\n");
                if (item_menu<0) //se apertar para cima no primeiro item, seleciona o ultimo
                    item_menu =7;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)//se tecla pressionada foi s ou seta
            {
                printf("key down||s\n");
                item_menu++;
                if (item_menu==8) //se apertar para cima no primeiro item, seleciona o ultimo
                    item_menu=0;
            }
        }

        if(al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            for(i=0; i<4; i++)
            {
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,0,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(blank,0,0,32,32,i*(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
                al_draw_scaled_bitmap(deswall,0,0,32,32,i*(*tela).largura/3,((*tela).altura/3.5)+(*tela).largura/3,(*tela).largura/3,(*tela).largura/3,0);
            }
            al_draw_scaled_bitmap(jogador,0,0,32,32,(*tela).largura-(*tela).largura/3-(*tela).largura/30,((*tela).altura/3.5)+(*tela).largura/3-(*tela).largura/4,(*tela).largura/4,(*tela).largura/4,ALLEGRO_FLIP_HORIZONTAL);
            al_draw_filled_rectangle(0,(*tela).altura,al_get_display_width(telaJogo),al_get_display_height(telaJogo),al_map_rgb(0,0,0));
            for(i=0; i<8; i++)
            {
                if(i==item_menu)
                {
                    color=al_map_rgb(255,255,255);
                    al_draw_textf(fonte_menu, color, 100,((50*i)+200), ALLEGRO_ALIGN_LEFT, submenu_settings[i]);
                    if(item_menu==3)
                        al_draw_textf(fonte_menu_small, color, 1000,355, ALLEGRO_ALIGN_CENTER, game_screen_mode[(*config).gameMode]);
                    if(item_menu==2)
                        al_draw_textf(fonte_menu_small, color, 1000,305, ALLEGRO_ALIGN_CENTER, on_off[(*config).map]);
                    if(item_menu==0)
                        al_draw_text(fonte_menu_small, color, 1000,205, ALLEGRO_ALIGN_CENTER, percent[item_menu1]);
                    if(item_menu==1)
                        al_draw_text(fonte_menu_small, color, 1000,255, ALLEGRO_ALIGN_CENTER, percent[item_menu2]);
                    if(item_menu==4)
                        al_draw_textf(fonte_menu_small, color, 1000,405, ALLEGRO_ALIGN_CENTER, on_off[(*config).bombMode]);

                }
                else
                {
                    color=al_map_rgb(0,0,0);
                    al_draw_textf(fonte_menu_small, color, 100,((50*i)+205), ALLEGRO_ALIGN_LEFT, submenu_settings[i]);
                    if(item_menu!=3)
                        al_draw_textf(fonte_menu_small, color, 1000,355, ALLEGRO_ALIGN_CENTER, game_screen_mode[(*config).gameMode]);
                    if(item_menu!=2)
                        al_draw_textf(fonte_menu_small, color, 1000,305, ALLEGRO_ALIGN_CENTER, on_off[(*config).map]);
                    if(item_menu!=0)
                        al_draw_text(fonte_menu_small, color, 1000,205, ALLEGRO_ALIGN_CENTER, percent[item_menu1]);
                    if(item_menu!=1)
                        al_draw_text(fonte_menu_small, color, 1000,255, ALLEGRO_ALIGN_CENTER, percent[item_menu2]);
                    if(item_menu!=4)
                        al_draw_textf(fonte_menu_small, color, 1000,405, ALLEGRO_ALIGN_CENTER, on_off[(*config).bombMode]);

                }


            }
            color=al_map_rgb(0,0,0);
            al_flip_display();
            desenha = 0;
        }
    }
}

void takeDmg(playerInfo* PlayerA, settings config)
{
    // essa funcao é chamada quando o player toma dano, é decrementado uma vida, retirados 100 pontos e ativa um efeito sonoro
    efeitoSonoro = al_load_sample("sons/classic_hurt.wav");
    (*PlayerA).vidas--;
    (*PlayerA).score = (*PlayerA).score - 100;
    if ((*PlayerA).score < 0)
    {
        (*PlayerA).score = 0;
    }
    if((*PlayerA).vidas >= 1)
    {
        al_play_sample(efeitoSonoro, (config).vols*1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}

void takeDmg2(playerInfo* PlayerB, settings config)
{
    efeitoSonoro = al_load_sample("sons/classic_hurt.wav");
    (*PlayerB).vidas--;
    (*PlayerB).score = (*PlayerB).score - 100;
    if ((*PlayerB).score < 0)
    {
        (*PlayerB).score = 0;
    }
    if((*PlayerB).vidas >= 1)
    {
        al_play_sample(efeitoSonoro, (config).vols*1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}

void updateEnemies(char gameMap[][MAXCOLUNA],posEntidade* player, posEntidade enemy[], char instrucao, playerInfo* PlayerA, settings config)
{
    // funcao com 2 instrucoes
    // 1) instrucao 'l' reconhecer os inimigos, validar a primeira direcao que eles vao andar e dar o comando para isso
    // 2) instrucao 'u' dar update na posicao dos inimigos, seguindo um certo comportamento definido aqui mesmo
    int i, valido, deltax, deltay;
    static int direcao[5];
    static int contador;

    if (instrucao == 'u')  // se a funcao for chamada por update de fps
    {
        if(contador > 0 && contador < 15)  // movimentacao aleatoria nos primeiros 15 movimentos
        {

            for(i=0; i<5; i++)
            {
                if (enemy[i].posX == -1)
                    continue;
                if (contador % 5 == 0)
                    direcao[i] = rand() % 4;
                switch(direcao[i])
                {
                case 0:
                    if (gameMap[enemy[i].posY - 1][enemy[i].posX] == 'w' || gameMap[enemy[i].posY - 1][enemy[i].posX] == 'a' || gameMap[enemy[i].posY - 1][enemy[i].posX] == 's' || gameMap[enemy[i].posY - 1][enemy[i].posX] == 'd')
                    {
                        takeDmg(PlayerA,config);
                        continue;
                    }
                    else if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ')
                    {
                        direcao[i] = 0;
                    }
                    else
                    {
                        if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY][enemy[i].posX + 1] != ' ' && gameMap[enemy[i].posY][enemy[i].posX - 1] != ' ')
                            continue;
                        valido = 0;
                        do
                        {
                            direcao[i] = rand() % 4;
                            switch(direcao[i])
                            {
                            case 0: //w
                                if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 1: //a
                                if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 2: //s
                                if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 3: //d
                                if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            }
                        }
                        while(valido != 1);
                    }
                    break;
                case 1:
                    if (gameMap[enemy[i].posY][enemy[i].posX - 1] == 'w' || gameMap[enemy[i].posY][enemy[i].posX - 1] == 'a' || gameMap[enemy[i].posY][enemy[i].posX - 1] == 's' || gameMap[enemy[i].posY][enemy[i].posX - 1] == 'd')
                    {
                        takeDmg(PlayerA,config);
                        continue;
                    }
                    else if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ')
                    {
                        direcao[i] = 1;
                    }
                    else
                    {
                        if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY][enemy[i].posX + 1] != ' ' && gameMap[enemy[i].posY][enemy[i].posX - 1] != ' ')
                            continue;
                        valido = 0;
                        do
                        {
                            direcao[i] = rand() % 4;
                            switch(direcao[i])
                            {
                            case 0: //w
                                if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 1: //a
                                if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 2: //s
                                if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 3: //d
                                if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            }
                        }
                        while(valido != 1);
                    }
                    break;
                case 2:
                    if (gameMap[enemy[i].posY + 1][enemy[i].posX] == 'w' || gameMap[enemy[i].posY + 1][enemy[i].posX] == 'a' || gameMap[enemy[i].posY + 1][enemy[i].posX] == 's' || gameMap[enemy[i].posY + 1][enemy[i].posX] == 'd')
                    {
                        takeDmg(PlayerA,config);
                        continue;
                    }
                    else if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ')
                    {
                        direcao[i] = 2;
                    }
                    else
                    {
                        if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY][enemy[i].posX + 1] != ' ' && gameMap[enemy[i].posY][enemy[i].posX - 1] != ' ')
                            continue;
                        valido = 0;
                        do
                        {
                            direcao[i] = rand() % 4;
                            switch(direcao[i])
                            {
                            case 0: //w
                                if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 1: //a
                                if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 2: //s
                                if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 3: //d
                                if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            }
                        }
                        while(valido != 1);
                    }
                    break;
                case 3:
                    if (gameMap[enemy[i].posY][enemy[i].posX + 1] == 'w' || gameMap[enemy[i].posY][enemy[i].posX + 1] == 'a' || gameMap[enemy[i].posY][enemy[i].posX + 1] == 's' || gameMap[enemy[i].posY][enemy[i].posX + 1] == 'd')
                    {
                        takeDmg(PlayerA,config);
                        continue;
                    }
                    else if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ')
                    {
                        direcao[i] = 3;
                    }
                    else
                    {
                        valido = 0;
                        do
                        {
                            direcao[i] = rand() % 4;
                            switch(direcao[i])
                            {
                            case 0: //w
                                if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 1: //a
                                if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 2: //s
                                if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 3: //d
                                if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            }
                        }
                        while(valido != 1);
                    }
                    break;
                }
                moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
            }
        }


        else if(contador >= 15 && contador < 30)  // 15 movimentos que o inimigo verifica se esta a esquerda ou direita do player >> tenta igualar sua posicao em x >> se em x for igual, tenta igualar em y
        {
            for(i=0; i<5; i++)
            {
                if (enemy[i].posX == -1)
                    continue;
                deltax = (*player).posX - enemy[i].posX;
                deltay = (*player).posY - enemy[i].posY;
                //if (gameMap[enemy[i].posY][enemy[i].posX] == ' ')
                // if (deltax > deltay){

                if (deltax < 0)
                {
                    if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ')
                    {
                        direcao[i] = 1;
                    }
                    else if (enemy[i].posY == (*player).posY && enemy[i].posX - 1 == (*player).posX)
                    {
                        takeDmg(PlayerA,config);
                        continue;
                    }
                    else
                    {
                        if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY][enemy[i].posX + 1] != ' ' && gameMap[enemy[i].posY][enemy[i].posX - 1] != ' ')
                            continue;
                        valido = 0;
                        do
                        {
                            direcao[i] = (rand() % 4);
                            switch(direcao[i])
                            {
                            case 0: //w
                                if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 1: //a
                                if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 2: //s
                                if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 3: //d
                                if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            }
                        }
                        while (valido != 1);
                    }
                }

                else if (deltax > 0)
                {
                    if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ')
                    {
                        direcao[i] = 3;
                    }
                    else if (enemy[i].posY == (*player).posY && enemy[i].posX + 1 == (*player).posX)
                    {
                        takeDmg(PlayerA,config);
                        continue;
                    }
                    else
                    {
                        if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY][enemy[i].posX + 1] != ' ' && gameMap[enemy[i].posY][enemy[i].posX - 1] != ' ')
                            continue;
                        valido = 0;
                        do
                        {
                            direcao[i] = (rand() % 4);
                            switch(direcao[i])
                            {
                            case 0: //w
                                if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 1: //a
                                if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 2: //s
                                if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            case 3: //d
                                if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ')
                                {
                                    valido = 1;
                                }
                                else
                                    continue;
                                break;
                            }
                        }
                        while (valido != 1);
                    }
                }

                else if (deltax == 0)
                {
                    if (deltay > 0)
                    {
                        if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ')
                        {
                            direcao[i] = 2;
                        }
                        else if (enemy[i].posY + 1 == (*player).posY)
                        {
                            takeDmg(PlayerA,config);
                            continue;
                        }
                        else
                        {
                            if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY][enemy[i].posX + 1] != ' ' && gameMap[enemy[i].posY][enemy[i].posX - 1] != ' ')
                                continue;
                            valido = 0;
                            do
                            {
                                direcao[i] = (rand() % 4);
                                switch(direcao[i])
                                {
                                case 0: //w
                                    if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ')
                                    {
                                        valido = 1;
                                    }
                                    else
                                        continue;
                                    break;
                                case 1: //a
                                    if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ')
                                    {
                                        valido = 1;
                                    }
                                    else
                                        continue;
                                    break;
                                case 2: //s
                                    if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ')
                                    {
                                        valido = 1;
                                    }
                                    else
                                        continue;
                                    break;
                                case 3: //d
                                    if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ')
                                    {
                                        valido = 1;
                                    }
                                    else
                                        continue;
                                    break;
                                }
                            }
                            while (valido != 1);
                        }
                    }
                    else if (deltay < 0)
                    {
                        if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ')
                        {
                            direcao[i] = 0;
                        }
                        else if (enemy[i].posY - 1 == (*player).posY)
                        {
                            takeDmg(PlayerA,config);
                            continue;
                        }
                        else
                        {
                            if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY - 1][enemy[i].posX] != ' ' && gameMap[enemy[i].posY][enemy[i].posX + 1] != ' ' && gameMap[enemy[i].posY][enemy[i].posX - 1] != ' ')
                                continue;
                            valido = 0;
                            do
                            {
                                direcao[i] = (rand() % 4);
                                switch(direcao[i])
                                {
                                case 0: //w
                                    if (gameMap[enemy[i].posY - 1][enemy[i].posX] == ' ')
                                    {
                                        valido = 1;
                                    }
                                    else
                                        continue;
                                    break;
                                case 1: //a
                                    if (gameMap[enemy[i].posY][enemy[i].posX - 1] == ' ')
                                    {
                                        valido = 1;
                                    }
                                    else
                                        continue;
                                    break;
                                case 2: //s
                                    if (gameMap[enemy[i].posY + 1][enemy[i].posX] == ' ')
                                    {
                                        valido = 1;
                                    }
                                    else
                                        continue;
                                    break;
                                case 3: //d
                                    if (gameMap[enemy[i].posY][enemy[i].posX + 1] == ' ')
                                    {
                                        valido = 1;
                                    }
                                    else
                                        continue;
                                    break;
                                }
                            }
                            while (valido != 1);
                        }
                    }
                }
                //}
                moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
            }
        }
        else if (contador >=30 && contador < 45)  // local onde poderia ser colocado mais um comportamento do inimigo, mas zera o contador, reiniciando o comportamento para aleatorio
        {
            contador = 0;
            for(i=0; i<5; i++)
            {

            }
        }
        contador++;

    }
    else if (instrucao == 'l')  // se a funcao for chamada pelo loadgame, escolhe as direcoes que os inimigos vao andar
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

void updateGame(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int* cont_tempo, settings config)
{
    // vai verificar constantemente o tempo das bombas e chamar a funcao de explodir quando necessario, conferir se o score do jogador é valido e definir a velocidade de atualização da posição dos inimigos
    int i;
    //gotoxy (0,33);
    //printf("tempo no updategame = %d",tempo);
    for(i=0; i<MAXBOMBAS; i++)
    {
        if((*PlayerA).posXBomba[i] == -1)
            continue;
        if (clock() >= (*PlayerA).tempoBomba[i] + 2500)
        {
            explodeBomb(gameMap, player, enemy, PlayerA, i,config);
        }
    }

    if ((*PlayerA).score < 0)
    {
        (*PlayerA).score = 0;
    }
    if (*cont_tempo >=  (1 / ( (*PlayerA).level)) *10) // controla a velocidade de movimento dos inimigos
    {
        updateEnemies(gameMap,player, enemy, 'u', PlayerA,config);
        *cont_tempo = 0;
    }

}

void updateGame2(char gameMap[][MAXCOLUNA], posEntidade* playerb, posEntidade enemy[], playerInfo* PlayerB, settings config)
{
    int i;
    for(i=0; i<MAXBOMBAS; i++)
    {
        if((*PlayerB).posXBomba[i] == -1)
            continue;
        if (clock() >= (*PlayerB).tempoBomba[i] + 2500)
        {
            explodeBomb2(gameMap, playerb, enemy, PlayerB, i,config);
        }
    }
    if ((*PlayerB).score < 0)
    {
        (*PlayerB).score = 0;
    }
}

void updatePos(char gameMap[][MAXCOLUNA], posEntidade* player, char keyPressed, playerInfo* PlayerA, settings config)
{
    // quando a tecla pressionada for uma tecla direcional, entra nessa funcao, ela verifica se o proximo espaço é valido e rotaciona o personagem, realiza tambem a mudança em gameMap e no cmd
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
            (*player).posY--;
            gotoxy((*player).posX,(*player).posY);
            printf("w");
        }
        else if (gameMap[(*player).posY - 1][(*player).posX] == 'E')
        {
            takeDmg(PlayerA,config); // andar em cima de inimigo
            gotoxy((*player).posX,(*player).posY);
            printf("w");
        }
        else
        {
            gotoxy((*player).posX,(*player).posY);
            printf("w");
        }
        break;

    case 'a':
        if (gameMap[(*player).posY][(*player).posX - 1] == ' ')
        {
            gameMap[(*player).posY][(*player).posX] = ' ';
            gotoxy((*player).posX,(*player).posY);
            printf(" ");
            (*player).posX--;
            gotoxy((*player).posX,(*player).posY);
            printf("a");
        }
        else if (gameMap[(*player).posY][(*player).posX - 1] == 'E')
        {
            takeDmg(PlayerA,config);
            gotoxy((*player).posX,(*player).posY);
            printf("a");
        }

        else
        {
            gotoxy((*player).posX,(*player).posY);
            printf("a");
        }
        break;

    case 's':
        if (gameMap[(*player).posY + 1][(*player).posX] == ' ')
        {
            gameMap[(*player).posY][(*player).posX] = ' ';
            gotoxy((*player).posX,(*player).posY);
            printf(" ");
            (*player).posY++;
            gotoxy((*player).posX,(*player).posY);
            printf("s");
        }
        else if (gameMap[(*player).posY + 1][(*player).posX] == 'E')
        {
            takeDmg(PlayerA,config);
            gotoxy((*player).posX,(*player).posY);
            printf("s");
        }
        else
        {
            gotoxy((*player).posX,(*player).posY);
            printf("s");
        }
        break;

    case 'd':
        if(gameMap[(*player).posY][(*player).posX + 1] == ' ')
        {
            gameMap[(*player).posY][(*player).posX] = ' ';
            gotoxy((*player).posX,(*player).posY);
            printf(" ");
            (*player).posX++;
            gotoxy((*player).posX,(*player).posY);
            printf("d");
        }
        else if (gameMap[(*player).posY][(*player).posX + 1] == 'E')
        {
            takeDmg(PlayerA,config);
            gotoxy((*player).posX,(*player).posY);
            printf("d");
        }
        else
        {
            gotoxy((*player).posX,(*player).posY);
            printf("d");
        }
        break;
    }
    gameMap[(*player).posY][(*player).posX] = keyPressed;
}

void updatePos2(char gameMap[][MAXCOLUNA], posEntidade* playerb, char keyPressed, playerInfo* PlayerB, settings config)  // A e D prontos
{
    gotoxy((*playerb).posX,(*playerb).posY);
    switch(keyPressed)
    {
    case 't':
        if (gameMap[(*playerb).posY - 1][(*playerb).posX] == ' ')
        {
            // matriz de caracteres
            gameMap[(*playerb).posY][(*playerb).posX] = ' ';
            gotoxy((*playerb).posX,(*playerb).posY);
            printf(" ");
            (*playerb).posY--;
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("w");
        }
        else if (gameMap[(*playerb).posY - 1][(*playerb).posX] == 'E')
        {
            takeDmg2(PlayerB,config); // andar em cima de inimigo
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("w");
        }
        else
        {
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("w");
        }
        break;

    case 'f':
        if (gameMap[(*playerb).posY][(*playerb).posX - 1] == ' ')
        {
            gameMap[(*playerb).posY][(*playerb).posX] = ' ';
            gotoxy((*playerb).posX,(*playerb).posY);
            printf(" ");
            (*playerb).posX--;
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("a");
        }
        else if (gameMap[(*playerb).posY][(*playerb).posX - 1] == 'E')
        {
            takeDmg2(PlayerB,config);
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("a");
        }
        else
        {
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("a");
        }
        break;

    case 'g':
        if (gameMap[(*playerb).posY + 1][(*playerb).posX] == ' ')
        {
            gameMap[(*playerb).posY][(*playerb).posX] = ' ';
            gotoxy((*playerb).posX,(*playerb).posY);
            printf(" ");
            (*playerb).posY++;
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("s");
        }
        else if (gameMap[(*playerb).posY + 1][(*playerb).posX] == 'E')
        {
            takeDmg2(PlayerB,config);
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("s");
        }
        else
        {
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("s");
        }
        break;

    case 'h':
        if(gameMap[(*playerb).posY][(*playerb).posX + 1] == ' ')
        {
            gameMap[(*playerb).posY][(*playerb).posX] = ' ';
            gotoxy((*playerb).posX,(*playerb).posY);
            printf(" ");
            (*playerb).posX++;
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("d");
        }
        else if (gameMap[(*playerb).posY][(*playerb).posX + 1] == 'E')
        {
            takeDmg2(PlayerB,config);
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("d");
        }
        else
        {
            gotoxy((*playerb).posX,(*playerb).posY);
            printf("d");
        }
        break;
    }
    gameMap[(*playerb).posY][(*playerb).posX] = keyPressed;
}
