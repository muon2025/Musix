#include "../include/ui.h"
#include "../include/data.h"
using namespace std;

void redrawScreen(WINDOW *innerWindow, int innerRow, int innerColumn, vector<string> &choices) {
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    int windowWidth = getmaxx(innerWindow);

    // Solid header-bar!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowWidth - 2);
    mvwprintw(innerWindow, 0, 2, " Menu ");
    wattroff(innerWindow, COLOR_PAIR(3));

    for(int index = 0; index < (int)choices.size(); index++) {
        if(index != innerRow) {
            mvwprintw(innerWindow, index + 2, 2, " %s", choices[index].c_str());
        }
        else {
            wattron(innerWindow, COLOR_PAIR(3));
            mvwhline(innerWindow, index + 2, 1, ' ', windowWidth - 2);
            mvwprintw(innerWindow, index + 2, 2, " %s", choices[index].c_str());
            wattroff(innerWindow, COLOR_PAIR(3));
        }
    }

    // Footer-control hints!
    wattron(innerWindow, COLOR_PAIR(2));
    mvwprintw(innerWindow, getmaxy(innerWindow) - 2, 2, "Enter: Select | Ctrl+Z: Back");
    wattroff(innerWindow, COLOR_PAIR(2));
    wrefresh(innerWindow);
}

void animation(WINDOW *win, vector<int> &ys, vector<int> &xs) {
    for (int i = 0; i < (int)ys.size(); i++) {
        mvwprintw(win, ys[i], xs[i], " ");
        wrefresh(win);
        napms(40);
        mvwprintw(win, ys[i], xs[i], "$");
    }
    wrefresh(win);
}

void displayAllPlaylists(WINDOW *innerWindow, vector<string> &playlists, int playlistPointer) {
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    int windowWidth = getmaxx(innerWindow);

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowWidth - 2);
    mvwprintw(innerWindow, 0, 2, " Your Playlists ");
    wattroff(innerWindow, COLOR_PAIR(3));

    for (int index = 0; index < (int)playlists.size(); index++){
        if (index != playlistPointer){
            mvwprintw(innerWindow, 2 + index, 2, " %s", playlists[index].c_str());
        } 
        else {
            wattron(innerWindow, COLOR_PAIR(3));
            mvwhline(innerWindow, 2 + index, 1, ' ', windowWidth - 2);
            mvwprintw(innerWindow, 2 + index, 2, " %s", playlists[index].c_str());
            wattroff(innerWindow, COLOR_PAIR(3));
        }
    }

    wrefresh(innerWindow);
}

void playlistIsEmpty(WINDOW *innerWindow) {
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    int windowY, windowX;
    getmaxyx(innerWindow, windowY, windowX);

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
    mvwprintw(innerWindow, 0, 2, " Playlist Empty ");
    wattroff(innerWindow, COLOR_PAIR(3));

    string message = "Playlist is Empty!";
    // Centering the message!
    mvwprintw(innerWindow, windowY/2, (windowX - message.length())/2, "%s", message.c_str());

    wrefresh(innerWindow);
    wgetch(innerWindow);
}

void queueIsEmpty(WINDOW *innerWindow, int sHeight, int sWidth) {
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    string message = "Queue is empty!";

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', sWidth - 2);
    mvwprintw(innerWindow, 0, 2, " Queue Empty ");
    wattroff(innerWindow, COLOR_PAIR(3));

    // Printing the message exactly in the center!
    mvwprintw(innerWindow, sHeight/2, (sWidth - message.length())/2, "%s", message.c_str());

    wrefresh(innerWindow);
    wgetch(innerWindow);
}

void viewPlaylistSongs(WINDOW *innerWindow, vector<string> &songs, int songPointer) {
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    int windowWidth = getmaxx(innerWindow);

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowWidth - 2);
    mvwprintw(innerWindow, 0, 2, " Playlist Songs ");
    wattroff(innerWindow, COLOR_PAIR(3));

    for (int index = 0; index < (int)songs.size(); index++){
        string displayName = getFileName(songs[index]);
        if (index != songPointer){
            mvwprintw(innerWindow, 2 + index, 2, " %s", displayName.c_str());
        } 
        else {
            wattron(innerWindow, COLOR_PAIR(3));
            mvwhline(innerWindow, 2 + index, 1, ' ', windowWidth - 2);
            mvwprintw(innerWindow, 2 + index, 2, " %s", displayName.c_str());
            wattroff(innerWindow, COLOR_PAIR(3));
        }
    }

    wrefresh(innerWindow);
}

void viewQueue(WINDOW *innerWindow, vector<string> &queue, int queuePointer) {
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    int windowWidth = getmaxx(innerWindow);

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowWidth - 2);
    mvwprintw(innerWindow, 0, 2, " Current Queue ");
    wattroff(innerWindow, COLOR_PAIR(3));

    for (int index = 0; index < (int)queue.size(); index++){
        string displayName = getFileName(queue[index]);
        if (index != queuePointer){
            mvwprintw(innerWindow, 2 + index, 2, " %s", displayName.c_str());
        } 
        else {
            wattron(innerWindow, COLOR_PAIR(3));
            mvwhline(innerWindow, 2 + index, 1, ' ', windowWidth - 2);
            mvwprintw(innerWindow, 2 + index, 2, " %s", displayName.c_str());
            wattroff(innerWindow, COLOR_PAIR(3));
        }
    }

    wrefresh(innerWindow);
}
