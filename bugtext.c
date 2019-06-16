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

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos=NULL;
ALLEGRO_TIMER *timer;
ALLEGRO_BITMAP *menubmp=NULL;
ALLEGRO_FONT *fonte_menu;
ALLEGRO_COLOR color;
void submenuchar();
void submenulevels();
int main(void)
{
    char menu_p[5][50] = {"INICIAR","MAPAS","ESCOLHER PERSONAGEM","CONFIGURAÇÕES","SAIR"};
    int imenu=0;
    // Inicializamos a biblioteca
    al_init();
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
    // Criamos a nossa janela - dimensões de 640x480 px
    janela = al_create_display(1200,720);
    al_init_image_addon();
    menubmp = al_load_bitmap("background.png");
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    fila_eventos = al_create_event_queue();
    timer = al_create_timer(1.0 / FPS);
    fonte_menu = al_load_font("arial.ttf", 48, 0);
    if (!fonte_menu){
        printf("erro carregando fonte");
        system("pause");
    }
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_start_timer(timer);
    color=al_map_rgb(255,0,255);
    // Preenchemos a janela de branco
    while(TRUE)
    {        ALLEGRO_EVENT evento;
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
                    imenu =
                    -1;
            }
            else if (evento.keyboard.keycode == ALLEGRO_KEY_DOWN || evento.keyboard.keycode == ALLEGRO_KEY_S)//se tecla pressionada foi s ou seta
            {
                printf("key down||s\n");

                //  imenu = (imenu+1)%TAM_MENU_P; //desce item no menu

            }
        if(desenha && al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            for(i=0;i<TAM_MENU_P;i++){
               al_draw_text(fonte_menu, color, 600, 200*(i+1), ALLEGRO_ALIGN_CENTRE, &menu_p[i][50]);

            }

            al_flip_display();
            desenha = 0;
}
        // Atualiza a tela
        //al_flip_display();
    }
    // Segura a execução por 10 segundos
   al_rest(10.0);

    // Finaliza a janela
    al_destroy_display(janela);

    return 0;
}
void submenuchar()
{

    char submenu_m[7][50]= {"BERTOTTO","WERMANN","SPIDER-MAN","BATMAN","LUKE SKYWALKER","STORMTROOPER","CARREGAR PERSONAGEM"}; //matriz menu de personagens


}
void submenulevels()
{
    char submenu_m[11][20]= {"LEVEL 1","LEVEL 2","LEVEL 3","LEVEL 4","LEVEL 5","LEVEL 6","LEVEL 7","LEVEL 8","LEVEL 9","LEVEL 10","CARREGAR MAPA"}; //matriz menu de niveis


}
