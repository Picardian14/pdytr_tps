import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as stats
import math

f = open("./tiempos", "r")
crude = f.read()
splited = crude.split('\n')
print(splited)
splited.remove('')
numeros = []
for i in splited:
    numeros.append(int(i.replace('.000000', '')))
arnums = np.array(numeros)


mu = arnums.mean()
variance = arnums.var()
sigma = math.sqrt(variance)
x = np.linspace(mu - 3*sigma, mu + 3*sigma, 100)
plt.plot(x, stats.norm.pdf(x, mu, sigma))
plt.savefig('ploteo.png', dpi=300, bbox_inches='tight')
plt.show()
