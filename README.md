
# 🎮 Pixelmon Showdown - College Project

Pixelmon Showdown is a **turn-based** battle game inspired by Pokémon, developed as a **college project** at **Cesar School**. The game is built using **C** and **Raylib**, focusing on fundamental **data structures**, **file handling**, **dynamic memory management**, and **linked lists**.

---

## 📋 Project Overview

This project was designed to meet the following **academic requirements**:

- **Linked List and Matrix Usage**
- **Structs**
- **Dynamic Memory Allocation**
- **File Manipulation (JSON)**

---

## 📂 Project Structure

```
Pixelmon Showdown/
├── assets/
│   ├── background/
│   ├── misc/
│   ├── sounds/
│   ├── sprites/
│   └── status/
├── data/
│   ├── move_data.json
│   └── type_chart.json
├── include/
│   ├── battle_screen.h
│   ├── cJSON.h
│   ├── file_loader.h
│   ├── menu.h
│   ├── player_loader.h
│   ├── pokemon_loader.h
│   ├── rival_loader.h
│   ├── team.h
│   └── team_select.h
├── lib/
│   └── cJSON.c
├── src/
│   ├── battle_screen.c
│   ├── file_loader.c
│   ├── main.c
│   ├── menu.c
│   ├── player_loader.c
│   ├── pokemon_loader.c
│   ├── rival_loader.c
│   ├── team.c
│   └── team_select.c
├── makefile
└── README.md
```

---

## ⚙️ Key Features

### 1. Core Battle System
- **Turn-Based Logic** - Alternates between player and rival.
- **Critical Hits** - Includes critical hit logic and sound effects.
- **Type Effectiveness** - Takes advantage of Pokémon type strengths and weaknesses.
- **Damage Over Time (DoT)** - Status effects like Burn and Poison cause continuous damage.

### 2. Pokémon Management
- **Team Selection** - Allows players to select a team of 6 Pokémon.
- **Dynamic Team Loading** - Loads Pokémon from structured JSON files.
- **Flexible Move Sets** - Pokémon can have up to 4 moves, each with unique effects.

### 3. Status Effects
- **Paralysis, Burn, Poison, Freeze, Sleep, Confusion** - Fully implemented status conditions.
- **Self-Stat Buffs** - Moves like **Calm Mind** and **Swords Dance** are supported.
- **Protect Logic** - Prevents incoming damage for one turn.

### 4. Sound and Visual Effects
- **Background Music** - Continuous battle theme.
- **Critical Hit Sound Effects** - Audio feedback for critical hits.
- **Status Icons** - Visual representation of status effects.

---

## 🛠️ Building and Running the Project

### Install Dependencies (Raylib)
```bash
# MacOS
brew install raylib

# Ubuntu
sudo apt install libraylib-dev
```

### Build and Run
```bash
make
./pixelmon_showdown
```

---

## 📑 License
This project is licensed under the **MIT License**. Feel free to modify and distribute.

---

## 👨‍💻 Author
Developed by **Ricardo Freitas / Thiago Fernandes / Gabriel Aniceto** as part of a college project at **Cesar School**.
