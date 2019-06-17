#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
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
#define ESTADO_FIM 4

#define FPS 30.0
#define MAIN_MENU 0
#define SINGLEPLAYER 1
#define SUBMENU_MULTIPLAYER 2
#define SUBMENU_CHAR 3
#define SUBMENU_SETTINGS 4
#define SAIR_JOGO 5
#define MAXLINHA 25
#define MAXCOLUNA 61
#define MAXBOMBAS 30

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos=NULL;
ALLEGRO_TIMER *timer;
ALLEGRO_BITMAP *menubmp=NULL;
ALLEGRO_BITMAP *wall=NULL;
ALLEGRO_BITMAP *deswall=NULL;
ALLEGRO_BITMAP *blank=NULL;
ALLEGRO_BITMAP *c_player=NULL;
ALLEGRO_BITMAP *player1=NULL;
ALLEGRO_BITMAP *player2=NULL;
ALLEGRO_BITMAP *player3=NULL;
ALLEGRO_BITMAP *player4=NULL;
ALLEGRO_BITMAP *player5=NULL;
ALLEGRO_BITMAP *player6=NULL;
ALLEGRO_BITMAP *player8=NULL;
ALLEGRO_FONT *fonte_menu;
ALLEGRO_COLOR color;

void submenuchar();
void submenulevels();
void mainmenu();
void submenusettings();

typedef struct entidades
{
    int posX,posY;
} posEntidade;

typedef struct informacoes
{
    int vidas;
    int numBombas;
    int posXBomba[MAXBOMBAS]; // esse numero dentro limita quantas bombas pode ter no mapa, se tiver mais q isso, buga
    int posYBomba[MAXBOMBAS];
} playerInfo;

