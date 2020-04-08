This is a very simple epidemiology model, describing the spread of a disease
similar to COVID-19 in a population of 300 million people.

The program is built with gcc -std=C99, Python v3.7.4, Cython v0.29.15,
TensorFlow v2.0.0 and Keras v2.3.1.  It has been tested with Linux and MinGW.

The model has a Python interface that supports reinforcement learning with
Keras, similar to OpenAI Gym.

To compile the model, install Cython and build with:
  python setup.py build_ext -i

To train the mitigation policy model, use
  python train.py

To test the performance of the model after training, use
  python test.py

Finally, to plot the outcome of the model, without mitigation and with
mitigation strategies determined by the AI, use
  python graph.py

Here is a typical output of graph.py, showing the effect of mitigation
strategies on the disease outbreak:
![Sample Output](https://github.com/asvlasenko/Epidemiology-with-RL/blob/master/mitigation.png)

TODO:
1.  Make the code comments reasonably decent.
2.  Make it impossible to change quarantine conditions on a dime - right now,
the model learns to change mitigation measures on a day-to-day basis, depending
on small changes in the situation.
3.  Put in a testing model.  The current model knows exactly how many people
are infected, but not which ones.  This should be the other way around, and
the information available should depend on choice of testing availability and
policy.
4.  Implement a multiple-population, hierarchical model with transport between
populations.  This will also allow travel restrictions to be put in as a
mitigation measure, and to impose quarantine on a per-population basis.
5.  Allow for activation of temporary hospital capacity, as well as longer-term
hospital capacity expansion.
