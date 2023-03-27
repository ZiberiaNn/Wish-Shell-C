#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#define MAX_HISTORY 10
#define MAX_LINE_LENGTH 256

int main() {
    char ch;
    char history[MAX_HISTORY][MAX_LINE_LENGTH] = {0};
    char line[MAX_LINE_LENGTH] = {0};

    int history_idx = 0;
    int line_idx = 0;
    int history_count = 0;
    struct termios old_term, new_term;

    // Save the current terminal settings
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;

    // Set the terminal to raw mode
    new_term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

    // Unending loop to read input
    while (1) {
        ch = getchar();

        if (ch == '\033') { // Escape sequence
            ch = getchar(); // '[' character
            ch = getchar(); // Arrow key code

            if (ch == 'A') { // Up arrow key
                if (history_count > 0 && history_idx > 0) {
                    history_idx--;
                    printf("\033[2K\r%s", history[history_idx]);
                    fflush(stdout);
                    line_idx = strlen(history[history_idx]);
                }
            } else if (ch == 'B') { // Down arrow key
                if (history_count > 0 && history_idx < history_count - 1) {
                    history_idx++;
                    printf("\033[2K\r%s", history[history_idx]);
                    fflush(stdout);
                    line_idx = strlen(history[history_idx]);
                }
            }
        } else if (ch == '\n') { // Enter key
            putchar('\n');

            if (line_idx > 0) {
                // Copy the current line to the history
                strcpy(history[history_count % MAX_HISTORY], "");
                strncpy(history[history_count % MAX_HISTORY], line, line_idx);
                history_count++;
                history_idx = history_count % MAX_HISTORY;
            }

            line_idx = 0;
            printf("> ");
            fflush(stdout);
        } else if (ch == 127) { // Backspace key
            if (line_idx > 0) {
                putchar('\b');
                putchar(' ');
                putchar('\b');
                line_idx--;
            }
        } else if (ch >= 32 && ch <= 126) { // Printable character
            if (line_idx < MAX_LINE_LENGTH - 1) {
                putchar(ch);
                line[line_idx] = ch;
                line_idx++;
            }
        }
    }

    // Restore the terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);

    return 0;
}