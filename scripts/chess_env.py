import gym
import numpy as np
import chess
from gym import spaces

class ChessEnv(gym.Env):
    def __init__(self):
        super(ChessEnv, self).__init__()
        self.board = chess.Board()
        self.action_space = spaces.Discrete(4672)  # The maximum number of legal moves in a chess position
        self.observation_space = spaces.Box(low=0, high=1, shape=(8, 8, 12), dtype=np.float32)
    
    def reset(self):
        self.board.reset()
        return self._get_obs()
    
    def step(self, action):
        move = self._decode_action(action)
        if move in self.board.legal_moves:
            self.board.push(move)
            reward = self._get_reward()
            done = self.board.is_game_over()
            obs = self._get_obs()
            return obs, reward, done, {}
        else:
            # Invalid move
            return self._get_obs(), -1, True, {}
    
    def _get_obs(self):
        board_array = np.zeros((8, 8, 12), dtype=np.float32)
        for i in range(64):
            piece = self.board.piece_at(i)
            if piece:
                row, col = divmod(i, 8)
                board_array[row, col, self._piece_index(piece)] = 1
        return board_array
    
    def _get_reward(self):
        if self.board.is_checkmate():
            if self.board.turn:
                return -1  # Black wins
            else:
                return 1  # White wins
        elif self.board.is_stalemate() or self.board.is_insufficient_material() or self.board.is_seventyfive_moves() or self.board.is_fivefold_repetition():
            return 0  # Draw
        else:
            # Intermediate rewards
            return 0
    
    def _decode_action(self, action):
        from_square = action // 64
        to_square = action % 64
        move = chess.Move(from_square, to_square)
        return move
    
    def _piece_index(self, piece):
        piece_type = piece.piece_type
        color_offset = 0 if piece.color == chess.WHITE else 6
        return piece_type - 1 + color_offset
    
    def render(self, mode='human'):
        print(self.board)
