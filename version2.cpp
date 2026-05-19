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
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
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

        else if(userInput == KEY_ENTER || userInput == '\n') {
            // A pointer for the inner window!
            int innerRow = 0, innerColumn = 0;
            
            // Creates a new-window with height 11, width 23, starting at Y co-ordinate 0 and X co-ordinate 0!
            WINDOW *innerWindow = newwin(17, 37, 0, 0);
            box(innerWindow, 0, 0);
            keypad(innerWindow, TRUE);

            // Choices in the playlist's window!
            vector<string> playlistChoices = {"Create new playlist", "Open existing playlist", "Exit playlists"};

            // Choices in the queue's window!
            vector<string> queueChoices = {"View current queue", "Delete current queue", "Exit queue"};

            if(pointerRow != choices.size() - 1) {
                // Handling playlists!
                if(choices[pointerRow] == "Playlists") {
                    while(true) {
                        wclear(innerWindow);
                        mvwprintw(innerWindow, innerRow, innerColumn, "> ");
                        for(int index = 0; index < playlistChoices.size(); index++) {
                            if(index != innerRow) {
                                mvwprintw(innerWindow, index, 2, "%s", playlistChoices[index].c_str());
                            }
                            else {
                                wattron(innerWindow, A_REVERSE);
                                mvwprintw(innerWindow, index, 2, "%s", playlistChoices[index].c_str());
                                wattroff(innerWindow, A_REVERSE);
                            }
                        }
                        wrefresh(innerWindow);
    
                        int innerUserInput = wgetch(innerWindow);
                        if(innerUserInput == KEY_UP) {
                            // Only move the pointer up-ward when it's on-screen, else don't!
                            if(innerRow > 0) {
                                innerRow--;
                            }
                            else {
                                continue;
                            }
                        }
                    
                        else if(innerUserInput == KEY_DOWN) {
                            // Only move the pointer down-ward when it's in bounds of choices, else don't!
                            if(innerRow < playlistChoices.size() - 1) {
                                innerRow++;
                            }
                            else {
                                continue;
                            }
                        }

                        else if(innerUserInput == KEY_ENTER || innerUserInput == '\n') {
                            // The other options!
                            if(innerRow != playlistChoices.size() - 1) {
                                // A window for that specific option goes here!
                                continue;
                            }
                            // Exit!
                            else {
                                delwin(innerWindow);
                                break;
                            }
                        }

                        else {
                            continue;
                        }
                    }
                }

                // Handling queues!
                else {
                    while(true) {
                        wclear(innerWindow);
                        mvwprintw(innerWindow, innerRow, innerColumn, "> ");
                        for(int index = 0; index < playlistChoices.size(); index++) {
                            if(index != innerRow) {
                                mvwprintw(innerWindow, index, 2, "%s", queueChoices[index].c_str());
                            }
                            else {
                                wattron(innerWindow, A_REVERSE);
                                mvwprintw(innerWindow, index, 2, "%s", queueChoices[index].c_str());
                                wattroff(innerWindow, A_REVERSE);
                            }
                        }
                        wrefresh(innerWindow);
    
                        int innerUserInput = wgetch(innerWindow);
                        if(innerUserInput == KEY_UP) {
                            // Only move the pointer up-ward when it's on-screen, else don't!
                            if(innerRow > 0) {
                                innerRow--;
                            }
                            else {
                                continue;
                            }
                        }
                    
                        else if(innerUserInput == KEY_DOWN) {
                            // Only move the pointer down-ward when it's in bounds of choices, else don't!
                            if(innerRow < queueChoices.size() - 1) {
                                innerRow++;
                            }
                            else {
                                continue;
                            }
                        }

                        else if(innerUserInput == KEY_ENTER || innerUserInput == '\n') {
                            // The other options!
                            if(innerRow != queueChoices.size() - 1) {
                                // A window for that specific option goes here!
                                continue;
                            }
                            // Exit!
                            else {
                                delwin(innerWindow);
                                break;
                            }
                        }

                        else {
                            continue;
                        }
                    }
                }
            }
            else {
                break;
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