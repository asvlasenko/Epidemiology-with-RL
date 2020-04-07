import numpy as np
from matplotlib import pyplot as plt

from keras.layers import Dense, Activation
from keras.models import Sequential, load_model
from keras.optimizers import Adam

# Construct neural network with two hidden layers
def construct_network(learning_rate, n_input, n_output, n1, n2):
    network = Sequential([Dense(n1, input_shape = (n_input, )),
                          Activation('relu'),
                          Dense(n2),
                          Activation('relu'),
                          Dense(n_output)])
    network.compile(optimizer = Adam(lr = learning_rate), loss = 'mse')
    return network
