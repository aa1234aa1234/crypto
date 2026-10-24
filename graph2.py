import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("test.csv")
df2 = pd.read_csv("ema50b.csv")
df3 = pd.read_csv("sma200b.csv")
df4 = pd.read_csv("ema9b.csv")
df5 = pd.read_csv("ema21b.csv")

print(df['Price'])
prev_close = 0
j = 1
test = []
test2 = []
buylogx=[48,118,152,241,321,327,330,336,475,479,981,1521,1537,1539,1560,1625,1627,1630,1741,1777,1895]
buylogy=[270750,273750,274500,281250,281000,281000,281250,281500,284250,284250,274750,270500,271750,271750,272500,275000,275000,275250,276000,276750,277500]
selllogx=[49,119,158,242,326,328,332,346,477,480,982,1523,1538,1540,1562,1626,1628,1639,1742,1779,1899]
selllogy=[271250,273500,274500,281500,281250,281250,281250,280500,284250,284000,274500,270750,271500,271250,273250,275250,275250,275000,276250,276750,278000]
for i in range(len(df)):
    if prev_close == df['Close'][i]:
        continue
    test.append(df['Close'][i])
    test2.append(j)
    prev_close = df['Close'][i]
    j+=1
print(test2)
plt.plot(test2, test, label='Close')
plt.plot(test2, df2['EMA50'], label='EMA50')
plt.plot(test2, df3['SMA200'], label='SMA200')
plt.plot(test2, df4['EMA9'], label='EMA9')
plt.plot(test2, df5['EMA21'], label='EMA21')
plt.legend()

for i in range(len(buylogx)):
    plt.text(buylogx[i],buylogy[i], 'buy', fontsize=10, ha='center')

for i in range(len(selllogx)):
    plt.text(selllogx[i],selllogy[i], 'sell', fontsize=10, ha='center')

plt.show()
