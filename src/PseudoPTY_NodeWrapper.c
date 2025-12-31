#include "WindowsPty.h"
#include "Engine.h"
#include "ProperInput.h"
#include "VirtualTerminalProcessing.h"

// Original functions
int PseudoPTY_Read(WindowsPty* pty, char* buffer, int len) {
    return WindowsPty_Read(pty, buffer, len);
}

int PseudoPTY_Write(WindowsPty* pty, const char* data, int len) {
    return WindowsPty_Write(pty, data, len);
}

// --- Wrappers for void* callbacks ---
static int read_wrapper(void* userdata, char* buffer, int len) {
    WindowsPty* pty = (WindowsPty*)userdata;
    return PseudoPTY_Read(pty, buffer, len);
}

static int write_wrapper(void* userdata, const char* data, int len) {
    WindowsPty* pty = (WindowsPty*)userdata;
    return PseudoPTY_Write(pty, data, len);
}

static int read_wrapper2(void* userdata, char* buffer, int len) {
    WindowsPty* pty = (WindowsPty*)userdata;
    return PseudoPTY_Read(pty, buffer, len);
}

// --- Public API ---
WindowsPty* PseudoPTY_Create(const char* command, int cols, int rows) {
    enable_virtual_terminal();
    WindowsPty* pty = WindowsPty_Create(command, cols, rows);
    if (pty) {
        flush_pty_output(read_wrapper, pty);
        interactive_input(write_wrapper, read_wrapper2, pty);
    }
    return pty;
}

void PseudoPTY_Destroy(WindowsPty* pty) {
    WindowsPty_Destroy(pty);
}

void PseudoPTY_Flush(WindowsPty* pty) {
    flush_pty_output(read_wrapper, pty);
}

void PseudoPTY_Interactive(WindowsPty* pty) {
    interactive_input(write_wrapper, read_wrapper2, pty);
}
