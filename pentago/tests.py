from pentago import Move, GameResult, Position, Negamax, PositionHistory

def test_legal_moves():
    position_history = PositionHistory()

    for i in range(36):
        legal_moves = position_history.get_last().legal_moves()

        assert len(legal_moves) == 288 - 8 * i

        position_history.push(legal_moves[0])
