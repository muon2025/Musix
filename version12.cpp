#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <map>
#include <thread>
#include <chrono>
#include <ncurses.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
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
    for (int i = 0; i < ys.size(); i++) {
        mvwprintw(win, ys[i], xs[i], " ");
        wrefresh(win);
        napms(40);
        mvwprintw(win, ys[i], xs[i], "$");
    }
    wrefresh(win);
}

void saveQueueToFile(vector<string> &queue) {
    ofstream file("queue.dat");
    for (const string &song : queue) {
        file << song << "\n";
    }
}

void loadQueueFromFile(vector<string> &queue) {
    ifstream file("queue.dat");
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (!line.empty()) queue.push_back(line);
    }
}

void savePlaylistsToFile(vector<string> &playlists, map<string, vector<string>> &playlistSongs) {
    ofstream file("playlists.dat");
    for (const string &playlist : playlists) {
        file << "PLAYLIST:" << playlist << "\n";
        for (const string &song : playlistSongs[playlist]) {
            file << "SONG:" << song << "\n";
        }
    }
}

void loadPlaylistsFromFile(vector<string> &playlists, map<string, vector<string>> &playlistSongs) {
    ifstream file("playlists.dat");
    if (!file.is_open()) return;
    string line;
    string currentPlaylist;
    while (getline(file, line)) {
        if (line.substr(0, 9) == "PLAYLIST:") {
            currentPlaylist = line.substr(9);
            playlists.push_back(currentPlaylist);
        } else if (line.substr(0, 5) == "SONG:" && !currentPlaylist.empty()) {
            playlistSongs[currentPlaylist].push_back(line.substr(5));
        }
    }
}

