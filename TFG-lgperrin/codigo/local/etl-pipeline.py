import pandas as pd
from sqlalchemy import create_engine
import time
import logging

logging.basicConfig(level=logging.INFO)

def measure_time(func):
    def wrapper(*args, **kwargs):
        start_time = time.time()
        result = func(*args, **kwargs)
        end_time = time.time()
        logging.info(f"{func.__name__} executed in {end_time - start_time} seconds")
        return result
    return wrapper

@measure_time
def extract(url):
    return pd.read_csv(url)

@measure_time
def transform(data):
    data = data.drop(columns=['Suppressed', 'Series_title_3', 'Series_title_4', 'Series_title_5'], errors='ignore')
    data = clean_dates(data)
    columna_nueva = crear_columna(data, 'Year', 'Data_value', 'Total_Transactions_Year')
    data = data.merge(columna_nueva, on='Year', how='left')
    data = pd.get_dummies(data, columns=['STATUS', 'UNITS'])
    return data.head()

@measure_time
def load(transformed_data, engine, table_name):
    """Load stage of ETL."""
    transformed_data.to_sql(table_name, engine, index=False, if_exists='replace')

def clean_dates(df):
    df['Year'] = df['Period'].apply(lambda x: int(x // 1))
    df['Month'] = df['Period'].apply(lambda x: int((x % 1) * 100))
    return df

def crear_columna(df, key1, key2, column_name):
    grouped_data = df.groupby(key1)[key2].sum().reset_index()
    grouped_data.columns = [key1, column_name]
    return grouped_data

# Ejecución del data pipeline ETL
if __name__ == "__main__":
    start_pipeline = time.time()

    # Extraer (E)
    url = 'https://raw.githubusercontent.com/lgperrin/TFG-lgperrin/main/datasets/electronic-card-transactions-october-2023-csv-tables.csv'
    data = extract(url)

    # Transformar (T)
    transformed_data = transform(data)

    # Cargar (L)
    username = "postgres"
    host = "localhost"
    database = "postgres"
    password = "postgres"
    port = "5432"
    database_url = f'postgresql://{username}:{password}@{host}:{port}/{database}'
    engine = create_engine(database_url)
    table_name = "table_transactions"
    load(transformed_data, engine, table_name)

    end_pipeline = time.time()
    logging.info(f"Total ETL pipeline executed in {end_pipeline - start_pipeline} seconds")
