import yfinance as yf
from datetime import datetime, timedelta

data = yf.download("AAPL", start=(datetime.now()-timedelta(days=300)).strftime('%Y-%m-%d'), end=datetime.now().strftime('%Y-%m-%d'))

data.to_csv("historical_data/a.csv")
