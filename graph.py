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
plt.legend()
plt.show()
