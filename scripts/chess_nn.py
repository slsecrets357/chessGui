import torch
import torch.nn as nn
import torch.nn.functional as F

class ChessNN(nn.Module):
    def __init__(self):
        super(ChessNN, self).__init__()
        # Convolutional layers
        self.conv1 = nn.Conv2d(12, 64, kernel_size=3, stride=1, padding=1)
        self.conv2 = nn.Conv2d(64, 128, kernel_size=3, stride=1, padding=1)
        self.conv3 = nn.Conv2d(128, 256, kernel_size=3, stride=1, padding=1)
        
        # Fully connected layers for shared feature extractor
        self.fc1 = nn.Linear(8 * 8 * 256, 1024)
        
        # Policy head
        self.policy_fc = nn.Linear(1024, 4672)
        
        # Value head
        self.value_fc1 = nn.Linear(1024, 256)
        self.value_fc2 = nn.Linear(256, 1)

    def forward(self, x):
        # Convolutional layers
        x = F.relu(self.conv1(x))
        x = F.relu(self.conv2(x))
        x = F.relu(self.conv3(x))
        
        # Flattening the tensor for fully connected layers
        x = x.view(-1, 8 * 8 * 256)
        
        # Shared fully connected layer
        x = F.relu(self.fc1(x))
        
        # Policy head
        policy = self.policy_fc(x)
        
        # Value head
        value = F.relu(self.value_fc1(x))
        value = torch.tanh(self.value_fc2(value))  # Value output in range [-1, 1]
        
        return policy, value

if __name__ == "__main__":
    # Example usage
    # Initialize the neural network
    chess_nn = ChessNN()

    # Dummy input for testing (batch size 1, 12 channels, 8x8 board)
    dummy_input = torch.zeros((1, 12, 8, 8))

    # Forward pass
    policy_output, value_output = chess_nn(dummy_input)

    print("Policy output shape:", policy_output.shape)
    print("Value output shape:", value_output.shape)
