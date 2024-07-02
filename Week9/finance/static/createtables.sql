CREATE TABLE stocks (
    id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    user_id INTEGER NOT NULL,
    shares INTEGER NOT NULL,
    symbol TEXT NOT NULL,
    price REAL NOT NULL
);

CREATE TABLE history (
    user_id INTEGER NOT NULL,
    type TEXT NOT NULL,
    symbol TEXT NOT NULL,
    shares INTEGER NOT NULL,
    price REAL NOT NULL,
    timestamp INTEGER NOT NULL
);