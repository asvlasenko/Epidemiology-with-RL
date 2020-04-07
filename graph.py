import matplotlib.pyplot as plt
import numpy as np

import epi_model as em
from environment import env
import agent

world = env(benchmark = True)

player = agent.Agent(8, 5, 0.0005, 0.99, p_random = 0.0, p_random_min = 0.0)
player.load()

done = False
score = 0.0
obs = world.reset()

day = []
n_susceptible = []
n_infected = []
n_recovered = []
n_dead = []
n_vaccinated = []
cost_function = []

while not done:
    action = player.act(obs)
    next, reward, done, output = world.step(action)
    score += reward
    player.record(obs, next, action, reward, done)
    obs = next

    day.append(output.day)
    n_susceptible.append(output.n_susceptible/1e6)
    n_infected.append(output.n_infected/1e6)
    n_recovered.append(output.n_recovered/1e6)
    n_dead.append(output.n_dead/1e6)
    n_vaccinated.append(output.n_vaccinated/1e6)

    cost_function.append(output.cost_function/1e9)

plt.xlabel("time, days")
plt.ylabel("number, millions")
plt.title("Control Test: Mitigation Measures")
plt.plot(day, n_susceptible, label = "susceptible")
plt.plot(day, n_infected, label = "active")
plt.plot(day, n_recovered, label = "recovered")
plt.plot(day, n_dead, label = "dead")
plt.plot(day, n_vaccinated, label = "vaccinated")

plt.legend()
plt.show()
