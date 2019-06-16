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
#define PLAY 1
#define SUBMENU_LEVELS 2
#define SUBMENU_CHAR 3
#define SUBMENU_SETTINGS 4
#define SAIR_JOGO 5

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos=NULL;
ALLEGRO_TIMER *timer;
ALLEGRO_BITMAP *menubmp=NULL;
ALLEGRO_FONT *fonte_menu;
ALLEGRO_COLOR color;
void submenuchar();
void submenulevels();
void mainmenu();
void submenusettings();
int main(void)
{

    // Inicializamos a biblioteca
    al_init();
    janela = al_create_display(1200,720);
    al_init_image_addon();
    menubmp = al_load_bitmap("background.png");
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

    int estado_menu=0;
    while(estado_menu!=SAIR_JOGO)
    {
        if (estado_menu == MAIN_MENU)//condicao padrao
        {
            mainmenu(estado_menu);
        }
        else if (estado_menu  == PLAY)//se no menu for selecionado jogar entra nesta condiçao
        {
            //jogo(estado_menu);//codigo do jogo
        }
        else if (estado_menu == SUBMENU_LEVELS)//se no menu for selecionado submenu de escolher levels entra nesta condiçao
        {
            submenulevels(estado_menu);
        }
        else if (estado_menu == SUBMENU_CHAR)//se no menu for selecionado submenu de escolher personagem entra nesta condiçao
        {
            submenuchar(estado_menu);
        }
        else if (estado_menu  == SUBMENU_SETTINGS)//se no menu for selecionado submenu de configuraçoes entra nesta condiçao
        {
            submenusettings(estado_menu);
        }
    }


// Segura a execução por 10 segundos
    al_rest(10.0);

// Finaliza a janela
    al_destroy_display(janela);

    return 0;
}
void mainmenu(int jogar)
{
    int i;
    int cont_tempo=0;
    int desenha=0;
    int estado=0;
    int submenumapa=0;
    float y_menu = 3 * 720.0/8;
    float x_menu = 1200/2;
    float menu_inc = (720-y_menu)/5;
    int submenupersonagens=0;
    int submenuconfig=0;
    char menu_p[5][50] = {"PLAY","CHOOSE LEVEL","CHOOSE CHARACTER","SETTINGS","EXIT GAME"};
    int imenu=0;
    ALLEGRO_EVENT evento;
    al_wait_for_event(fila_eventos, &evento);
    if(evento.type == ALLEGRO_EVENT_TIMER)
    {
        al_draw_bitmap(menubmp,0,0,0);
        cont_tempo++;
        desenha = 1;
    }
    else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)//se tecla foi pressionada

        if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE||evento.keyboard.keycode == ALLEGRO_KEY_ENTER)// se tecla pressionada foi espaço ou enter
        {
            printf("space||enter");
            if (imenu == 0)  //inicia jogo com level 1
            {
                jogar=1;
                estado++;
            }
            else if (imenu == 1)  //submenu mapas
            {

                submenumapa=1;
                // estado++;
            }
            else if (imenu == 2)//submenu personagens
            {
                submenupersonagens=1;
            }
            else if (imenu == 3)  //configuraçoes
            {
                submenuconfig=1;
            }
            else if (imenu == 4)  //sair
            {
                estado = ESTADO_FIM;
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
    //desenha=1;
    if(desenha && al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
    {
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

        al_flip_display();
        desenha = 0;
    }
    // Atualiza a tela
    //al_flip_display();
}



void submenuchar()
{
    char submenu_m[7][50]= {"BERTOTTO","WERMANN","SPIDER-MAN","BATMAN","LUKE SKYWALKER","STORMTROOPER","LOAD CHARACTER"}; //matriz menu de personagens


}
void submenulevels()
{
    char submenu_m[11][20]= {"LEVEL 1","LEVEL 2","LEVEL 3","LEVEL 4","LEVEL 5","LEVEL 6","LEVEL 7","LEVEL 8","LEVEL 9","LEVEL 10","LOAD LEVEL"}; //matriz menu de niveis


}
void submenusettings(){






}