/*void loadGame(char gameMap[][MAXCOLUNA], char keyPressed, posEntidade* player, posEntidade enemy[], playerInfo* PlayerA)
{
    FILE *fp;
    int i,j,k = 0;

    switch (keyPressed)
    {
    case 'n':
        system("cls");
        fp = fopen("mapa0.txt", "r");
        for (i = 0; i < MAXLINHA; i++)
        {
            for (j = 0; j < MAXCOLUNA; j++)
            {
                fscanf(fp, "%c", &gameMap[i][j]);
            }
        }
        fclose(fp);
        fp = fopen("infomapa0.txt","r");
        (*PlayerA).vidas = fgetc(fp) - 48;
        (*PlayerA).numBombas = fgetc(fp) - 48;
        fclose(fp);
        break;
    case 'l':
        system("cls");
        printf("Digite qual save quer acessar: (1,2,3)");

        do
        {
            keyPressed = getch();
        }
        while (keyPressed != '1' && keyPressed != '2' && keyPressed != '3');

        switch (keyPressed)
        {
        case '1':
            fp = fopen("save1.txt", "r");
            for (i = 0; i < MAXLINHA; i++)
            {
                for (j = 0; j < MAXCOLUNA; j++)
                {
                    fscanf(fp, "%c", &gameMap[i][j]);
                }
            }
            fclose(fp);
            fp = fopen("infoSave1.txt","r");
            (*PlayerA).vidas = fgetc(fp) - 48;
            (*PlayerA).numBombas = fgetc(fp) - 48;
            fclose(fp);
            break;
        case '2':
            fp = fopen("save2.txt", "r");
            for (i = 0; i < MAXLINHA; i++)
            {
                for (j = 0; j < MAXCOLUNA; j++)
                {
                    fscanf(fp, "%c", &gameMap[i][j]);
                }
            }
            fclose(fp);
            fp = fopen("infoSave2.txt","r");
            (*PlayerA).vidas = fgetc(fp) - 48;
            (*PlayerA).numBombas = fgetc(fp) - 48;
            fclose(fp);
            break;
        case '3':
            fp = fopen("save3.txt", "r");
            for (i = 0; i < MAXLINHA; i++)
            {
                for (j = 0; j < MAXCOLUNA; j++)
                {
                    fscanf(fp, "%c", &gameMap[i][j]);
                }
            }
            fclose(fp);
            fp = fopen("infoSave3.txt","r");
            (*PlayerA).vidas = fgetc(fp) - 48;
            (*PlayerA).numBombas = fgetc(fp) - 48;
            fclose(fp);
            break;
        }
    }
    //renderAllegro(gameMap);
    k = 0;
    for (i = 0; i < MAXLINHA; i++)   // depois de passar o mapa do arquivo para a matriz do jogo, analisa a matriz do jogo caracter por caracter
    {
        for (j = 0; j < MAXCOLUNA; j++)   // se for o caracter J, define a posição do jogador iniciando naquele local
        {
            if (gameMap[i][j] == 'J' || gameMap[i][j] == 'a' || gameMap[i][j] == 's' || gameMap[i][j] == 'd' || gameMap[i][j] == 'w')
            {
                (*player).posX = j;
                (*player).posY = i;
            }
            else if (gameMap[i][j] == 'E')  // e faz o mesmo com os inimigos, descobrindo a posição deles e numerando de acordo com o que vem primeiro
            {
                enemy[k].posX = j;
                enemy[k].posY = i;
                k++;
            }
        }
    }
   // renderMap(gameMap,player,enemy);
    //setBomb(gameMap, player, PlayerA, 'l');
   // updateEnemies(gameMap, enemy, 'l', PlayerA);
}*/
int main()
{
    int tamsprite=32;
    char gameMap[MAXLINHA][MAXCOLUNA];

    posEntidade player, enemy[5];
    playerInfo PlayerA;
    int estado_menu = 0;
    // Inicializamos a biblioteca
    al_init();
    janela = al_create_display(1200,720);
    al_init_image_addon();
    menubmp = al_load_bitmap("menu.png");
    wall=al_load_bitmap("wall1.png");
    deswall=al_load_bitmap("wall.png");
    blank=al_load_bitmap("blank.png");
    player1 = al_load_bitmap("spritesheetbertotto.png");
    player2 = al_load_bitmap("spritesheetwermann.png");
    player3 = al_load_bitmap("spritesheetspidey.png");
    player4 = al_load_bitmap("spritesheetbat.png");
    player5 = al_load_bitmap("spritesheetskywalker.png");
    player6 = al_load_bitmap("spritesheetrobocop.png");
    player8 = al_load_bitmap("dino.png");
    al_convert_mask_to_alpha(player1, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player2, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player3, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player4, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player5, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player6, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(player8, al_map_rgb(255, 0, 255));
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    fila_eventos = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);
    fonte_menu = al_load_font("arial.ttf", 48, 0);
    if (!fonte_menu)
    {
        printf("erro carregando fonte");
    }
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_start_timer(timer);


    int char_escolhido=0;
    int level_escolhido=0;
    int load_level=0;
    while(estado_menu != SAIR_JOGO)
    {
        if (estado_menu == MAIN_MENU)//condicao padrao
        {
            mainmenu(&estado_menu);
        }
        else if (estado_menu  == SINGLEPLAYER)//se no menu for selecionado jogar entra nesta condiçao
        {
            submenulevels(&estado_menu,&level_escolhido,&load_level);
            //jogo(estado_menu);//codigo do jogo
        }
        else if (estado_menu == SUBMENU_MULTIPLAYER)//se no menu for selecionado submenu de escolher levels entra nesta condiçao
        {
            estado_menu=0;
        }
        else if (estado_menu == SUBMENU_CHAR)//se no menu for selecionado submenu de escolher personagem entra nesta condiçao
        {
            submenuchar(&estado_menu,&char_escolhido);
        }
        else if (estado_menu  == SUBMENU_SETTINGS)//se no menu for selecionado submenu de configuraçoes entra nesta condiçao
        {
            submenusettings(&estado_menu);
        }
        else if (load_level==TRUE)//se no menu for selecionado submenu de configuraçoes entra nesta condiçao
        {
            //char keyPressed = getch();
            //loadGame(gameMap, keyPressed, player, enemy, PlayerA);
        }
    }



    al_destroy_display(janela);

    return 0;
}
void mainmenu(int *estado_menu)
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

            for(i=0; i<TAM_MENU_P; i++)
            {
                if(i==imenu)
                {
                    color=al_map_rgb(255,255,255);
                }
                else
                {
                    color=al_map_rgb(255,0,255);
                }
                al_draw_textf(fonte_menu, color, 600,50*(i+1), ALLEGRO_ALIGN_CENTRE, menu_p[i]);

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
            c_player=player1;
        }
        else if (imenu == 1)
        {
            c_player=player2;
        }
        else if (imenu == 2)
        {
            c_player=player3;
        }
        else if (imenu == 3)
        {
            c_player=player4;
        }
        else if (imenu == 4)
        {
            c_player=player5;
        }
        else if (imenu == 5)
        {
            c_player=player6;
        }
        else if (imenu == 6)
        {
            c_player=player6;
        }
        else if (imenu == 8)
        {
            c_player=player8;
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
                if(i==imenu)
                {
                    color=al_map_rgb(255,0,255);
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
                al_draw_scaled_bitmap(c_player,coluna_sprite,linha_sprite,32,32,110,110,500,500,0);
            }
            else if(char_escolhido!=0)
            {
                al_draw_scaled_bitmap(c_player,coluna_sprite,linha_sprite,32,32,110,110,500,500,0);
            }
            al_flip_display();
            desenha = 0;
        }
    }
}

