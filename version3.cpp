#include <iostream>
#include <vector>
#include <string>
#include <ncurses.h>
using namespace std;
void animation(WINDOW* win, vector<int>& ys, vector<int>& xs) {
    for (int i = 0; i < ys.size(); i++) {
        mvwprintw(win, ys[i], xs[i], " "); // the cursor moves without any charecter
        wrefresh(win); // To update the changes in the memory
        napms(40);  //Have to learn what this is

        mvwprintw(win, ys[i], xs[i], "$"); //The cursor leaves behind a trail of '$'s
    }
    wrefresh(win); //To update the change in the window to the terminal
}
int main() {
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

    int xm, ym;
    getmaxyx(stdscr, ym, xm);

    int winHeight = 25;
    int winWidth = xm - 10;
    int startx = (xm - winWidth)/2;
    int starty = (ym - winHeight)/2;

    //initialize window
    WINDOW* win = newwin(winHeight,winWidth,starty,startx);
    box(win, 0, 0);

    vector<int> ys;
    vector<int> xs;
    int spacing = 10;
    int letterWidth = 7;
    //totalWidth is used to determine the total space required to write MUSIX on the terminal
    int totalWidth = (4 * spacing) + letterWidth;
    //sx and sy use the totalWidth to accurately center the text
    int sx = (winWidth - totalWidth)/2;
    int sy = winHeight/2 + 4;
    // =============== M ===============
    for(int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx);
    }
    ys.push_back(sy - 6);
    xs.push_back(sx + 1);
    ys.push_back(sy - 5);
    xs.push_back(sx + 2);
    ys.push_back(sy - 4);
    xs.push_back(sx + 3);
    ys.push_back(sy - 5);
    xs.push_back(sx + 4);
    ys.push_back(sy - 6);
    xs.push_back(sx + 5);
    for(int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx + 6);
    }
    // =============== U ===============
    sx += spacing;
    for(int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx);
    }
    for(int i = 1; i <= 4; i++) {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    for(int i = 7; i >= 0; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx + 5);
    }
    // =============== S ===============
    sx += spacing;
    for(int i = 5; i >= 1; i--) {
        ys.push_back(sy - 7);
        xs.push_back(sx + i);
    }
    for(int i = 6; i >= 4; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx);
    }
    for(int i = 1; i <= 4; i++) {
        ys.push_back(sy - 3);
        xs.push_back(sx + i);
    }
    for(int i = 2; i >= 0; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx + 5);
    }
    for(int i = 4; i >= 1; i--) {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    // =============== I ===============
    sx += spacing;
    for(int i = 0; i < 6; i++) {
        ys.push_back(sy - 7);
        xs.push_back(sx + i);
    }
    for(int i = 6; i >= 0; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx + 3);
    }
    for(int i = 0; i < 6; i++) {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    // =============== X ===============
    sx += spacing;
    for(int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx + i);
    }
    for(int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx + 7 - i);
    }
    animation(win, ys, xs);
    napms(800);
    delwin(win);
    clear();
    refresh();

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
            } else {
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
            } else {
                continue;
            }
        } else if(userInput == KEY_DOWN) {

            // Only move the pointer down-ward when it's in bounds of choices, else don't!
            if(pointerRow < choices.size() - 1) {
                pointerRow++;
            }
            else {
                continue;
            }
        } else if(userInput == KEY_ENTER || userInput == '\n') {

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
                        box(innerWindow, 0, 0);

                        mvwprintw(innerWindow, innerRow + 1, innerColumn + 1, "> ");

                        for(int index = 0; index < playlistChoices.size(); index++) {
                            if(index != innerRow) {
                                mvwprintw(innerWindow, index + 1, 3, "%s", playlistChoices[index].c_str());
                            } else {
                                wattron(innerWindow, A_REVERSE);
                                mvwprintw(innerWindow, index + 1, 3, "%s", playlistChoices[index].c_str());
                                wattroff(innerWindow, A_REVERSE);
                            }
                        }
                        wrefresh(innerWindow);
                        int innerUserInput = wgetch(innerWindow);

                        if(innerUserInput == KEY_UP) {
                            // Only move the pointer up-ward when it's on-screen, else don't!
                            if(innerRow > 0) {
                                innerRow--;
                            } else {
                                continue;
                            }
                        } else if(innerUserInput == KEY_DOWN) {
                            // Only move the pointer down-ward when it's in bounds of choices, else don't!
                            if(innerRow < playlistChoices.size() - 1) {
                                innerRow++;
                            } else {
                                continue;
                            }
                        } else if(innerUserInput == KEY_ENTER || innerUserInput == '\n') {
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
                        } else {
                            continue;
                        }
                    }
                }/*Handling queues!*/ else {
                    while(true) {
                        wclear(innerWindow);

                        box(innerWindow, 0, 0);
                        mvwprintw(innerWindow, innerRow + 1, innerColumn + 1, "> ");
                        for(int index = 0; index < queueChoices.size(); index++) {
                            if(index != innerRow) {
                                mvwprintw(innerWindow, index + 1, 3, "%s", queueChoices[index].c_str());
                            }
                            else {
                                wattron(innerWindow, A_REVERSE);
                                mvwprintw(innerWindow, index + 1, 3, "%s", queueChoices[index].c_str());
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