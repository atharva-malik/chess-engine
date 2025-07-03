# chess-engine

<p align="center">
  <img src="img/Logo.svg" />
</p>

## Project Overview
This project introduces a novel chess engine designed to offer a challenging yet ultimately beatable opponent for human players. Moving beyond the typical focus on raw strength, this engine incorporates heuristics that emulate human-like thought processes, such as extended 'pondering' over complex positions. My aim is to foster a more enjoyable, engaging, and rewarding human-computer chess experience, addressing the current frustration many players face when confronted by overwhelmingly powerful and "emotionless" AI.

## Features
- Ready to deploy.
- Written in clean code and easily editable.
- Comes with UCI (Universal Chess Interface) support.
- Supports all standard chess rules.
- Written in the robust language, C++.
- Incorporates the latest chess advancements such as NNUEs.

## Technology Used
The engine was made with one main technology, C++17 but uses lots of different ideas/technologies in the background, like [Lichess Bot Hosting](https://github.com/lichess-bot-devs/lichess-bot) and a mini-max style development approach, to make it work.

## Installation

Install the repository, download the latest version of the G++ compiler, open a terminal in the repository directory. Navigate to `/Engine` and compile the `main.cpp` file. You can do this by running the following commands in your terminal:

```bash
g++ -Ofast -march=native main.cpp -o engine
```

### Pre-compiled Binaries
If you do not want to compile the engine yourself, you can download the pre-compiled binaries from the [Releases](https://github.com/atharva-malik/chess-engine/releases/tag/v8.1) page. It has been pre-compiled for `x64` on `Windows`. If you encounter any issues with the pre-compiled binaries, please compile your own version using the instructions above.

### Hosting
To host the engine on Lichess, you can use the [Lichess Bot Hosting](https://github.com/lichess-bot-devs/lichess-bot) repository. Create a Lichess account, then follow the instructions in the repository to set up your bot. Once you have your bot set up, you can place the compiled `engine` executable in the `engines` folder of the Lichess Bot Hosting repository. Make sure to set the correct permissions for the executable in the `config.yml` file. Again, refer to the Lichess Bot Hosting 
repository for detailed instructions.

## Credits
Finally, I want to thank the following people/organisations for their contributions to this project:
- [Niels Lohmann](https://github.com/nlohmann/json/) - For providing the JSON library for C++ which allowed the creation of the opening book to be as simple as it was.
- [Max (Disservin)](https://github.com/Disservin/chess-library) - For providing the blazing fast library that is at the backbone of the chess engine, without which the engine might not be performing as fast as it is.
- [Sebastian Lague](https://github.com/SebLague/Chess-Coding-Adventure) - For providing the idea and motivation for this project
- [Chess Programming](https://github.com/maksimKorzh) - For teaching me how chess engines work in the backend.
- [Stockfish Evaluation Guide](https://hxim.github.io/Stockfish-Evaluation-Guide/) - For providing a simple, yet powerful, graphical tool to understand how NNUEs work

## Development

Want to contribute? Great! Pull requests and issues are welcome! [Here] is an excellent guide on how to create pull requests and forks to request changes.

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job.)

   [Unity 2022]: <https://unity.com/releases/2022-lts>
   [C#]: <https://learn.microsoft.com/en-us/dotnet/csharp/>
   [Here]: <https://www.dataschool.io/how-to-contribute-on-github/>
