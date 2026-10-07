import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("aa.csv")
df2 = pd.read_csv("ema50.csv")
df3 = pd.read_csv("sma200.csv")
df4 = pd.read_csv("ema9.csv")
df5 = pd.read_csv("ema21.csv")

print(df['Price'])
prev_close = 0
j = 1
test = []
test2 = []
for i in range(2160):
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
plt.text(120,261000, 'buy', fontsize=10, ha='center')
plt.text(123,262750, 'sell', fontsize=10, ha='center')

plt.text(168,264500, 'sell', fontsize=10, ha='center')

plt.text(268,269000, 'buy', fontsize=10, ha='center')
plt.text(280,267000, 'sell', fontsize=10, ha='center')

plt.text(357,267000, 'buy', fontsize=10, ha='center')
plt.text(359,268000, 'sell', fontsize=10, ha='center')

plt.text(361,267500, 'buy', fontsize=10, ha='center')
plt.text(373,268250, 'sell', fontsize=10, ha='center')

plt.text(378,268250, 'buy', fontsize=10, ha='center')
plt.text(385,269000, 'sell', fontsize=10, ha='center')

plt.text(386,268500, 'buy', fontsize=10, ha='center')
plt.text(398,267250, 'sell', fontsize=10, ha='center')

plt.text(921,253500, 'buy', fontsize=10, ha='center')
plt.text(935,252500, 'sell', fontsize=10, ha='center')

plt.text(977,253500, 'buy', fontsize=10, ha='center')
plt.text(985,255750, 'sell', fontsize=10, ha='center')

plt.text(990,255500, 'buy', fontsize=10, ha='center')
plt.text(995,256500, 'sell', fontsize=10, ha='center')

plt.text(999,256500, 'buy', fontsize=10, ha='center')
plt.text(1003,257500, 'sell', fontsize=10, ha='center')

plt.text(1008,257500, 'buy', fontsize=10, ha='center')
plt.text(1011,258750, 'sell', fontsize=10, ha='center')

plt.text(1014,258000, 'buy', fontsize=10, ha='center')
plt.text(1022,256000, 'sell', fontsize=10, ha='center')

plt.text(1078,258500, 'buy', fontsize=10, ha='center')
plt.text(1090,260750, 'sell', fontsize=10, ha='center')

plt.text(1092,260000, 'buy', fontsize=10, ha='center')
plt.text(1110,258750, 'sell', fontsize=10, ha='center')
plt.legend()

plt.show()
