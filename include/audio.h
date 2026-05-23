#pragma once
#include <ncurses.h>
#include <vector>
#include <string>
using namespace std;

int playSong(WINDOW *innerWindow, vector<string> &songs, int index);