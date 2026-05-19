#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <chrono>
#include <ncurses.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
using namespace std;

void redrawScreen(WINDOW *innerWindow, int innerRow, int innerColumn, vector<string> &choices){
    wclear(innerWindow);
    box(innerWindow, 0, 0);

    mvwprintw(innerWindow, innerRow + 1, innerColumn + 1, "> ");
    for (int index = 0; index < choices.size(); index++) {
        if (index != innerRow) {
            mvwprintw(innerWindow, index + 1, 3, "%s", choices[index].c_str());
        } else {
            wattron(innerWindow, A_REVERSE);
            mvwprintw(innerWindow, index + 1, 3, "%s", choices[index].c_str());
            wattroff(innerWindow, A_REVERSE);
        }
    }
    wrefresh(innerWindow);
}

void animation(WINDOW *win, vector<int> &ys, vector<int> &xs) {
    for (int i = 0; i < ys.size(); i++) {
        mvwprintw(win, ys[i], xs[i], " ");
        wrefresh(win);
        napms(40);
        mvwprintw(win, ys[i], xs[i], "$");
    }
    wrefresh(win);
}

void createNewPlaylist(WINDOW *innerWindow, char playlistName[16], vector<string> &playlists) {
    echo();
    curs_set(1);
    mvwprintw(innerWindow, 5, 2, "Playlist Name: ");
    wmove(innerWindow, 5, 17);

    wgetnstr(innerWindow, playlistName, 16);

    playlists.push_back(playlistName);

    noecho();
    curs_set(0);
}

void displayAllPlaylists(WINDOW *innerWindow, vector<string> &playlists, int playlistPointer) {
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    mvwprintw(innerWindow, 1, 2, "Your Playlists:");

    for (int index = 0; index < playlists.size(); index++){
        if (index != playlistPointer){
            mvwprintw(innerWindow, 3 + index, 4, "%s", playlists[index].c_str());
        } else {
            mvwprintw(innerWindow, 3 + index, 2, ">");
            wattron(innerWindow, A_REVERSE);

            mvwprintw(innerWindow, 3 + index, 4, "%s", playlists[index].c_str());

            wattroff(innerWindow, A_REVERSE);
        }
    }

    wrefresh(innerWindow);
}

void addToQueue(WINDOW *innerWindow, vector<string> &queue, char songPath[100]) {
    echo();

    curs_set(1);

    wclear(innerWindow);
    box(innerWindow, 0, 0);

    mvwprintw(innerWindow, 5, 2, "Song Path:");
    wmove(innerWindow, 5, 14);

    wgetnstr(innerWindow, songPath, 100);
    queue.push_back(songPath);

    noecho();
    curs_set(0);
}

