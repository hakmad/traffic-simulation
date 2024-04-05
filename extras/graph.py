import pandas
import matplotlib.pyplot as plt

# Get data.
data = pandas.read_csv("result.csv")

lp = data.loc[:, ["Left Period"]]
lar = data.loc[:, ["Left Arrival Rate"]]
lnoc = data.loc[:, ["Left Number of Cars"]]
lawt = data.loc[:, ["Left Average Waiting Time"]]
lmwt = data.loc[:, ["Left Maximum Waiting Time"]]
lttc = data.loc[:, ["Left Time to Clear"]]

rp = data.loc[:, ["Right Period"]]
rar = data.loc[:, ["Right Arrival Rate"]]
rnoc = data.loc[:, ["Right Number of Cars"]]
rawt = data.loc[:, ["Right Average Waiting Time"]]
rmwt = data.loc[:, ["Right Maximum Waiting Time"]]
rttc = data.loc[:, ["Right Time to Clear"]]

# Setup figure and plot.
fig = plt.figure()
ax = fig.add_subplot(projection="3d")

# Uncomment one of the following.
#ax.scatter(lp, lar, lnoc, label="Left Number of Cars")
#ax.scatter(lp, lar, lawt, label="Left Average Waiting Time")
#ax.scatter(lp, lar, lmwt, label="Left Maximum Waiting Time")
#ax.scatter(lp, lar, lttc, label="Left Time to Clear")

ax.scatter(rp, rar, rnoc, label="Right Number of Cars", c="tab:pink")
#ax.scatter(rp, rar, rawt, label="Right Average Waiting Time")
#ax.scatter(rp, rar, rmwt, label="Right Maximum Waiting Time")
#ax.scatter(rp, rar, rttc, label="Right Time to Clear")

# Add labels and legends.
ax.set_xlabel("Period")
ax.set_ylabel("Arrival Rate")
ax.legend()

# Show the figure.
plt.show()
