
typedef struct informacoes
{
    int vidas;
    int numBombas;
    int posXBomba[MAXBOMBAS]; // esse numero dentro limita quantas bombas pode ter no mapa, se tiver mais q isso, buga
    int posYBomba[MAXBOMBAS];
    int timerBomba[MAXBOMBAS]=0;
    int bombsplaced;
} playerInfo;

void setBomb(char gameMap[][MAXCOLUNA], posEntidade* player, playerInfo* PlayerA, char keyPressed)
{
    int i,j,k;
    if((*PlayerA).numBombas == 0)
    {
        return;
    }
    if (keyPressed == 'b')
    {
        switch(gameMap[(*player).posY][(*player).posX])
        {
        case 'w':
            if (gameMap[(*player).posY - 1][(*player).posX] == ' ')
            {
                (*PlayerA).numBombas--;
                bombsplaced++;
                gameMap[(*player).posY - 1][(*player).posX] = 'B';
                gotoxy((*player).posX, (*player).posY - 1);
                printf("B");
            }
            break;
        case 'a':
            if (gameMap[(*player).posY][(*player).posX - 1] == ' ')
            {
                (*PlayerA).numBombas--;
                bombsplaced++;
                gameMap[(*player).posY][(*player).posX - 1] = 'B';
                gotoxy((*player).posX - 1, (*player).posY);
                printf("B");
            }
            break;
        case 's':
            if (gameMap[(*player).posY + 1][(*player).posX] == ' ')
            {
                (*PlayerA).numBombas--;
                bombsplaced++;
                gameMap[(*player).posY + 1][(*player).posX] = 'B';
                gotoxy((*player).posX, (*player).posY + 1);
                printf("B");
            }
            break;
        case 'd':
            if (gameMap[(*player).posY][(*player).posX + 1] == ' ')
            {
                (*PlayerA).numBombas--;
                bombsplaced++;
                gameMap[(*player).posY][(*player).posX + 1] = 'B';
                gotoxy((*player).posX + 1, (*player).posY);
                printf("B");
            }
            break;

        default:
            return;
        }
    }
    if (keyPressed == 'l')
    {
        k = 0;
        int x = 0;
        for(i=0; i<MAXLINHA; i++)
        {
            for(j=0; j<MAXCOLUNA; j++)
            {
                if (gameMap[i][j] == 'B')
                {
                    (*PlayerA).posXBomba[k] = j;
                    (*PlayerA).posYBomba[k] = i;
                    k++;
                }
            }
        }
    }

}

void explode(int k)
{
    int i= ((*PlayerA).posXBomba[k]-2);
    int j= ((*PlayerA).posYBomba[k]-2);

    for(i; i<5; i++)
    {
        if (gameMap[(*PlayerA).posYBomba[k]][i]='K')
        {
            gameMap[(*PlayerA).posYBomba[k]][i]='C';
        }
        else if(gameMap[(*PlayerA).posYBomba[k]][i]='w'||gameMap[(*PlayerA).posYBomba[k]][i]='a'||gameMap[(*PlayerA).posYBomba[k]][i]='s'||gameMap[(*PlayerA).posYBomba[k]][i]='d')
        {
            takeDmg();
        }
        else if(gameMap[(*PlayerA).posYBomba[k]][i]='D')
        {
            gameMap[(*PlayerA).posYBomba[k]][i]=' ';

        }
        else if(gameMap[(*PlayerA).posYBomba[k]][i]='E')
        {
            gameMap[(*PlayerA).posYBomba[k]][i]=' ';

        }
    }
    for(j; j<5; j++)
    {
        if (gameMap[j][(*PlayerA).posXBomba[k]]='K')
        {
            gameMap[j][(*PlayerA).posXBomba[k]]='C';
        }
        else if(gameMap[j][(*PlayerA).posXBomba[k]]='w'||gameMap[j][(*PlayerA).posXBomba[k]]='a'||gameMap[j][(*PlayerA).posXBomba[k]]='s'||gameMap[j][(*PlayerA).posXBomba[k]]='d')
        {
            takeDmg();
        }
        else if(gameMap[j][(*PlayerA).posXBomba[k]]='D')
        {
            gameMap[j][(*PlayerA).posXBomba[k]]=' ';

        }
        else if(gameMap[j][(*PlayerA).posXBomba[k]]='E')
        {
            gameMap[j][(*PlayerA).posXBomba[k]]=' ';

        }
    }

    (*PlayerA).numBombas++;


}






int main()
{
    if(evento.type == ALLEGRO_EVENT_TIMER)
    {
        /*


        LOOP DO JOGO


        */




        if(	bombsplaced>1)
        {
            timerBomba[k]++;//contador de frames para cada bomba colocada
            if (cont_bomba>=90)
            {
                explode(k);
            }

        }

        cont_tempo++;//contador de frames
    }

}
