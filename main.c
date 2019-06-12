#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

#define MAXLINHA 25
#define MAXCOLUNA 61
#define MAXBOMBAS 30
typedef struct entidades{
    int posX,posY;
}posEntidade;

typedef struct informacoes{
	int vidas;
	int numBombas;
	int posXBomba[MAXBOMBAS]; // esse numero dentro limita quantas bombas pode ter no mapa, se tiver mais q isso, buga
	int posYBomba[MAXBOMBAS];
}playerInfo;

void gotoxy(int x, int y){
   COORD coord = {0,0};
   coord.X = x; coord.Y = y;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// prototipos
void mainMenu		(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void comandoJogador (char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], char keyPressed, playerInfo* PlayerA);
void renderMap		(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[]);
void updatePos		(char gameMap[][MAXCOLUNA], posEntidade* player, char keyPressed, playerInfo* PlayerA);
void loadGame		(char gameMap[][MAXCOLUNA], char keyPressed, posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
int gamePause		(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void saveGame		(char gameMap[][MAXCOLUNA], playerInfo* PlayerA);
void updateGame		(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA);
void setBomb		(char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA, char keyPressed);
/*
void multiplayer();
*/

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
	int i,j,k;
	
	switch (keyPressed){
	case 'n':
		system("cls");
		fp = fopen("mapa0.txt", "r");
		for (i = 0; i < MAXLINHA; i++) {
			for (j = 0; j < MAXCOLUNA; j++) {
				fscanf(fp, "%c", &gameMap[i][j]);
			}
		}
		fp = fopen("infomapa0.txt","r");
		(*PlayerA).vidas = fgetc(fp) - 48;
		(*PlayerA).numBombas = fgetc(fp) - 48;
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
			fp = fopen("infoSave1.txt","r");
			(*PlayerA).vidas = fgetc(fp) - 48;
			(*PlayerA).numBombas = fgetc(fp) - 48;
			break;
		case '2':
			fp = fopen("save2.txt", "r");
			for (i = 0; i < MAXLINHA; i++) {
				for (j = 0; j < MAXCOLUNA; j++) {
					fscanf(fp, "%c", &gameMap[i][j]);
				}
			}
			fp = fopen("infoSave2.txt","r");
			(*PlayerA).vidas = fgetc(fp) - 48;
			(*PlayerA).numBombas = fgetc(fp) - 48;
			break;
		case '3':
			fp = fopen("save3.txt", "r");
			for (i = 0; i < MAXLINHA; i++) {
				for (j = 0; j < MAXCOLUNA; j++) {
					fscanf(fp, "%c", &gameMap[i][j]);
				}
			}
			fp = fopen("infoSave3.txt","r");
			(*PlayerA).vidas = fgetc(fp) - 48;
			(*PlayerA).numBombas = fgetc(fp) - 48;
			break;
		}
	}
	
	for (i = 0; i < MAXLINHA; i++) { // depois de passar o mapa do arquivo para a matriz do jogo, analisa a matriz do jogo caracter por caracter
		for (j = 0; j < MAXCOLUNA; j++) { // se for o caracter J, define a posição do jogador iniciando naquele local
			if (gameMap[i][j] == 'J' || gameMap[i][j] == 'a' || gameMap[i][j] == 's' || gameMap[i][j] == 'd' || gameMap[i][j] == 'w') {
				(*player).posX = j;
				(*player).posY = i;
			}
			if (gameMap[i][j] == 'E'){ // e faz o mesmo com os inimigos, descobrindo a posição deles e numerando de acordo com o que vem primeiro
				enemy[k].posX = j;
				enemy[k].posY = i;
				k++;
			}
		}
	}
	renderMap(gameMap,player,enemy);
	setBomb(gameMap, player, PlayerA, 'l');

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
		setBomb(gameMap,player,PlayerA,'b');
		break;
	}
}

void takeDmg(playerInfo* PlayerA){
	(*PlayerA).vidas--;
}

