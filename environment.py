# Environment for machine learning agent

import numpy as np
from random import random

import epi_model as em

class env:
    # Number of observations
    # For now, keep track of:

    # ratio of susceptible to total,
    # ratio of infected to total,
    # ratio of dead to total
    # ratio of critical to hospital beds,
    # availability of vaccine

    n_obs = 5

    # p_no_outbreak is the chance that an outbreak does not occur at all.
    # start_day is the earliest and latest possible outbreak times.
    # t_vaccine is the shortest and longest time possible between outbreak and
    # vaccine availability.
    def __init__(this, p_no_outbreak = 0.5, start_day = (0,300),
            t_vaccine = (400,700)):

        this.p_no_outbreak = p_no_outbreak
        this.start_day = start_day
        this.t_vaccine = t_vaccine

        this.running = false

        this.reset()

    # Reset the world
    def reset():
        sc = em.EpiScenario()
        # Control case: no outbreak occurs
        x = random()
        if x < p_no_outbreak:
            sc.t_initial = -1
            sc.t_max = 1000
        # Random disease start date
        x = random()
        sc.t_initial = (1.0-x)*start_day[0] + x*start_day[1]
        # Random time to vaccination
        x = random()
        sc.t_vaccine = sc.t_initial + (1.0-x)*t_vaccine[0] + x*t_vaccine[1]
        this.world = em.EpiModel(sc)

    # Step the world forward based on action, generating output
    # Action is a boolean array, one for each possible control measure
    # Output follows that of AI Gym: observation, reward, done, info
    # For now, info is the same as observation, however when the testing
    # model is implemented, info will describe the true situation, while
    # observation will describe what is visible to the agent.
    def step(action):
        # Translate action into EpiInput
        input = em.EpiInput()
        input.dist_recommend = bool(action[0])
        input.dist_home_symp = bool(action[1])
        input.dist_home_all = bool(action[2])

        # Take a one-day step
        this.world.step(input)

        # Get observations and other info
        output = this.world.get_observables()

        # Observations
        obs = np.zeros(this.n_obs, dtype = np.float32)
        n_total = float(obs.n_susceptible + obs.n_infected + obs.n_recovered
                + obs.n_vaccinated + obs.n_dead)
        # ratio of susceptible to total
        obs[0] = float(output.n_susceptible) / n_total
        # ratio of infected to total
        obs[1] = float(output.n_infected) / n_total
        # ratio of dead to total
        obs[2] = float(output.n_dead) / n_total
        # ratio of critical to hospital beds
        obs[3] = float(output.n_critical) / output.hosp_capacity
        # availability of vaccine
        obs[4] = float(output.vaccine_available)

        # Reward: negative of cost function
        reward = -output.cost_function

        # done: either maximum time reached, or disease eradicated
        done = output.finished

        # info: just return the output structure, for now
        info = output

        return obs, reward, done, info
