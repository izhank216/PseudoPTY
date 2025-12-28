#ifndef PROPER_INPUT_H
#define PROPER_INPUT_H

#ifdef _WIN32
#include <stdio.h>

static void flush_pty_output(int (*Pty_ReadFunc)(void*, char*, int), void* pty) {
    char buffer[256];
    int n = Pty_ReadFunc(pty, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);
        fflush(stdout);
    }
}

static void wake_shell(int (*Pty_WriteFunc)(void*, const char*, int), void* pty) {
    Pty_WriteFunc(pty, "\n", 1);
}

#else

static void flush_pty_output(void* pty) {}
static void wake_shell(void* pty) {}

#endif

#endif
