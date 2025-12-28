#ifndef PROPER_INPUT_H
#define PROPER_INPUT_H

#ifdef _WIN32
#include <stdio.h>

static void flush_pty_output(int (*Pty_ReadFunc)(void*, char*, int), void* pty) {
    char buffer[512];
    int n;
    while ((n = Pty_ReadFunc(pty, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[n] = '\0';
        fputs(buffer, stdout);
        fflush(stdout);
    }
}

#else

static void flush_pty_output(void* pty) {}

#endif

#endif
