#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include "TTY_manager.h"

static struct termios stored_settings;

void unset_canonical(void)
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
void set_canonical(void)
{
        tcsetattr(0,TCSANOW,&stored_settings);
        return;
}




