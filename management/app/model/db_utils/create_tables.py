create_tables = {
    "tokens": """
            CREATE TABLE IF NOT EXISTS TOKENS (
                ID integer PRIMARY KEY
            ); 
            """,

    "locks": """
            CREATE TABLE IF NOT EXISTS LOCKS (
                ID integer PRIMARY KEY
            );
            """,

    "auth_mappings": """
            CREATE TABLE IF NOT EXISTS AUTH_MAPPINGS (
                ID integer PRIMARY KEY,
                TOKEN_ID integer,
                LOCK_ID integer,
                FOREIGN KEY(TOKEN_ID) REFERENCES tokens(id),
                FOREIGN KEY(LOCK_ID) REFERENCES locks(id)
            );
            """,

    "accesses": """
            CREATE TABLE IF NOT EXISTS ACCESSES (
                ID integer PRIMARY KEY,
                TOKEN_ID integer,
                LOCK_ID integer,
                FOREIGN KEY(TOKEN_ID) REFERENCES tokens(id),
                FOREIGN KEY(LOCK_ID) REFERENCES locks(id),
                GRANTED integer,
                DATE timestamp
            );
            """
}
