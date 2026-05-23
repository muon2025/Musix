#include "../include/listoperations.h"
#include "../include/data.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

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

void addSongToPlaylist(WINDOW *innerWindow, vector<string> &songs) {
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
    
    string inputBuffer = "";
    while(true) {
        int userCharacter = wgetch(innerWindow);
        
        // Ctrl+Z to Cancel!
        if(userCharacter == 26) {
            curs_set(0);
            return;
        }

        // Enter to Confirm!
        else if(userCharacter == '\n') {
            break;
        }

        // Backspace!
        else if(userCharacter == KEY_BACKSPACE || userCharacter == 127) {
            if(!inputBuffer.empty()) {
                inputBuffer.pop_back();
                // Removing the character from visible screen!
                int Y, X;
                getyx(innerWindow, Y, X);
                wmove(innerWindow, Y, X - 1);
                waddch(innerWindow, ' ');
                wmove(innerWindow, Y, X - 1);
            }
        }
        
        // Standard characters that can be printed/typed!
        else if(isprint(userCharacter) && inputBuffer.length() < 99) {
            inputBuffer += userCharacter;
            waddch(innerWindow, userCharacter);
        }
        wrefresh(innerWindow);
    }

    songs.push_back(inputBuffer);
    curs_set(0);
}

void addToQueue(WINDOW *innerWindow, vector<string> &queue) {
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

    string inputBuffer = "";
    while(true) {
        int userCharacter = wgetch(innerWindow);
        
        // Ctrl+Z to Cancel!
        if(userCharacter == 26) {
            noecho();
            curs_set(0);
            return;
        }

        // Enter to Confirm!
        else if(userCharacter == '\n') {
            break;
        }
        
        // Backspace!
        else if(userCharacter == KEY_BACKSPACE || userCharacter == 127) {
            if(!inputBuffer.empty()) {
                inputBuffer.pop_back();
                int Y, X;
                getyx(innerWindow, Y, X);
                wmove(innerWindow, Y, X - 1);
                waddch(innerWindow, ' ');
                wmove(innerWindow, Y, X - 1);
            }
        }
        
        // Standard characters that can be printed/typed!
        else if(isprint(userCharacter) && inputBuffer.length() < 99) {
            inputBuffer += userCharacter;
            waddch(innerWindow, userCharacter);
        }
        wrefresh(innerWindow);
    }
    queue.push_back(inputBuffer);
    curs_set(0);
}

