import numpy as np
import matplotlib.pyplot as plt

A = np.genfromtxt("inside.txt",delimiter=',')
A = A[:,:-1]

plt.figure()
plt.imshow(A)
plt.show()
