#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#define MAX_HISTORY 10
#define MAX_LINE_LENGTH 256

int main() {
    char ch;
    char *history[MAX_HISTORY] = {NULL};
    int history_idx = 0;
    int line_idx = 0;
    int history_count = 0;
    char *line = (char*)malloc(sizeof(char) * MAX_LINE_LENGTH);
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
                    memset(line, 0, MAX_LINE_LENGTH);
                    strncpy(line, history[history_idx], strlen(history[history_idx]));
                    line_idx = strlen(line);
                    printf("\033[2K\r%s", line);
                    fflush(stdout);
                }
            } else if (ch == 'B') { // Down arrow key
                if (history_count > 0 && history_idx < history_count - 1) {
                    history_idx++;
                    memset(line, 0, MAX_LINE_LENGTH);
                    strncpy(line, history[history_idx], strlen(history[history_idx]));
                    line_idx = strlen(line);
                    printf("\033[2K\r%s", line);
                    fflush(stdout);
                }
            }
        } else if (ch == '\n') { // Enter key
            putchar('\n');

            if (line_idx > 0) {
                // Copy the current line to the history
                if (history_count == MAX_HISTORY) {
                    free(history[0]);
                    memmove(history, history + 1, (MAX_HISTORY - 1) * sizeof(char *));
                    history_count--;
                }
                history[history_count] = malloc((line_idx + 1) * sizeof(char));
                strncpy(history[history_count], line, line_idx);
                history[history_count][line_idx] = '\0';
                history_count++;
                history_idx = history_count;

                printf("Entered command: %s\n", line);
            }

            memset(line, 0, MAX_LINE_LENGTH);
            line_idx = 0;
            printf("> ");
            fflush(stdout);
        } else if (ch == 127) { // Backspace key
            if (line_idx > 0) {
                putchar('\b');
                putchar(' ');
                putchar('\b');
                line_idx--;
                line[line_idx] = 0;
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