void deleteFromQueue(WINDOW *innerWindow, vector<string> &queue) {
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

    string inputBuffer = "";
    while(true) {
        int userCharacter = wgetch(innerWindow);
        
        // Ctrl+Z to Cancel!
        if(userCharacter == 26) {
            noecho();
            curs_set(0);
            return;
        }

        // Enter to Confirm!
        else if(userCharacter == '\n') {
            break;
        }

        // Backspace!
        else if(userCharacter == KEY_BACKSPACE || userCharacter == 127) {
            if(!inputBuffer.empty()) {
                inputBuffer.pop_back();
                int Y, X;
                getyx(innerWindow, Y, X);
                wmove(innerWindow, Y, X - 1);
                waddch(innerWindow, ' ');
                wmove(innerWindow, Y, X - 1);
            }
        }
        
        // Standard characters that can be printed/typed!
        else if(isprint(userCharacter) && inputBuffer.length() < 99) {
            inputBuffer += userCharacter;
            waddch(innerWindow, userCharacter);
        }
        wrefresh(innerWindow);
    }

    auto it = find(queue.begin(), queue.end(), inputBuffer);

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

    curs_set(0);
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
    for (int i = 0; i < (int)queue.size(); i++) {
        string displayName = getFileName(queue[i]);
        // Stop printing if the bottom-area is about to be hit!
        if(2 + i > windowY - 6) {
            break;
        }
        wattron(innerWindow, COLOR_PAIR(2));
        mvwprintw(innerWindow, 2 + i, 2, "%2d:", i + 1);
        wattroff(innerWindow, COLOR_PAIR(2));
        mvwprintw(innerWindow, 2 + i, 6, "%s", displayName.c_str());
    }

    // Get first index!
    mvwprintw(innerWindow, windowY - 4, 4, "Swap song number: ");
    wmove(innerWindow, windowY - 4, 22);
    wrefresh(innerWindow);
    string inputOne = "";
    while(true) {
        int userCharacter = wgetch(innerWindow);
        
        // Ctrl+Z to Cancel!
        if(userCharacter == 26) {
            noecho();
            curs_set(0);
            return;
        }

        // Enter to Confirm!
        else if(userCharacter == '\n') {
            break;
        }

        // Backspace!
        else if(userCharacter == KEY_BACKSPACE || userCharacter == 127) {
            if(!inputOne.empty()) {
                inputOne.pop_back();
                int Y, X;
                getyx(innerWindow, Y, X);
                wmove(innerWindow, Y, X - 1);
                waddch(innerWindow, ' ');
                wmove(innerWindow, Y, X - 1);
            }
        }
        
        // Standard characters that can be printed/typed!
        else if(isprint(userCharacter) && inputOne.length() < 99) {
            inputOne += userCharacter;
            waddch(innerWindow, userCharacter);
        }
        wrefresh(innerWindow);
    }
    
    // Get second index!
    mvwprintw(innerWindow, windowY - 3, 4, "With song number: ");
    wmove(innerWindow, windowY - 3, 22);
    wrefresh(innerWindow);

    string inputTwo = "";
    while(true) {
        int userCharacter = wgetch(innerWindow);
        
        // Ctrl+Z to Cancel!
        if(userCharacter == 26) {
            noecho();
            curs_set(0);
            return;
        }

        // Enter to Confirm!
        else if(userCharacter == '\n') {
            break;
        }

        // Backspace!
        else if(userCharacter == KEY_BACKSPACE || userCharacter == 127) {
            if(!inputTwo.empty()) {
                inputTwo.pop_back();
                int Y, X;
                getyx(innerWindow, Y, X);
                wmove(innerWindow, Y, X - 1);
                waddch(innerWindow, ' ');
                wmove(innerWindow, Y, X - 1);
            }
        }
        
        // Standard characters that can be printed/typed!
        else if(isprint(userCharacter) && inputTwo.length() < 99) {
            inputTwo += userCharacter;
            waddch(innerWindow, userCharacter);
        }
        wrefresh(innerWindow);
    }

    int pos1 = atoi(inputOne.c_str()) - 1;  // Convert to 0-based indexing!
    int pos2 = atoi(inputTwo.c_str()) - 1;
    
    wclear(innerWindow);
    box(innerWindow, 0, 0);

    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowWidth - 2);
    mvwprintw(innerWindow, 0, 2, " Re-order Queue ");
    wattroff(innerWindow, COLOR_PAIR(3));

    string message;
    if (pos1 < 0 || pos1 >= (int)queue.size() || pos2 < 0 || pos2 >= (int)queue.size()) {
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
    curs_set(0);
}

void shufflePlaylist(WINDOW *innerWindow, vector<string> &playlist) {
    srand(time(0));
    for(int first = playlist.size() - 1; first > 0; first--) {
        int second = rand() % (first + 1);
        swap(playlist[first], playlist[second]);
    }

    wclear(innerWindow);
    box(innerWindow, 0, 0);
    int windowY, windowX;
    getmaxyx(innerWindow, windowY, windowX);

    // Header!
    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
    mvwprintw(innerWindow, 0, 2, " Shuffle Playlist ");
    wattroff(innerWindow, COLOR_PAIR(3));

    string message = "Playlist Shuffled!";
    string hint = "Press any key to continue!";
    mvwprintw(innerWindow, windowY/2 - 1, (windowX - message.length())/2, "%s", message.c_str());
    wattron(innerWindow, COLOR_PAIR(2));
    mvwprintw(innerWindow, windowY/2 + 1, (windowX - hint.length())/2, "%s", hint.c_str());
    wattroff(innerWindow, COLOR_PAIR(2));

    wrefresh(innerWindow);
    wgetch(innerWindow);
}

