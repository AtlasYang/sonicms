CREATE KEYSPACE IF NOT EXISTS cms_data WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 3};

USE cms_data;

CREATE TABLE IF NOT EXISTS collections (
    collection_id UUID PRIMARY KEY,
    collection_name TEXT,
    collection_schema TEXT, -- JSON schema
    enable_auto_embedding BOOLEAN,
    embedding_keys SET<TEXT>,
    created_at TIMESTAMP,
    updated_at TIMESTAMP
);
CREATE INDEX IF NOT EXISTS ON collections (collection_name);

CREATE TABLE IF NOT EXISTS collection_entries (
    collection_id UUID,
    entry_id UUID,
    data TEXT, -- JSON data
    created_at TIMESTAMP,
    updated_at TIMESTAMP,
    PRIMARY KEY (collection_id, entry_id)
);