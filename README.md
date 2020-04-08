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
https://github.com/asvlasenko/Epidemiology-with-RL/blob/master/mitigation.png