void reorderPlaylist(WINDOW *innerWindow, vector<string> &playlist) {
    curs_set(1);
    wclear(innerWindow);
    box(innerWindow, 0, 0);
    int windowWidth = getmaxx(innerWindow);
    int windowY, windowX;
    getmaxyx(innerWindow, windowY, windowX);

    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
    mvwprintw(innerWindow, 0, 2, " Re-order Playlist ");
    wattroff(innerWindow, COLOR_PAIR(3));

    for(int index = 0; index < (int)playlist.size(); index++) {
        // Stop printing if bottom-area is about to be hit!
        if(2 + index > windowY - 6) {
            break;
        }
        string displayName = getFileName(playlist[index]);
        wattron(innerWindow, COLOR_PAIR(2));
        mvwprintw(innerWindow, 2 + index, 2, "%2d:", index + 1);
        wattroff(innerWindow, COLOR_PAIR(2));
        mvwprintw(innerWindow, 2 + index, 6, "%s", displayName.c_str());
    }

    mvwprintw(innerWindow, windowY - 4, 4, "Swap song number: ");
    wmove(innerWindow, windowY - 4, 22);
    wrefresh(innerWindow);
    string inputOne = "";
    while(true) {
        int userCharacter = wgetch(innerWindow);
        // Ctrl+Z to break!
        if(userCharacter == 26) {
            noecho();
            curs_set(0);
            return;
        }

        // Enter!
        else if(userCharacter == '\n') {
            break;
        }

        // Back-space!
        else if(userCharacter == KEY_BACKSPACE || userCharacter == 127) {
            if(!inputOne.empty()) {
                inputOne.pop_back();
                int Y, X;
                getyx(innerWindow, Y, X);
                wmove(innerWindow, Y, X - 1);
                waddch(innerWindow, ' ');
                wmove(innerWindow, Y, X - 1);
            }
        }

        // Standard-characters that can be typed/printed!
        else if(isprint(userCharacter) && inputOne.length() < 99) {
            inputOne += userCharacter;
            waddch(innerWindow, userCharacter);
        }
        wrefresh(innerWindow);
    }

    mvwprintw(innerWindow, windowY - 3, 4, "With song number: ");
    wmove(innerWindow, windowY - 3, 22);
    wrefresh(innerWindow);
    string inputTwo = "";
    while(true) {
        int userCharacter = wgetch(innerWindow);
        // Ctrl+Z to break!
        if(userCharacter == 26) {
            noecho();
            curs_set(0);
            return;
        }

        // Enter!
        else if(userCharacter == '\n') {
            break;
        }

        // Back-space!
        else if(userCharacter == KEY_BACKSPACE || userCharacter == 127) {
            if(!inputTwo.empty()) {
                inputTwo.pop_back();
                int Y, X;
                getyx(innerWindow, Y, X);
                wmove(innerWindow, Y, X - 1);
                waddch(innerWindow, ' ');
                wmove(innerWindow, Y, X - 1);
            }
        }

        // Standard-characters that can be typed/printed!
        else if(isprint(userCharacter) && inputTwo.length() < 99) {
            inputTwo += userCharacter;
            waddch(innerWindow, userCharacter);
        }
        wrefresh(innerWindow);
    }

    int positionOne = atoi(inputOne.c_str()) - 1;
    int positionTwo = atoi(inputTwo.c_str()) - 1;

    wclear(innerWindow);
    box(innerWindow, 0, 0);

    wattron(innerWindow, COLOR_PAIR(3));
    mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
    mvwprintw(innerWindow, 0, 2, " Re-order Playlist ");
    wattroff(innerWindow, COLOR_PAIR(3));

    string message;
    // Checking for out-of-bounds song-numbers!
    if(positionOne < 0 || positionOne > (int)playlist.size() - 1 || positionTwo < 0 || positionTwo > (int)playlist.size() - 1) {
        message = "Invalid song numbers!";
    }
    // Checking if both chosen songs to be swapped are the same!
    else if(positionOne == positionTwo) {
        message = "Same song selected, nothing changed!";
    }
    else {
        swap(playlist[positionOne], playlist[positionTwo]);
        message = "Songs swapped successfully!";
    }

    string hint = "Press any key to continue!";
    mvwprintw(innerWindow, windowY/2 - 1, (windowWidth - message.length())/2, "%s", message.c_str());
    wattron(innerWindow, COLOR_PAIR(2));
    mvwprintw(innerWindow, windowY/2 + 1, (windowWidth - hint.length())/2, "%s", hint.c_str());
    wattroff(innerWindow, COLOR_PAIR(2));

    wrefresh(innerWindow);
    wgetch(innerWindow);
    curs_set(0);
}
