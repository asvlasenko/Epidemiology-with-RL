import numpy as np

import epi_model as em
from environment import env
import agent

world = env()
player = agent.Agent(8, 5, 0.0005, 0.99)

n_runs = 500
losses = []

for i in range(n_runs):
    done = False
    loss = 0.0
    obs = world.reset()

    while not done:
        action = player.act(obs)
        next, reward, done, info = world.step(action)
        score += reward
        player.record(obs, next, action, reward, done)
        obs = next
        player.learn()

    losses.append(loss)
    avg_loss = np.mean(losses[max(0,i-100):(i+1)])
    print("run: ", i, " loss: %i" % int(loss),
          " avg: %i" % int(avg_loss))

    if i % 10 == 9:
        player.save()
