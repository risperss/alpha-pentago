#include "stdlib.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <unordered_map>

#include "pentago/position.h"
#include "utils/bitops.h"

namespace pen {

std::unordered_map<std::uint64_t, int> lookup;

/*
 * For BoardSquare:
 *   ###
 *   ###
 *   ###
 *
 * The weights are as follows:
 *   313
 *   151
 *   313
 */

std::uint64_t kSidesMask = 0b010010101101010010010010101101010010;
std::uint64_t kCornersMask = 0b101101000000101101101101000000101101;
std::uint64_t kCentresMask = 0b000000010010000000000000010010000000;

int heuristic_value(Position position)
{
    std::uint64_t ours = position.GetBoard().our_pieces().as_int();
    std::uint64_t theirs = position.GetBoard().their_pieces().as_int();

    int ourScore = count(ours & kSidesMask) + 3 * count(ours & kCornersMask) + 5 * count(ours & kCentresMask);

    int theirScore = count(theirs & kSidesMask) + 3 * count(theirs & kCornersMask) + 5 * count(theirs & kCentresMask);

    int score = ourScore - theirScore;

    if (position.IsBlackToMove()) {
        score = -score;
    }

    return score;
}

std::pair<Move, int> minimax(Position position, Move prevMove, int depth, int alpha, int beta)
{
    GameResult result = GameResult::UNDECIDED;

    if (position.GetPlyCount() >= 9) {
        result = position.ComputeGameResult();
    }

    if (depth == 0 || result != GameResult::UNDECIDED) {
        int value;

        if (result == GameResult::WHITE_WON) {
            value = 1000;
        } else if (result == GameResult::DRAW) {
            value = 0;
        } else if (result == GameResult::BLACK_WON) {
            value = -1000;
        } else {
            value = heuristic_value(position);
        }

        return std::pair<Move, int> { prevMove, value };
    }

    MoveList legalMoves = position.GetBoard().GenerateLegalMoves();

    if (position.IsBlackToMove()) {
        int minEval = INT32_MAX;
        Move move;

        for (Move m : legalMoves) {
            Position p = Position(position, m);
            std::uint64_t hash = p.Hash();
            int eval;

            if (lookup.find(hash) != lookup.end()) {
                eval = lookup.find(hash)->second;
            } else {
                eval = std::get<int>(minimax(p, m, depth - 1, alpha, beta));
                lookup[hash] = eval;
            }

            if (eval < minEval) {
                minEval = eval;
                move = m;
            }
            beta = std::min(eval, beta);

            if (beta <= alpha) {
                break;
            }
        }
        return std::pair<Move, int>(move, minEval);
    } else {
        int maxEval = -INT32_MAX;
        Move move;

        for (Move m : legalMoves) {
            Position p = Position(position, m);
            std::uint64_t hash = p.Hash();
            int eval;

            if (lookup.find(hash) != lookup.end()) {
                eval = lookup.find(hash)->second;
            } else {
                eval = std::get<int>(minimax(p, m, depth - 1, alpha, beta));
                lookup[hash] = eval;
            }

            if (eval > maxEval) {
                maxEval = eval;
                move = m;
            }
            alpha = std::max(eval, alpha);

            if (beta <= alpha) {
                break;
            }
        }
        return std::pair<Move, int>(move, maxEval);
    }
}

std::pair<Move, int> value(Position position, int depth = 5)
{
    lookup.clear();
    return minimax(position, Move("d6-4R"), depth, -INT32_MAX, INT32_MAX);
}
}

void selfPlay()
{
    pen::PentagoBoard board = pen::PentagoBoard();
    pen::Position starting = pen::Position(board);
    pen::PositionHistory history = pen::PositionHistory(starting);

    while (history.ComputeGameResult() == pen::GameResult::UNDECIDED) {
        std::pair<pen::Move, int> value = pen::value(history.Last());

        std::cout << "Ply Count: " << history.Last().GetPlyCount() << std::endl;
        std::cout << "To Move: " << (history.Last().IsBlackToMove() ? "Black" : "White") << std::endl;
        std::cout << "Value: " << std::get<int>(value) << std::endl;
        std::cout << "Heuristic Value: " << pen::heuristic_value(history.Last()) << std::endl;
        std::cout << "Move to be made: " << std::get<pen::Move>(value).as_string() << std::endl;
        std::cout << history.Last().DebugString() << std::endl;
        std::cout << "------------------------------" << std::endl;

        history.Append(std::get<pen::Move>(value));
    }

    std::cout << history.Last().DebugString() << std::endl;
    std::cout << pen::resultString.find(history.ComputeGameResult())->second << std::endl;
}

void vsHuman()
{
    pen::PentagoBoard board = pen::PentagoBoard();
    pen::Position starting = pen::Position(board);
    pen::PositionHistory history = pen::PositionHistory(starting);

    bool humanMove = true;

    while (history.ComputeGameResult() == pen::GameResult::UNDECIDED) {
        pen::Move move;

        if (humanMove) {
            std::cout << history.Last().DebugString() << std::endl;

            std::string moveString;

            std::cout << "Enter a move: ";
            std::cin >> moveString;

            move = pen::Move(moveString);
        } else {
            std::pair<pen::Move, int> value = pen::value(history.Last());

            move = std::get<pen::Move>(value);

            std::cout << "CPU Played: " << move.as_string() << std::endl;
        }

        history.Append(move);
        humanMove = !humanMove;
    }

    std::cout << history.Last().DebugString() << std::endl;
    std::cout << pen::resultString.find(history.ComputeGameResult())->second << std::endl;
}

int main(void)
{
    selfPlay();
    // vsHuman();

    return 0;
}
