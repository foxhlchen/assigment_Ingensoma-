
# coding: utf-8

# In[1]:

import pandas as pd


# In[2]:

df = pd.read_table('StockTick.txt')


# In[3]:

df


# In[4]:

df.keys()


# In[5]:

# trim emptyspaces and transfer to lower case to keynames
df = df.rename(index=str, columns={oldk: oldk.strip().lower() for oldk in df.keys()})
df


# In[6]:

# now apply function to 'time' strip millisecond
df['time'] = df['time'].apply(lambda x: int(x / 100))
df


# In[48]:

# group by symbol and second, them aggregate price and qty information.
agg_rs = df.groupby(['code', 'time']).agg({'bid qty': 'sum', 'bid price': 'mean', 'ask price': 'mean', 'ask qty': 'sum'})
agg_rs


# In[71]:

agg_rs.loc[60000344]


# In[72]:

for symbol in [60000344, 60000364]:
    agg_rs.loc[symbol].to_csv(str(symbol) + '.csv')

