#include "WindowsPty.h"
#include "Engine.h"
#include "ProperInput.h"
#include "VirtualTerminalProcessing.h"

int PseudoPTY_Read(WindowsPty* pty, char* buffer, int len);
int PseudoPTY_Write(WindowsPty* pty, const char* data, int len);

WindowsPty* PseudoPTY_Create(const char* command, int cols, int rows) {
    enable_virtual_terminal();
    WindowsPty* pty = WindowsPty_Create(command, cols, rows);
    if (pty) {
        flush_pty_output(PseudoPTY_Read, pty);
        interactive_input(PseudoPTY_Write, PseudoPTY_Read, pty);
    }
    return pty;
}

void PseudoPTY_Destroy(WindowsPty* pty) {
    WindowsPty_Destroy(pty);
}

int PseudoPTY_Write(WindowsPty* pty, const char* data, int len) {
    return WindowsPty_Write(pty, data, len);
}

int PseudoPTY_Read(WindowsPty* pty, char* buffer, int len) {
    return WindowsPty_Read(pty, buffer, len);
}

void PseudoPTY_Flush(WindowsPty* pty) {
    flush_pty_output(PseudoPTY_Read, pty);
}

void PseudoPTY_Interactive(WindowsPty* pty) {
    interactive_input(PseudoPTY_Write, PseudoPTY_Read, pty);
}
