#ifndef VIRTUAL_TERMINAL_PROCESSING_H
#define VIRTUAL_TERMINAL_PROCESSING_H

#ifdef _WIN32
#include <windows.h>

static void enable_virtual_terminal() {
    static int enabled = 0;
    if (enabled) return;
    enabled = 1;

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE || hOut == NULL) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

#else
static void enable_virtual_terminal() {}
#endif

#endif
