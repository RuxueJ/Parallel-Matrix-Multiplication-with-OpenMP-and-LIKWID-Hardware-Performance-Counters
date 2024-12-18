"""

E. Wes Bethel, Copyright (C) 2022

October 2022

Description: This code loads a .csv file and creates a 3-variable plot

Inputs: the named file "sample_data_3vars.csv"

Outputs: displays a chart with matplotlib

Dependencies: matplotlib, pandas modules

Assumptions: developed and tested using Python version 3.8.8 on macOS 11.6

"""

import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import LogLocator, ScalarFormatter
import numpy as np


# fname = "basic_speedup.csv"
fname = "block_speedup.csv"
df = pd.read_csv(fname, comment="#")
print(df)

var_names = list(df.columns)

print("var names =", var_names)

# split the df into individual vars
# assumption: column order - 0=problem size, 1=blas time, 2=basic time

problem_sizes = df[var_names[0]].values.tolist()
# print(problem_sizes)
block4_thread4 = df[var_names[1]].values.tolist()
# print(code1_time)
block4_thread16 = df[var_names[2]].values.tolist()
# print(code2_time)
block4_thread64 = df[var_names[3]].values.tolist()
# print(code3_time)
block16_thread4 = df[var_names[4]].values.tolist()

block16_thread16 = df[var_names[5]].values.tolist()
# print(code3_time)
block16_thread64 = df[var_names[6]].values.tolist()




xlocs = [i for i in range(len(problem_sizes))]

plt.figure(figsize=(10, 6))

plt.xticks(xlocs, problem_sizes)

# here, we are plotting the raw values read from the input .csv file, which
# we interpret as being "time" that maps directly to the y-axis.
#
# what if we want to plot MFLOPS instead? How do we compute MFLOPS from
# time and problem size? You may need to add some code here to compute
# MFLOPS, then modify the plt.plot() lines below to plot MFLOPS rather than time.


plt.plot(block4_thread4, "b-x",linewidth=1)
plt.plot(block4_thread16, "g-^",linewidth=1)
plt.plot(block4_thread64, "r-o",linewidth=1)
plt.plot(block16_thread4, "b-x",linewidth=2)
plt.plot(block16_thread16, "g-^",linewidth=2)
plt.plot(block16_thread64,  "r-o",linewidth=2)
plt.xlabel("Problem Sizes")
plt.ylabel("speedup")
plt.title("Block MMUL OpenMP Parallelization")  # Set title for

#plt.xscale("log")
# plt.yscale("log")


# varNames = [ var_names[2], var_names[3]]
varNames = [var_names[1], var_names[2], var_names[3],var_names[4],var_names[5],var_names[6]]
plt.legend(varNames, loc="best")

plt.grid(axis='both')

plt.show()

# EOF
