import yfinance as yf
from datetime import datetime, timedelta
a=2
while(True):
    if (datetime.now()-timedelta(days=a)).weekday() > 4:
        a += 1
    else:
        break;

data = yf.download("005930.KS", start=(datetime.now()-timedelta(days=a)).strftime('%Y-%m-%d'), end=(datetime.now()+timedelta(days=1)).strftime('%Y-%m-%d'), interval='1m')

data.to_csv("historical_data/a.csv")
