# MUSIX 

A fully terminal-based music player written in C++ using `ncurses` and `SDL2_mixer`.

MUSIX combines a retro terminal UI with real audio playback, playlist management, queue systems, persistent storage, and interactive controls — all running directly inside the Linux terminal.

---

# Preview

```txt
┌──────────────────────────────────────────┐
│                 MUSIX                    │
├──────────────────────────────────────────┤
│ > Playlists                             │
│   Queue                                 │
│   Exit                                  │
└──────────────────────────────────────────┘
```

Animated startup logo:

```txt
M U S I X
```

drawn dynamically using ncurses frame-by-frame rendering.

---

# Features

## Terminal UI
- Built completely using `ncurses`
- Interactive menu system
- Keyboard navigation
- Real-time window updates
- Dynamic rendering
- Animated startup splash-screen

---

## Real Audio Playback
Powered by `SDL2_mixer`.

Supports:
- MP3 playback
- Pause / Resume
- Restart track
- Next song
- Previous song
- Stop playback

Playback controls:

| Key | Action |
|---|---|
| P | Pause |
| C | Continue |
| B | Restart |
| N | Next Song |
| S | Previous Song |
| Q | Stop Playback |

---

## Queue Management

Features:
- Add songs to queue
- Delete songs from queue
- View current queue
- Shuffle queue
- Reorder queue
- Persistent queue storage
- Playback directly from queue

---

## Playlist System

Features:
- Create playlists
- Add songs to playlists
- Delete songs from playlists
- Browse playlists
- Play songs directly from playlists
- Persistent playlist storage

---

## Persistent Storage

MUSIX automatically saves:
- Queue data
- Playlist names
- Playlist songs

Data persists across sessions using local `.dat` files.

---

# Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Core application logic |
| ncurses | Terminal UI |
| SDL2 | Audio subsystem |
| SDL2_mixer | Music playback |
| STL | Data structures & algorithms |

---

# Architecture

The project follows a state-driven terminal UI architecture.

Core concepts used:
- Nested menu systems
- Dynamic window rendering
- Stateful navigation
- Queue-based playback
- Persistent file storage
- Recursive audio playback navigation

---

# Project Structure

```txt
Musix/
│
├── final_version.cpp
├── queue.dat
├── playlists.dat
├── songs/
│   ├── song1.mp3
│   ├── song2.mp3
│   └── ...
```

---

# Installation

## Fedora

Install dependencies:

```
sudo dnf install ncurses-devel SDL2 SDL2-devel SDL2_mixer SDL2_mixer-devel
```

---

# Compilation

```
g++ main.cpp -std=c++17 -lncurses -lSDL2 -lSDL2_mixer
```

---

# Running

```
./a.out
```

---

# Adding Songs

Create a `songs/` folder:

```bash
mkdir songs
```

Place your `.mp3` files inside:

```txt
songs/starboy.mp3
songs/phonk.mp3
songs/interstellar.mp3
```

Then inside MUSIX:
- Queue → Add song path
- Enter:

```txt
songs/starboy.mp3
```

---

# Navigation

| Key | Action |
|---|---|
| ↑ / ↓ | Move selection |
| ENTER | Select |
| CTRL + Z | Exit sub-window |
| CTRL + Q | Quit application |

---

# Queue Features

## Shuffle Queue
Randomizes queue order using Fisher-Yates shuffle.

## Reorder Queue
Swap positions of two songs directly by index.

---

# Interesting Technical Features

## Animated Splash Screen
The startup logo is rendered point-by-point using coordinate vectors.

## Recursive Song Playback
Next/previous song playback is implemented recursively for cleaner navigation logic.

## Dynamic Window Rendering
Menus are continuously redrawn to simulate a real application UI inside the terminal.

## Persistent Sessions
Queues and playlists automatically reload when the application starts.

---

# Future Improvements

Planned upgrades:
- Multi-panel UI
- Volume control
- Metadata extraction
- Auto-directory scanning
- Album art support
- Search system
- Theme customization
- Threaded playback engine
- Visual audio spectrum

---

# Why This Project Is Interesting

MUSIX is not just a basic ncurses menu app.

It combines:
- systems programming
- terminal rendering
- real-time input handling
- multimedia playback
- persistent storage
- UI state management

into a fully interactive terminal application.

The project demonstrates:
- low-level UI programming
- event-driven architecture
- Linux multimedia integration
- advanced C++ STL usage

---

# Author

Built by Sidharth Vutukuru and Vinay Kumar Das
