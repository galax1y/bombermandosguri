#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#define MAXLINHA 25
#define MAXCOLUNA 61
#define FPS 30

//Variaveis globais(ponteiros) da Allegro
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *imagem = NULL;
ALLEGRO_EVENT evento;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_DISPLAY_MODE janela_info;
ALLEGRO_SAMPLE *morri=NULL;
ALLEGRO_BITMAP *wall,*blank;
ALLEGRO_BITMAP *back,*playerbmp,*box,*key,*enemy,*blank,*walls;



typedef struct entidades
{
    int posX,posY;
} posEntidade;


void mainMenu();
void gamePause();
void loadGame();
bool colisao(char gameMap[][MAXCOLUNA], char direcao, posEntidade* player);
void saveGame();
void renderback(char gameMap[][MAXCOLUNA],ALLEGRO_DISPLAY *janela,int PPC);
void render(char gameMap[][MAXCOLUNA], ALLEGRO_DISPLAY* janela, posEntidade* player, char instrucao, int PPC);
char updatePos2(char gameMap[][MAXCOLUNA], posEntidade* player, char keyPressed);
void updateGame();
void comandoJogador();
void renderback();
int initializeAllegro();




void gotoxy(int x, int y)
{
    COORD coord = {0,0};
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int initializeAllegro()
{
    if (!al_init())
    {
        printf("Falha ao inicializar a Allegro");
        return 0;
    }
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW); //seta tela para fullscreen
    janela = al_create_display(0, 0); //cria display. o tamanho vai ser automatico
    if(!janela)
    {
        printf("Falha ao criar janela");
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
    timer = al_create_timer(1.0 / FPS);
    //al_get_display_mode(al_get_num_display_modes() - 1, &janela_info);

    fila_eventos = al_create_event_queue();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(64);
    blank = al_load_bitmap("blank.png");
    wall = al_load_bitmap("paredeindes.png");
    enemy = al_load_bitmap("enemy.png");
    playerbmp = al_load_bitmap("dino2.png");
    box = al_load_bitmap("box.png");
    morri = al_load_sample("morri.wav");

    //al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    //al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    //al_register_event_source(fila_eventos, al_get_mouse_event_source());
    //al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_start_timer(timer);
    return 1;
}

int main()
{
    int i,j;
    int rend=0,sair = 0;
    char keyPressed, instrucao;
    char gameMap[MAXLINHA][MAXCOLUNA];
    posEntidade player, enemy[5];
    if (!initializeAllegro())
    {
        return -1;
    }
    int PPC = (al_get_display_width(janela)*1.0/(MAXCOLUNA-1));
    loadGame(gameMap, &player, enemy);
    renderback(gameMap, janela, PPC);
 printf("to aqui");

    render(gameMap, janela, &player, 'l',PPC);

    //while(1)
    for(i=0; i<10; i++)
    {
        al_wait_for_event(fila_eventos, &evento);
        keyPressed = getch();
        //keyPressed = 'w';
        instrucao = updatePos2(gameMap, &player, keyPressed);
        //while(!kbhit())
        //{
        if(evento.type == ALLEGRO_EVENT_TIMER)
        {
            printf("instrucao = %c",instrucao);
            render(gameMap, janela, &player, instrucao,PPC);
        }
        //}
        fflush(stdin);
        /*if(rend && al_is_event_queue_empty(fila_eventos))
        {
            render(gameMap, janela, &playerposX, &playerposY);
            rend=0;
        }*/
    }
    al_play_sample(morri, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_ONCE,NULL);
    al_destroy_timer(timer);

    al_destroy_bitmap(playerbmp);
    al_destroy_bitmap(enemy);
    al_destroy_bitmap(box);
    al_destroy_bitmap(back);
    al_destroy_bitmap(blank);
    al_destroy_bitmap(walls);
    al_destroy_bitmap(blank);
    al_destroy_display(janela);
    al_destroy_bitmap(imagem);
    al_destroy_event_queue(fila_eventos);
}


void loadGame(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[5])
{
    FILE *fp;
    // recebe arquivo
    fp = fopen("save1.txt","r");
    int i, j, k = 0;

    for(i = 0; i<MAXLINHA; i++)
    {
        for (j = 0; j<MAXCOLUNA; j++)
        {
            fscanf(fp, "%c",&gameMap[i][j]);
            //printf("%c",gameMap[i][j]);
        }
    }

    for(i = 0; i<MAXLINHA; i++)
    {
        for (j = 0; j<MAXCOLUNA; j++)
        {
            if (gameMap[i][j] == 'J' || gameMap[i][j] == 'w' || gameMap[i][j] == 'a' || gameMap[i][j] == 's' || gameMap[i][j] == 'd')
            {
                (*player).posX = j;
                (*player).posY = i;
            }
        }
    }
}

void renderback(char gameMap[][MAXCOLUNA],ALLEGRO_DISPLAY *janela,int PPC)//funcao le todas as paredes indstrutiveis e espaços em branco e salva em um png para salvar processamento
{
    int i,j;
    ALLEGRO_BITMAP *background = al_create_bitmap(1400,900);
    ALLEGRO_BITMAP *walls = al_create_bitmap(1400,900);
    al_set_target_bitmap(background);
    for(i=0; i<MAXLINHA; i++)
    {
        for(j=0; j<MAXCOLUNA; j++)
        {
            al_draw_scaled_bitmap(blank,0,0,22,22,PPC*j,PPC*i, PPC, PPC, 0);
        }
    }


    al_save_bitmap("background.png", background);
    al_set_target_bitmap(walls);
    al_clear_to_color(al_map_rgb(255,0,255));
    for(i=0; i<MAXLINHA; i++)
    {
        for(j=0; j<MAXCOLUNA; j++)
        {
            if(gameMap[i][j] == 'W')
            {
                wall = al_load_bitmap("paredeindes.png");
                al_draw_scaled_bitmap(wall,0,0,22,22,PPC*j,PPC*i, PPC, PPC, 0);
            }

        }
    }
    al_save_bitmap("walls.png", walls);
    al_destroy_bitmap(background);
    al_destroy_bitmap(wall);
    al_destroy_bitmap(walls);
    al_destroy_bitmap(blank);
}



void render(char gameMap[][MAXCOLUNA], ALLEGRO_DISPLAY* janela, posEntidade* player, char instrucao, int PPC)
{
    int i,j;
    //al_clear_to_color(al_map_rgb(195,195,195));
    back = al_load_bitmap("background.png");
    walls = al_load_bitmap("walls.png");
    al_convert_mask_to_alpha(walls, al_map_rgb(255,0,255));
    al_convert_mask_to_alpha(enemy, al_map_rgb(255,0,255));
    al_convert_mask_to_alpha(playerbmp, al_map_rgb(255,0,255));

    al_draw_bitmap(back,0,0,0);
    al_draw_bitmap(walls,0,0,0);

    switch(instrucao)
    {
    case 'w':
        j = (*player).posX;
        i = (*player).posY;
        al_draw_scaled_bitmap(playerbmp,0, 0,40,43,PPC*j, PPC*i, 40*(PPC/43.0), PPC, 0);
        break;
    case 'a':
        j = (*player).posX;
        i = (*player).posY;
        al_draw_scaled_bitmap(playerbmp,0, 0,40,43,PPC*j, PPC*i, 40*(PPC/43.0), PPC, ALLEGRO_FLIP_HORIZONTAL);
        break;
    case 's':
        j = (*player).posX;
        i = (*player).posY;
        al_draw_scaled_bitmap(playerbmp,0, 0,40,43,PPC*j, PPC*i, 40*(PPC/43.0), PPC, 0);
        break;
    case 'd':
        j = (*player).posX;
        i = (*player).posY;
        al_draw_scaled_bitmap(playerbmp,0, 0,40,43,PPC*j, PPC*i, 40*(PPC/43.0), PPC, 0);
        break;

    case 'l':
        for(i=0; i<MAXLINHA; i++)
        {
            for(j=0; j<MAXCOLUNA; j++)
            {

                if (gameMap[i][j] == 's')
                {
                    al_draw_scaled_bitmap(playerbmp,0, 0,40,43,PPC*j, PPC*i, 40*(PPC/43.0), PPC, 0);
                }
                else if(gameMap[i][j] == 'w')
                {
                    al_draw_scaled_bitmap(playerbmp,0, 0,40,43,PPC*j, PPC*i, 40*(PPC/43.0), PPC, 0);
                }

                else if(gameMap[i][j] == 'a' || gameMap[i][j] == 'd')
                {
                    switch(gameMap[i][j])
                    {
                    case 'a':
                        al_draw_bitmap(playerbmp, PPC*j, PPC*i,ALLEGRO_FLIP_HORIZONTAL);
                        break;
                    case 'd':
                        al_draw_bitmap(playerbmp, PPC*j, PPC*i,0);
                        break;
                    }
                }

                else if(gameMap[i][j] == 'D' || gameMap[i][j] == 'K' || gameMap[i][j] == 'B')
                {

                    al_draw_scaled_bitmap(box,0, 0,14,14,PPC*j, PPC*i, PPC,PPC, 0);
                }
                else if(gameMap[i][j] == 'E')
                {

                    al_convert_mask_to_alpha(enemy, al_map_rgb(255,0,255));
                    al_draw_bitmap(enemy, PPC*j, PPC*i, 0);
                }
            }
        }
        break;
    case 'x':
        al_convert_mask_to_alpha(playerbmp, al_map_rgb(255,0,255));
        j = (*player).posX;
        i = (*player).posY;
        al_draw_scaled_bitmap(playerbmp,0, 0,40,43, PPC*j, PPC*i, 40*(PPC/43.0), PPC, 0);

        switch(gameMap[i][j])
        {
        case 'd':
            al_draw_bitmap(blank, PPC*j, PPC*i, 0);
            al_draw_scaled_bitmap(playerbmp,0, 0,40,43, PPC*j, PPC*i, 40*(PPC/43.0), PPC, 0);
            break;
        case 'a':
            al_draw_bitmap(blank, PPC*j, PPC*i, 0);
            al_draw_scaled_bitmap(playerbmp,0, 0,40,43, PPC*j, PPC*i, 40*(PPC/43.0), PPC, ALLEGRO_FLIP_HORIZONTAL);
            break;
        }
        break;

    }
    al_flip_display();


    system("cls");
    for(i=0; i<MAXLINHA; i++)
    {
        for(j=0; j<MAXCOLUNA; j++)
        {
            printf("%c",gameMap[i][j]);
        }
    }
}

void takeDmg()
{

}

bool colisao(char gameMap[][MAXCOLUNA], char direcao, posEntidade* player)
{

    switch(direcao)
    {
    case 'w':
        if (gameMap[(*player).posY - 1][(*player).posX] == ' ')
        {
            return 0;
        }
        else
            return 1;
    case 'a':
        if (gameMap[((*player).posY)][((*player).posX) - 1] == ' ')
        {
            return 0;
        }
        else
            return 1;
    case 's':
        if (gameMap[((*player).posY) + 1][(*player).posX] == ' ')
        {
            return 0;
        }
        else
            return 1;
    case 'd':
        if (gameMap[(*player).posY][((*player).posX) + 1] == ' ')
        {
            return 0;
        }
        else
            return 1;
    }
}
char updatePos2(char gameMap[][MAXCOLUNA], posEntidade* player, char keyPressed)
{
    switch(keyPressed)
    {
    case 'l':
        return 'l';
    case 'w':
        if (colisao(gameMap, 'w', player) == 0)
        {
            gameMap[(*player).posY][(*player).posX] = ' ';
            (*player).posY--;
            gameMap[(*player).posY][(*player).posX] = 'w';
            return 'w';
        }
        else
            gameMap[(*player).posY][(*player).posX] = 'w';
        return 'x';
        break;

    case 'a':
        if (colisao(gameMap, 'a', player) == 0)
        {
            gameMap[(*player).posY][(*player).posX] = ' ';
            (*player).posX--;
            gameMap[(*player).posY][(*player).posX] = 'a';
            return 'a';
        }
        else
            gameMap[(*player).posY][(*player).posX] = 'a';
        return 'x';
        break;

    case 's':
        if (colisao(gameMap, 's', player) == 0)
        {
            gameMap[(*player).posY][(*player).posX] = ' ';
            (*player).posY++;
            gameMap[(*player).posY][(*player).posX] = 's';
            return 's';
        }
        else
            gameMap[(*player).posY][(*player).posX] = 's';
        return 'x';
        break;

    case 'd':
        if (colisao(gameMap, 'd', player) == 0)
        {
            gameMap[(*player).posY][(*player).posX] = ' ';
            (*player).posX++;
            gameMap[(*player).posY][(*player).posX] = 'd';
            return 'd';
        }
        else
            gameMap[(*player).posY][(*player).posX] = 'd';
        return 'x';
        break;
    }
}