void deleteFromQueue(WINDOW *innerWindow, vector<string> &queue, char songName[100])
{
    echo();
    curs_set(1);

    wclear(innerWindow);
    box(innerWindow, 0, 0);

    mvwprintw(innerWindow, 5, 2, "Song Path:");

    wmove(innerWindow, 5, 14);

    wrefresh(innerWindow);

    wgetnstr(innerWindow, songName, 100);

    auto it = find(queue.begin(), queue.end(), songName);

    wclear(innerWindow);
    box(innerWindow, 0, 0);

    if (it != queue.end()) {
        queue.erase(it);

        mvwprintw(innerWindow, 5, 2, "Song deleted successfully!");
    } else {
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

    mvwprintw(innerWindow, 1, 2, "Current Queue:");

    for (int i = 0; i < queue.size(); i++) {
        if (i != queuePointer) {
            mvwprintw(innerWindow, 3 + i, 4, "%s", queue[i].c_str());
        } else {
            mvwprintw(innerWindow, 3 + i, 2, ">");

            wattron(innerWindow, A_REVERSE);

            mvwprintw(innerWindow, 3 + i, 4, "%s", queue[i].c_str());

            wattroff(innerWindow, A_REVERSE);
        }
    }
    wrefresh(innerWindow);
}

void playSong(WINDOW *innerWindow, string songPath)
{
    Mix_Music *song = Mix_LoadMUS(songPath.c_str());

    if (song == NULL)
    {
        wclear(innerWindow);

        box(innerWindow, 0, 0);

        mvwprintw(innerWindow, 5, 2, "Failed to load song!");
        mvwprintw(innerWindow, 7, 2, "%s", Mix_GetError());

        wrefresh(innerWindow);
        wgetch(innerWindow);
        return;
    }

    Mix_PlayMusic(song, 1);

    while (Mix_PlayingMusic())
    {
        wclear(innerWindow);

        box(innerWindow, 0, 0);

        mvwprintw(innerWindow, 2, 2, "NOW PLAYING:");

        mvwprintw(innerWindow, 4, 4, "%s", songPath.c_str());

        mvwprintw(innerWindow, 7, 2, "Press Q to stop playback");

        wrefresh(innerWindow);
        timeout(100);

        int input = getch();
        if (input == 'q' || input == 'Q')
        {
            Mix_HaltMusic();
            break;
        }

        this_thread::sleep_for(
            chrono::milliseconds(100));
    }
    Mix_FreeMusic(song);
}

int main()
{
    initscr();

    SDL_Init(SDL_INIT_AUDIO);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    raw();

    keypad(stdscr, TRUE);

    noecho();

    curs_set(0);

    int xm, ym;

    getmaxyx(stdscr,ym,xm);

    int winHeight = 25;
    int winWidth = xm - 10;

    int startx = (xm - winWidth) / 2;

    int starty = (ym - winHeight) / 2;

    WINDOW *win = newwin(winHeight,winWidth,starty,startx);

    box(win, 0, 0);

    vector<int> ys;
    vector<int> xs;

    int spacing = 10;
    int letterWidth = 7;

    int totalWidth = (4*spacing) + letterWidth;

    int sx = (winWidth - totalWidth)/2;
    int sy = winHeight/2 + 4;
    // ================= M =================
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
    // ================= U =================
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
    // ================= S =================
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
    // ================= I =================
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
    // ================= X =================
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
    animation(win,ys,xs);
    napms(800);
    delwin(win);
    clear();
    refresh();

    int pointerRow = 0;
    int pointerColumn = 0;

    vector<string> choices =
        {
            "Playlists",
            "Queue",
            "Exit"};

    vector<string> playlists;

    vector<string> queue;

    start_color();

    init_pair(1,COLOR_WHITE,COLOR_BLACK);

    wbkgd(stdscr,COLOR_PAIR(1));

    clear();

    refresh();

    while (true)
    {
        clear();

        mvprintw(pointerRow,pointerColumn,"> ");

        for (int index = 0; index < choices.size(); index++) {
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
            if (pointerRow > 0)
            {
                pointerRow--;
            }
        }

        else if (userInput == KEY_DOWN)
        {
            if (pointerRow < choices.size() - 1)
            {
                pointerRow++;
            }
        }

        else if (userInput == KEY_ENTER || userInput == '\n')
        {
            int innerRow = 0;
            int innerColumn = 0;

            WINDOW *innerWindow = newwin(17,50,0,0);
            box(innerWindow,0,0);
            keypad(innerWindow,TRUE);

            vector<string> playlistChoices =
                {
                    "Create new playlist",
                    "Open existing playlist",
                    "Exit playlists"};

            vector<string> queueChoices =
                {
                    "Add song path",
                    "Delete from queue",
                    "View current queue",
                    "Play selected song",
                    "Delete current queue",
                    "Exit queue"};

            if (pointerRow != choices.size() - 1)
            {
                if (choices[pointerRow] == "Playlists")
                {
                    while (true)
                    {
                        redrawScreen(innerWindow,innerRow,innerColumn,playlistChoices);

                        int innerUserInput = wgetch(innerWindow);

                        if (innerUserInput == KEY_UP)
                        {
                            if (innerRow > 0)
                            {
                                innerRow--;
                            }
                        }

                        else if (innerUserInput == KEY_DOWN)
                        {
                            if (innerRow < playlistChoices.size() - 1)
                            {
                                innerRow++;
                            }
                        }

                        else if (innerUserInput == KEY_ENTER || innerUserInput == '\n')
                        {
                            if (innerRow == 0)
                            {
                                char playlistName[16];
                                createNewPlaylist(innerWindow,playlistName,playlists);
                            }

                            else if (innerRow == 1)
                            {
                                if (playlists.empty())
                                {
                                    wclear(innerWindow);

                                    box(innerWindow,0,0);

                                    mvwprintw(innerWindow,5,2,"No playlists created!");

                                    wrefresh(innerWindow);

                                    wgetch(innerWindow);

                                    continue;
                                }

                                int playlistPointer = 0;

                                while (true)
                                {
                                    displayAllPlaylists(innerWindow,playlists,playlistPointer);

                                    int playlistInput = wgetch(innerWindow);

                                    if (playlistInput == KEY_UP && playlistPointer > 0)
                                    {
                                        playlistPointer--;
                                    }

                                    else if (playlistInput == KEY_DOWN && playlistPointer < playlists.size() - 1)
                                    {
                                        playlistPointer++;
                                    }

                                    else if (playlistInput == 26)
                                    {
                                        break;
                                    }
                                }
                            }

                            else
                            {
                                delwin(innerWindow);

                                break;
                            }
                        }
                    }
                }

                else
                {
                    while (true)
                    {
                        redrawScreen(innerWindow, innerRow, innerColumn, queueChoices);

                        int innerUserInput = wgetch(innerWindow);

                        if (innerUserInput == KEY_UP)
                        {
                            if (innerRow > 0)
                            {
                                innerRow--;
                            }
                        }

                        else if (innerUserInput == KEY_DOWN)
                        {
                            if (innerRow <
                                queueChoices.size() - 1)
                            {
                                innerRow++;
                            }
                        }
                        else if (innerUserInput == KEY_ENTER || innerUserInput == '\n')
                        {
                            // Add Song Path

                            if (innerRow == 0)
                            {
                                char songPath[100];
                                addToQueue(innerWindow, queue, songPath);
                            }

                            // Delete From Queue

                            else if (innerRow == 1)
                            {
                                if (queue.empty())
                                {
                                    wclear(innerWindow);

                                    box(innerWindow,0,0);

                                    mvwprintw(innerWindow,5,2,"Queue is empty!");

                                    wrefresh(innerWindow);

                                    wgetch(innerWindow);

                                    continue;
                                }

                                char songPath[100];

                                deleteFromQueue(innerWindow,queue,songPath);
                            }

                            // View Queue

                            else if (innerRow == 2)
                            {
                                if (queue.empty())
                                {
                                    wclear(innerWindow);

                                    box(innerWindow,0,0);

                                    mvwprintw(innerWindow,5,2,"Queue is empty!");

                                    wrefresh(innerWindow);

                                    wgetch(innerWindow);

                                    continue;
                                }

                                int queuePointer = 0;

                                while (true)
                                {
                                    viewQueue(innerWindow,queue,queuePointer);

                                    int queueInput = wgetch(innerWindow);

                                    if (queueInput == KEY_UP && queuePointer > 0)
                                    {
                                        queuePointer--;
                                    }

                                    else if (queueInput == KEY_DOWN &&queuePointer < queue.size() - 1)
                                    {
                                        queuePointer++;
                                    }

                                    else if (queueInput == 26)
                                    {
                                        break;
                                    }
                                }
                            }

                            // Play Selected Song

                            else if (innerRow == 3)
                            {
                                if (queue.empty())
                                {
                                    wclear(innerWindow);

                                    box(innerWindow,0,0);

                                    mvwprintw(innerWindow,5,2,"Queue is empty!");

                                    wrefresh(innerWindow);

                                    wgetch(innerWindow);

                                    continue;
                                }

                                int queuePointer = 0;

                                while (true)
                                {
                                    viewQueue(innerWindow,queue,queuePointer);
                                    int queueInput = wgetch(innerWindow);
                                    if (queueInput == KEY_UP && queuePointer > 0)
                                    {
                                        queuePointer--;
                                    }

                                    else if (queueInput == KEY_DOWN && queuePointer < queue.size() - 1)
                                    {
                                        queuePointer++;
                                    }

                                    else if (queueInput == KEY_ENTER || queueInput == '\n')
                                    {
                                        playSong(innerWindow,queue[queuePointer]);
                                    }

                                    else if (queueInput == 26)
                                    {
                                        break;
                                    }
                                }
                            }

                            // Delete Entire Queue

                            else if (innerRow == 4)
                            {
                                queue.clear();

                                wclear(innerWindow);

                                box(innerWindow,0,0);

                                mvwprintw(innerWindow,5,2,"Queue cleared!");

                                wrefresh(innerWindow);

                                wgetch(innerWindow);
                            }
                            // Exit Queue
                            else
                            {
                                delwin(innerWindow);
                                break;
                            }
                        }
                    }
                }
            }

            else
            {
                break;
            }
        }

        else if (userInput == 17)
        {
            break;
        }
    }

    Mix_CloseAudio();

    SDL_Quit();

    endwin();

    return 0;
}