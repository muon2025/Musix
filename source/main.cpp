#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ncurses.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../include/data.h"
#include "../include/ui.h"
#include "../include/listoperations.h"
#include "../include/audio.h"

using namespace std;

int main()
{
    initscr();
    use_default_colors(); // Enables terminal-transparency!
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    start_color();
    init_pair(1, COLOR_WHITE, -1);
    init_pair(2, COLOR_CYAN, -1);
    init_pair(3, COLOR_BLACK, COLOR_CYAN);
    init_pair(4, COLOR_GREEN, -1);
    init_pair(5, COLOR_RED, -1);
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

    vector<string> choices = {"Playlists", "Queue", "Exit"};
    vector<string> playlists;
    map<string, vector<string>> playlistSongs;
    vector<string> queue;
    loadQueueFromFile(queue);
    loadPlaylistsFromFile(playlists, playlistSongs);

    while (true) {
        clear();
        // New-window!
        // Centered title!
        int screenX, screenY;
        getmaxyx(stdscr, screenY, screenX);
        string title = " M U S I X ";
        attron(COLOR_PAIR(2) | A_BOLD);
        mvprintw(screenY/2 - 4, (screenX - title.size())/2, "%s", title.c_str());
        attroff(COLOR_PAIR(2) | A_BOLD);

        // Centered menu items!
        int menuStartY = screenY/2 - 1;
        int menuStartX = (screenX - 20)/2;
        for(int index = 0; index < (int)choices.size(); index++) {
            if(index != pointerRow) {
                mvprintw(menuStartY + index, menuStartX, "  %s", choices[index].c_str());
            }
            else {
                attron(COLOR_PAIR(3));
                // %-18s pads the string with spaces on the right!
                mvprintw(menuStartY + index, menuStartX - 2, " %-18s ", choices[index].c_str());
                attroff(COLOR_PAIR(3));
            }
        }

        // Bottom-hint-centered!
        attron(COLOR_PAIR(1) | A_DIM);
        string footerHint = "Arrow keys: Navigate | Enter: Select | Ctrl + Z: Back";
        mvprintw(screenY - 2, (screenX - footerHint.length())/2, "%s", footerHint.c_str());
        attroff(COLOR_PAIR(1) | A_DIM);
        refresh();

        int userInput = getch();
        if (userInput == KEY_UP) {
            if (pointerRow > 0) {
                pointerRow--;
            }
        }
        else if (userInput == KEY_DOWN) {
            if (pointerRow < (int)choices.size() - 1) {
                pointerRow++;
            }
        }
        else if (userInput == KEY_ENTER || userInput == '\n') {
            int innerRow = 0;
            int innerColumn = 0;

            // Centralizing the sub-windows!
            int sHeight = min(20, screenY - 4);
            int sWidth = min(60, screenX - 8);
            int sStartY = (ym - sHeight)/2;
            int sStartX = (xm - sWidth)/2;
            WINDOW *innerWindow = newwin(sHeight, sWidth, sStartY, sStartX);
            box(innerWindow, 0, 0);
            keypad(innerWindow, TRUE);

            vector<string> playlistChoices = {"Create new playlist", "Open existing playlist", "Exit playlists"};
            vector<string> queueChoices = {"Add song path", "Delete from queue", "View current queue", "Play selected song", "Shuffle queue", "Re-order queue", "Delete current queue", "Exit queue"};

            if (pointerRow != (int)choices.size() - 1) {
                //playlist handling
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
                            if (innerRow < (int)playlistChoices.size() - 1) {
                                innerRow++;
                            }
                        }
                        else if (innerUserInput == KEY_ENTER || innerUserInput == '\n') {
                            if (innerRow == 0) {
                                createNewPlaylist(innerWindow, playlists);
                                savePlaylistsToFile(playlists, playlistSongs);
                            }

                            else if (innerRow == 1) {
                                if (playlists.empty()) {
                                    wclear(innerWindow);
                                    box(innerWindow,0,0);
                                    
                                    wattron(innerWindow, COLOR_PAIR(3));
                                    mvwhline(innerWindow, 0, 1, ' ', sWidth - 2);
                                    mvwprintw(innerWindow, 0, 2, " Playlists ");
                                    wattroff(innerWindow, COLOR_PAIR(3));
                                    
                                    string message = "No playlists created!";
                                    mvwprintw(innerWindow, sHeight/2, (sWidth - message.length())/2, "%s", message.c_str());
                                    
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

                                    else if (playlistInput == KEY_DOWN && playlistPointer < (int)playlists.size() - 1) {
                                        playlistPointer++;
                                    }

                                    else if(playlistInput == KEY_ENTER || playlistInput == '\n') {
                                        string currentPlaylist = playlists[playlistPointer];
                                        vector<string> songMenu = {"Add song", "View songs", "Play song", "Delete song", "Shuffle playlist", "Re-order playlist", "Delete this playlist", "Back"};
                                        int songMenuPointer = 0;

                                        while(true) {
                                            redrawScreen(innerWindow, songMenuPointer, 0, songMenu);
                                            int songMenuInput = wgetch(innerWindow);
                                            if(songMenuInput == KEY_UP && songMenuPointer > 0) {
                                                songMenuPointer--;
                                            }
                                            else if(songMenuInput == KEY_DOWN && songMenuPointer < (int)songMenu.size() - 1) {
                                                songMenuPointer++;
                                            }
                                            else if(songMenuInput == KEY_ENTER || songMenuInput == '\n') {
                                                if(songMenuPointer == 0) {
                                                    addSongToPlaylist(innerWindow, playlistSongs[currentPlaylist]);
                                                    savePlaylistsToFile(playlists, playlistSongs);
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
                                                        else if(songInput == KEY_DOWN && songPointer < (int)playlistSongs[currentPlaylist].size() - 1) {
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
                                                        else if(songInput == KEY_DOWN && songPointer < (int)playlistSongs[currentPlaylist].size() - 1) {
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
                                                        else if(songInput == KEY_DOWN && songPointer < (int)playlistSongs[currentPlaylist].size() - 1) {
                                                            songPointer++;
                                                        }
                                                        else if(songInput == KEY_ENTER || songInput == '\n') {
                                                            playlistSongs[currentPlaylist].erase(playlistSongs[currentPlaylist].begin() + songPointer);
                                                            savePlaylistsToFile(playlists, playlistSongs);
                                                            break;
                                                        }
                                                        else if(songInput == 26) {
                                                            break;
                                                        }
                                                    }
                                                }
                                                // Shuffling the playlist!
                                                else if(songMenuPointer == 4) {
                                                    if(playlistSongs[currentPlaylist].empty()) {
                                                        playlistIsEmpty(innerWindow);
                                                        continue;
                                                    }
                                                    shufflePlaylist(innerWindow, playlistSongs[currentPlaylist]);
                                                    savePlaylistsToFile(playlists, playlistSongs);
                                                }

                                                // Re-ordering the playlist!
                                                else if(songMenuPointer == 5) {
                                                    if(playlistSongs[currentPlaylist].empty()) {
                                                        playlistIsEmpty(innerWindow);
                                                        continue;
                                                    }
                                                    reorderPlaylist(innerWindow, playlistSongs[currentPlaylist]);
                                                    savePlaylistsToFile(playlists, playlistSongs);
                                                }

                                                // Deleting the current playlist!
                                                else if(songMenuPointer == 6) {
                                                    playlistSongs.erase(currentPlaylist);
                                                    playlists.erase(playlists.begin() + playlistPointer);
                                                    savePlaylistsToFile(playlists, playlistSongs);
                                                    
                                                    wclear(innerWindow);
                                                    box(innerWindow, 0, 0);
                                                    
                                                    wattron(innerWindow, COLOR_PAIR(3));
                                                    mvwhline(innerWindow, 0, 1, ' ', sWidth - 2);
                                                    mvwprintw(innerWindow, 0, 2, " Delete Playlist ");
                                                    wattroff(innerWindow, COLOR_PAIR(3));
                                                    
                                                    string message = "Playlist successfully deleted!";
                                                    wattron(innerWindow, COLOR_PAIR(4) | A_BOLD); // Keeps your green success text!
                                                    mvwprintw(innerWindow, sHeight/2, (sWidth - message.length())/2, "%s", message.c_str());
                                                    wattroff(innerWindow, COLOR_PAIR(4) | A_BOLD);
                                                    
                                                    wrefresh(innerWindow);
                                                    wgetch(innerWindow);
                                                    break;
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
                        // Ctrl+Z to go back!
                        else if(innerUserInput == 26) {
                            delwin(innerWindow);
                            break;
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
                            if (innerRow < (int)queueChoices.size() - 1) {
                                innerRow++;
                            }
                        }
                        else if (innerUserInput == KEY_ENTER || innerUserInput == '\n') {
                            // Add song path!
                            if (innerRow == 0) {
                                addToQueue(innerWindow, queue);
                                saveQueueToFile(queue);
                            }

                            // Delete from the queue!
                            else if (innerRow == 1) {
                                if (queue.empty()) {
                                    queueIsEmpty(innerWindow, sHeight, sWidth);
                                    continue;
                                }
                                deleteFromQueue(innerWindow, queue);
                                saveQueueToFile(queue);
                            }

                            // View the queue!
                            else if (innerRow == 2) {
                                if (queue.empty()) {
                                    queueIsEmpty(innerWindow, sHeight, sWidth);
                                    continue;
                                }

                                int queuePointer = 0;
                                while (true) {
                                    viewQueue(innerWindow, queue, queuePointer);
                                    int queueInput = wgetch(innerWindow);
                                    if (queueInput == KEY_UP && queuePointer > 0) {
                                        queuePointer--;
                                    }
                                    else if (queueInput == KEY_DOWN &&queuePointer < (int)queue.size() - 1) {
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
                                    queueIsEmpty(innerWindow, sHeight, sWidth);
                                    continue;
                                }

                                int queuePointer = 0;
                                while (true) {
                                    viewQueue(innerWindow, queue, queuePointer);
                                    int queueInput = wgetch(innerWindow);
                                    if (queueInput == KEY_UP && queuePointer > 0) {
                                        queuePointer--;
                                    }
                                    else if (queueInput == KEY_DOWN && queuePointer < (int)queue.size() - 1) {
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
                            // Shuffle queue!
                            else if (innerRow == 4) {
                                if (queue.empty()) {
                                    queueIsEmpty(innerWindow, sHeight, sWidth);
                                    continue;
                                }
                                shuffleQueue(innerWindow, queue);
                                saveQueueToFile(queue);
                            }

                            // Reorder queue!
                            else if (innerRow == 5) {
                                if (queue.empty()) {
                                    queueIsEmpty(innerWindow, sHeight, sWidth);
                                    continue;
                                }
                                reorderQueue(innerWindow, queue);
                                saveQueueToFile(queue);
                            }
                            // Delete entire queue!
                            else if (innerRow == 6) {
                                queue.clear();
                                saveQueueToFile(queue);
                                
                                wclear(innerWindow);
                                box(innerWindow,0,0);
                                
                                wattron(innerWindow, COLOR_PAIR(3));
                                mvwhline(innerWindow, 0, 1, ' ', sWidth - 2);
                                mvwprintw(innerWindow, 0, 2, " Clear Queue ");
                                wattroff(innerWindow, COLOR_PAIR(3));
                                
                                string message = "Queue cleared!";
                                mvwprintw(innerWindow, sHeight/2, (sWidth - message.length())/2, "%s", message.c_str());
                                
                                wrefresh(innerWindow);
                                wgetch(innerWindow);
                            }
                            // Exit the queue!
                            else {
                                delwin(innerWindow);
                                break;
                            }
                        }
                        // Ctrl+Z to go back!
                        else if(innerUserInput == 26) {
                            delwin(innerWindow);
                            break;
                        }
                    }
                }
            }

            else {
                break;
            }
        }
    }
    saveQueueToFile(queue);
    savePlaylistsToFile(playlists, playlistSongs);
    
    // Closing all music-libraries and terminating the window!
    Mix_CloseAudio();
    SDL_Quit();
    endwin();
    return 0;
}
