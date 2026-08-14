import yfinance
from datetime import datetime, timedelta

data = yfinance.download("005930.KS", start=(datetime.now()-timedelta(days=200)).strftime("%Y-%m-%d"), end=datetime.now().strftime("%Y-%m-%d"))
data.to_csv("a.csv")
