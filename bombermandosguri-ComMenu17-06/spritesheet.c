#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#define TAM_MENU_P 5
#define MAXLINHA 25
#define MAXCOLUNA 61
#define MAXBOMBAS 30
#define FPS 30.0
#define LPPC 22
#define APPC 22
ALLEGRO_BITMAP *jogador, *paredeDES, *paredeIND, *caixa, *bomba, *blank, *inimigo, *icon, *key = NULL;
ALLEGRO_DISPLAY *telaJogo = NULL;

ALLEGRO_TIMER *tempo = NULL;

ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;


ALLEGRO_BITMAP *menubmp=NULL;
ALLEGRO_BITMAP *wall=NULL;
ALLEGRO_BITMAP *deswall=NULL;

ALLEGRO_BITMAP *c_player=NULL;
ALLEGRO_BITMAP *player1=NULL;
ALLEGRO_BITMAP *player2=NULL;
ALLEGRO_BITMAP *player3=NULL;
ALLEGRO_BITMAP *player4=NULL;
ALLEGRO_BITMAP *player5=NULL;
ALLEGRO_BITMAP *player6=NULL;
ALLEGRO_BITMAP *player8=NULL;


ALLEGRO_COLOR color;


int initializeAllegro()
{
    if (!al_init())
    {
        printf("Falha ao inicializar a Allegro");
        return 0;
    }
     //seta tela para fullscreen
    telaJogo = al_create_display(1000, 1000); //cria display. o tamanho vai ser automatico
    if(!telaJogo)
    {
        printf("Falha ao criar telaJogo");
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




printf("to aq");
    player1 = al_load_bitmap("imagens/spritesheetbertotto.png");


    al_convert_mask_to_alpha(player1, al_map_rgb(255, 0, 255));
printf("to aq");




    al_register_event_source(fila_eventos, al_get_timer_event_source(tempo));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(telaJogo));
    printf("to aq");
    //al_register_event_source(fila_eventos, al_get_mouse_event_source());
    //al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_start_timer(tempo);
    return 1;
}

int main()
    {
        initializeAllegro();
        printf("to aq");
    ALLEGRO_EVENT evento;
    int i;
    int cont_tempo=0;
    int desenha=0;
    int imenu=0;
    int coluna_sprite=0;
    int coluna_atual=0;
    int linha_sprite=0;
    c_player=player1;
    int char_escolhido=0;
 printf("to aq");
    while(TRUE)
    {

        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_TIMER)
        {

            cont_tempo++;
            desenha = 1;

            if (cont_tempo >= 1)
            {
                //linha_sprite=64;
                cont_tempo=0;
                coluna_atual++;
                if (coluna_atual >= 5)
                {

                    coluna_atual=0;

                }
            }
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN)//se tecla foi pressionada
        {
            char_escolhido=1;
        }
        if(desenha && al_is_event_queue_empty(fila_eventos))  //desenha menu na tela
        {
            al_clear_to_color(al_map_rgb(192,192,192));

            if(char_escolhido==0)
            {
                al_draw_scaled_bitmap(c_player,0,0,32,32,0,0,500,500,0);
            }
            else if(char_escolhido!=0)
            {
                if(coluna_atual==0){
                al_draw_scaled_bitmap(c_player,64,0,32,32,0,0,500,500,0);
                al_draw_scaled_bitmap(c_player,64,0,32,32,0,500,500,500,0);
                }
               else if(coluna_atual==1){
                al_draw_scaled_bitmap(c_player,160,0,32,32,125,0,500,500,0);
                al_draw_scaled_bitmap(c_player,224,0,32,32,125,500,500,500,0);
                }
                else if(coluna_atual==2){
                al_draw_scaled_bitmap(c_player,192,0,32,32,250,0,500,500,0);
                al_draw_scaled_bitmap(c_player,192,0,32,32,250,500,500,500,0);
                }
                else if(coluna_atual==3){
                al_draw_scaled_bitmap(c_player,224,0,32,32,375,0,500,500,0);
                al_draw_scaled_bitmap(c_player,160,0,32,32,375,500,500,500,0);
                }
                else if(coluna_atual==4){
                al_draw_scaled_bitmap(c_player,64,0,32,32,500,0,500,500,0);
                al_draw_scaled_bitmap(c_player,64,0,32,32,500,500,500,500,0);
                }
            }
            al_flip_display();
            desenha = 0;

        }
        //al_rest(1);
    }
}








