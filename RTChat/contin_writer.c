#include <stdlib.h>
#include <stdio.h>

#include <termios.h>
#include <string.h>
#include <unistd.h>
#include "contin_writer.h"

static struct termios stored_settings;

void set_keypress(void)
{
        struct termios new_settings;
        tcgetattr(0,&stored_settings);
        new_settings = stored_settings;

        /* 
                Отключение канонического режима и вывода на экран 
                и установка буфера ввода размером в 1 байт 
        */
        new_settings.c_lflag &= (~ICANON);
        new_settings.c_lflag &= (~ECHO);
        new_settings.c_cc[VTIME] = 0;
        new_settings.c_cc[VMIN] = 1;

        tcsetattr(0,TCSANOW,&new_settings);
        return;
}
void reset_keypress(void)
{
        tcsetattr(0,TCSANOW,&stored_settings);
        return;
}
