### INSTALLING:
1. Clone repo
2. To install package: `pip install .`
3. To debug: `./build.sh; ./build/alpha_pentago`

### TODO:
- ~~Store lookups for 8 variations of each position~~
- Implement iterative deepening
- ~~Improve heuristic evaluation~~
- ~~Implement CMA-ES~~
- ~~Favour longest losing sequence when all moves lose~~
- ~~Don't discard mappings for terminal node lookups~~
- ~~Formalize Makefile/build structure~~

### MOVING FORWARD:
- Once traditional game engine can beat me consistently, implement neural network
- Use games from traditional engine to train supervised engine
- Try and implement Zero Knowledge engine

### NICE TO HAVES:
- GUI for playing against engine
- Opening books (if they exist)
- Server to host code (website to play against it)
