#include "Pty.h"
#include "ProperInput.h"
#include "VirtualTerminalProcessing.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Pty* Pty_Create(const char* command, int cols, int rows) {
    Pty* pty = (Pty*)malloc(sizeof(Pty));
    if (!pty) return NULL;

    pty->pty = WindowsPty_Create(command, cols, rows);
    if (!pty->pty) {
        free(pty);
        return NULL;
    }

    return pty;
}

void Pty_Destroy(Pty* pty) {
    if (!pty) return;
    WindowsPty_Destroy(pty->pty);
    free(pty);
}

int Pty_Write(Pty* pty, const char* data, int len) {
    return WindowsPty_Write(pty->pty, data, len);
}

int Pty_Read(Pty* pty, char* buffer, int len) {
    return WindowsPty_Read(pty->pty, buffer, len);
}

int main(int argc, char** argv) {
       enable_virtual_terminal(); 
    
    
    if (argc < 3 || strcmp(argv[1], "--terminal") != 0) {
        printf("Usage: %s --terminal <cmd.exe|powershell.exe>\n", argv[0]);
        return 1;
    }

    const char* terminal = argv[2];
    Pty* pty = Pty_Create(terminal, 80, 25);
    if (!pty) {
        printf("Failed to create PTY\n");
        return 1;
    }

       flush_pty_output(Pty_Read, pty);
       interactive_input(Pty_Write, Pty_Read, pty);

    char buffer[512];
    while (1) {
        int n = Pty_Read(pty, buffer, sizeof(buffer) - 1);
        if (n <= 0) break;
        buffer[n] = '\0';
        printf("%s", buffer);

        if (fgets(buffer, sizeof(buffer), stdin)) {
            Pty_Write(pty, buffer, (int)strlen(buffer));
        }
    }

    Pty_Destroy(pty);
    return 0;
}
