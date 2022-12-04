#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

namespace pen {

class BoardNode {
public:
    constexpr BoardNode() { }

    constexpr BoardNode(std::uint8_t num)
        : node_(num)
    {
    }

    constexpr BoardNode(int row, int col)
        : BoardNode(row * 6 + col)
    {
    }

    BoardNode(const std::string& str)
        : BoardNode(str[1] - '1', str[0] - 'a')
    {
    }

    constexpr std::uint8_t as_int() const { return node_; }

    constexpr std::uint64_t as_board() const { return 1ULL << node_; }

    void set(int row, int col) { node_ = row * 6 + col; }

    int row() const { return node_ / 6; }

    int col() const { return node_ % 6; }

    int squarePos() const
    {
        if (row() < 3 && col() < 3) {
            return 0;
        } else if (row() < 3) {
            return 1;
        } else if (col() < 3) {
            return 2;
        } else {
            return 3;
        }
    }

    static bool IsValidCoord(int x) { return x >= 0 && x < 6; }

    static bool isValid(int row, int col)
    {
        return IsValidCoord(row) && IsValidCoord(col);
    }

    constexpr bool operator==(const BoardNode& other) const
    {
        return node_ == other.node_;
    }

    constexpr bool operator!=(const BoardNode& other) const
    {
        return node_ != other.node_;
    }

    std::string as_string() const
    {
        return std::string(1, 'a' + col()) + std::string(1, '1' + row());
    }

private:
    std::uint8_t node_ = 0;
};

class BoardSquare {
public:
    BoardSquare() = default;

    constexpr BoardSquare(std::uint16_t square)
        : square_(square)
    {
    }

    BoardSquare(const BoardSquare&) = default;

    BoardSquare(std::uint64_t board, int squarePos)
        : BoardSquare(
            std::uint16_t((board >> shiftValue(squarePos) & kSquareMask)))
    {
    }

    constexpr std::uint16_t as_int() const { return square_; }

    constexpr std::uint64_t as_board(int squarePos) const
    {
        return std::uint64_t(square_) << shiftValue(squarePos);
    }

    static constexpr int shiftValue(int squarePos)
    {
        return (3 * (squarePos % 2) + 18 * (squarePos / 2));
    }

    bool empty() const { return square_ == 0; }

    bool IsCrossSymmetrical() const { return square_ - kCrossSymmetryMask == 0; }

    bool IsXSymmetrical() const { return square_ - kXSymmetryMask == 0; }

    bool IsSymmetrical()
    {
        return empty() || IsCrossSymmetrical() || IsXSymmetrical();
    }

    void rotate(bool clockwise)
    {
        clockwise ? rotateClockwise() : rotateCounterClockwise();
    }

    std::string DebugString() const
    {
        std::string res;
        std::uint16_t kOneBitMask = 0b0000000000000001;

        for (int i = 2; i >= 0; --i) {
            for (int j = 0; j < 3; ++j) {
                if (as_int() & (kOneBitMask << (i * 6 + j))) {
                    res += '#';
                } else {
                    res += '.';
                }
            }
            res += '\n';
        }
        return res;
    }

private:
    std::uint16_t square_ = 0;

    void rotateClockwise()
    {
        std::uint16_t square = 0;

        square |= (square_ & kClockwiseLeft12Mask) << 12;
        square |= (square_ & kClockwiseRight12Mask) >> 12;
        square |= (square_ & kClockwiseLeft7Mask) << 7;
        square |= (square_ & kClockwiseRight7Mask) >> 7;
        square |= (square_ & kClockwiseLeft5Mask) << 5;
        square |= (square_ & kClockwiseRight5Mask) >> 5;
        square |= (square_ & kClockwiseLeft2Mask) << 2;
        square |= (square_ & kClockwiseRight2Mask) >> 2;

        square_ = square;
    }

