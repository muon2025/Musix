#include "../include/data.h"
#include <fstream>
using namespace std;

string getFileName(string &path) {
    // Finding the index of the last occurance of '/'!
    size_t position = path.find_last_of("/\\");
    if(position != string::npos) {
        return path.substr(position + 1);
    }
    return path;
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
