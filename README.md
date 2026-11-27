<div align="center">

<a id="top"></a>

[![The Locker Heist](https://capsule-render.vercel.app/api?type=waving&color=0:0b1020,45:172554,100:7f1d1d&height=210&section=header&text=THE%20LOCKER%20HEIST&fontColor=f8fafc&fontSize=46&fontAlignY=37&animation=fadeIn&desc=Every%20clue%20has%20a%20price.%20Every%20suspect%20has%20a%20secret.&descSize=16&descAlignY=61&descColor=fbbf24)](https://github.com/)

[![Typing SVG](https://readme-typing-svg.demolab.com?font=Fira+Code&weight=600&size=19&duration=2800&pause=900&color=FBBF24&center=true&vCenter=true&width=760&height=42&lines=INTERROGATE.+INVESTIGATE.+ACCUSE.;A+detective+game+built+with+C%2B%2B+and+OOP.;Can+you+solve+the+locker+heist%3F)](https://git.io/typing-svg)

[![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Visual Studio](https://img.shields.io/badge/Visual%20Studio-Solution-5C2D91?style=for-the-badge&logo=visualstudio&logoColor=white)](https://visualstudio.microsoft.com/)
[![Genre](https://img.shields.io/badge/Genre-Detective%20%2F%20Mystery-7f1d1d?style=for-the-badge&logo=target&logoColor=white)](#gameplay)
[![Status](https://img.shields.io/badge/Status-Playable-16a34a?style=for-the-badge&logo=checkmarx&logoColor=white)](#getting-started)

<p>
<strong>A console-based detective mystery powered by object-oriented programming.</strong>  

  Search the scene. Interview the people. Decode the words. Name the culprit.
</p> </div>

---

## The case file

A high-value locker heist has taken place, and the evidence is scattered across a suspicious cottage. Seven suspects. Hidden clues. Two secret words. One culprit.

**The Locker Heist** is a C++ console detective game created to demonstrate core object-oriented programming concepts through an interactive investigation. Every playthrough chooses a culprit at random, so the case changes every time you launch it.

<div align="center">

![Gameplay loop](https://img.shields.io/badge/CASE%20FILE-OPEN-111827?style=flat-square&labelColor=7f1d1d&color=fbbf24)

</div>

## Gameplay

```
┌──────────────────────────────────────────────────────────────┐
│  THE LOCKER HEIST                                            │
│  ───────────────────────────────────────────────────────────  │
│  [1] Visit a Location      Search seven locations for clues  │
│  [2] Interview a Person    Question the suspects             │
│  [3] View Scrabble Board   Track the letters you collect     │
│  [4] Form a Word           Crack the two hidden words        │
│  [5] Accuse a Suspect      Make the final call               │
│  [6] Quit                  Leave the case                    │
└──────────────────────────────────────────────────────────────┘
```

Your investigation is limited by turns and credibility. A wrong move costs credibility, while a correct deduction brings you closer to the final accusation. Search carefully: clues are placed at specific board positions, and the evidence is designed to assemble into two words.

### Investigation loop

| Phase | What happens |
| --- | --- |
| **Search** | Explore the cottage and collect Word 1 clues. |
| **Interview** | Question suspects and collect Word 2 clues. |
| **Decode** | Use the Scrabble-style board to reconstruct both words. |
| **Accuse** | Choose the culprit before your turns or credibility run out. |
| **Record** | Your result is appended to `scoreBoard.txt`. |

## What this project demonstrates

<div align="center">

[![C++](https://skillicons.dev/icons?i=cpp&theme=dark)](https://isocpp.org/)
[![Visual Studio](https://skillicons.dev/icons?i=visualstudio&theme=dark)](https://visualstudio.microsoft.com/)

</div>

- **Encapsulation:** game state and behaviour are organized inside focused classes.

- **Inheritance:** victims, suspects, detectives, and specialized suspect roles share a common character model.

- **Polymorphism:** characters override display and interview behaviour according to their role.

- **Virtual inheritance:** shared character state is handled safely through the suspect role hierarchy.

- **Composition:** the engine manages the cottage, board, racks, characters, rules, and scoreboard.

- **Dynamic memory:** custom `DynamicArray<T>` and game objects demonstrate resource ownership.

- **File I/O:** clues, locations, characters, rules, and scores are loaded from plain-text data files.

- **Exception handling:** invalid input and missing/corrupt data are reported through `GameException`.

## Getting started

### Requirements

- Windows with Visual Studio and the **Desktop development with C++** workload.

- A Windows SDK installed through Visual Studio Installer.

- The project files and data files kept in the same project folder.

### Run in Visual Studio

1. Clone or download this repository.

1. Open **`OOP FINAL PROJECT.sln`** — use the `.sln`, not the `.slnx`, for the broadest compatibility.

1. Choose `Debug` and `x64` from the configuration bar. `Win32` is also available.

1. Build with **Build → Build Solution**.

1. Start with **Debug → Start Without Debugging** so the console stays open.

1. Keep the working directory pointed at the project folder. The game reads its `.txt` files at runtime.

> **Important:** `characters.txt`, `clue.txt`, `locations.txt`, `rules.txt`, `about.txt`, and `scoreBoard.txt` must remain beside the executable's working directory. If the game reports that a data file is missing, check the Debugging → Working Directory setting first.

## Project map

```
.
├── OOP FINAL PROJECT.sln       # Recommended Visual Studio solution
├── OOP FINAL PROJECT.slnx      # Newer solution format
├── OOP FINAL PROJECT.vcxproj   # C++ project configuration
├── main.cpp                    # Program entry point
├── heist.cpp                   # Game implementation
├── heist.h                     # Classes, templates, and declarations
├── characters.txt              # Suspect and character data
├── clue.txt                    # Secret word mappings
├── locations.txt               # Location clue data
├── rules.txt                   # Turn and credibility rules
├── about.txt                   # In-game about screen
└── scoreBoard.txt              # Results log written by the game
```

## A quick playthrough

```
1. Start a new game and enter a detective name.
2. Visit locations until Word 1 is complete.
3. Interview people until Word 2 is complete.
4. Form both words on the Scrabble board.
5. Accuse the suspect you believe is responsible.
6. Check the score board and run the case again.
```

## Design notes

The game intentionally keeps its content in editable text files. That makes it easy to add new suspects, locations, rules, and clue sets without changing the overall game structure. The singleton `GameEngine` coordinates the investigation, while smaller classes own individual gameplay responsibilities.

The data files are part of the game, not optional documentation. Do not rename them or move them away from the runtime working directory unless you also update the file-loading code.



## Credits

Built as an Object-Oriented Programming final project by **Abdur Rehman and Hajra Ali**.

<div align="center">

**The case is open. The clock is running.**

[Back to top](#top)

![Footer](https://capsule-render.vercel.app/api?type=waving&color=0:7f1d1d,55:172554,100:0b1020&height=120&section=footer&animation=fadeIn)

</div> <!--
Visual assets are intentionally remote embeds so this repository stays lightweight:
  - Capsule Render: https://capsule-render.vercel.app/
  - Readme Typing SVG: https://readme-typing-svg.demolab.com/
  - Shields: https://shields.io/
  - Skill Icons: https://skillicons.dev/
-->
