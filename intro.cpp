#include <iostream>
#include <vector>
#include <ncurses.h>
using namespace std;
void animation(WINDOW* win, vector<int>& ys, vector<int>& xs) {
    for (int i = 0; i < ys.size(); i++) {
        mvwprintw(win, ys[i], xs[i], " "); // the cursor moves without any charecter
        wrefresh(win); // To update the changes in the memory
        napms(40);  //Have to learn what this is
        
        mvwprintw(win, ys[i], xs[i], "$"); //The cursor leaves behind a trail of '$'s
    }
    wrefresh(win); //To update the change in the window to the terminal
}
int main() {
    initscr();
    //cursor is invisible
    noecho();
    raw();
    curs_set(0);
    int xm, ym;
    getmaxyx(stdscr, ym, xm);

    int winHeight = 25;
    int winWidth = xm - 10;
    int startx = (xm - winWidth) / 2;
    int starty = (ym - winHeight) / 2;
    //initialize window
    
    WINDOW* win = newwin(winHeight,winWidth,starty,startx);
    box(win, 0, 0);
    vector<int> ys;
    vector<int> xs;
    int spacing = 10;
    int letterWidth = 7;

    //totalWidth is used to determine the total space required to write MUSIX on the terminal
    int totalWidth = (4 * spacing) + letterWidth;
    //sx and sy use the totalWidth to accurately center the text
    int sx = (winWidth - totalWidth) / 2;
    int sy = winHeight/2 + 4;
    // =============== M ===============
    for(int i = 0; i < 8; i++) {
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
    for(int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx + 6);
    }
    // =============== U ===============
    sx += spacing;
    for(int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx);
    }
    for(int i = 1; i <= 4; i++) {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    for(int i = 7; i >= 0; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx + 5);
    }
    // =============== S ===============
    sx += spacing;
    for(int i = 5; i >= 1; i--) {
        ys.push_back(sy - 7);
        xs.push_back(sx + i);
    }
    for(int i = 6; i >= 4; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx);
    }
    for(int i = 1; i <= 4; i++) {
        ys.push_back(sy - 3);
        xs.push_back(sx + i);
    }
    for(int i = 2; i >= 0; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx + 5);
    }
    for(int i = 4; i >= 1; i--) {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    // =============== I ===============
    sx += spacing;
    for(int i = 0; i < 6; i++) {
        ys.push_back(sy - 7);
        xs.push_back(sx + i);
    }
    for(int i = 6; i >= 0; i--) {
        ys.push_back(sy - i);
        xs.push_back(sx + 3);
    }
    for(int i = 0; i < 6; i++) {
        ys.push_back(sy);
        xs.push_back(sx + i);
    }
    // =============== X ===============
    sx += spacing;
    for(int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx + i);
    }
    for(int i = 0; i < 8; i++) {
        ys.push_back(sy - i);
        xs.push_back(sx + 7 - i);
    }
    animation(win, ys, xs);

    getch();
    return 0;
}   