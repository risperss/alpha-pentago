#include <iostream>
#include <chrono>
#include <unordered_map>
#include "stdlib.h"

#include "pentago/position.h"

namespace pen {

    std::unordered_map<std::uint64_t, int> lookup;

    // 110011 111111 011110 011110 111111 110011
    std::uint64_t goodNodesMask = 0xCFF79EFF3;

    double good_squares_score(BitBoard ourBoard, BitBoard theirBoard) {
        int ourGoodSquares = ourBoard.count();
        int theirGoodSquares = theirBoard.count();

        return (ourGoodSquares - theirGoodSquares) / 36.0;
    }

    double heuristic_value(Position position) {
        BitBoard ourBoard = BitBoard(position.GetBoard().our_pieces() & goodNodesMask);
        BitBoard theirBoard = BitBoard(position.GetBoard().their_pieces() & goodNodesMask);

        int ourGoodSquares = ourBoard.count();
        int theirGoodSquares = theirBoard.count();

        double value = (ourGoodSquares - theirGoodSquares) / 36.0;

        if (position.IsBlackToMove()) {
            value = -value;
        }

        return value;
    }

    std::pair<Move, double> minimax(Position position, Move prevMove, int depth, double alpha, double beta) {
        GameResult result = GameResult::UNDECIDED;

        if (position.GetPlyCount() >= 9) {
            result = position.ComputeGameResult();
        }

        if (depth == 0 || result != GameResult::UNDECIDED) {
            double value;

            if (result == GameResult::WHITE_WON) {
                value = 1.0;
            } else if (result == GameResult::DRAW) {
                value = 0.0;
            } else if (result == GameResult::BLACK_WON) {
                value = -1.0;
            } else {
                value = heuristic_value(position);
            }

            return std::pair<Move, double>{prevMove, value};
        }

        if (position.IsBlackToMove()) {
            double minEval = 2.0;
            Move move;

            for (Move m: position.GetBoard().GenerateLegalMoves()) {
                Position p = Position(position, m);
                std::uint64_t hash = p.Hash();
                double eval;

                if (lookup.find(hash) != lookup.end()) {
                    eval = lookup.find(hash)->second;
                } else {
                    eval = std::get<double>(minimax(p, m, depth - 1, alpha, beta));
                    lookup[hash] = eval;
                }

                if (eval < minEval) {
                    minEval = eval;
                    move = m;
                }
                beta = std::min(beta, eval);

                if (beta <= alpha) {
                    break;
                }
            }
            return std::pair<Move, double>(move, minEval);
        } else {
            double maxEval = -2.0;
            Move move;

            for (Move m: position.GetBoard().GenerateLegalMoves()) {
                Position p = Position(position, m);
                std::uint64_t hash = p.Hash();
                double eval;

                if (lookup.find(hash) != lookup.end()) {
                    eval = lookup.find(hash)->second;
                } else {
                    eval = std::get<double>(minimax(p, m, depth - 1, alpha, beta));
                    lookup[hash] = eval;
                }

                if (eval > maxEval) {
                    maxEval = eval;
                    move = m;
                }
                alpha = std::max(alpha, eval);

                if (beta <= alpha) {
                    break;
                }
            }
            return std::pair<Move, double>(move, maxEval);
        }
    }

    std::pair<Move, double> value(Position position, int depth = 4) {
        // TODO: how to deal with this Move I have to pass...
        lookup.clear();
        return minimax(position, Move("d6-4R"), depth, -2, 2);
    }

}

int main(void) {
    pen::PentagoBoard board = pen::PentagoBoard("w....w/.b..b./....../....../.b..b./w....w");
    pen::Position starting = pen::Position(board);
    pen::PositionHistory history = pen::PositionHistory(starting);

    while (history.ComputeGameResult() == pen::GameResult::UNDECIDED) {
        std::pair<pen::Move, double> value = pen::value(history.Last());

        std::cout << history.Last().DebugString() << std::endl;
        std::cout << "Value: " << std::get<double>(value) << std::endl;
        std::cout << "------------------------------" << std::endl;
        std::cout << "Move: " << std::get<pen::Move>(value).as_string() << std::endl;

        history.Append(std::get<pen::Move>(value));
    }

    std::cout << history.Last().DebugString() << std::endl;
    std::cout << pen::resultString.find(history.ComputeGameResult())->second << std::endl;

    return 0;
}