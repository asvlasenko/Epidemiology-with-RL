import numpy as np

import epi_model as em
from environment import env
import agent

world = env()
player = agent.Agent(8, 5, 0.0005, 0.99)

n_runs = 500
scores = []

for i in range(n_runs):
    done = False
    score = 0.0
    obs = world.reset()

    while not done:
        action = player.act(obs)
        next, reward, done, info = world.step(action)
        score += reward
        player.record(obs, next, action, reward, done)
        obs = next
        player.learn()

    scores.append(score)
    avg_score = np.mean(scores[max(0,i-100):(i+1)])
    print("episode ", i, " score %i" % int(score),
          " average score %i" % int(avg_score))
