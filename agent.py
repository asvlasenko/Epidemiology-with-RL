import numpy as np
from matplotlib import pyplot as plt

from keras.layers import Dense, Activation
from keras.models import Sequential, load_model
from keras.optimizers import Adam

# Construct neural network with two hidden layers
def build_net(n_output, n_input, n1, n2, learning_rate):
    model = Sequential([Dense(n1, input_shape=(n_input, )),
                        Activation('relu'),
                        Dense(n2),
                        Activation('relu'),
                        Dense(n_output)])
    model.compile(optimizer=Adam(lr=learning_rate), loss = 'mse')
    return model

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

    # Store an event
    # Action is a Boolean array, describing which possible actions are active
    def store(self, state, next_state, action, reward, done):
        i = self.counter % self.size    # Circular buffer
        self.state[i] = state
        self.next_state[i] = next_state
        self.reward[i] = reward
        self.running[i] = 1 - int(done)

        actions = np.zeros(self.n_actions, dtype = np.int8)
        actions[action] = 1.0
        self.action[i] = actions

        self.counter += 1

    # Draw a batch of stored events
    def batch(self, batch_size):
        max_index = min(self.counter, self.size)
        batch = np.random.choice(max_index, batch_size)
        states = self.state[batch]
        next_states = self.next_state[batch]
        actions = self.action[batch]
        rewards = self.reward[batch]
        running = self.running[batch]

        return states, next_states, actions, rewards, running

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

    # Choose a set of actions
    def act(self, state):
        state = state[np.newaxis, :]
        x = np.random.random()

        # Chance to explore by taking a random action
        if x < self.p_random:
            action = np.random.randint(0, self.n_actions)
        else:
            actions = self.brain.predict(state)
            action = np.argmax(actions)

        return action

    def learn(self):
        if self.memory.counter < self.batch_size:
            return

        state, next_state, action, reward, running = \
            self.memory.batch(self.batch_size)

        eval = self.brain.predict(state)
        next = self.brain.predict(next_state)
        target = eval.copy()

        batch_index = np.arange(self.batch_size, dtype = np.int32)

        target[batch_index, action] = \
            reward + self.discount * np.max(next, axis=1) * running
        _ = self.q_eval.fit(state, target, verbose=False)

        if self.p_random > self.p_random_min:
            self.p_random = \
                max(self.p_random * self.p_random_dec, self.p_random_min)

        def save(self):
            self.brain.save(self.fname)

        def load(self):
            self.brain.load(self.fname)
