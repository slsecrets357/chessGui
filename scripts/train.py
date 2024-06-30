import numpy as np
import chess
import chess.engine
import torch
import torch.nn.functional as F
from collections import defaultdict
from chess_env import ChessEnv
from chess_nn import ChessNN
import tqdm

class MCTS:
    def __init__(self, policy_value_fn, c_puct=1.4, n_playout=1000):
        self.policy_value_fn = policy_value_fn
        self.c_puct = c_puct
        self.n_playout = n_playout
        self.Q = defaultdict(float)
        self.N = defaultdict(float)
        self.P = {}
        self.states = {}

    def _playout(self, board):
        for _ in range(self.n_playout):
            current_board = board.copy()
            states, actions = [], []
            while not current_board.is_game_over():
                legal_moves = list(current_board.legal_moves)
                board_str = str(current_board)
                if board_str not in self.P:
                    state_tensor = self._state_to_tensor(current_board)
                    policy, value = self.policy_value_fn(state_tensor)
                    policy = policy.detach().numpy().flatten()
                    value = value.detach().item()
                    self.P[board_str] = policy
                    self.states[board_str] = value
                    for move in legal_moves:
                        move_index = self._encode_action(move)
                        self.Q[(board_str, move_index)] = 0
                        self.N[(board_str, move_index)] = 0
                    break
                action = self._select_action(current_board)
                current_board.push(action)
                states.append(board_str)
                actions.append(self._encode_action(action))
            reward = self._evaluate(current_board)
            for state, action in zip(states, actions):
                self._update(state, action, reward)
                reward = -reward

    def _select_action(self, board):
        legal_moves = list(board.legal_moves)
        board_str = str(board)
        total_visit = sum(self.N[(board_str, self._encode_action(move))] for move in legal_moves)
        best_move = max(
            legal_moves,
            key=lambda move: self.Q[(board_str, self._encode_action(move))] + self.c_puct * self.P[board_str][self._encode_action(move)] * np.sqrt(total_visit) / (1 + self.N[(board_str, self._encode_action(move))])
        )
        return best_move

    def _update(self, state, action, reward):
        self.Q[(state, action)] += reward
        self.N[(state, action)] += 1

    def _evaluate(self, board):
        if board.is_checkmate():
            return 1 if board.turn == chess.WHITE else -1
        if board.is_stalemate() or board.is_insufficient_material() or board.is_seventyfive_moves() or board.is_fivefold_repetition():
            return 0
        return self.states[str(board)]

    def _state_to_tensor(self, board):
        board_array = np.zeros((12, 8, 8), dtype=np.float32)
        for i in range(64):
            piece = board.piece_at(i)
            if piece:
                row, col = divmod(i, 8)
                board_array[self._piece_index(piece), row, col] = 1
        state_tensor = torch.tensor(board_array, dtype=torch.float32).unsqueeze(0)
        return state_tensor

    def _piece_index(self, piece):
        piece_type = piece.piece_type
        color_offset = 0 if piece.color == chess.WHITE else 6
        return piece_type - 1 + color_offset

    def _encode_action(self, move):
        return move.from_square * 64 + move.to_square

    def get_action_probs(self, board, temp=1e-3):
        self._playout(board)
        legal_moves = list(board.legal_moves)
        print("legal moves: ", legal_moves)
        action_probs = np.zeros(4672)
        board_str = str(board)
        for move in legal_moves:
            move_index = self._encode_action(move)
            action_probs[move_index] = self.N[(board_str, move_index)]
        action_probs = action_probs ** (1 / temp)

        # Adding a small epsilon to avoid division by zero
        action_probs += 1e-8

        action_probs /= np.sum(action_probs)
        return action_probs

def train():
    chess_nn = ChessNN()
    optimizer = torch.optim.Adam(chess_nn.parameters(), lr=1e-4)
    mcts = MCTS(chess_nn.forward)

    num_episodes = 1000  # Number of self-play games
    for episode in tqdm.tqdm(range(num_episodes), desc="Training Episodes"):
        env = ChessEnv()
        states, mcts_probs, rewards = [], [], []

        # Self-play game
        state = env.reset()
        step_count = 0
        while True:
            state_tensor = mcts._state_to_tensor(env.board)
            action_probs = mcts.get_action_probs(env.board, temp=1e-1)  # Adjusted temp value
            action = np.random.choice(4672, p=action_probs)
            next_state, reward, done, _ = env.step(action)
            
            states.append(state_tensor)
            mcts_probs.append(action_probs)
            rewards.append(reward)

            state = next_state
            step_count += 1
            if done:
                print(f"Episode {episode} finished after {step_count} steps with reward {reward}")
                break

        # Train neural network
        for i in range(len(states)):
            target_v = rewards[i]
            target_p = mcts_probs[i]
            state_tensor = states[i]

            optimizer.zero_grad()
            policy, value = chess_nn(state_tensor)
            value_loss = F.mse_loss(value, torch.tensor([target_v], dtype=torch.float32))
            policy_loss = -torch.mean(torch.sum(torch.tensor(target_p, dtype=torch.float32) * torch.log(policy), dim=1))
            loss = value_loss + policy_loss
            loss.backward()
            optimizer.step()

        print(f"Episode {episode} completed with loss: {loss.item()}")

train()

# Example usage
# policy_value_fn = ChessNN().forward
# mcts = MCTS(policy_value_fn)
# env = ChessEnv()

# state = env.reset()
# action_probs = mcts.get_action_probs(env.board)
