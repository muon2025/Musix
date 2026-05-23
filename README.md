# Musix

A sleep, minimalist and highly-responsive terminal-based music player written in C++.

## Key Features

1. **Minimalist Text-based User Interface:** Edge-to-edge solid selection bars, dynamic window centering and terminal transparency support.

2. **Advanced Playback Engine:** 
	- Real-time, two-tone dynamic progress bar displaying elapsed time and total time.
	- Auto-advances to the next song seamlessly.
	- Native controls for pause, resume, stop, next, previous and restart.

3. **Smart Library Management:** Create, edit and manage multiple custom playlists.

4. **Interactive Queue System:** Add, delete, shuffle, re-order or clear the queue.

5. Automatically truncate absolute long file paths to display clean track names.

6. **Persistent Data:** Queues and playlists are automatically serialized to local '.dat' files and restored on your next session.

7. **Fluid Navigation:** Ctrl + Z logic to back out of menus or cancel text-inputs instantly without breaking the UI flow. 

## Project Structure

Musix is built with a modular, maintainable C++ architecture.

Musix/
- include/
	- audio.h
	- data.h
	- listoperations.h
	- ui.h
- source/
	- audio.cpp
	- data.cpp
	- listoperations.cpp
	- ui.cpp
	- main.cpp
- songs/
- versions/
- Makefile
- README.md

## Prerequisites

To compile and run Musix, ensure you have g++, make and the required C++ development libraries installed on your system.

**For Ubuntu/Debian-based systems:**
- sudo apt update
- sudo apt install build-essential libncurses5-dev libncursesw5-dev libsdl2-dev libsdl2-mixer-dev

**For Arch Linux:**
- sudo pacman -S base-devel ncurses sld2 sdl2_mixer

## Installation and Build

Musix uses a custom Makefile for rapid, modular compilation.

1. **Clone/download the repository and navigate to the project root:**
	- cd path/to/Musix

2. **Compile the project using make:**
	- make

3. **Run the application:**
	- ./musix

*Note: To remove compiled object files and the executable, run* **make clean**.

## Controls and Keybindings

1. **Menu Navigation**
	- ↑/↓ - Navigate through menu options
	- Enter - Select an option or confirm text-input
	- Ctrl + Z - Return to the previous menu or cancel active text-input
	- Backspace - Delete characters during text-input

2. **Audio Playback**
	- P - Pause play-back
	- C - Continue/Resume play-back
	- B - Restart the current track
	- N - Skip to the next track
	- S - Skip to the previous track
	- Q - Quit play-back and return to the menu

## Usage Notes

- **Audio Formats:** Play-back compatibility depends on your local SDL2_mixer configuration, but natively supports .mp3, .wav, .ogg and .flac.

- **Adding Songs:** When prompted for a 'Song Path', you can either provide the:
	1. Absolute file path on your machine, such as /home/user/Music/track.mp3, or
	2. Relative file path by placing the track in the provided 'songs' directory, such as songs/track.mp3.

- **Data Files:** Running the app will generate queue.dat and playlists.dat in your project root. Deleting these files resets your Musix library, but does not affect your actual audio files.