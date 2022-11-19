#include "bitboard.h"

namespace apentago {

BoardSquare::BoardSquare(const BitBoard& board, int squarePos)  {
        int shiftValue = 0;
        switch (squarePos) {
        case 0:
            break;
        case 1:
            shiftValue = 3;
            break;
        case 2:
            shiftValue = 18;
            break;
        case 3:
            shiftValue = 21;
            break;
        default:
            assert(false);
        }
        square_ = (board.as_int() >> shiftValue) & kSquareMask;
    }


}

int main(void) {
	return 0;
}