void submenulevels(int *estado_menu, int *level_escolhido, int *load_level)
{
    char submenu_levels[12][20]= {"LEVEL 1","LEVEL 2","LEVEL 3","LEVEL 4","LEVEL 5","LEVEL 6","LEVEL 7","LEVEL 8","LEVEL 9","LEVEL 10","LOAD LEVEL","BACK TO MAIN MENU"}; //matriz menu de niveis
    ALLEGRO_EVENT evento;
    int i;
    int cont_tempo=0;
    int desenha=0;
    int imenu=0;
    while(*estado_menu ==1)
    {

        al_wait_for_event(fila_eventos, &evento);
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
                    *estado_menu=0;
                    break;
                }
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W)//se tecla pressionada foi w ou seta
            {
                imenu--;
                printf("key up||w\n");
                if (imenu<0) //se apertar para cima no primeiro item, seleciona o ultimo
                    imenu =11;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)//se tecla pressionada foi s ou seta
            {
                printf("key down||s\n");
                imenu++;
                if (imenu==12) //se apertar para cima no primeiro item, seleciona o ultimo
                    imenu=0;



            }
        }
        //desenha=1;
        if(desenha && al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            for(i=0; i<12; i++)
            {
                if(i==imenu)
                {
                    color=al_map_rgb(255,255,255);
                }
                else
                {
                    color=al_map_rgb(255,0,255);
                }
                al_draw_textf(fonte_menu, color, 600,50*(i+1), ALLEGRO_ALIGN_CENTRE, submenu_levels[i]);

            }

            al_flip_display();
            desenha = 0;
        }
    }
}
void submenusettings(int *estado_menu)
{
    char submenu_settings[6][50]= {"EFEITOS SONOROS","MUSICA DE FUNDO","TAMANHO TELA DE JOGO","MODO DE JOGO","CREDITS","BACK TO MAIN MENU"};
    char screen_sizes[4][10]= {"800x450","1200x720","1600x900","1920x1080"};
    char percent[11][4]= {"100%","90%","80%","70%","60%","50%","40%","30%","20%","10%","0%"};
    char game_screen_mode[2][50]= {"FULLSCREEN","ZOOM"};
    int i;
    int cont_tempo=0;
    int desenha=0;
    int settings=0;
    int imenu=0;
    int imenu1=0;
    int imenu2=0;
    int imenu4=1;
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

            if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE||evento.keyboard.keycode == ALLEGRO_KEY_ENTER)// se tecla pressionada foi espaço ou enter
            {
                printf("space||enter");

                if (imenu == 4)//submenu personagens
                {
                    settings=5;//display credits
                }
                else if (imenu == 5)  //configuraçoes
                {
                    *estado_menu=0;
                    break;
                }

            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_LEFT || evento.keyboard.keycode == ALLEGRO_KEY_A)// se tecla pressionada foi espaço ou enter
            {
                printf("space||enter");

                if (imenu == 0)
                {
                        imenu1++;
                        if (imenu1>=11)
                        imenu1=0;

                }
                else if (imenu == 2)
                {
                        imenu2--;
                        if (imenu2<0)
                        imenu2=3;
                }
                else if (imenu == 3)
                {
                        imenu4--;
                        if (imenu4<0)
                        imenu4=1;
                }

            }
           else if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT || evento.keyboard.keycode == ALLEGRO_KEY_D)// se tecla pressionada foi espaço ou enter
            {
                printf("space||enter");

                if (imenu == 0)
                {
                    imenu1--;
                        if (imenu1<0)
                        imenu1=10;
                }
                else if (imenu == 2)
                {
                        imenu2++;
                        if (imenu2>=4)
                        imenu2=0;
                }
                else if (imenu == 3)
                {
                        imenu4++;
                        if (imenu4>=2)
                        imenu4=0;
                }

            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_UP || evento.keyboard.keycode == ALLEGRO_KEY_W)//se tecla pressionada foi w ou seta
            {
                imenu--;
                printf("key up||w\n");
                if (imenu<0) //se apertar para cima no primeiro item, seleciona o ultimo
                    imenu =5;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)//se tecla pressionada foi s ou seta
            {
                printf("key down||s\n");
                imenu++;
                if (imenu==6) //se apertar para cima no primeiro item, seleciona o ultimo
                    imenu=0;



            }
        }
        //desenha=1;
        if(desenha && al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            for(i=0; i<6; i++)
            {
                if(i==imenu)
                {
                    color=al_map_rgb(255,255,255);
                }
                else
                {
                    color=al_map_rgb(255,0,255);
                }
                al_draw_textf(fonte_menu, color, 100,((50*i)+200), ALLEGRO_ALIGN_LEFT, submenu_settings[i]);

            }
            al_draw_textf(fonte_menu, color, 900,200, ALLEGRO_ALIGN_RIGHT, percent[imenu1]);
            al_draw_textf(fonte_menu, color, 900,250, ALLEGRO_ALIGN_RIGHT, percent[imenu1]);
            al_draw_textf(fonte_menu, color, 900,300, ALLEGRO_ALIGN_CENTER, screen_sizes[imenu2]);
            al_draw_textf(fonte_menu, color, 900,350, ALLEGRO_ALIGN_CENTER, game_screen_mode[imenu4]);
            al_flip_display();
            desenha = 0;
        }
    }
}
