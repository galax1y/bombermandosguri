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
#define LPPC 22
#define APPC 22

#define MAIN_MENU 0
#define PLAY 1
#define SUBMENU_LEVELS 2
#define SUBMENU_CHAR 3
#define SUBMENU_SETTINGS 4
#define SAIR_JOGO 5





typedef struct entidades{
    int posX,posY;
}posEntidade;

typedef struct informacoes{
	int vidas;
	int numBombas;
	int posXBomba[MAXBOMBAS]; // esse numero dentro limita quantas bombas pode ter no mapa, se tiver mais q isso, buga
	int posYBomba[MAXBOMBAS];
	int score;
}playerInfo;

ALLEGRO_BITMAP *jogador, *paredeDES, *paredeIND, *caixa, *bomba, *blank, *inimigo, *icon, *key = NULL;
ALLEGRO_DISPLAY *telaJogo = NULL;
ALLEGRO_SAMPLE *efeitoSonoro = NULL;
ALLEGRO_TIMER *tempo = NULL;
ALLEGRO_AUDIO_STREAM *musicaFundo = NULL;
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

ALLEGRO_FONT *fonte_menu;
ALLEGRO_COLOR color;







void gotoxy(int x, int y){
   COORD coord = {0,0};
   coord.X = x; coord.Y = y;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// prototipos
void submenuchar();
void submenulevels();
void mainmenu();
void submenusettings();

void executaJogo        (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void punch              (char gameMap[][MAXCOLUNA], posEntidade* player);
bool avalia_punch       (char obstaculo, char lado);
void moveEnemy          (char gameMap[][MAXCOLUNA], posEntidade enemy[], int direcao, int i, playerInfo* PlayerA);
void updateEnemies      (char gameMap[][MAXCOLUNA], posEntidade enemy[], char instrucao, playerInfo* PlayerA);
void updatePosAllegro   (char gamemap[][MAXCOLUNA], posEntidade* player, char keyPressed);
void renderAllegro      (char gameMap[][MAXCOLUNA]);
void mainMenu		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void comandoJogador     (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], char keyPressed, playerInfo* PlayerA);
void renderMap		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[]);
void updatePos		    (char gameMap[][MAXCOLUNA], posEntidade* player, char keyPressed, playerInfo* PlayerA);
void loadGame		    (char gameMap[][MAXCOLUNA], char keyPressed, posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
int gamePause		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void saveGame		    (char gameMap[][MAXCOLUNA], playerInfo* PlayerA);
void updateGame		    (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int* cont_tempo);
void setBomb		    (char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA, char keyPressed);
void showPos            (posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void explodeBomb        (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int indice);
/*
void multiplayer();
*/

int initializeAllegro()
{
    if (!al_init())
    {
        printf("Falha ao inicializar a Allegro");
        return 0;
    }
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW); //seta tela para fullscreen
    telaJogo = al_create_display(0, 0); //cria display. o tamanho vai ser automatico
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
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(64);
    al_init_font_addon();
    al_init_ttf_addon();

    jogador = al_load_bitmap("spritesheetwermann.png");
    bomba = al_load_bitmap("bomb.png");
    caixa = al_load_bitmap("box.png");
    paredeDES = al_load_bitmap("nuvem.png");
    paredeIND = al_load_bitmap("paredeindes.png");
    blank = al_load_bitmap("blank.png");
    inimigo = al_load_bitmap("enemy.png");
    icon = al_load_bitmap("dinoicon.png");
    menubmp = al_load_bitmap("mainmenu.png");
    wall = al_load_bitmap("wall1.png");
    deswall = al_load_bitmap("wall.png");
    key = al_load_bitmap("key.png");

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
    al_convert_mask_to_alpha(jogador, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(bomba, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(inimigo, al_map_rgb(255, 0, 255));
    al_convert_mask_to_alpha(key, al_map_rgb(255, 0, 255));

    fonte_menu = al_load_font("arial.ttf", 48, 0);

    musicaFundo = al_load_audio_stream("soundtrack.wav",4,1024);
    al_attach_audio_stream_to_mixer(musicaFundo, al_get_default_mixer());
    al_set_audio_stream_playmode(musicaFundo, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_gain(musicaFundo, 0);


    al_register_event_source(fila_eventos, al_get_timer_event_source(tempo));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(telaJogo));
    //al_register_event_source(fila_eventos, al_get_mouse_event_source());
    //al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_start_timer(tempo);
    return 1;
}




void renderAllegro(char gameMap[][MAXCOLUNA]){
    int i,j;
    al_clear_to_color(al_map_rgb(195,195,195));
    for (i=0; i<MAXLINHA;i++){
        for(j=0; j<MAXCOLUNA; j++){
            switch(gameMap[i][j]){
            case 'W':
                al_draw_bitmap(paredeIND, j*LPPC, i*APPC, 0);
                break;
            case 'D':
                al_draw_bitmap(paredeDES, j*LPPC, i*APPC, 0);
                break;
            case 'K':
                al_draw_bitmap(caixa, j*LPPC, i*APPC, 0);
                break;
            case 'B':
                al_draw_bitmap(bomba, j*LPPC, i*APPC, 0);
                break;
            case 'E':
                al_draw_bitmap(inimigo, j*LPPC, i*APPC, 0);
                break;
            case 'J':
                al_draw_scaled_bitmap(jogador, 0,0,32,32,j*LPPC, i*APPC,APPC,APPC, 0);
                break;
            case 'w':
                al_draw_scaled_bitmap(jogador, 32,128,32,32,j*LPPC, i*APPC,APPC,APPC, 0);
                break;
            case 'a':
                al_draw_scaled_bitmap(jogador, 0,0,32,32,j*LPPC, i*APPC,APPC,APPC, ALLEGRO_FLIP_HORIZONTAL);
                break;
            case 's':
                al_draw_scaled_bitmap(jogador, 32,0,32,32,j*LPPC, i*APPC,APPC,APPC, 0);
                break;
            case 'd':
                al_draw_scaled_bitmap(jogador, 0,0,32,32,j*LPPC, i*APPC,APPC,APPC, 0);
                break;
            default:
                al_draw_bitmap(blank, j*LPPC, i*APPC, 0);
                break;
            }
        }
    }
    al_flip_display();
}

void renderMap(char gameMap[MAXLINHA][MAXCOLUNA], posEntidade* player, posEntidade enemy[]) { //
	int i, j, k, l;
	//k = 0;
	system("cls");
	for (i = 0; i < MAXLINHA; i++) {
		for (j = 0; j < MAXCOLUNA; j++) {
			if (gameMap[i][j] == 'W'){
				printf("#");
			}
			else if (gameMap[i][j] == 'D'){
				printf("&");
			}
			else if (gameMap[i][j] == 'K'){
				printf("K");
			}
			else if (gameMap[i][j] == 'B'){
				printf("B");
			}
			else printf("%c",gameMap[i][j]);
		}
	}
}


void loadGame(char gameMap[][MAXCOLUNA], char keyPressed, posEntidade* player, posEntidade enemy[], playerInfo* PlayerA){
	FILE *fp;
	int i,j,k = 0;

	switch (keyPressed){
	case 'n':
		system("cls");
		fp = fopen("mapa0.txt", "r");
		for (i = 0; i < MAXLINHA; i++) {
			for (j = 0; j < MAXCOLUNA; j++) {
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

		do{
			keyPressed = getch();
		}while (keyPressed != '1' && keyPressed != '2' && keyPressed != '3');

		switch (keyPressed){
		case '1':
			fp = fopen("save1.txt", "r");
			for (i = 0; i < MAXLINHA; i++) {
				for (j = 0; j < MAXCOLUNA; j++) {
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
			for (i = 0; i < MAXLINHA; i++) {
				for (j = 0; j < MAXCOLUNA; j++) {
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
			for (i = 0; i < MAXLINHA; i++) {
				for (j = 0; j < MAXCOLUNA; j++) {
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
	renderMap(gameMap, player, enemy);
	renderAllegro(gameMap);
    k = 0;
	for (i = 0; i < MAXLINHA; i++) { // depois de passar o mapa do arquivo para a matriz do jogo, analisa a matriz do jogo caracter por caracter
		for (j = 0; j < MAXCOLUNA; j++) { // se for o caracter J, define a posição do jogador iniciando naquele local
			if (gameMap[i][j] == 'J' || gameMap[i][j] == 'a' || gameMap[i][j] == 's' || gameMap[i][j] == 'd' || gameMap[i][j] == 'w') {
				(*player).posX = j;
				(*player).posY = i;
			}
			else if (gameMap[i][j] == 'E'){ // e faz o mesmo com os inimigos, descobrindo a posição deles e numerando de acordo com o que vem primeiro
				enemy[k].posX = j;
				enemy[k].posY = i;
				k++;
			}
		}
	}
	setBomb(gameMap, player, PlayerA, 'l');
	updateEnemies(gameMap, enemy, 'l', PlayerA);
}

int gamePause(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA){
	char opcao;
	system("cls");
	printf("Jogo pausado.\n\nContinuar jogo (ESC)\nCarregar jogo (l)\nSalvar jogo (s)\nVoltar ao menu principal (e)\n");
	do{
		opcao = getch();
	}while (opcao != 27 && opcao != 'l' && opcao != 's' && opcao != 'e');

	switch (opcao){
		case 27: // funcional
			break;
		case 'l': // funcional
			loadGame(gameMap, opcao, player, enemy, PlayerA);
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

void mainMenu(char gameMap[][MAXCOLUNA], posEntidade *player, posEntidade enemy[], playerInfo* PlayerA){
    menubmp = al_load_bitmap("mainmenu.png");
    al_draw_bitmap(menubmp, 0, 0, 0);
    al_flip_display();
	system("cls");
    printf("BOMBERMAN MAIS **DA DA GALAXIA\n\nNew game (n)\nLoad Game(l)\nMultiplayer (m)\nExit Game(ESC)\n");
    char keyPressed = getch();
    switch (keyPressed){
    case 'n':
        loadGame(gameMap, keyPressed, player, enemy, PlayerA);
        break;
    case 'l':
        loadGame(gameMap, keyPressed, player, enemy, PlayerA);
        break;
    case 'm':
        //multiplayer();
        break;
    case 27:
        exit(0);
    }
}

void comandoJogador(char gameMap[MAXLINHA][MAXCOLUNA], posEntidade* player, posEntidade enemy[], char keyPressed, playerInfo* PlayerA) {
	switch(keyPressed){
	case 27:
		if(gamePause(gameMap, player, enemy, PlayerA) == 1) mainMenu(gameMap, player, enemy, PlayerA);
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
        punch(gameMap, player);
        //renderMap(gameMap, player, enemy);
        break;
    default:
        return;
	}
}

void punch(char gameMap[][MAXCOLUNA], posEntidade* player){
    char lado = gameMap[(*player).posY][(*player).posX];
    switch(lado){
        case 'w':
            if (avalia_punch(gameMap[(*player).posY - 1][(*player).posX], lado) == 1){
                gameMap[(*player).posY - 1][(*player).posX] = ' ';
                al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY - 1) * 22, 0);
            }
            break;
        case 'a':
            if (avalia_punch(gameMap[(*player).posY][(*player).posX - 1], lado) == 1){
                gameMap[(*player).posY][(*player).posX - 1] = ' ';
                al_draw_bitmap(blank, ((*player).posX - 1)*22, ((*player).posY) * 22, 0);
            }
            break;
        case 's':
            if (avalia_punch(gameMap[(*player).posY + 1][(*player).posX], lado) == 1){
                gameMap[(*player).posY + 1][(*player).posX] = ' ';
                al_draw_bitmap(blank, ((*player).posX)*22, ((*player).posY + 1) * 22, 0);
            }
            break;
        case 'd':
            if (avalia_punch(gameMap[(*player).posY][(*player).posX + 1], lado) == 1){
                gameMap[(*player).posY][(*player).posX + 1] = ' ';
                al_draw_bitmap(blank, ((*player).posX+1)*22, (*player).posY * 22, 0);
            }
            break;
        default:
            return;
    }
    al_flip_display();
    al_play_sample(efeitoSonoro, 0.4, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

bool avalia_punch(char obstaculo, char lado){
    switch(obstaculo){
    case 'W':
        efeitoSonoro = al_load_sample("punchwall.wav");
        return 0;
    case 'E':
        efeitoSonoro = al_load_sample("punch.wav");
        return 1;
    case ' ':
        efeitoSonoro = al_load_sample("failpunch.wav");
        return 0;
    default:
        efeitoSonoro = al_load_sample("soco.wav");
        return 1;
    }
}

void takeDmg(playerInfo* PlayerA){
    efeitoSonoro = al_load_sample("classic_hurt.wav");
	(*PlayerA).vidas--;
	(*PlayerA).score = (*PlayerA).score - 100;
	if ((*PlayerA).score < 0){
        (*PlayerA).score = 0;
	}
	if((*PlayerA).vidas >= 1){
        al_play_sample(efeitoSonoro, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}
}

void updatePos(char gameMap[][MAXCOLUNA], posEntidade* player, char keyPressed, playerInfo* PlayerA){ // A e D prontos
    gotoxy((*player).posX,(*player).posY);
    switch(keyPressed){
    case 'w':
        if (gameMap[(*player).posY - 1][(*player).posX] == ' '){

            // matriz de caracteres
			gameMap[(*player).posY][(*player).posX] = ' ';
			gotoxy((*player).posX,(*player).posY);
        	printf(" ");

        	// display allegro

        	al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);

			(*player).posY--;

			//al_draw_bitmap(jogador, (*player).posX * 22, (*player).posY * 22, 0);
			gotoxy((*player).posX,(*player).posY);
        	printf("w");
        }
		else if (gameMap[(*player).posY - 1][(*player).posX] == 'E'){
			takeDmg(PlayerA); // andar em cima de inimigo
			gotoxy((*player).posX,(*player).posY);
			printf("w");
			al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
		}else {
			gotoxy((*player).posX,(*player).posY);
			printf("w");
			al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
		}
		al_draw_scaled_bitmap(jogador, 128,32,32,32,(*player).posX * 22,  (*player).posY * 22,APPC,APPC, 0);
        break;

    case 'a':
        if (gameMap[(*player).posY][(*player).posX - 1] == ' '){

        	gameMap[(*player).posY][(*player).posX] = ' ';
        	gotoxy((*player).posX,(*player).posY);
        	printf(" ");


            al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
        	(*player).posX--;

        	al_draw_scaled_bitmap(jogador, 0,0,32,32,(*player).posX * 22,  (*player).posY * 22,APPC,APPC, ALLEGRO_FLIP_HORIZONTAL);

        	gotoxy((*player).posX,(*player).posY);
        	printf("a");
		}
		else if (gameMap[(*player).posY][(*player).posX - 1] == 'E'){
			takeDmg(PlayerA);
			gotoxy((*player).posX,(*player).posY);
			printf("a");

			al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
		}

		else {
			gotoxy((*player).posX,(*player).posY);
			printf("a");

            al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
		}
		al_draw_scaled_bitmap(jogador, 0,0,32,32,(*player).posX * 22,  (*player).posY * 22,APPC,APPC, ALLEGRO_FLIP_HORIZONTAL);
        break;

    case 's':
        if (gameMap[(*player).posY + 1][(*player).posX] == ' '){
        	gameMap[(*player).posY][(*player).posX] = ' ';
        	gotoxy((*player).posX,(*player).posY);
        	printf(" ");

        	al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);

        	(*player).posY++;


			gotoxy((*player).posX,(*player).posY);
        	printf("s");
		}
		else if (gameMap[(*player).posY + 1][(*player).posX] == 'E'){
			takeDmg(PlayerA);
			gotoxy((*player).posX,(*player).posY);
			printf("s");
			al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
		}
		else {
			gotoxy((*player).posX,(*player).posY);
			printf("s");
			al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
		}
		al_draw_scaled_bitmap(jogador, 0,32,32,32,(*player).posX * 22,  (*player).posY * 22,APPC,APPC, 0);
        break;

    case 'd':
        if(gameMap[(*player).posY][(*player).posX + 1] == ' '){
        	gameMap[(*player).posY][(*player).posX] = ' ';
        	gotoxy((*player).posX,(*player).posY);
        	printf(" ");

            al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);

        	(*player).posX++;



			gotoxy((*player).posX,(*player).posY);
        	printf("d");
		}
		else if (gameMap[(*player).posY][(*player).posX + 1] == 'E'){
			takeDmg(PlayerA);
			gotoxy((*player).posX,(*player).posY);
			printf("d");

            al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
		}
		else {
			gotoxy((*player).posX,(*player).posY);
			printf("d");
            al_draw_bitmap(blank, (*player).posX * 22, (*player).posY * 22, 0);
		}
		al_draw_scaled_bitmap(jogador, 0,0,32,32,(*player).posX * 22,  (*player).posY * 22,APPC,APPC, 0);
        break;
    }
    gameMap[(*player).posY][(*player).posX] = keyPressed;
    al_flip_display();
}
void saveGame(char gameMap[][MAXCOLUNA], playerInfo* PlayerA){
	FILE *fp;
	int i,j;
	system("cls");
	printf("Digite o save que voce quer sobrescrever: (1,2,3)\n");
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
	fclose(fp);
}

void moveEnemy(char gameMap[][MAXCOLUNA], posEntidade enemy[], int direcao, int i, playerInfo* PlayerA){
    switch(direcao){
    case 0: // w
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        al_draw_bitmap(blank,(enemy[i].posX) * 22,enemy[i].posY * 22,0);

        enemy[i].posY--;

        al_draw_bitmap(inimigo,(enemy[i].posX) * 22, enemy[i].posY * 22,0);
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");
        break;
    case 1: // a
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        al_draw_bitmap(blank,(enemy[i].posX) * 22,enemy[i].posY * 22,0);

        enemy[i].posX--;

        al_draw_bitmap(inimigo,(enemy[i].posX) * 22, enemy[i].posY * 22,0);
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");
        break;
    case 2: // s
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        al_draw_bitmap(blank,(enemy[i].posX) * 22,enemy[i].posY * 22,0);

        enemy[i].posY++;

        al_draw_bitmap(inimigo,(enemy[i].posX) * 22, enemy[i].posY * 22,0);
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");

        break;
    case 3: // d
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf(" ");
        gameMap[enemy[i].posY][enemy[i].posX] = ' ';
        al_draw_bitmap(blank,(enemy[i].posX) * 22, enemy[i].posY * 22,0);

        enemy[i].posX++;

        al_draw_bitmap(inimigo,(enemy[i].posX) * 22, enemy[i].posY * 22,0);
        gameMap[enemy[i].posY][enemy[i].posX] = 'E';
        gotoxy(enemy[i].posX, enemy[i].posY);
        printf("E");
        break;
    }
    al_flip_display(); // provavelmente nao e o lugar ideal
}

void updateEnemies(char gameMap[][MAXCOLUNA], posEntidade enemy[], char instrucao, playerInfo* PlayerA){
    int i;
    static int direcao[5];

    if (instrucao == 'u'){ // se a funcao for chamada por update de fps (na main)
        for(i=0; i<5; i++){
            switch(direcao[i]){
            case 0: // w y-1
                if (gameMap[enemy[i].posY - 1][enemy[i].posX] != ' '){
                    if (gameMap[enemy[i].posY - 1][enemy[i].posX] == 'w' || gameMap[enemy[i].posY - 1][enemy[i].posX] == 'a' || gameMap[enemy[i].posY - 1][enemy[i].posX] == 's' || gameMap[enemy[i].posY - 1][enemy[i].posX] == 'd'){
                        takeDmg(PlayerA);
                    }
                    direcao[i]++;
                    if (direcao[i] > 3) direcao[i] = 0;
                }
                else{
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
            case 1: // a x-1
                if (gameMap[enemy[i].posY][enemy[i].posX - 1] != ' '){
                    if (gameMap[enemy[i].posY][enemy[i].posX - 1] == 'w' || gameMap[enemy[i].posY ][enemy[i].posX - 1] == 'a' || gameMap[enemy[i].posY ][enemy[i].posX - 1] == 's' || gameMap[enemy[i].posY ][enemy[i].posX - 1] == 'd'){
                        takeDmg(PlayerA);
                    }
                    direcao[i]++;
                    if (direcao[i] > 3) direcao[i] = 0;
                }
                else{
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
            case 2: // s y+1
                if (gameMap[enemy[i].posY + 1][enemy[i].posX] != ' '){
                    if (gameMap[enemy[i].posY + 1][enemy[i].posX] == 'w' || gameMap[enemy[i].posY + 1][enemy[i].posX] == 'a' || gameMap[enemy[i].posY + 1][enemy[i].posX] == 's' || gameMap[enemy[i].posY + 1][enemy[i].posX] == 'd' ){
                        takeDmg(PlayerA);
                    }
                    direcao[i]++;
                    if (direcao[i] > 3) direcao[i] = 0;
                }
                else{
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
            case 3: // d x+1
                if (gameMap[enemy[i].posY][enemy[i].posX + 1] != ' '){
                    if (gameMap[enemy[i].posY][enemy[i].posX + 1] == 'w' || gameMap[enemy[i].posY ][enemy[i].posX + 1] == 'a' || gameMap[enemy[i].posY ][enemy[i].posX + 1] == 's' || gameMap[enemy[i].posY ][enemy[i].posX + 1] == 'd'){
                        takeDmg(PlayerA);
                    }
                    direcao[i]++;
                    if (direcao[i] > 3) direcao[i] = 0;
                }
                else{
                    moveEnemy(gameMap, enemy, direcao[i], i, PlayerA);
                    break;
                }
                //al_flip_display(); aqui fica esquisito
            }
        }
    }
    else if (instrucao == 'l'){ // se a funcao for chamada pelo loadgame, escolhe as direcoes que os inimigos vao andar (NAO ANDA AINDA)
        for(i=0; i<5; i++){
            direcao[i] = 8;
        }  // inicializa todas as direcoes a -1 (que é o invalido)
        for (i=0; i<5; i++){
            do{
                if (enemy[i].posX + 1 != ' ' && enemy[i].posX - 1 != ' ' && enemy[i].posY + 1 != ' ' && enemy[i].posY - 1 != ' '){
                    direcao[i] = 2;
                    continue;
                }else{
                    direcao[i] = rand() % 4; // aleatoriza uma direcao
                    switch(direcao[i]){ // 0 = norte/w          1 = oeste/a       2 = sul/s      3 = leste/d
                    case 0: // case W
                        if (gameMap[(enemy[i].posY) - 1][enemy[i].posX] == ' '){ // se o proximo espaço for vazio
                            direcao[i] = 0; // a direcao que vai começar a andar se torna aquela
                        }else{
                            direcao[i] = 8;
                        }
                        break;
                    case 1: // case A
                        if (gameMap[enemy[i].posY][(enemy[i].posX) - 1] == ' '){
                            direcao[i] = 1;
                        }else{
                            direcao[i] = 8;
                        }
                        break;
                    case 2: // case S
                        if (gameMap[(enemy[i].posY) + 1][enemy[i].posX] == ' '){
                            direcao[i] = 2;
                        }else{
                            direcao[i] = 8;
                        }
                        break;
                    case 3: // case D
                        if (gameMap[enemy[i].posY][(enemy[i].posX) + 1] == ' '){
                            direcao[i] = 3;
                        }else{
                            direcao[i] = 8;
                        }
                        break;
                    }
                }
            }while(direcao[i] == 8); // se direcao continuar sendo invalida, repete a parte de coletar aleatorio
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
	printf("Vidas Player = %d\nNumBombas = %d\n",(*PlayerA).vidas,(*PlayerA).numBombas);
}

void updateGame(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int* cont_tempo){
	int i;

	if((*PlayerA).vidas <= 0){
		system("cls");
		printf("vose morel otareo\n");
		efeitoSonoro = al_load_sample("morri.wav");
		al_clear_to_color(al_map_rgb(0,0,0));
		al_flip_display();
		al_play_sample(efeitoSonoro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		system("PAUSE");
		mainMenu(gameMap,player,enemy,PlayerA);
	}
	if (*cont_tempo >= 15){
        updateEnemies(gameMap, enemy, 'u', PlayerA);
        *cont_tempo = 0;
//	if (PASSOU X FRAMES)
	//updateEnemies();
	}
	gotoxy(0, 26);
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
					al_draw_bitmap(bomba,((*player).posX) * 22,((*player).posY - 1) * 22,0);

					(*PlayerA).numBombas--;
					for (i=0; i<MAXBOMBAS; i++){
                        if ((*PlayerA).posXBomba[i] == -1){
                            (*PlayerA).posXBomba[i] = (*player).posX;
                            (*PlayerA).posYBomba[i] = (*player).posY - 1;
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
                    al_draw_bitmap(bomba,((*player).posX - 1) * 22,((*player).posY) * 22,0);

					(*PlayerA).numBombas--;
					for (i=0; i<MAXBOMBAS; i++){
                        if ((*PlayerA).posXBomba[i] == -1){
                            (*PlayerA).posXBomba[i] = (*player).posX - 1;
                            (*PlayerA).posYBomba[i] = (*player).posY;
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
                    al_draw_bitmap(bomba,((*player).posX) * 22,((*player).posY + 1) * 22,0);

					(*PlayerA).numBombas--;
					for (i=0; i<MAXBOMBAS; i++){
                        if ((*PlayerA).posXBomba[i] == -1){
                            (*PlayerA).posXBomba[i] = (*player).posX;
                            (*PlayerA).posYBomba[i] = (*player).posY + 1;
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
                    al_draw_bitmap(bomba,((*player).posX + 1) * 22,((*player).posY) * 22,0);

					(*PlayerA).numBombas--;
					for (i=0; i<MAXBOMBAS; i++){
                        if ((*PlayerA).posXBomba[i] == -1){
                            (*PlayerA).posXBomba[i] = (*player).posX + 1;
                            (*PlayerA).posYBomba[i] = (*player).posY;
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
	if (keyPressed == 'l'){
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
					k++;
				}
			}
		}
	}
}

void explodeBomb(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA, int indice){
    int i,j;

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

    for (i=1; i<3; i++){
        if (gameMap[(*PlayerA).posYBomba[indice] - i][(*PlayerA).posXBomba[indice]] == 'W') break;
        else if (gameMap[(*PlayerA).posYBomba[indice] - i][(*PlayerA).posXBomba[indice]] == gameMap[(*player).posY][(*player).posX]){
            takeDmg(PlayerA);
        }
        else if (gameMap[(*PlayerA).posYBomba[indice] - i][(*PlayerA).posXBomba[indice]] == 'K'){
            gameMap[(*PlayerA).posYBomba[indice] - i][(*PlayerA).posXBomba[indice]] = 'C';
            al_draw_bitmap(blank, (*PlayerA).posXBomba[indice]*22, ((*PlayerA).posYBomba[indice] - i)*22, 0);
            al_draw_bitmap(key,(*PlayerA).posXBomba[indice] * 22, ((*PlayerA).posYBomba[indice] - i) *22, 0);
        }
        else{
            gameMap[(*PlayerA).posYBomba[indice] - i][(*PlayerA).posXBomba[indice]] = ' ';
            al_draw_bitmap(blank, (*PlayerA).posXBomba[indice]*22, ((*PlayerA).posYBomba[indice] - i)*22, 0);
            continue;
        }
    }

    for (i=1; i<3; i++){
        if (gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice] - i] == 'W') break;
        else if (gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice] - i] == gameMap[(*player).posY][(*player).posX]){
            takeDmg(PlayerA);
        }
        else if (gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice] - i] == 'K'){
            gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice] - i] = 'C';
            al_draw_bitmap(blank, ((*PlayerA).posXBomba[indice] - i)*22, (*PlayerA).posYBomba[indice]*22, 0);
            al_draw_bitmap(key, ((*PlayerA).posXBomba[indice] - i)*22, (*PlayerA).posYBomba[indice]*22, 0);
        }
        else{
            gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice] - i] = ' ';
            al_draw_bitmap(blank, ((*PlayerA).posXBomba[indice] - i)*22, (*PlayerA).posYBomba[indice]*22, 0);
            continue;
        }
    }

    for (i=1; i<3; i++){
        if (gameMap[(*PlayerA).posYBomba[indice] + i][(*PlayerA).posXBomba[indice]] == 'W') break;
        else if (gameMap[(*PlayerA).posYBomba[indice] + i][(*PlayerA).posXBomba[indice]] == gameMap[(*player).posY][(*player).posX]){
            takeDmg(PlayerA);
        }
        else if (gameMap[(*PlayerA).posYBomba[indice] + i][(*PlayerA).posXBomba[indice]] == 'K'){
            gameMap[(*PlayerA).posYBomba[indice] + i][(*PlayerA).posXBomba[indice]] = 'C';
            al_draw_bitmap(blank, (*PlayerA).posXBomba[indice]*22, ((*PlayerA).posYBomba[indice] + i)*22, 0);
            al_draw_bitmap(key, (*PlayerA).posXBomba[indice]*22, ((*PlayerA).posYBomba[indice] + i)*22, 0);
        }
        else{
            gameMap[(*PlayerA).posYBomba[indice] + i][(*PlayerA).posXBomba[indice]] = ' ';
            al_draw_bitmap(blank, (*PlayerA).posXBomba[indice]*22, ((*PlayerA).posYBomba[indice] + i)*22, 0);
            continue;
        }
    }

    for (i=1; i<3; i++){
        if (gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice] + i] == 'W') break;
        else if (gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice] + i] == gameMap[(*player).posY][(*player).posX]){
            takeDmg(PlayerA);
        }
        else if (gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice] + i] == 'K'){
            gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice] + i] = 'C';
            al_draw_bitmap(blank, ((*PlayerA).posXBomba[indice] + i)*22, (*PlayerA).posYBomba[indice]*22, 0);
            al_draw_bitmap(key, ((*PlayerA).posXBomba[indice] + i)*22, (*PlayerA).posYBomba[indice]*22, 0);
        }
        else{
            gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice] + i] = ' ';
            al_draw_bitmap(blank, ((*PlayerA).posXBomba[indice] + i)*22, (*PlayerA).posYBomba[indice]*22, 0);
            continue;
        }
    }

    //gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice]] = ' ';
    //al_draw_bitmap(blank,((*PlayerA).posXBomba[indice] + i)*22 , ((*PlayerA).posYBomba[indice] + i)*22, 0);

    al_draw_bitmap(blank, (*PlayerA).posXBomba[indice]*22, (*PlayerA).posYBomba[indice]*22, 0);
    gameMap[(*PlayerA).posYBomba[indice]][(*PlayerA).posXBomba[indice]] = ' ';
    renderMap(gameMap,player,enemy);
    (*PlayerA).posXBomba[indice] = -1; // invalida a bomba, permite sobrescrever em outro setBomb()

}

int main(){
    char gameMap[MAXLINHA][MAXCOLUNA];
    int estado_menu = 0;

    posEntidade player, enemy[5];
    playerInfo PlayerA;
    srand(time(NULL));
    initializeAllegro();
    //system("pause");
    //mainMenu(gameMap, &player, enemy, &PlayerA);

    int char_escolhido=0;
    int level_escolhido=0;
    int load_level=0;

    while(estado_menu != SAIR_JOGO)
    {
        if (estado_menu == MAIN_MENU)//condicao padrao
        {
            mainmenu(&estado_menu);
        }
        else if (estado_menu  == PLAY)//se no menu for selecionado jogar entra nesta condiçao
        {
            executaJogo(gameMap, &player, enemy, &PlayerA);
        }
        else if (estado_menu == SUBMENU_LEVELS)//se no menu for selecionado submenu de escolher levels entra nesta condiçao
        {
            submenulevels(&estado_menu,&level_escolhido,&load_level);
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


    al_destroy_bitmap(menubmp);
    al_destroy_display(telaJogo);
    al_destroy_sample(efeitoSonoro);
    al_destroy_audio_stream(musicaFundo);
}

void executaJogo(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA){
    char keyPressed;
    int cont_tempo = 0;
    int desenha = 0;

    loadGame(gameMap, 'n', player, enemy, PlayerA);
    while(1){
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);
        if(evento.type == ALLEGRO_EVENT_TIMER)
        {
            cont_tempo++;
            desenha = 1;
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(evento.keyboard.keycode){
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
                keyPressed = 27;
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
            desenha = 0;
            //al_flip_display();
        }
    }
}

void mainmenu(int *estado_menu)
{
    ALLEGRO_EVENT evento;
    int i;
    int cont_tempo=0;
    int desenha=0;
    char menu_p[5][50] = {"PLAY","CHOOSE LEVEL","CHOOSE CHARACTER","SETTINGS","EXIT GAME"};
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
    while(*estado_menu ==2)
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
    char percent[11][5]= {"(100%)","(90%)","(80%)","(70%)","(60%)","(50%)","(40%)","(30%)","(20%)","(10%)","0%"};
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
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN)//se tecla foi pressionada
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

    // parametros para jogar posEntidade em funcao >> &player/&enemy
    // receber como *posEntidade player


    	//showPos(&player, enemy, &PlayerA);
    	//al_wait_for_event(fila_eventos, &evento);
       /* while(!kbhit()){// (evento.type == ALLEGRO_EVENT_TIMER){
                Sleep(100);
                updateGame(gameMap, &player, enemy, &PlayerA);
                showPos(&player, enemy, &PlayerA);
        }*/



