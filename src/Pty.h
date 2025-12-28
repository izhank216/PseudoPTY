#ifndef PTY_H
#define PTY_H

#include "WindowsPty.h"

typedef struct {
    WindowsPty* pty;
} Pty;

Pty* Pty_Create(const char* command, int cols, int rows);
void Pty_Destroy(Pty* pty);
int Pty_Write(Pty* pty, const char* data, int len);
int Pty_Read(Pty* pty, char* buffer, int len);

#endif
