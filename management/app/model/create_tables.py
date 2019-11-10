create_tables = {
    "cards": """
            CREATE TABLE IF NOT EXISTS CARDS (
                ID integer PRIMARY KEY
            ); 
            """,

    "locks": """
            CREATE TABLE IF NOT EXISTS LOCKS (
                ID integer PRIMARY KEY
            );
            """,

    "cards_locks": """
            CREATE TABLE IF NOT EXISTS CARDS_LOCKS (
                ID integer PRIMARY KEY,
                CARD_ID integer,
                LOCK_ID integer
            );
            """
}
