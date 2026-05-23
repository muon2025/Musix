#include "../include/audio.h"
#include "../include/data.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <thread>
#include <chrono>
using namespace std;

int playSong(WINDOW *innerWindow, vector<string> &songs, int index) {
    string songPath = songs[index];
    Mix_Music *song = Mix_LoadMUS(songPath.c_str());
    if (song == NULL) {
        wclear(innerWindow);
        box(innerWindow, 0, 0);
        int windowY, windowX;
        getmaxyx(innerWindow, windowY, windowX);

        wattron(innerWindow, COLOR_PAIR(3));
        mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
        mvwprintw(innerWindow, 0, 2, " Error ");
        wattroff(innerWindow, COLOR_PAIR(3));

        string message = "Failed to load song!";
        string errorMessage = Mix_GetError();
        
        mvwprintw(innerWindow, windowY/2 - 1, (windowX - message.length())/2, "%s", message.c_str());
        wattron(innerWindow, COLOR_PAIR(5) | A_BOLD);
        mvwprintw(innerWindow, windowY/2 + 1, (windowX - errorMessage.length())/2, "%s", errorMessage.c_str());
        wattroff(innerWindow, COLOR_PAIR(5) | A_BOLD);
        
        wrefresh(innerWindow);
        wgetch(innerWindow);
        return index;
    }

    Mix_PlayMusic(song, 1);
    bool paused = false;

    // Time-tracking variables!
    double totalSeconds = Mix_MusicDuration(song);
    double elapsedTime = 0.0;
    auto lastTime = chrono::steady_clock::now();

    while (Mix_PlayingMusic() || paused) {
        // Calculating elapsed-time!
        auto currentTime = chrono::steady_clock::now();
        chrono::duration<double> difference = currentTime - lastTime;
        lastTime = currentTime;

        // Only-update if song is not paused!
        if(!paused) {
            elapsedTime += difference.count();
        }

        // Cap elapsed-time at total duration to prevent over-flow!
        if(totalSeconds > 0 && elapsedTime > totalSeconds) {
            elapsedTime = totalSeconds;
        }

        wclear(innerWindow);
        box(innerWindow, 0, 0);
        int windowWidth = getmaxx(innerWindow);

        // Header!
        wattron(innerWindow, COLOR_PAIR(3));
        mvwhline(innerWindow, 1, 1, ' ', getmaxx(innerWindow) - 2);
        mvwprintw(innerWindow, 1, 2, " Now Playing ");
        wattroff(innerWindow, COLOR_PAIR(3));

        // Song-path-centered!
        string displayName = getFileName(songPath);
        wattron(innerWindow, COLOR_PAIR(1) | A_BOLD);
        int pathX = max(2, (int)(windowWidth - displayName.length())/2);
        mvwprintw(innerWindow, 3, pathX, "%s", displayName.c_str());
        wattroff(innerWindow, COLOR_PAIR(1) | A_BOLD);

        // Status-centered!
        wattron(innerWindow, paused ? COLOR_PAIR(5) : COLOR_PAIR(4));
        string statusText = paused ? "[ Paused ]" : "[ Playing ]";
        int statusX = (windowWidth - statusText.length())/2;
        mvwprintw(innerWindow, 5, statusX, "%s", statusText.c_str());
        wattroff(innerWindow, paused ? COLOR_PAIR(5) : COLOR_PAIR(4));

        // Progress-bar!
        int currentSeconds = (int)elapsedTime % 60;
        int totalMinutes = (totalSeconds > 0) ? (int)totalSeconds/60 : 0;
        int currentMinutes = (int)elapsedTime/60;
        int totalSECONDS = (totalSeconds > 0) ? (int)totalSeconds % 60 : 0;

        // Bar-width leaves 10 characters padded on the left and right for time-stamps!
        int barWidth = windowWidth - 20;
        float progress = (totalSeconds > 0) ? (elapsedTime/totalSeconds) : 0.0f;
        int filled = progress * barWidth;

        string filledBar = "";
        string emptyBar = "";
        for(int index = 0; index < barWidth; index++) {
            if(index < filled) {
                filledBar += "-";
            }
            else if(index == filled) {
                filledBar += "o";
            }
            else {
                emptyBar += "-";
            }
        }

        // Rendering the bar!
        wattron(innerWindow, COLOR_PAIR(2));
        if(totalSeconds > 0) {
            // Current time-elapsed!
            wattron(innerWindow, COLOR_PAIR(1));
            mvwprintw(innerWindow, 7, 3, "%02d:%02d [", currentMinutes, currentSeconds);
            wattroff(innerWindow, COLOR_PAIR(1));

            // Portion of song completed progress-bar! 
            wattron(innerWindow, COLOR_PAIR(2) | A_BOLD);
            wprintw(innerWindow, "%s", filledBar.c_str());
            wattroff(innerWindow, COLOR_PAIR(2) | A_BOLD);

            // Portion of song left progress-bar!
            wattron(innerWindow, COLOR_PAIR(1) | A_DIM);
            wprintw(innerWindow, "%s", emptyBar.c_str());
            wattroff(innerWindow, COLOR_PAIR(1) | A_DIM);

            // Total duration of song!
            wattron(innerWindow, COLOR_PAIR(1));
            wprintw(innerWindow, "] %02d:%02d", totalMinutes, totalSECONDS);
            wattroff(innerWindow, COLOR_PAIR(1));
        }
        else {
            // When the audio-file is corrupted!
            string fallback = "Un-supported format duration!";
            mvwprintw(innerWindow, 7, (windowWidth - fallback.length())/2, "%s", fallback.c_str());
        }
        wattroff(innerWindow, COLOR_PAIR(2));

        // Controls!
        wattron(innerWindow, COLOR_PAIR(2));
        string controlsRow1 = "P:Pause | C:Continue | B:Restart | Q:Stop";
        string controlsRow2 = "N:Next | S:Previous";
        mvwprintw(innerWindow, 9, (windowWidth - controlsRow1.length())/2, "%s", controlsRow1.c_str());
        mvwprintw(innerWindow, 10, (windowWidth - controlsRow2.length())/2, "%s", controlsRow2.c_str());
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
            elapsedTime = 0.0;
        }
        else if (input == 'n' || input == 'N') {
            Mix_HaltMusic();
            Mix_FreeMusic(song);
            if (index + 1 >= (int)songs.size()) {
                wclear(innerWindow);
                box(innerWindow, 0, 0);
                int windowY, windowX;
                getmaxyx(innerWindow, windowY, windowX);

                wattron(innerWindow, COLOR_PAIR(3));
                mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
                mvwprintw(innerWindow, 0, 2, " Notice ");
                wattroff(innerWindow, COLOR_PAIR(3));

                string message = "End of List!";
                string hint = "Press any key to continue.";

                mvwprintw(innerWindow, windowY/2 - 1, (windowX - message.length())/2, "%s", message.c_str());
                wattroff(innerWindow, COLOR_PAIR(2));
                wattron(innerWindow, COLOR_PAIR(2));
                mvwprintw(innerWindow, windowY/2 + 1, (windowX - hint.length())/2, "%s", hint.c_str());

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
                int windowY, windowX;
                getmaxyx(innerWindow, windowY, windowX);

                wattron(innerWindow, COLOR_PAIR(3));
                mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
                mvwprintw(innerWindow, 0, 2, " Notice ");
                wattroff(innerWindow, COLOR_PAIR(3));

                string message = "End of List!";
                string hint = "Press any key to continue.";

                mvwprintw(innerWindow, windowY/2 - 1, (windowX - message.length())/2, "%s", message.c_str());
                wattron(innerWindow, COLOR_PAIR(2));
                mvwprintw(innerWindow, windowY/2 + 1, (windowX - hint.length())/2, "%s", hint.c_str());
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
    // Auto-advanced logic!
    if(index + 1 > (int)songs.size() - 1) {
        wclear(innerWindow);
        box(innerWindow, 0, 0);
        int windowY, windowX;
        getmaxyx(innerWindow, windowY, windowX);

        wattron(innerWindow, COLOR_PAIR(3));
        mvwhline(innerWindow, 0, 1, ' ', windowX - 2);
        mvwprintw(innerWindow, 0, 2, " Notice ");
        wattroff(innerWindow, COLOR_PAIR(3));

        string message = "End of List!";
        string hint = "Press any key to continue!";

        mvwprintw(innerWindow, windowY/2 - 1, (windowX - message.length())/2, "%s", message.c_str());
        wattron(innerWindow, COLOR_PAIR(2));
        mvwprintw(innerWindow, windowY/2 + 1, (windowX - hint.length())/2, "%s", hint.c_str());
        wattroff(innerWindow, COLOR_PAIR(2));

        wrefresh(innerWindow);
        wgetch(innerWindow);
        return index;
    } 

    // Recursive-call to start playing the next song!
    return playSong(innerWindow, songs, index + 1);
}