void updatePos(char gameMap[][MAXCOLUNA], posEntidade* player, char keyPressed, playerInfo* PlayerA){ // pronto
    gotoxy((*player).posX,(*player).posY);
    switch(keyPressed){
    case 'w':
        if (gameMap[(*player).posY - 1][(*player).posX] == ' '){
			gameMap[(*player).posY][(*player).posX] = ' ';
			gotoxy((*player).posX,(*player).posY);
        	printf(" ");
			(*player).posY--;
			gameMap[(*player).posY][(*player).posX] = 'w';
			gotoxy((*player).posX,(*player).posY);
        	printf("w");
        }
		else if (gameMap[(*player).posY - 1][(*player).posX] == 'E'){
			takeDmg(PlayerA); // andar em cima de inimigo
			gotoxy((*player).posX,(*player).posY);
			printf("w");
		}else {
			gotoxy((*player).posX,(*player).posY);
			printf("w");
			return;
		}
        break;
        
    case 'a':
        if (gameMap[(*player).posY][(*player).posX - 1] == ' '){
        	gameMap[(*player).posY][(*player).posX] = ' ';
        	gotoxy((*player).posX,(*player).posY);
        	printf(" ");
        	(*player).posX--;
			gameMap[(*player).posY][(*player).posX] = 'a';
        	gotoxy((*player).posX,(*player).posY);
        	printf("a");
		}
		else if (gameMap[(*player).posY][(*player).posX - 1] == 'E'){
			takeDmg(PlayerA);
			gotoxy((*player).posX,(*player).posY);
			printf("a");
		}
		
		else {
			gotoxy((*player).posX,(*player).posY);
			printf("a");
			return;
		}
        break;
        
    case 's':
        if (gameMap[(*player).posY + 1][(*player).posX] == ' '){
        	gameMap[(*player).posY][(*player).posX] = ' ';
        	gotoxy((*player).posX,(*player).posY);
        	printf(" ");
        	(*player).posY++;
			gameMap[(*player).posY][(*player).posX] = 's';
			gotoxy((*player).posX,(*player).posY);
        	printf("s");
		}
		else if (gameMap[(*player).posY + 1][(*player).posX] == 'E'){
			takeDmg(PlayerA);
			gotoxy((*player).posX,(*player).posY);
			printf("s");
		}
		else {
			gotoxy((*player).posX,(*player).posY);
			printf("s");
			return;
		}
        break;
        
    case 'd':
        if(gameMap[(*player).posY][(*player).posX + 1] == ' '){
        	gameMap[(*player).posY][(*player).posX] = ' ';
        	gotoxy((*player).posX,(*player).posY);
        	printf(" ");
        	(*player).posX++;
			gameMap[(*player).posY][(*player).posX] = 'd';
			gotoxy((*player).posX,(*player).posY);
        	printf("d");
		}
		else if (gameMap[(*player).posY][(*player).posX + 1] == 'E'){
			takeDmg(PlayerA);
			gotoxy((*player).posX,(*player).posY);
			printf("d");
		}
		else {
			gotoxy((*player).posX,(*player).posY);
			printf("d");
			return;
		}
        break;
    }
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

void showPos(posEntidade* player, posEntidade enemy[], playerInfo* PlayerA){
	int i;
	gotoxy(0,26);
	printf("pos player = [%d][%d]\n",(*player).posX,(*player).posY);
	for(i=0;i<5;i++){
		printf("pos enemy[%d] = [%d][%d]\n",i,enemy[i].posX,enemy[i].posY);
	}
	printf("Vidas Player = %d\nNumBombas = %d\n",(*PlayerA).vidas,(*PlayerA).numBombas);
}

void updateGame(char gameMap[][MAXCOLUNA], posEntidade* player, posEntidade enemy[], playerInfo* PlayerA){
	int i;
	
	
	
	if((*PlayerA).vidas == 0){
		system("cls");
		printf("vose morel otareo\n");
		system("PAUSE");
		mainMenu(gameMap,player,enemy,PlayerA);
	}
	
	
	
	
}

void setBomb(char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA, char keyPressed){
	int i,j,k;
	if((*PlayerA).numBombas == 0){
		return;
	}
	if (keyPressed == 'b'){
		switch(gameMap[(*player).posY][(*player).posX]) {
			case 'w':
				if (gameMap[(*player).posY - 1][(*player).posX] == ' '){
					(*PlayerA).numBombas--;
					gameMap[(*player).posY - 1][(*player).posX] = 'B';
					gotoxy((*player).posX, (*player).posY - 1);
					printf("B");
				}
				break;
			case 'a':
				if (gameMap[(*player).posY][(*player).posX - 1] == ' '){
					(*PlayerA).numBombas--;
					gameMap[(*player).posY][(*player).posX - 1] = 'B';
					gotoxy((*player).posX - 1, (*player).posY);
					printf("B");
				}
				break;
			case 's':
				if (gameMap[(*player).posY + 1][(*player).posX] == ' '){
					(*PlayerA).numBombas--;
					gameMap[(*player).posY + 1][(*player).posX] = 'B';
					gotoxy((*player).posX, (*player).posY + 1);
					printf("B");
				}
				break;
			case 'd':
				if (gameMap[(*player).posY][(*player).posX + 1] == ' '){
					(*PlayerA).numBombas--;
					gameMap[(*player).posY][(*player).posX + 1] = 'B';
					gotoxy((*player).posX + 1, (*player).posY);
					printf("B");
				}
				break;
			
			default:
				return;
		}
	}
	if (keyPressed == 'l'){
		k = 0;
		int x = 0;
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

int main(){
    char gameMap[MAXLINHA][MAXCOLUNA];
    char keyPressed;
    posEntidade player, enemy[5];
    playerInfo PlayerA;
    
    mainMenu(gameMap, &player, enemy, &PlayerA);
    while(1){
    	showPos(&player, enemy, &PlayerA);

        while(!kbhit()){
            Sleep(33);
            updateGame(gameMap, &player, enemy, &PlayerA);
        }
        keyPressed = getch();
        comandoJogador(gameMap, &player, enemy, keyPressed, &PlayerA);
    }
}

    // parametros para jogar posEntidade em funcao >> &player/&enemy
    // receber como *posEntidade player
