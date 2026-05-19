#include <iostream>
#include <vector>
#include <string>
#include <ncurses.h>
using namespace std;
int main()
{
    // Initiate the screen!
    initscr(); 

    // Detect all key combinations by value!
    raw();

    // Enable arrow keys!
    keypad(stdscr, TRUE);

    // Typed characters do not appear on-to the screen!
    noecho();

    // Makes the current cursor invisible!
    curs_set(0);

    // Co-ordinates of the pointer!
    int pointerRow = 0, pointerColumn = 0;

    // Choices for the user!
    vector<string> choices = {"Playlists", "Queue", "Exit"};

    // Declaring a color pair, and setting it for the entire program!
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    wbkgd(stdscr, COLOR_PAIR(1));

    clear();
    refresh();

    while(true) {
        clear();
        
        // Updating the position of the pointer!
        mvprintw(pointerRow, pointerColumn, "> ");
        
        // Choices for the user!
        for(int index = 0; index < choices.size(); index++) {
            if(index != pointerRow) {
                mvprintw(index, 2, "%s", choices[index].c_str());
            }
            else {
                attron(A_REVERSE);
                mvprintw(index, 2, "%s", choices[index].c_str());
                attroff(A_REVERSE);
            }
        }
        
        refresh();

        int userInput = getch();

        if(userInput == KEY_UP) {
            // Only move the pointer up-ward when it's on-screen, else don't!
            if(pointerRow > 0) {
                pointerRow--;
            }
            else {
                continue;
            }
        }

        else if(userInput == KEY_DOWN) {
            // Only move the pointer down-ward when it's in bounds of choices, else don't!
            if(pointerRow < choices.size() - 1) {
                pointerRow++;
            }
            else {
                continue;
            }
        }

        // Ctrl + Q to exit the program!
        else if(userInput == 17) {
            break;
        }

        else {
            continue;
        }
    }

    // Terminate the screen!
    endwin();
}