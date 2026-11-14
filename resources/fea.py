
import datetime
from datetime import datetime as dt, timedelta
import pandas as pd

from pyqqq.data.minutes import get_all_day_data
from pyqqq.datatypes import DataExchange

# Find the most recent weekday
a = 3

while True:
    target_date = (dt.now() - timedelta(days=a)).date()

    if target_date.weekday() > 4:
        a += 1
    else:
        break

# Date range
start_date = target_date
end_date = dt.now().date()

# Download NXT 1-minute data
all_data = []

current_date = start_date

while current_date <= end_date:

    # Skip weekends
    if current_date.weekday() < 5:

        try:
            data = get_all_day_data(
                date=current_date,
                codes=["005930"],
                period=timedelta(minutes=1),
                source="kis",
                adjusted=True,
                ascending=True,
                exchange=DataExchange.NXT
            )

            df = data["005930"]

            if not df.empty:
                all_data.append(df)
                print(current_date, len(df), "rows")

        except Exception as e:
            print(current_date, e)

    current_date += timedelta(days=1)

# Combine all dates
if all_data:
    final_data = pd.concat(all_data).sort_index()

    final_data.to_csv(
        "historical_data/a.csv",
        encoding="utf-8-sig"
    )

    print(final_data.tail())
else:
    print("No data available")
