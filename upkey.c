#include <curses.h>

int main() {
  
    int ch;
    
    /* Curses Initialisations */ 
    initscr();                  /* curses initialization */
    keypad(stdscr, TRUE);       /* enable KEY_UP/KEY_DOWN/KEY_RIGHT/KEY_LEFT */
    noecho();                   /* prevent displaying if other keys */
    
    printw("Press # to Exit\n");
 
    while ((ch = getch ()) != '#') {
        switch (ch) {
            case KEY_UP:
                printw ("\nUp Arrow");
                break;
            case KEY_DOWN:
                printw ("\nDown Arrow");
                break;
            case KEY_RIGHT:
                printw ("\nLeft Arrow");
                break;
            case KEY_LEFT:
                printw ("\nRight Arrow");
                break;
        }
    }
 
    return 0;
}