    void rotateCounterClockwise()
    {
        std::uint16_t square = 0;

        square |= (square_ & kCounterClockwiseLeft12Mask) << 12;
        square |= (square_ & kCounterClockwiseRight12Mask) >> 12;
        square |= (square_ & kCounterClockwiseLeft7Mask) << 7;
        square |= (square_ & kCounterClockwiseRight7Mask) >> 7;
        square |= (square_ & kCounterClockwiseLeft5Mask) << 5;
        square |= (square_ & kCounterClockwiseRight5Mask) >> 5;
        square |= (square_ & kCounterClockwiseLeft2Mask) << 2;
        square |= (square_ & kCounterClockwiseRight2Mask) >> 2;

        square_ = square;
    }

    enum Masks : uint16_t {
        kSquareMask = 0b0111000101000111,
        kCrossSymmetryMask = 0b0010000101000010,
        kXSymmetryMask = 0b0101000000000101,

        kClockwiseLeft12Mask = 0b0000000000000001,
        kClockwiseRight12Mask = 0b0100000000000000,
        kClockwiseLeft7Mask = 0b0000000001000000,
        kClockwiseRight7Mask = 0b0000000100000000,
        kClockwiseLeft5Mask = 0b0000000000000010,
        kClockwiseRight5Mask = 0b0010000000000000,
        kClockwiseLeft2Mask = 0b0001000000000000,
        kClockwiseRight2Mask = 0b0000000000000100,

        kCounterClockwiseLeft12Mask = 0b0000000000000100,
        kCounterClockwiseRight12Mask = 0b0001000000000000,
        kCounterClockwiseLeft7Mask = 0b0000000000000010,
        kCounterClockwiseRight7Mask = 0b0010000000000000,
        kCounterClockwiseLeft5Mask = 0b0000000100000000,
        kCounterClockwiseRight5Mask = 0b0000000001000000,
        kCounterClockwiseLeft2Mask = 0b0000000000000001,
        kCounterClockwiseRight2Mask = 0b0100000000000000,
    };
};

class BitBoard {
public:
    constexpr BitBoard(std::uint64_t board)
        : board_(board)
    {
    }

    BitBoard() = default;

    BitBoard(const BitBoard&) = default;

    BitBoard& operator=(const BitBoard&) = default;

    std::uint64_t as_int() const { return board_; }

    void clear() { board_ = 0; }

    int count() const
    {
#if defined(NO_POPCNT)
        std::uint64_t x = board_;
        x -= (x >> 1) & 0x5555555555555555;
        x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
        x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F;
        return (x * 0x0101010101010101) >> 56;
#elif defined(_MSC_VER) && defined(_WIN64)
        return _mm_popcnt_u64(board_);
#elif defined(_MSC_VER)
        return __popcnt(board_) + __popcnt(board_ >> 32);
#else
        return __builtin_popcountll(board_);
#endif
    }

    int count_few() const
    {
#if defined(NO_POPCNT)
        std::uint64_t x = board_;
        int count;
        for (count = 0; x != 0; ++count) {
            // Clear the rightmost set bit.
            x &= x - 1;
        }
        return count;
#else
        return count();
#endif
    }

    void set_if(BoardNode node, bool cond) { set_if(node.as_int(), cond); }

    void set_if(std::uint8_t pos, bool cond)
    {
        board_ |= (std::uint64_t(cond) << pos);
    }

    void set_if(int row, int col, bool cond)
    {
        set_if(BoardNode(row, col), cond);
    }

    void set(BoardNode node) { set(node.as_int()); }

    void set(std::uint8_t pos) { board_ |= (std::uint64_t(1) << pos); }

    void set(int row, int col) { set(BoardNode(row, col)); }

    void set(BoardSquare square, int squarePos)
    {
        std::uint64_t kSquareMask = std::uint64_t(0b0111000101000111) << BoardSquare::shiftValue(squarePos);
        std::uint64_t newBoard = as_int() & ~kSquareMask;
        board_ = newBoard | square.as_board(squarePos);
    }

