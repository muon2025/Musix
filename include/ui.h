#pragma once
#include <ncurses.h>
#include <vector>
#include <string>
using namespace std;

void redrawScreen(WINDOW *innerWindow, int innerRow, int innerColumn, vector<string> &choices);
void animation(WINDOW *win, vector<int> &ys, vector<int> &xs);
void displayAllPlaylists(WINDOW *innerWindow, vector<string> &playlists, int playlistPointer);
void playlistIsEmpty(WINDOW *innerWindow);
void queueIsEmpty(WINDOW *innerWindow, int sHeight, int sWidth);
void viewPlaylistSongs(WINDOW *innerWindow, vector<string> &songs, int songPointer);
void viewQueue(WINDOW *innerWindow, vector<string> &queue, int queuePointer);