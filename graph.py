import matplotlib.pyplot as plt
import epi_model as em

print("Constructing model")
sc = em.EpiScenario()
model = em.EpiModel(sc)
print("  success!")

day = []
n_susceptible = []
n_infected = []
n_recovered = []
n_dead = []
n_vaccinated = []

input = em.EpiInput()
output = model.get_observables()
while not output.finished:
    model.step(input)
    output = model.get_observables()
    day.append(output.day)
    n_susceptible.append(output.n_susceptible)
    n_infected.append(output.n_infected)
    n_recovered.append(output.n_recovered)
    n_dead.append(output.n_dead)
    n_vaccinated.append(output.n_vaccinated)

fig, (p1, p2) = plt.subplots(1, 2)
fig.set_figheight(4)
fig.set_figwidth(8)

p1.set_xlabel("time, days")
p1.set_ylabel("number of people")
p1.set_yscale("log")
p1.set_title("No Mitigation Measures")
p1.plot(day, n_susceptible, label = "susceptible")
p1.plot(day, n_infected, label = "active")
p1.plot(day, n_recovered, label = "recovered")
p1.plot(day, n_dead, label = "dead")
p1.plot(day, n_vaccinated, label = "vaccinated")

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

while not done:
    action = player.act(obs)
    next, reward, done, output = world.step(action)
    score += reward
    player.record(obs, next, action, reward, done)
    obs = next

    day.append(output.day)
    n_susceptible.append(output.n_susceptible)
    n_infected.append(output.n_infected)
    n_recovered.append(output.n_recovered)
    n_dead.append(output.n_dead)
    n_vaccinated.append(output.n_vaccinated)

p2.set_xlabel("time, days")
p2.set_yscale("log")
p2.set_title("With Mitigation Measures")
p2.plot(day, n_susceptible, label = "susceptible")
p2.plot(day, n_infected, label = "active")
p2.plot(day, n_recovered, label = "recovered")
p2.plot(day, n_dead, label = "dead")
p2.plot(day, n_vaccinated, label = "vaccinated")

plt.legend()
plt.show()