    void reset(BoardNode node) { reset(node.as_int()); }

    void reset(std::uint8_t pos) { board_ &= ~(std::uint64_t(1) << pos); }

    void reset(int row, int col) { reset(BoardNode(row, col)); }

    bool get(BoardNode node) const { return get(node.as_int()); }

    bool get(std::uint8_t pos) const
    {
        return board_ & (std::uint64_t(1) << pos);
    }

    bool get(int row, int col) const { return get(BoardNode(row, col)); }

    bool empty() const { return board_ == 0; }

    bool intersects(const BitBoard& other) const { return board_ & other.board_; }

    bool operator==(const BitBoard& other) const
    {
        return board_ == other.board_;
    }

    bool operator!=(const BitBoard& other) const
    {
        return board_ != other.board_;
    }

    std::string DebugString() const
    {
        std::string res;
        for (int i = 5; i >= 0; --i) {
            for (int j = 0; j < 6; ++j) {
                if (get(i, j)) {
                    res += '#';
                } else {
                    res += '.';
                }
                res += ' ';
            }
            res += '\n';
        }
        return res;
    }

    BitBoard& operator&=(const BitBoard& a)
    {
        board_ &= a.board_;
        return *this;
    }

    friend void swap(BitBoard& a, BitBoard& b)
    {
        using std::swap;
        swap(a.board_, b.board_);
    }

    friend BitBoard operator|(const BitBoard& a, const BitBoard& b)
    {
        return { a.board_ | b.board_ };
    }

    friend BitBoard operator&(const BitBoard& a, const BitBoard& b)
    {
        return { a.board_ & b.board_ };
    }

    friend BitBoard operator-(const BitBoard& a, const BoardNode& b)
    {
        return { a.board_ & ~b.as_board() };
    }

    friend BitBoard operator-(const BitBoard& a, const BitBoard& b)
    {
        return { a.board_ & ~b.board_ };
    }

private:
    std::uint64_t board_ = 0;
};

class Move {
public:
    Move() = default;

    constexpr Move(std::uint16_t num)
        : data_(num)
    {
    }

    constexpr Move(BoardNode node, int squarePos, bool clockwise)
        : data_(node.as_int() + (squarePos << 6) + (std::uint16_t(clockwise) << 8))
    {
    }

    Move(const std::string& str)
        : Move(BoardNode(str.substr(0, 2)), str[3] - '1', str[4] == 'R')
    {
    }

    Move(const char* str)
        : Move(std::string(str))
    {
    }

    constexpr std::uint16_t as_int() const { return data_; }

    BoardNode node() const { return BoardNode(data_ & kNodeMask); }

    int squarePos() const { return (data_ & kSquarePosMask) >> 6; }

    bool clockwise() const { return (data_ & kClockwiseMask) >> 8; }

    void SetNode(BoardNode node) { data_ = (data_ & ~kNodeMask) | node.as_int(); }

    void SetSquarePos(int square)
    {
        data_ = (data_ & ~kSquarePosMask) | (square << 6);
    }

    void SetClockwise(bool clockwise)
    {
        data_ = (data_ & ~kClockwiseMask) | (std::uint16_t(clockwise) << 8);
    }

    bool nodeOnSquare() const
    {
        return squarePos() == node().squarePos();
    }

    std::string as_string()
    {
        return node().as_string()
            + "-"
            + std::string(1, '1' + squarePos())
            + std::string(1, clockwise() ? 'R' : 'L');
    }

private:
    std::uint16_t data_ = 0;
    // Move, using the following encoding:
    // bits 0..5 "to"-node
    // bits 6..7 rotation-square-index
    // bit 8 clockwise-rotation

    enum Masks : uint16_t {
        kNodeMask = 0b0000000000111111,
        kSquarePosMask = 0b0000000011000000,
        kClockwiseMask = 0b0000000100000000,
    };
};

using MoveList = std::vector<Move>;

} // namespace pen
