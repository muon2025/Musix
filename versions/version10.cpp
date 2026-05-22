#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <map>
#include <thread>
#include <chrono>
#include <ncurses.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
using namespace std;
void redrawScreen(WINDOW *innerWindow, int innerRow, int innerColumn, vector<string> &choices){
    // Old re-draw function!
    // wclear(innerWindow);
    // box(innerWindow, 0, 0);

    // mvwprintw(innerWindow, innerRow + 1, innerColumn + 1, "> ");
    // for (int index = 0; index < choices.size(); index++) {
    //     if (index != innerRow) {
    //         mvwprintw(innerWindow, index + 1, 3, "%s", choices[index].c_str());
    //     } else {
    //         wattron(innerWindow, A_REVERSE);
    //         mvwprintw(innerWindow, index + 1, 3, "%s", choices[index].c_str());
    //         wattroff(innerWindow, A_REVERSE);
    //     }
    // }

    wclear(innerWindow);
    box(innerWindow, 0, 0);

    // Header-bar!
    int windowWidth = getmaxx(innerWindow);
    wattron(innerWindow, COLOR_PAIR(3) | A_BOLD);
    mvwhline(innerWindow, 0, 1, ACS_HLINE, windowWidth - 2); // Re-draw top-border styled!
    mvwprintw(innerWindow, 0, 2, " Menu ");
    wattroff(innerWindow, COLOR_PAIR(3) | A_BOLD);

    for(int index = 0; index < (int)choices.size(); index++) {
        if(index != innerRow) {
            mvwprintw(innerWindow, index + 1, 3, "  %s", choices[index].c_str());
        }
        else {
            mvwprintw(innerWindow, index + 1, 3, ">");
            wattron(innerWindow, COLOR_PAIR(6) | A_BOLD);
            mvwprintw(innerWindow, index + 1, 5, "%s", choices[index].c_str());
            wattroff(innerWindow, COLOR_PAIR(6) | A_BOLD);
        }
    }

    // Footer-control hints!
    wattron(innerWindow, A_DIM);
    mvwprintw(innerWindow, getmaxy(innerWindow) - 2, 2, "Enter: Select | Ctrl+Z: Back");
    wattroff(innerWindow, A_DIM);

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

void playlistIsEmpty(WINDOW *innerWindow) {
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    mvwprintw(innerWindow, 5, 2, "Playlist is empty!");
    wrefresh(innerWindow);
    wgetch(innerWindow);
}

void addSongToPlaylist(WINDOW *innerWindow, vector<string> &songs, char songPath[64]) {
    echo();
    curs_set(1);

    wclear(innerWindow);
    box(innerWindow, 0, 0);

    mvwprintw(innerWindow, 5, 2, "Song path: ");
    wmove(innerWindow, 5, 14);
    wgetnstr(innerWindow, songPath, 64);

    songs.push_back(songPath);

    noecho();
    curs_set(0);
}

void viewPlaylistSongs(WINDOW *innerWindow, vector<string> &songs, int songPointer) {
    wclear(innerWindow);
    box(innerWindow, 0, 0);

    mvwprintw(innerWindow, 1, 2, "Playlist songs: ");
    for(int index = 0; index < songs.size(); index++) {
        if(index != songPointer) {
            mvwprintw(innerWindow, 3 + index, 4, "%s", songs[index].c_str());
        }
        else {
            mvwprintw(innerWindow, 3 + index, 2, ">");
            wattron(innerWindow, A_REVERSE);
            mvwprintw(innerWindow, 3 + index, 4, "%s", songs[index].c_str());
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

void deleteFromQueue(WINDOW *innerWindow, vector<string> &queue, char songName[100]) {
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
        wattron(innerWindow, COLOR_PAIR(4) | A_BOLD);
        mvwprintw(innerWindow, 5, 2, "Song deleted successfully!");
        wattroff(innerWindow, COLOR_PAIR(4) | A_BOLD);
    } 
    else {
        wattron(innerWindow, COLOR_PAIR(5) | A_BOLD);
        mvwprintw(innerWindow, 5, 2, "Song not found in queue!");
        wattroff(innerWindow, COLOR_PAIR(5) | A_BOLD);
    }

    mvwprintw(innerWindow, 7, 2, "Press any-key to continue!");
    wrefresh(innerWindow);
    wgetch(innerWindow);

    noecho();
    curs_set(0);
}

void viewQueue(WINDOW *innerWindow, vector<string> &queue, int queuePointer) {
    wclear(innerWindow);
    box(innerWindow, 0, 0);

    mvwprintw(innerWindow, 1, 2, "Current Queue:");

    for (int i = 0; i < queue.size(); i++) {
        if (i != queuePointer) {
            mvwprintw(innerWindow, 3 + i, 4, "%s", queue[i].c_str());
        } 
        else {
            mvwprintw(innerWindow, 3 + i, 2, ">");
            wattron(innerWindow, A_REVERSE);
            mvwprintw(innerWindow, 3 + i, 4, "%s", queue[i].c_str());
            wattroff(innerWindow, A_REVERSE);
        }
    }
    wrefresh(innerWindow);
}

void shuffleQueue(WINDOW *innerWindow, vector<string> &queue) {
    srand(time(0));
    for (int i = queue.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(queue[i], queue[j]);
    }
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    mvwprintw(innerWindow, 5, 2, "Queue shuffled!");
    mvwprintw(innerWindow, 7, 2, "Press any key to continue.");
    wrefresh(innerWindow);
    wgetch(innerWindow);
}

void reorderQueue(WINDOW *innerWindow, vector<string> &queue) {
    echo();
    curs_set(1);
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    // Display queue with indices
    mvwprintw(innerWindow, 1, 2, "Current Queue:");
    for (int i = 0; i < queue.size(); i++) {
        mvwprintw(innerWindow, 3 + i, 2, "%d: %s", i + 1, queue[i].c_str());
    }
    // Get first index
    char input1[4], input2[4];
    mvwprintw(innerWindow, 3 + queue.size() + 1, 2, "Swap song number: ");
    wmove(innerWindow, 3 + queue.size() + 1, 20);
    wrefresh(innerWindow);
    wgetnstr(innerWindow, input1, 3);
    // Get second index
    mvwprintw(innerWindow, 3 + queue.size() + 2, 2, "With song number: ");
    wmove(innerWindow, 3 + queue.size() + 2, 20);
    wrefresh(innerWindow);
    wgetnstr(innerWindow, input2, 3);
    int pos1 = atoi(input1) - 1;  // convert to 0-based
    int pos2 = atoi(input2) - 1;
    wclear(innerWindow);
    box(innerWindow, 0, 0);

    if (pos1 < 0 || pos1 >= queue.size() || pos2 < 0 || pos2 >= queue.size()) {
        wattron(innerWindow, COLOR_PAIR(5) | A_BOLD);
        mvwprintw(innerWindow, 5, 2, "Invalid song numbers!");
        wattroff(innerWindow, COLOR_PAIR(5) | A_BOLD);
    } 
    else if (pos1 == pos2) {
        mvwprintw(innerWindow, 5, 2, "Same song selected, nothing changed.");
    } 
    else {
        swap(queue[pos1], queue[pos2]);
        wattron(innerWindow, COLOR_PAIR(4) | A_BOLD);
        mvwprintw(innerWindow, 5, 2, "Songs swapped successfully!");
        wattroff(innerWindow, COLOR_PAIR(4) | A_BOLD);
    }

    mvwprintw(innerWindow, 7, 2, "Press any key to continue.");
    wrefresh(innerWindow);
    wgetch(innerWindow);
    noecho();
    curs_set(0);
}

int playSong(WINDOW *innerWindow, vector<string> &songs, int index) {
    string songPath = songs[index];
    Mix_Music *song = Mix_LoadMUS(songPath.c_str());
    if (song == NULL) {
        wclear(innerWindow);
        box(innerWindow, 0, 0);
        wattron(innerWindow, COLOR_PAIR(5) | A_BOLD);
        mvwprintw(innerWindow, 5, 2, "Failed to load song!");
        wattroff(innerWindow, COLOR_PAIR(5) | A_BOLD);
        mvwprintw(innerWindow, 7, 2, "%s", Mix_GetError());
        wrefresh(innerWindow);
        wgetch(innerWindow);
        return index;
    }

    Mix_PlayMusic(song, 1);
    bool paused = false;

    while (Mix_PlayingMusic() || paused) {
        wclear(innerWindow);
        box(innerWindow, 0, 0);

        // Old-logic!
        // mvwprintw(innerWindow, 2, 2, "NOW PLAYING:");
        // mvwprintw(innerWindow, 4, 4, "%s", songPath.c_str());
        // mvwprintw(innerWindow, 6, 2, paused ? "[ PAUSED ]" : "[ PLAYING ]");
        // mvwprintw(innerWindow, 8, 2, "P: Pause | C: Continue | B: Restart | Q: Stop");
        // mvwprintw(innerWindow, 9, 2, "N: Next   | S: Previous");

        // Header!
        wattron(innerWindow, COLOR_PAIR(3) | A_BOLD);
        mvwprintw(innerWindow, 1, 2, " NOW PLAYING ");
        mvwhline(innerWindow, 2, 1, ACS_HLINE, getmaxx(innerWindow) - 2);
        wattroff(innerWindow, COLOR_PAIR(3) | A_BOLD);

        // Song-path!
        wattron(innerWindow, COLOR_PAIR(2));
        mvwprintw(innerWindow, 4, 4, "%s", songPath.c_str());
        wattroff(innerWindow, COLOR_PAIR(2));

        // Status!
        wattron(innerWindow, paused ? COLOR_PAIR(5) : COLOR_PAIR(4));
        mvwprintw(innerWindow, 6, 2, paused ? "[ PAUSED ]" : "[ PLAYING ]");
        wattroff(innerWindow, paused ? COLOR_PAIR(5) : COLOR_PAIR(4));

        // Controls!
        wattron(innerWindow, A_DIM);
        mvwprintw(innerWindow, 8, 2, "P:Pause  C:Continue  B:Restart  Q:Stop");
        mvwprintw(innerWindow, 9, 2, "N:Next   S:Previous");
        wattroff(innerWindow, A_DIM);

        wrefresh(innerWindow);
        timeout(100);

        int input = getch();
        if (input == 'q' || input == 'Q') {
            Mix_HaltMusic();
            Mix_FreeMusic(song);
            return index;
        }
        else if (input == 'p' || input == 'P') {
            if (!paused) { Mix_PauseMusic(); paused = true; }
        }
        else if (input == 'c' || input == 'C') {
            if (paused) { Mix_ResumeMusic(); paused = false; }
        }
        else if (input == 'b' || input == 'B') {
            Mix_RewindMusic();
            paused = false;
        }
        else if (input == 'n' || input == 'N') {
            Mix_HaltMusic();
            Mix_FreeMusic(song);
            if (index + 1 >= songs.size()) {
                wclear(innerWindow);
                box(innerWindow, 0, 0);
                mvwprintw(innerWindow, 5, 2, "End of List!");
                mvwprintw(innerWindow, 7, 2, "Press any key to continue.");
                wrefresh(innerWindow);
                wgetch(innerWindow);
                return index;
            }
            return playSong(innerWindow, songs, index + 1);
        }
        else if (input == 's' || input == 'S') {
            Mix_HaltMusic();
            Mix_FreeMusic(song);
            if (index - 1 < 0) {
                wclear(innerWindow);
                box(innerWindow, 0, 0);
                mvwprintw(innerWindow, 5, 2, "End of List!");
                mvwprintw(innerWindow, 7, 2, "Press any key to continue.");
                wrefresh(innerWindow);
                wgetch(innerWindow);
                return index;
            }
            return playSong(innerWindow, songs, index - 1);
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }

    Mix_FreeMusic(song);
    return index;
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

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_BLACK, COLOR_CYAN);
    wbkgd(stdscr,COLOR_PAIR(1));
    clear();
    refresh();

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
    // Letter M!
    for (int i = 0; i < 8; i++) {
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
    for (int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx + 6);
    }
    // Letter U!
    sx += spacing;
    for (int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx);
    }
    for (int i = 1; i <= 4; i++) {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    for (int i = 7; i >= 0; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx + 5);
    }
    // Letter S!
    sx += spacing;
    for (int i = 5; i >= 1; i--) {
        ys.push_back(sy - 7);
        xs.push_back(sx + i);
    }
    for (int i = 6; i >= 4; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx);
    }
    for (int i = 1; i <= 4; i++) {
        ys.push_back(sy - 3);
        xs.push_back(sx + i);
    }
    for (int i = 2; i >= 0; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx + 5);
    }
    for (int i = 4; i >= 1; i--) {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    // Letter I!
    sx += spacing;
    for (int i = 0; i < 6; i++) {
        ys.push_back(sy - 7);
        xs.push_back(sx + i);
    }
    for (int i = 6; i >= 0; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx + 3);
    }
    for (int i = 0; i < 6; i++) {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    // Letter X!
    sx += spacing;
    for (int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx + i);
    }
    for (int i = 0; i < 8; i++) {
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

    vector<string> choices = {"Playlists", "Queue", "Exit"};
    vector<string> playlists;
    map<string, vector<string>> playlistSongs;
    vector<string> queue;

    while (true) {
        clear();
        // Old-window!
        // mvprintw(pointerRow,pointerColumn,"> ");
        // for (int index = 0; index < choices.size(); index++) {
        //     if (index != pointerRow) {
        //         mvprintw(index, 2, "%s", choices[index].c_str());
        //     }
        //     else {
        //         attron(A_REVERSE);
        //         mvprintw(index, 2, "%s", choices[index].c_str());
        //         attroff(A_REVERSE);
        //     }
        // }

        // New-window!
        // Centered title!
        int screenX, screenY;
        getmaxyx(stdscr, screenY, screenX);
        string title = "- - Musix - -";
        attron(COLOR_PAIR(3) | A_BOLD);
        mvprintw(screenY/2 - 4, (screenX - title.size())/2, "%s", title.c_str());
        attroff(COLOR_PAIR(3) | A_BOLD);

        // Centered menu items!
        int menuStartY = screenY/2 - 1;
        int menuStartX = (screenX - 20)/2;
        for(int index = 0; index < (int)choices.size(); index++) {
            if(index != pointerRow) {
                mvprintw(menuStartY + index, menuStartX, "  %s", choices[index].c_str());
            }
            else {
                mvprintw(menuStartY + index, menuStartX, ">");
                attron(COLOR_PAIR(6) | A_BOLD);
                mvprintw(menuStartY + index, menuStartX + 2, "%s", choices[index].c_str());
                attroff(COLOR_PAIR(6) | A_BOLD);
            }
        }

        // Bottom-hint!
        attron(COLOR_PAIR(1) | A_DIM);
        mvprintw(screenY - 2, (screenX - 36)/2, "Arrow keys: Navigate | Enter: Select | Ctrl+Q: Quit | Ctrl + Z: Back");
        attroff(COLOR_PAIR(1) | A_DIM);
        refresh();

        int userInput = getch();
        if (userInput == KEY_UP) {
            if (pointerRow > 0) {
                pointerRow--;
            }
        }
        else if (userInput == KEY_DOWN) {
            if (pointerRow < choices.size() - 1) {
                pointerRow++;
            }
        }
        else if (userInput == KEY_ENTER || userInput == '\n') {
            int innerRow = 0;
            int innerColumn = 0;

            // Old sub-window!
            // WINDOW *innerWindow = newwin(17,50,0,0);
            // box(innerWindow,0,0);

            // Centralizing the sub-windows!
            int sHeight = 17, sWidth = 50;
            int sStartY = (ym - sHeight)/2;
            int sStartX = (xm - sWidth)/2;
            WINDOW *innerWindow = newwin(sHeight, sWidth, sStartY, sStartX);
            box(innerWindow, 0, 0);

            keypad(innerWindow,TRUE);

            vector<string> playlistChoices = {"Create new playlist", "Open existing playlist", "Exit playlists"};
            vector<string> queueChoices = {"Add song path", "Delete from queue", "View current queue", "Play selected song", "Shuffle queue", "Reorder queue", "Delete current queue", "Exit queue"};

            if (pointerRow != choices.size() - 1) {
                // Playlist handling!
                if (choices[pointerRow] == "Playlists") {
                    while (true) {
                        redrawScreen(innerWindow, innerRow, innerColumn, playlistChoices);
                        int innerUserInput = wgetch(innerWindow);
                        if (innerUserInput == KEY_UP)
                        {
                            if (innerRow > 0) {
                                innerRow--;
                            }
                        }

                        else if (innerUserInput == KEY_DOWN) {
                            if (innerRow < playlistChoices.size() - 1) {
                                innerRow++;
                            }
                        }

                        else if (innerUserInput == KEY_ENTER || innerUserInput == '\n') {
                            if (innerRow == 0) {
                                char playlistName[16];
                                createNewPlaylist(innerWindow,playlistName,playlists);
                            }

                            else if (innerRow == 1) {
                                if (playlists.empty()) {
                                    wclear(innerWindow);
                                    box(innerWindow,0,0);
                                    mvwprintw(innerWindow,5,2,"No playlists created!");
                                    wrefresh(innerWindow);
                                    wgetch(innerWindow);
                                    continue;
                                }

                                int playlistPointer = 0;
                                while (true) {
                                    displayAllPlaylists(innerWindow, playlists, playlistPointer);
                                    int playlistInput = wgetch(innerWindow);
                                    if (playlistInput == KEY_UP && playlistPointer > 0) {
                                        playlistPointer--;
                                    }

                                    else if (playlistInput == KEY_DOWN && playlistPointer < playlists.size() - 1) {
                                        playlistPointer++;
                                    }

                                    else if(playlistInput == KEY_ENTER || playlistInput == '\n') {
                                        string currentPlaylist = playlists[playlistPointer];
                                        vector<string> songMenu = {"Add song", "View songs", "Play song", "Delete song", "Back"};
                                        int songMenuPointer = 0;

                                        while(true) {
                                            redrawScreen(innerWindow, songMenuPointer, 0, songMenu);
                                            int songMenuInput = wgetch(innerWindow);
                                            if(songMenuInput == KEY_UP && songMenuPointer > 0) {
                                                songMenuPointer--;
                                            }
                                            else if(songMenuInput == KEY_DOWN && songMenuPointer < songMenu.size() - 1) {
                                                songMenuPointer++;
                                            }
                                            else if(songMenuInput == KEY_ENTER || songMenuInput == '\n') {
                                                if(songMenuPointer == 0) {
                                                    char songPath[64];
                                                    addSongToPlaylist(innerWindow, playlistSongs[currentPlaylist], songPath);
                                                }
                                                else if(songMenuPointer == 1) {
                                                    if(playlistSongs[currentPlaylist].empty()) {
                                                        playlistIsEmpty(innerWindow);
                                                        continue;
                                                    }
                                                    
                                                    int songPointer = 0;
                                                    while(true) {
                                                        viewPlaylistSongs(innerWindow, playlistSongs[currentPlaylist], songPointer);
                                                        int songInput = wgetch(innerWindow);
                                                        if(songInput == KEY_UP && songPointer > 0) {
                                                            songPointer--;
                                                        }
                                                        else if(songInput == KEY_DOWN && songPointer < playlistSongs[currentPlaylist].size() - 1) {
                                                            songPointer++;
                                                        }
                                                        else if(songInput == 26) {
                                                            break;
                                                        }
                                                    }
                                                }
                                                else if(songMenuPointer == 2) {
                                                    if(playlistSongs[currentPlaylist].empty()) {
                                                        playlistIsEmpty(innerWindow);
                                                        continue;
                                                    }
                                                    
                                                    int songPointer = 0;
                                                    while(true) {
                                                        viewPlaylistSongs(innerWindow, playlistSongs[currentPlaylist], songPointer);
                                                        int songInput = wgetch(innerWindow);
                                                        if(songInput == KEY_UP && songPointer > 0) {
                                                            songPointer--;
                                                        }
                                                        else if(songInput == KEY_DOWN && songPointer < playlistSongs[currentPlaylist].size() - 1) {
                                                            songPointer++;
                                                        }
                                                        else if(songInput == KEY_ENTER || songInput == '\n') {
                                                           songPointer = playSong(innerWindow, playlistSongs[currentPlaylist], songPointer);
                                                        }
                                                        else if(songInput == 26) {
                                                            break;
                                                        }
                                                    }
                                                }
                                                else if(songMenuPointer == 3) {
                                                    if(playlistSongs[currentPlaylist].empty()) {
                                                        playlistIsEmpty(innerWindow);
                                                        continue;
                                                    }
                                                         
                                                    int songPointer = 0;
                                                    while(true) {
                                                        viewPlaylistSongs(innerWindow, playlistSongs[currentPlaylist], songPointer);
                                                        int songInput = wgetch(innerWindow);
                                                        if(songInput == KEY_UP && songPointer > 0) {
                                                            songPointer--;
                                                        }
                                                        else if(songInput == KEY_DOWN && songPointer < playlistSongs[currentPlaylist].size() - 1) {
                                                            songPointer++;
                                                        }
                                                        else if(songInput == KEY_ENTER || songInput == '\n') {
                                                            playlistSongs[currentPlaylist].erase(playlistSongs[currentPlaylist].begin() + songPointer);
                                                            break;
                                                        }
                                                        else if(songInput == 26) {
                                                            break;
                                                        }
                                                    }
                                                }
                                                else {
                                                    break;
                                                }
                                            }
                                            else if(songMenuInput == 26) {
                                                break;
                                            }
                                        }
                                    }
                                    else if (playlistInput == 26) {
                                        break;
                                    }
                                }
                            }
                            else {
                                delwin(innerWindow);

                                break;
                            }
                        }
                    }
                }
                // Queue handling!
                else {
                    while (true) {
                        redrawScreen(innerWindow, innerRow, innerColumn, queueChoices);
                        int innerUserInput = wgetch(innerWindow);
                        if (innerUserInput == KEY_UP) {
                            if (innerRow > 0) {
                                innerRow--;
                            }
                        }
                        else if (innerUserInput == KEY_DOWN) {
                            if (innerRow <
                                queueChoices.size() - 1) {
                                innerRow++;
                            }
                        }
                        else if (innerUserInput == KEY_ENTER || innerUserInput == '\n') {
                            // Add song path!
                            if (innerRow == 0) {
                                char songPath[100];
                                addToQueue(innerWindow, queue, songPath);
                            }

                            // Delete from the queue!
                            else if (innerRow == 1) {
                                if (queue.empty()) {
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

                            // View the queue!
                            else if (innerRow == 2) {
                                if (queue.empty()) {
                                    wclear(innerWindow);
                                    box(innerWindow,0,0);
                                    mvwprintw(innerWindow,5,2,"Queue is empty!");
                                    wrefresh(innerWindow);
                                    wgetch(innerWindow);
                                    continue;
                                }

                                int queuePointer = 0;
                                while (true) {
                                    viewQueue(innerWindow,queue,queuePointer);
                                    int queueInput = wgetch(innerWindow);
                                    if (queueInput == KEY_UP && queuePointer > 0) {
                                        queuePointer--;
                                    }
                                    else if (queueInput == KEY_DOWN &&queuePointer < queue.size() - 1) {
                                        queuePointer++;
                                    }
                                    // Ctrl + Z to exit the window!
                                    else if (queueInput == 26) {
                                        break;
                                    }
                                }
                                }

                            // Play selected song!
                            else if (innerRow == 3) {
                                if (queue.empty()) {
                                    wclear(innerWindow);
                                    box(innerWindow,0,0);
                                    mvwprintw(innerWindow,5,2,"Queue is empty!");
                                    wrefresh(innerWindow);
                                    wgetch(innerWindow);
                                    continue;
                                }

                                int queuePointer = 0;
                                while (true) {
                                    viewQueue(innerWindow,queue,queuePointer);
                                    int queueInput = wgetch(innerWindow);
                                    if (queueInput == KEY_UP && queuePointer > 0) {
                                        queuePointer--;
                                    }
                                    else if (queueInput == KEY_DOWN && queuePointer < queue.size() - 1) {
                                        queuePointer++;
                                    }
                                    else if (queueInput == KEY_ENTER || queueInput == '\n') {
                                        queuePointer = playSong(innerWindow, queue, queuePointer);
                                    }
                                    // Ctrl + Z to exit the window!
                                    else if (queueInput == 26) {
                                        break;
                                    }
                                }
                            }
                            // Shuffle queue
                            else if (innerRow == 4) {
                                if (queue.empty()) {
                                    wclear(innerWindow);
                                    box(innerWindow, 0, 0);
                                    mvwprintw(innerWindow, 5, 2, "Queue is empty!");
                                    wrefresh(innerWindow);
                                    wgetch(innerWindow);
                                    continue;
                                }
                                shuffleQueue(innerWindow, queue);
                            }

                            // Reorder queue
                            else if (innerRow == 5) {
                                if (queue.empty()) {
                                    wclear(innerWindow);
                                    box(innerWindow, 0, 0);
                                    mvwprintw(innerWindow, 5, 2, "Queue is empty!");
                                    wrefresh(innerWindow);
                                    wgetch(innerWindow);
                                    continue;
                                }
                                reorderQueue(innerWindow, queue);
                            }
                            // Delete entire queue!
                            else if (innerRow == 6) {
                                queue.clear();
                                wclear(innerWindow);
                                box(innerWindow,0,0);
                                mvwprintw(innerWindow,5,2,"Queue cleared!");
                                wrefresh(innerWindow);
                                wgetch(innerWindow);
                            }
                            // Exit the queue!
                            else {
                                delwin(innerWindow);
                                break;
                            }
                        }
                    }
                }
            }

            else {
                break;
            }
        }

        // Ctrl + Q to exit the window!
        else if (userInput == 17) {
            break;
        }
    }

    // Closing all music-libraries and terminating the window!
    Mix_CloseAudio();
    SDL_Quit();
    endwin();
    return 0;
}