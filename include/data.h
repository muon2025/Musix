#pragma once
#include <vector>
#include <string>
#include <map>
using namespace std;

string getFileName(string &path);
void saveQueueToFile(vector<string> &queue);
void loadQueueFromFile(vector<string> &queue);
void savePlaylistsToFile(vector<string> &playlists, map<string, vector<string>> &playlistSongs);
void loadPlaylistsFromFile(vector<string> &playlists, map<string, vector<string>> &playlistSongs);