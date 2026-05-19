#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ncurses.h>
using namespace std;
void redrawScreen(WINDOW *innerWindow, int innerRow, int innerColumn, vector<string> &choices)
{
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    mvwprintw(innerWindow, innerRow + 1, innerColumn + 1, "> ");
    for (int index = 0; index < choices.size(); index++)
    {
        if (index != innerRow)
        {
            mvwprintw(innerWindow, index + 1, 3, "%s", choices[index].c_str());
        }
        else
        {
            wattron(innerWindow, A_REVERSE);
            mvwprintw(innerWindow, index + 1, 3, "%s", choices[index].c_str());
            wattroff(innerWindow, A_REVERSE);
        }
    }
    wrefresh(innerWindow);
}

void animation(WINDOW *win, vector<int> &ys, vector<int> &xs)
{
    for (int i = 0; i < ys.size(); i++)
    {
        mvwprintw(win, ys[i], xs[i], " "); // the cursor moves without any charecter
        wrefresh(win);                     // To update the changes in the memory
        napms(40);                         // Have to learn what this is

        mvwprintw(win, ys[i], xs[i], "$"); // The cursor leaves behind a trail of '$'s
    }
    wrefresh(win); // To update the change in the window to the terminal
}

void createNewPlaylist(WINDOW *innerWindow, char playlistName[16], vector<string> &playlists)
{
    // Allowing user to see what they type!
    echo();
    curs_set(1);

    // Taking user-input, storing playlist name!
    mvwprintw(innerWindow, 5, 2, "Playlist Name: ");
    wmove(innerWindow, 5, 17);
    wgetnstr(innerWindow, playlistName, 16);
    playlists.push_back(playlistName);

    noecho();
    curs_set(0);
}

