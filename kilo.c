#include <ctype.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

struct termios orig_termios; // Saved copy of users ternimal before use

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    
    atexit(disableRawMode); //! For future refrence this is a very good function to know "atexit()"

    struct termios raw = orig_termios;
    tcgetattr(STDIN_FILENO, &raw);

    //Turning off terminal flags
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)) {
            printf("%d\r\n", c);
        } else {
            printf("%d ('%c')\r\n", c, c);
        }
    }
    return 1;
}