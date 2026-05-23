#pragma once
#include <ncurses.h>
#include <vector>
#include <string>
using namespace std;

void createNewPlaylist(WINDOW *innerWindow, vector<string> &playlists);
void addSongToPlaylist(WINDOW *innerWindow, vector<string> &songs);
void addToQueue(WINDOW *innerWindow, vector<string> &queue);
void deleteFromQueue(WINDOW *innerWindow, vector<string> &queue);
void shuffleQueue(WINDOW *innerWindow, vector<string> &queue);
void reorderQueue(WINDOW *innerWindow, vector<string> &queue);
void shufflePlaylist(WINDOW *innerWindow, vector<string> &playlist);
void reorderPlaylist(WINDOW *innerWindow, vector<string> &playlist);