# Environment for machine learning agent

import numpy as np

import epi_model as em

# Get observable information from model output
def observations(output, n_obs):
    obs = np.zeros(n_obs, dtype = np.float32)
    n_total = float(output.n_susceptible + output.n_infected \
            + output.n_recovered + output.n_vaccinated + output.n_dead)
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
    return obs

class env:
    # Number of observations
    # For now, keep track of:

    # ratio of susceptible to total,
    # ratio of infected to total,
    # ratio of dead to total
    # ratio of critical to hospital beds,
    # availability of vaccine

    n_obs = 5
    n_actions = 8

    # p_no_outbreak is the chance that an outbreak does not occur at all.
    # start_day is the earliest and latest possible outbreak times.
    # t_vaccine is the shortest and longest time possible between outbreak and
    # vaccine availability.
    def __init__(self, p_no_outbreak = 0.5, start_day = (0,300),
            t_vaccine = (400,700)):

        self.p_no_outbreak = p_no_outbreak
        self.start_day = start_day
        self.t_vaccine = t_vaccine

        self.reset()

    # Reset the world
    # Returns an initial observation, just as with Gym
    def reset(self):
        sc = em.EpiScenario()
        # Control case: no outbreak occurs
        x = np.random.random()
        if x < self.p_no_outbreak:
            sc.t_initial = -1
            sc.t_max = 1000
        # Random disease start date
        x = np.random.random()
        sc.t_initial = (1.0-x)*self.start_day[0] + x*self.start_day[1]
        # Random time to vaccination
        x = np.random.random()
        sc.t_vaccine = sc.t_initial + \
            (1.0-x)*self.t_vaccine[0] + x*self.t_vaccine[1]
        self.world = em.EpiModel(sc)

        output = self.world.get_observables()
        obs = observations(output, self.n_obs)
        return obs

    # Step the world forward based on action, generating output
    # Action is a set of flags, one for each possible control measure
    # Output follows that of AI Gym: observation, reward, done, info
    # For now, info is the same as observation, however when the testing
    # model is implemented, info will describe the true situation, while
    # observation will describe what is visible to the agent.
    def step(self, action):
        assert(action < self.n_actions)
        # Translate action into EpiInput
        input = em.EpiInput()
        input.dist_recommend = bool(action & int('0001', 2))
        input.dist_home_symp = bool(action & int('0010', 2))
        input.dist_home_all = bool(action & int('0100', 2))

        # Take a one-day step
        self.world.step(input)

        # Get observations and other info
        output = self.world.get_observables()
        obs = observations(output, this.n_obs)

        # Reward: negative of cost function
        reward = -output.cost_function

        # done: either maximum time reached, or disease eradicated
        done = output.finished

        # info: just return the output structure, for now
        info = output

        return obs, reward, done, info
