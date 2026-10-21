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
buylogx=[48,60,117,131,155,233,247,332,339,446,454,475,981,1166,1518,1521,1537,1560,1576,1620,1630,1779,2119,2533,2539]
buylogy=[270750,271500,273500,274000,274500,280500,281000,281250,281500,284000,284500,284250,274750,274000,270250,270500
,271750,272500,274000,274500,275250,276750,278750,266500,267000]
selllogx=[51,77,125,137,158,236,253,334,346,450,461,483,985,1170,1520,1527,1548,1563,1581,1622,1639,1785,2127,2535,2543]

selllogy=[271750,272000,273500,273750,274500,281500,280500,282000,280500,285000,283250,283000,273500,274000,271000,271500,271500,273500,273000,275250,275000,276250,276750,267500,266500]
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
