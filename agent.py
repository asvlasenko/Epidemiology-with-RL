import numpy as np
from matplotlib import pyplot as plt

from keras.layers import Dense, Activation
from keras.models import Sequential, load_model
from keras.optimizers import Adam

# Construct neural network with two hidden layers
def build_net(n_output, n_input, n1, n2, learning_rate):
    network = Sequential([Dense(n1, input_shape = (n_input, )),
                          Activation('relu'),
                          Dense(n2),
                          Activation('relu'),
                          Dense(n_output)])
    network.compile(optimizer = Adam(lr = learning_rate), loss = 'mse')
    return network

# Memory record for reinforcement learning
class Memory:
    def __init__(self, size, n_actions, n_input):
        self.size = size
        self.n_actions = n_actions
        self.n_input = n_input

        self.counter = 0

        self.state = np.zeros((self.size, n_input), dtype = np.float32)
        self.next_state = np.zeros((self.size, n_input), dtype = np.float32)
        self.action = np.zeros((self.size, n_actions), dtype = np.int8)
        self.reward = np.zeros(self.size, dtype = np.float32)
        self.running = np.zeros(self.size, dtype = np.float32)

# Agent class, consisting of a neural network plus memory record
class Agent:
    def __init__(self, n_actions, n_input, learning_rate, discount,
        p_random = 1.0, p_random_dec = 0.995, p_random_min = 0.01,
        mem_size = 1024*1024, batch_size = 64, fname = "agent_model.h5"):

        self.n_actions = n_actions
        self.n_input = n_input
        self.learning_rate = learning_rate
        self.discount = discount
        self.p_random = p_random
        self.p_random_dec = p_random_dec
        self.p_random_min = p_random_min
        self.mem_size = mem_size
        self.batch_size = batch_size
        self.fname = fname

        self.brain = build_net(n_actions, n_input, 128, 128, learning_rate)
        self.memory = Memory(mem_size, n_actions, n_input)

        #TODO: self.memory
