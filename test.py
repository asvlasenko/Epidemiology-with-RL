import environment as env
import agent

network = agent.construct_network(n_input = 3,
                                  n_output = 5,
                                  n1 = 100,
                                  n2 = 100,
                                  learning_rate = 0.0005)
