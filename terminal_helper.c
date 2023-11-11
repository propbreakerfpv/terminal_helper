#include <termios.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>

#include "terminal_helper.h"

struct termios orig_termios;

char read_timeout(int miliseconds) {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = miliseconds * 1000;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

    if (FD_ISSET(STDIN_FILENO, &fds) != 0) {
        char buf[1];
        read(STDIN_FILENO, &buf, 1);
        return buf[0];
    }
    return -1;
}

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
