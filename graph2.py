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
buylogx=[48,52,67,116,152,217,240,332,336,432,439,451,475,974,1162,1394,1508,1517,1531,1556,1560,1569,1574,1616,1623,1744,1779,1895,2103,2114,2538]
buylogy=[270750,271250,271750,273750,274500,280500,281500,281250,281500,283500,284000,284750,284250,275000,275250,270750
,270000,270500,271250,272250,272500,273750,274250,274500,275000,276000,276750,277500,277500,278750,267250]
selllogx=[51,66,114,151,168,236,253,334,346,438,450,461,484,987,1183,1401,1516,1527,1550,1559,1563,1573,1584,1622,1646,1762,1787,1909,2112,2128,2549]
selllogy=[271750,272000,274000,274750,273500,281500,280500,282000,280500,284500,285000,283250,282750,273500,273000,265750,270750,271500,271250,273000,273500,274500,273000,275250,272500,275750,275750,276000,278750,277000,266250]
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