void createNewPlaylist(WINDOW *innerWindow, vector<string> &playlists) {
    // Old-input logic: Used to capture entire input by string, not character by character!
    // New-input logic: Capture input character by character, allowing user to exit playlist creation with Ctrl + Z!
    curs_set(1);
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    int windowWidth = getmaxx(innerWindow);
    int windowY, windowX;
    getmaxyx(innerWindow, windowY, windowX);

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowWidth - 2);
    mvwprintw(innerWindow, 0, 2, " Create Playlist ");
    wattroff(innerWindow, COLOR_PAIR(3));

    // Prompt-layout!
    string promptLabel = "Name: ";
    string cancelHint = "[Ctrl+Z to Cancel]";
    int centerY = windowY/2;
    int promptStartX = (windowX - 30)/2; // Assuming  30-ish characters for label + input!

    mvwprintw(innerWindow, centerY - 1, promptStartX, "%s", promptLabel.c_str());
    wattron(innerWindow, COLOR_PAIR(2));
    mvwprintw(innerWindow, centerY + 1, (windowX - cancelHint.length())/2, "%s", cancelHint.c_str());
    wattroff(innerWindow, COLOR_PAIR(2));

    wmove(innerWindow, centerY - 1, promptStartX + promptLabel.length());

    string playlistName = "";
    while(true) {
        int userCharacter = wgetch(innerWindow);
        // Ctrl + Z!
        if(userCharacter == 26) {
            noecho();
            curs_set(0);
            return;
        }
        // Enter!
        else if(userCharacter == '\n') {
            if(!playlistName.empty()) {
                playlists.push_back(playlistName);
            }
            break;
        }
        // Back-space!
        else if(userCharacter == KEY_BACKSPACE || userCharacter == 127) {
            if(!playlistName.empty()) {
                playlistName.pop_back();
                // Removing the character from visible screen!
                int Y, X;
                getyx(innerWindow, Y, X);
                wmove(innerWindow, Y, X - 1);
                waddch(innerWindow, ' ');
                wmove(innerWindow, Y, X - 1);
            }
        }
        // Regular characters! 'isprint()' checks if the mentioned characters is printable or not!
        else if(isprint(userCharacter)) {
            playlistName += userCharacter;
            // Adding the character to visible screen!
            waddch(innerWindow, userCharacter);
        }
        wrefresh(innerWindow);
    }
    curs_set(0);
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

    for (int index = 0; index < playlists.size(); index++){
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

void addSongToPlaylist(WINDOW *innerWindow, vector<string> &songs, char songPath[64]) {
    echo();
    curs_set(1);

    wclear(innerWindow);
    box(innerWindow, 0, 0);

    int windowY, windowX;
    getmaxyx(innerWindow, windowY, windowX);

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
    mvwprintw(innerWindow, 0, 2, " Add Song to Playlist ");
    wattroff(innerWindow, COLOR_PAIR(3));

    string promptLabel = "Song Path: ";
    int centerY = windowY/2;
    int promptStartX = (windowX - 40)/2; // Assuming 40-ish characters for label + input!
    mvwprintw(innerWindow, centerY, promptStartX, "%s", promptLabel.c_str());
    wmove(innerWindow, centerY, promptStartX + promptLabel.length());
    wgetnstr(innerWindow, songPath, 64);
    songs.push_back(songPath);
    
    noecho();
    curs_set(0);
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

    for (int index = 0; index < songs.size(); index++){
        if (index != songPointer){
            mvwprintw(innerWindow, 2 + index, 2, " %s", songs[index].c_str());
        } 
        else {
            wattron(innerWindow, COLOR_PAIR(3));
            mvwhline(innerWindow, 2 + index, 1, ' ', windowWidth - 2);
            mvwprintw(innerWindow, 2 + index, 2, " %s", songs[index].c_str());
            wattroff(innerWindow, COLOR_PAIR(3));
        }
    }

    wrefresh(innerWindow);
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

void addToQueue(WINDOW *innerWindow, vector<string> &queue, char songPath[100]) {
    echo();
    curs_set(1);

    wclear(innerWindow);
    box(innerWindow, 0, 0);
    int windowY, windowX;
    getmaxyx(innerWindow, windowY, windowX);

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
    mvwprintw(innerWindow, 0, 2, " Add to Queue ");
    wattroff(innerWindow, COLOR_PAIR(3));

    // Centered input!
    string promptLabel = "Song Path: ";
    int centerY = windowY/2;
    int promptStartX = (windowX - 40)/2; // Assuming 40-ish characters for label + input!

    mvwprintw(innerWindow, centerY, promptStartX, "%s", promptLabel.c_str());
    wmove(innerWindow, centerY, promptStartX + promptLabel.length());

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

    int windowY, windowX;
    getmaxyx(innerWindow, windowY, windowX);

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
    mvwprintw(innerWindow, 0, 2, " Delete from Queue ");
    wattroff(innerWindow, COLOR_PAIR(3));

    string promptLabel = "Song Path:";
    int centerY = windowY/2;
    int promptStartX = (windowX - 40)/2; // Assuming 40-ish characters for label + input!

    mvwprintw(innerWindow, centerY - 1, promptStartX, "%s", promptLabel.c_str());
    wmove(innerWindow, centerY - 1, promptStartX + promptLabel.length());
    wrefresh(innerWindow);

    wgetnstr(innerWindow, songName, 100);

    auto it = find(queue.begin(), queue.end(), songName);

    wclear(innerWindow);
    box(innerWindow, 0, 0);
    
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
    mvwprintw(innerWindow, 0, 2, " Delete from Queue ");
    wattroff(innerWindow, COLOR_PAIR(3));
    
    string resultMessage = "";
    if (it != queue.end()) {
        queue.erase(it);
        resultMessage = "Song deleted successfully!";
    } 
    else {
        resultMessage = "Song not found in queue!";
    }
    // Centering the result message!
    mvwprintw(innerWindow, centerY - 1, (windowX - resultMessage.length())/2, "%s", resultMessage.c_str());

    // Centering the continue message!
    string continueMessage = "Press any key to continue!";
    wattron(innerWindow, COLOR_PAIR(2));
    mvwprintw(innerWindow, centerY + 1, (windowX - continueMessage.length())/2, "%s", continueMessage.c_str());
    wattroff(innerWindow, COLOR_PAIR(2));
    wrefresh(innerWindow);
    wgetch(innerWindow);

    noecho();
    curs_set(0);
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

    for (int index = 0; index < queue.size(); index++){
        if (index != queuePointer){
            mvwprintw(innerWindow, 2 + index, 2, " %s", queue[index].c_str());
        } 
        else {
            wattron(innerWindow, COLOR_PAIR(3));
            mvwhline(innerWindow, 2 + index, 1, ' ', windowWidth - 2);
            mvwprintw(innerWindow, 2 + index, 2, " %s", queue[index].c_str());
            wattroff(innerWindow, COLOR_PAIR(3));
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
    int windowY, windowX;
    getmaxyx(innerWindow, windowY, windowX);

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
    mvwprintw(innerWindow, 0, 2, " Shuffle Queue ");
    wattroff(innerWindow, COLOR_PAIR(3));

    string message = "Queue Shuffled!";
    string hint = "Press any key to continue!";

    // Centered-messages!
    mvwprintw(innerWindow, windowY/2 - 1, (windowX - message.length())/2, "%s", message.c_str());
    wattron(innerWindow, COLOR_PAIR(2));
    mvwprintw(innerWindow, windowY/2 + 1, (windowX - hint.length())/2, "%s", hint.c_str());
    wattroff(innerWindow, COLOR_PAIR(2));

    wrefresh(innerWindow);
    wgetch(innerWindow);
}

void reorderQueue(WINDOW *innerWindow, vector<string> &queue) {
    echo();
    curs_set(1);
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    int windowWidth = getmaxx(innerWindow);
    int windowY, windowX;
    getmaxyx(innerWindow, windowY, windowX);

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowWidth - 2);
    mvwprintw(innerWindow, 0, 2, " Re-order Queue ");
    wattroff(innerWindow, COLOR_PAIR(3));

    // Display queue with indices!
    for (int i = 0; i < queue.size(); i++) {
        // mvwprintw(innerWindow, 3 + i, 2, "%d: %s", i + 1, queue[i].c_str());
        // Stop printing if the bottom-area is about to be hit!
        if(2 + i > windowY - 6) {
            break;
        }

        wattron(innerWindow, COLOR_PAIR(2));
        mvwprintw(innerWindow, 2 + i, 2, "%2d:", i + 1);
        wattroff(innerWindow, COLOR_PAIR(2));
        mvwprintw(innerWindow, 2 + i, 6, "%s", queue[i].c_str());
    }

    // Get first index!
    char input1[4], input2[4];
    mvwprintw(innerWindow, windowY - 4, 4, "Swap song number: ");
    wmove(innerWindow, windowY - 4, 22);
    wrefresh(innerWindow);
    wgetnstr(innerWindow, input1, 3);
    
    // Get second index!
    mvwprintw(innerWindow, windowY - 3, 4, "With song number: ");
    wmove(innerWindow, windowY - 3, 22);
    wrefresh(innerWindow);
    wgetnstr(innerWindow, input2, 3);
    int pos1 = atoi(input1) - 1;  // Convert to 0-based indexing!
    int pos2 = atoi(input2) - 1;
    
    wclear(innerWindow);
    box(innerWindow, 0, 0);

    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowWidth - 2);
    mvwprintw(innerWindow, 0, 2, " Re-order Queue ");
    wattroff(innerWindow, COLOR_PAIR(3));

    string message;
    if (pos1 < 0 || pos1 >= queue.size() || pos2 < 0 || pos2 >= queue.size()) {
        message = "Invalid song numbers!";
    } 
    else if (pos1 == pos2) {
        message = "Same song selected, nothing changed.";
    } 
    else {
        swap(queue[pos1], queue[pos2]);
        message = "Songs swapped successfully!";
    }

    string hint = "Press any key to continue.";
    
    // Centered-messages!
    mvwprintw(innerWindow, windowY/2 - 1, (windowWidth - message.length())/2, "%s", message.c_str());
    wattron(innerWindow, COLOR_PAIR(2));
    mvwprintw(innerWindow, windowY/2 + 1, (windowWidth - hint.length())/2, "%s", hint.c_str());
    wattroff(innerWindow, COLOR_PAIR(2));

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
        int winY, winX;
        getmaxyx(innerWindow, winY, winX);

        wattron(innerWindow, COLOR_PAIR(3));
        mvwhline(innerWindow, 0, 1, ' ', winX - 2);
        mvwprintw(innerWindow, 0, 2, " Error ");
        wattroff(innerWindow, COLOR_PAIR(3));

        string message = "Failed to load song!";
        string errorMessage = Mix_GetError();
        
        mvwprintw(innerWindow, winY/2 - 1, (winX - message.length())/2, "%s", message.c_str());
        wattron(innerWindow, COLOR_PAIR(5) | A_BOLD); // Red for error
        mvwprintw(innerWindow, winY/2 + 1, (winX - errorMessage.length())/2, "%s", errorMessage.c_str());
        wattroff(innerWindow, COLOR_PAIR(5) | A_BOLD);
        
        wrefresh(innerWindow);
        wgetch(innerWindow);
        return index;
    }

    Mix_PlayMusic(song, 1);
    bool paused = false;

    while (Mix_PlayingMusic() || paused) {
        wclear(innerWindow);
        box(innerWindow, 0, 0);
        int windowWidth = getmaxx(innerWindow);

        // Header!
        wattron(innerWindow, COLOR_PAIR(3));
        mvwhline(innerWindow, 1, 1, ' ', getmaxx(innerWindow) - 2);
        mvwprintw(innerWindow, 1, 2, " Now Playing ");
        wattroff(innerWindow, COLOR_PAIR(3));

        // Song-path-centered!
        wattron(innerWindow, COLOR_PAIR(1) | A_BOLD);
        int pathX = max(2, (int)(windowWidth - songPath.length())/2);
        mvwprintw(innerWindow, 3, pathX, "%s", songPath.c_str());
        wattroff(innerWindow, COLOR_PAIR(1) | A_BOLD);

        // Status-centered!
        wattron(innerWindow, paused ? COLOR_PAIR(5) : COLOR_PAIR(4));
        string statusText = paused ? "[ Paused ]" : "[ Playing ]";
        int statusX = (windowWidth - statusText.length())/2;
        mvwprintw(innerWindow, 5, statusX, "%s", statusText.c_str());
        wattroff(innerWindow, paused ? COLOR_PAIR(5) : COLOR_PAIR(4));

        // Controls!
        wattron(innerWindow, COLOR_PAIR(2));
        string controlsRow1 = "P:Pause   C:Continue   B:Restart   Q:Stop";
        string controlsRow2 = "N:Next    S:Previous";
        mvwprintw(innerWindow, 7, (windowWidth - controlsRow1.length())/2, "%s", controlsRow1.c_str());
        mvwprintw(innerWindow, 8, (windowWidth - controlsRow2.length())/2, "%s", controlsRow2.c_str());
        wattroff(innerWindow, COLOR_PAIR(2));

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
                int winY, winX;
                getmaxyx(innerWindow, winY, winX);

                wattron(innerWindow, COLOR_PAIR(3));
                mvwhline(innerWindow, 0, 1, ' ', winX - 2);
                mvwprintw(innerWindow, 0, 2, " Notice ");
                wattroff(innerWindow, COLOR_PAIR(3));

                string message = "End of List!";
                string hint = "Press any key to continue.";

                mvwprintw(innerWindow, winY/2 - 1, (winX - message.length())/2, "%s", message.c_str());
                wattron(innerWindow, COLOR_PAIR(2));
                mvwprintw(innerWindow, winY/2 + 1, (winX - hint.length())/2, "%s", hint.c_str());
                wattroff(innerWindow, COLOR_PAIR(2));

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
                int winY, winX;
                getmaxyx(innerWindow, winY, winX);

                wattron(innerWindow, COLOR_PAIR(3));
                mvwhline(innerWindow, 0, 1, ' ', winX - 2);
                mvwprintw(innerWindow, 0, 2, " Notice ");
                wattroff(innerWindow, COLOR_PAIR(3));

                string message = "End of List!";
                string hint = "Press any key to continue.";

                mvwprintw(innerWindow, winY/2 - 1, (winX - message.length())/2, "%s", message.c_str());
                wattron(innerWindow, COLOR_PAIR(2));
                mvwprintw(innerWindow, winY/2 + 1, (winX - hint.length())/2, "%s", hint.c_str());
                wattroff(innerWindow, COLOR_PAIR(2));

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
    int pointerColumn = 0;

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
        string footerHint = "Arrow keys: Navigate | Enter: Select | Ctrl+Q: Quit | Ctrl + Z: Back";
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
            if (pointerRow < choices.size() - 1) {
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
            vector<string> queueChoices = {"Add song path", "Delete from queue", "View current queue", "Play selected song", "Shuffle queue", "Reorder queue", "Delete current queue", "Exit queue"};

            if (pointerRow != choices.size() - 1) {
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
                            if (innerRow < playlistChoices.size() - 1) {
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

                                    else if (playlistInput == KEY_DOWN && playlistPointer < playlists.size() - 1) {
                                        playlistPointer++;
                                    }

                                    else if(playlistInput == KEY_ENTER || playlistInput == '\n') {
                                        string currentPlaylist = playlists[playlistPointer];
                                        vector<string> songMenu = {"Add song", "View songs", "Play song", "Delete song", "Delete this playlist", "Back"};
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
                                                            savePlaylistsToFile(playlists, playlistSongs);
                                                            break;
                                                        }
                                                        else if(songInput == 26) {
                                                            break;
                                                        }
                                                    }
                                                }
                                                // Deleting the current playlist!
                                                else if(songMenuPointer == 4) {
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
                                saveQueueToFile(queue);
                            }

                            // Delete from the queue!
                            else if (innerRow == 1) {
                                if (queue.empty()) {
                                    queueIsEmpty(innerWindow, sHeight, sWidth);
                                    continue;
                                }
                                char songPath[100];
                                deleteFromQueue(innerWindow, queue, songPath);
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
    saveQueueToFile(queue);
    savePlaylistsToFile(playlists, playlistSongs);
    
    // Closing all music-libraries and terminating the window!
    Mix_CloseAudio();
    SDL_Quit();
    endwin();
    return 0;
}