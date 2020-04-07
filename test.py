import epi_model as em

print("Constructing model")
sc = em.EpiScenario()
model = em.EpiModel(sc)
print("  success!")

print("Test run")
input = em.EpiInput()
output = model.get_observables()
while not output.finished:
    model.step(input)
    output = model.get_observables()
print("  success!")
print("  day = ", output.day)
