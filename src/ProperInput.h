#ifndef PROPER_INPUT_H
#define PROPER_INPUT_H

#ifdef _WIN32
#include <stdio.h>
#include <conio.h>

static void flush_pty_output(int (*Pty_ReadFunc)(void*, char*, int), void* pty) {
    char buffer[512];
    int n;
    while ((n = Pty_ReadFunc(pty, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[n] = '\0';
        fputs(buffer, stdout);
        fflush(stdout);
    }
}

static void interactive_input(int (*Pty_WriteFunc)(void*, const char*, int), void* pty) {
    while (1) {
        if (_kbhit()) {
            int c = _getch();

            if (c == 3) { // Ctrl+C
                const char ctrl_c = 3;
                Pty_WriteFunc(pty, &ctrl_c, 1);
                fputs("^C\n", stdout);
                fflush(stdout);
            } else if (c == 13) { // Enter
                const char enter = '\r';
                Pty_WriteFunc(pty, &enter, 1);
                fputs("\n", stdout);
                fflush(stdout);
            } else if (c == 8) { // Backspace
                const char bs = 8;
                Pty_WriteFunc(pty, &bs, 1);
                fputs("\b \b", stdout);
                fflush(stdout);
            } else { // Normal characters
                char ch = (char)c;
                Pty_WriteFunc(pty, &ch, 1);
                fputc(ch, stdout);
                fflush(stdout);
            }
        }

        flush_pty_output(Pty_ReadFunc, pty);
    }
}

#else

static void flush_pty_output(void* pty) {}
static void interactive_input(void* pty) {}

#endif

#endif
