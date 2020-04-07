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
cost_function = []

print("Test run")
input = em.EpiInput()
output = model.get_observables()
while not output.finished:
    model.step(input)
    output = model.get_observables()
    day.append(output.day)
    n_susceptible.append(output.n_susceptible/1e6)
    n_infected.append(output.n_infected/1e6)
    n_recovered.append(output.n_recovered/1e6)
    n_dead.append(output.n_dead/1e6)
    n_vaccinated.append(output.n_vaccinated/1e6)

    cost_function.append(output.cost_function/1e9)

print("  success!")
print("  day = ", output.day)

plt.xlabel("time, days")
plt.ylabel("number, millions")
plt.title("Control Test: No Measures Taken")
plt.plot(day, n_susceptible, label = "susceptible")
plt.plot(day, n_infected, label = "active")
plt.plot(day, n_recovered, label = "recovered")
plt.plot(day, n_dead, label = "dead")
plt.plot(day, n_vaccinated, label = "vaccinated")

plt.legend()
plt.show()

plt.xlabel("time, days")
plt.ylabel("cost function")
plt.plot(day, cost_function, label = "cost function")
plt.show()
