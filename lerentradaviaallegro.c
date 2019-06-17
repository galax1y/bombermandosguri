void ler_entrada_allegro(ALLEGRO_EVENT evento, char nome_arquivo[])
{
    if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
    {
        if (strlen(nome_arquivo) <= 50)
        {
            char temporario[50] = {evento.keyboard.unichar, '\0'};
            if (evento.keyboard.unichar == ' ')
            {
                strcat(nome_arquivo, temporario);
            }
            else if (evento.keyboard.unichar >= '0' &&
                     evento.keyboard.unichar <= '9')
            {
                strcat(nome_arquivo, temporario);
            }
            else if (evento.keyboard.unichar >= 'A' &&
                     evento.keyboard.unichar <= 'Z')
            {
                strcat(nome_arquivo, temporario);
            }
            else if (evento.keyboard.unichar >= 'a' &&
                     evento.keyboard.unichar <= 'z')
            {
                strcat(nome_arquivo, temporario);
            }
            else if (evento.keyboard.unichar =='.')
            {
                strcat(nome_arquivo, temporario);
            }
        }

        if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(nome_arquivo) != 0)
        {
            nome_arquivo[strlen(nome_arquivo) - 1] = '\0';
        }
    }
