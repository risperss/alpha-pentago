#include <string>


class BoardNode {
public:
	constexpr BoardNode() {}
	constexpr BoardNode(std::uint8_t num) : node_(num) {}
	constexpr BoardNode(int row, int col) : BoardNode(row * 6 + col) {}
	BoardNode(const std::string& str)
		: BoardNode(str[1] - '1', str[0] - 'a') {}

	constexpr std::uint8_t as_int() const { return node_; }
	constexpr std::uint64_t as_board() const { return 1ULL << node_; }

	void set(int row, int col) { node_ = row * 6 + col; }

	int row() const { return node_ / 6 }
	int col() const { return node_ % 6 }

	static bool IsValidCoord(int x) { return x >= 0 && x < 6; }
	static bool isValid(int row, int col) {
		return IsValidCoord(row) && IsValidCoord(col);
	}

	constexpr bool operator==(const BoardNode& other) const {
		return node_ == other.node_;
	}
	constexpr bool operator!=(const BoardNode& other) const {
		return node_ != other.node_;
	}

	std::string as_string() const {
		return std::string(1, 'a' + col()) + std::string(1, '1' + row());
	}

private:
	std::uint8_t node_ = 0;
};


class BitBoard {
public:
	constexpr BitBoard(std::uint64_t board) : board_(board) {}
	BitBoard() = default;
	BitBoard(const BitBoard&) = default;
	BitBoard& operator=(const BitBoard&) = default;

	std::uint64_t as_int() const { return board_; }
	void clear() { board_ = 0; }

    int count() const {
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

	int count_few() const {
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
	void set_if(std::uint8_t pos, bool cond) {
		board_ |= (std::uint64_t(cond) << pos);
	}
	void set_if(int row, int col, bool cond) {
		set_if(BoardNode(row, col), cond);
	}

	void set(BoardNode node) { set(node.as_int()); }
  	void set(std::uint8_t pos) { board_ |= (std::uint64_t(1) << pos); }
  	void set(int row, int col) { set(BoardNode(row, col)); }

 	void reset(BoardNode node) { reset(node.as_int()); }
  	void reset(std::uint8_t pos) { board_ &= ~(std::uint64_t(1) << pos); }
  	void reset(int row, int col) { reset(BoardNode(row, col)); }

 	bool get(BoardNode node) const { return get(node.as_int()); }
  	bool get(std::uint8_t pos) const {
    	return board_ & (std::uint64_t(1) << pos);
  	}
  	bool get(int row, int col) const { return get(BoardNode(row, col)); }

  	bool empty() const { return board_ == 0; }

  	bool intersects(const BitBoard& other) const { return board_ & other.board_; }


	bool operator==(const BitBoard& other) const {
	   return board_ == other.board_;
	}

    bool operator!=(const BitBoard& other) const {
       return board_ != other.board_;
    }

    std::string DebugString() const {
        std::string res;
        for (int i = 5; i >= 0; --i) {
            for (int j = 0; j < 6; ++j) {
                if (get(i, j)) {
                    res += '#';
                } else {
                    res += '.';
                }
            }
            res += '\n';
        }
        return res;
    }

    BitBoard& operator&=(const BitBoard& a) {
        board_ &= a.board_;
        return *this;
    }

    friend void swap(BitBoard& a, BitBoard& b) {
        using std::swap;
        swap(a.board_, b.board_);
    }

    friend BitBoard operator|(const BitBoard& a, const BitBoard& b) {
        return {a.board_ | b.board_};
    }

    friend BitBoard operator&(const BitBoard& a, const BitBoard& b) {
        return {a.board_ & b.board_};
    }

    friend BitBoard operator-(const BitBoard& a, const BoardNode& b) {
        eturn {a.board_ & ~b.as_board()};
    }

    friend BitBoard operator-(const BitBoard& a, const BitBoard& b) {
        return {a.board_ & ~b.board_};
    }

private:
    std::uint64_t board_ = 0;
};


class BoardSquare {

}


class Move {
public:
    Move() = default;
    constexpr Move(BoardNode node, BoardSquare square, bool clockwise)
      : data_(node.as_int() + (square.as_int() << 6) + 
            (std::uint8_t(clockwise) << 8)) {}
    Move(const std::string& str);
    Move(const char* str) : Move(std::string(str)) {}

    BoardNode node() const { return BoardNode(data_ & kToMask); }
    BoardSquare square() const { return BoardSquare((data_ & kSquareMask) >> 6); }
    bool clockwise() const { return (data_ & kPromoMask) >> 8; }

private:
    uint16_t data_ = 0;
    // Move, using the following encoding:
    // bits 0..5 "to"-node
    // bits 6..7 rotation-square
    // bit 8 clockwise-rotation

    enum Masks : uint16_t {
        kToMask = 0b0000000000111111,
        kSquareMask = 0b0000000011000000,
        kClockwiseMask = 0b0000000100000000,
    };
};


















