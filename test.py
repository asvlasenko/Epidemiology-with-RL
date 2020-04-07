import epi_model as em
from environment import env
import agent

world = env()

player = agent.Agent(8, 5, 0.0005, 0.99)
