### BUILDING:
1. Install `cmake`
2. `mkdir build; cd build`
3. `cmake ..`
4. `make`
5. `./alpha_pentago`

### TODO:
- Store lookups for 8 variations of each position
- Implement iterative deepening
- Improve heuristic evaluation
- Implement CMA-ES
- Favour longest losing sequence when all moves technically lose
- ~~Don't discard mappings for terminal node lookups~~
- Formalize Makefile/build structure

### MOVING FORWARD:
- Once traditional game engine can beat me consistently, implement neural network
- Use games from traditional engine to train supervised engine
- Try and implement Zero Knowledge engine

### NICE TO HAVES:
- GUI for playing against engine
- Opening books (if they exist)
- Server to host code (website to play against it)
