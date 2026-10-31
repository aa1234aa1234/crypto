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
for i in range(len(df)):
    if prev_close == df['Close'][i]:
        continue
    test.append(df['Close'][i])
    test2.append(j)
    prev_close = df['Close'][i]
    j+=1
print(test2)
buylogx=[133,189,194,273,328,330,332,357,361,378,563,873,915,961,1002,1028,1065,1078,1086,1141,1143,1145,1188,1210,1270,1272,1499]
buylogy=[263750,264750,265000,268750,266000,266250,266250,267000,267500,268250,265000,251750,252500,252500,257000,257250,257000,258500,258500,259500,259750,259750,259500,260500,260750,260750,252250]
selllogx=[188,190,199,280,329,331,342,359,375,399,572,887,917,983,1022,1031,1071,1085,1110,1142,1144,1152,1209,1231,1271,1274,1508]
selllogy=[265000,264500,264000,267000,266500,266500,265250,268000,268500,267500,262750,250500,254000,254500,256000,256000,258000,259000,258750,260000,260000,258250,260750,261500,261000,253500,251000]
plt.plot(test2, test, label='Close')
plt.plot(test2, df2['EMA50'], label='EMA50')
plt.plot(test2, df3['SMA200'], label='SMA200')
plt.plot(test2, df4['EMA9'], label='EMA9')
plt.plot(test2, df5['EMA21'], label='EMA21')
for i in range(len(buylogx)):
    plt.text(buylogx[i],buylogy[i], 'buy', fontsize=10, ha='center')

for i in range(len(selllogx)):
    plt.text(selllogx[i],selllogy[i], 'sell', fontsize=10, ha='center')
plt.legend()

plt.show()