void displayAllPlaylists(WINDOW *innerWindow, vector<string> &playlists, int playlistPointer)
{
    // The original innerWindow had static-data, easy to recover! So we can safely use it again to display all the playlists!
    wclear(innerWindow);
    box(innerWindow, 0, 0);

    mvwprintw(innerWindow, 1, 2, "Your Playlists: ");

    for (int index = 0; index < playlists.size(); index++)
    {
        if (index != playlistPointer)
        {
            mvwprintw(innerWindow, 3 + index, 4, "%s", playlists[index].c_str());
        }
        else
        {
            mvwprintw(innerWindow, 3 + index, 2, ">");
            wattron(innerWindow, A_REVERSE);
            mvwprintw(innerWindow, 3 + index, 4, "%s", playlists[index].c_str());
            wattroff(innerWindow, A_REVERSE);
        }
    }
    wrefresh(innerWindow);
}
void addToQueue(WINDOW *innerWindow, vector<string> &queue, char songName[50])
{
    echo();
    curs_set(1);

    // Taking user-input, storing song name!
    mvwprintw(innerWindow, 5, 2, "Song Name: ");
    wmove(innerWindow, 5, 17);
    wgetnstr(innerWindow, songName, 50);
    queue.push_back(songName);

    noecho();
    curs_set(0);
}
void deleteFromQueue(WINDOW *innerWindow, vector<string> &queue, char songName[50])
{
    echo();
    curs_set(1);

    wclear(innerWindow);
    box(innerWindow, 0, 0);
    mvwprintw(innerWindow, 5, 2, "Song Name: ");
    wmove(innerWindow, 5, 13);
    wrefresh(innerWindow);
    wgetnstr(innerWindow, songName, 50);
    auto it = find(queue.begin(), queue.end(), songName);

    wclear(innerWindow);
    box(innerWindow, 0, 0);

    if (it != queue.end())
    {
        queue.erase(it);
        mvwprintw(innerWindow, 5, 2, "Song deleted successfully!");
    }
    else
    {
        mvwprintw(innerWindow, 5, 2, "Song not found in queue!");
    }
    mvwprintw(innerWindow, 7, 2, "Press any-key to continue!");
    wrefresh(innerWindow);
    wgetch(innerWindow);
    noecho();
    curs_set(0);
}
void viewQueue(WINDOW *innerWindow, vector<string> &queue, int queuePointer)
{
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    mvwprintw(innerWindow, 1, 2, "Current Queue: ");
    for (int i = 0; i < queue.size(); i++)
    {
        if (i != queuePointer)
        {
            mvwprintw(innerWindow, 3 + i, 4, "%s", queue[i].c_str());
        }
        else
        {
            mvwprintw(innerWindow, 3 + i, 2, ">");
            wattron(innerWindow, A_REVERSE);
            mvwprintw(innerWindow, 3 + i, 4, "%s", queue[i].c_str());
            wattroff(innerWindow, A_REVERSE);
        }
    }
    wrefresh(innerWindow);
}
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

    int xm, ym;
    getmaxyx(stdscr, ym, xm);

    int winHeight = 25;
    int winWidth = xm - 10;
    int startx = (xm - winWidth) / 2;
    int starty = (ym - winHeight) / 2;

    // initialize window
    WINDOW *win = newwin(winHeight, winWidth, starty, startx);
    box(win, 0, 0);

    vector<int> ys;
    vector<int> xs;
    int spacing = 10;
    int letterWidth = 7;
    // totalWidth is used to determine the total space required to write MUSIX on the terminal
    int totalWidth = (4 * spacing) + letterWidth;
    // sx and sy use the totalWidth to accurately center the text
    int sx = (winWidth - totalWidth) / 2;
    int sy = winHeight / 2 + 4;
    // =============== M ===============
    for (int i = 0; i < 8; i++)
    {
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
    for (int i = 0; i < 8; i++)
    {
        ys.push_back(sy - i);
        xs.push_back(sx + 6);
    }
    // =============== U ===============
    sx += spacing;
    for (int i = 0; i < 8; i++)
    {
        ys.push_back(sy - i);
        xs.push_back(sx);
    }
    for (int i = 1; i <= 4; i++)
    {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    for (int i = 7; i >= 0; i--)
    {
        ys.push_back(sy - i);
        xs.push_back(sx + 5);
    }
    // =============== S ===============
    sx += spacing;
    for (int i = 5; i >= 1; i--)
    {
        ys.push_back(sy - 7);
        xs.push_back(sx + i);
    }
    for (int i = 6; i >= 4; i--)
    {
        ys.push_back(sy - i);
        xs.push_back(sx);
    }
    for (int i = 1; i <= 4; i++)
    {
        ys.push_back(sy - 3);
        xs.push_back(sx + i);
    }
    for (int i = 2; i >= 0; i--)
    {
        ys.push_back(sy - i);
        xs.push_back(sx + 5);
    }
    for (int i = 4; i >= 1; i--)
    {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    // =============== I ===============
    sx += spacing;
    for (int i = 0; i < 6; i++)
    {
        ys.push_back(sy - 7);
        xs.push_back(sx + i);
    }
    for (int i = 6; i >= 0; i--)
    {
        ys.push_back(sy - i);
        xs.push_back(sx + 3);
    }
    for (int i = 0; i < 6; i++)
    {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    // =============== X ===============
    sx += spacing;
    for (int i = 0; i < 8; i++)
    {
        ys.push_back(sy - i);
        xs.push_back(sx + i);
    }
    for (int i = 0; i < 8; i++)
    {
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

    // List of all playlists!
    vector<string> playlists;

    vector<string> queue;

    // Declaring a color pair, and setting it for the entire program!
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    wbkgd(stdscr, COLOR_PAIR(1));

    clear();
    refresh();

    while (true)
    {
        clear();
        // Updating the position of the pointer!
        mvprintw(pointerRow, pointerColumn, "> ");

        // Choices for the user!
        for (int index = 0; index < choices.size(); index++)
        {
            if (index != pointerRow)
            {
                mvprintw(index, 2, "%s", choices[index].c_str());
            }
            else
            {
                attron(A_REVERSE);
                mvprintw(index, 2, "%s", choices[index].c_str());
                attroff(A_REVERSE);
            }
        }

        refresh();

        int userInput = getch();

        if (userInput == KEY_UP)
        {
            // Only move the pointer up-ward when it's on-screen, else don't!
            if (pointerRow > 0)
            {
                pointerRow--;
            }
            else
            {
                continue;
            }
        }
        else if (userInput == KEY_DOWN)
        {

            // Only move the pointer down-ward when it's in bounds of choices, else don't!
            if (pointerRow < choices.size() - 1)
            {
                pointerRow++;
            }
            else
            {
                continue;
            }
        }
        else if (userInput == KEY_ENTER || userInput == '\n')
        {

            // A pointer for the inner window!
            int innerRow = 0, innerColumn = 0;

            // Creates a new-window with height 11, width 23, starting at Y co-ordinate 0 and X co-ordinate 0!
            WINDOW *innerWindow = newwin(17, 37, 0, 0);

            box(innerWindow, 0, 0);

            keypad(innerWindow, TRUE);

            // Choices in the playlist's window!
            vector<string> playlistChoices = {"Create new playlist", "Open existing playlist", "Exit playlists"};

            // Choices in the queue's window!
            vector<string> queueChoices = {"Add to queue", "Delete from queue", "View current queue", "Delete current queue", "Exit queue"};

            if (pointerRow != choices.size() - 1)
            {
                // Handling playlists!
                if (choices[pointerRow] == "Playlists")
                {

                    while (true)
                    {
                        // Re-draw after every single window-update!
                        redrawScreen(innerWindow, innerRow, innerColumn, playlistChoices);

                        int innerUserInput = wgetch(innerWindow);

                        if (innerUserInput == KEY_UP)
                        {
                            // Only move the pointer up-ward when it's on-screen, else don't!
                            if (innerRow > 0)
                            {
                                innerRow--;
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else if (innerUserInput == KEY_DOWN)
                        {
                            // Only move the pointer down-ward when it's in bounds of choices, else don't!
                            if (innerRow < playlistChoices.size() - 1)
                            {
                                innerRow++;
                            }
                            else
                            {
                                continue;
                            }
                        }
                        else if (innerUserInput == KEY_ENTER || innerUserInput == '\n')
                        {
                            // Creating a new playlist!
                            if (innerRow == 0)
                            {
                                char playlistName[16];
                                createNewPlaylist(innerWindow, playlistName, playlists);
                            }
                            // Modify existing playlists!
                            else if (innerRow == 1)
                            {
                                // Handling the case when no playlists exist!
                                if (playlists.empty())
                                {
                                    wclear(innerWindow);
                                    box(innerWindow, 0, 0);
                                    mvwprintw(innerWindow, 5, 2, "No playlists created! Press any-key to go back!");

                                    // To see the message temporarily!
                                    wrefresh(innerWindow);
                                    wgetch(innerWindow);
                                    continue;
                                }

                                // Pointer for the playlist selection!
                                int playlistPointer = 0;

                                while (true)
                                {
                                    // Re-draw window after every update!
                                    displayAllPlaylists(innerWindow, playlists, playlistPointer);

                                    int playlistInput = wgetch(innerWindow);

                                    if (playlistInput == KEY_UP && playlistPointer > 0)
                                    {
                                        playlistPointer--;
                                    }
                                    else if (playlistInput == KEY_DOWN && playlistPointer < playlists.size() - 1)
                                    {
                                        playlistPointer++;
                                    }
                                    else if (playlistInput == KEY_ENTER || playlistInput == '\n')
                                    {
                                        // Clearing the inner-window and using it to display selected playlist!
                                        wclear(innerWindow);
                                        box(innerWindow, 0, 0);
                                        mvwprintw(innerWindow, 1, 2, "Opened playlist: %s", playlists[playlistPointer].c_str());
                                        mvwprintw(innerWindow, 2, 2, "Press any-key to go back!");

                                        // To see that a playlist has been opened!
                                        wrefresh(innerWindow);
                                        wgetch(innerWindow);
                                    }
                                    // Ctrl + Z to exit playlist selection!
                                    else if (playlistInput == 26)
                                    {
                                        break;
                                    }
                                }
                            }
                            // Exit!
                            else
                            {
                                delwin(innerWindow);
                                break;
                            }
                        }
                        else
                        {
                            continue;
                        }
                    }
                } /*Handling queues!*/
                else
                {
                    while (true)
                    {
                        // Re-draw after every single window-update!
                        redrawScreen(innerWindow, innerRow, innerColumn, queueChoices);

                        int innerUserInput = wgetch(innerWindow);

                        if (innerUserInput == KEY_UP)
                        {
                            // Only move the pointer up-ward when it's on-screen, else don't!
                            if (innerRow > 0)
                            {
                                innerRow--;
                            }
                            else
                            {
                                continue;
                            }
                        }

                        else if (innerUserInput == KEY_DOWN)
                        {
                            // Only move the pointer down-ward when it's in bounds of choices, else don't!
                            if (innerRow < queueChoices.size() - 1)
                            {
                                innerRow++;
                            }
                            else
                            {
                                continue;
                            }
                        }

                        else if (innerUserInput == KEY_ENTER || innerUserInput == '\n')
                        {
                            // Add to queue!
                            if (innerRow == 0)
                            {
                                char songName[50];

                                addToQueue(innerWindow,
                                           queue,
                                           songName);
                            }

                            // Delete from queue!
                            else if (innerRow == 1)
                            {
                                // Handling empty queue!
                                if (queue.empty())
                                {
                                    wclear(innerWindow);

                                    box(innerWindow, 0, 0);

                                    mvwprintw(innerWindow,
                                              5,
                                              2,
                                              "Queue is empty! Press any-key!");

                                    wrefresh(innerWindow);

                                    wgetch(innerWindow);

                                    continue;
                                }

                                char songName[50];

                                deleteFromQueue(innerWindow,
                                                queue,
                                                songName);
                            }

                            // View current queue!
                            else if (innerRow == 2)
                            {
                                // Handling empty queue!
                                if (queue.empty())
                                {
                                    wclear(innerWindow);

                                    box(innerWindow, 0, 0);

                                    mvwprintw(innerWindow,
                                              5,
                                              2,
                                              "Queue is empty! Press any-key to go back!");

                                    wrefresh(innerWindow);

                                    wgetch(innerWindow);

                                    continue;
                                }

                                int queuePointer = 0;

                                while (true)
                                {
                                    // Re-draw queue after every update!
                                    viewQueue(innerWindow,
                                              queue,
                                              queuePointer);

                                    int queueInput = wgetch(innerWindow);

                                    if (queueInput == KEY_UP &&
                                        queuePointer > 0)
                                    {
                                        queuePointer--;
                                    }

                                    else if (queueInput == KEY_DOWN &&
                                             queuePointer < queue.size() - 1)
                                    {
                                        queuePointer++;
                                    }

                                    else if (queueInput == KEY_ENTER ||
                                             queueInput == '\n')
                                    {
                                        wclear(innerWindow);

                                        box(innerWindow, 0, 0);

                                        mvwprintw(innerWindow,
                                                  1,
                                                  2,
                                                  "Playing: %s",
                                                  queue[queuePointer].c_str());

                                        mvwprintw(innerWindow,
                                                  2,
                                                  2,
                                                  "Press any-key to go back!");

                                        wrefresh(innerWindow);

                                        wgetch(innerWindow);
                                    }

                                    // Ctrl + Z exits queue view
                                    else if (queueInput == 26)
                                    {
                                        break;
                                    }
                                }
                            }

                            // Delete current queue!
                            else if (innerRow == 3)
                            {
                                queue.clear();

                                wclear(innerWindow);

                                box(innerWindow, 0, 0);

                                mvwprintw(innerWindow,
                                          5,
                                          2,
                                          "Queue cleared! Press any-key!");

                                wrefresh(innerWindow);

                                wgetch(innerWindow);
                            }

                            // Exit queue
                            else
                            {
                                delwin(innerWindow);
                                break;
                            }
                        }

                        else
                        {
                            continue;
                        }
                    }
                }
            }
            else
            {
                break;
            }
        }
        // Ctrl + Q to exit the program!
        else if (userInput == 17)
        {
            break;
        }

        else
        {
            continue;
        }
    }

    // Terminate the screen!
    endwin();
}