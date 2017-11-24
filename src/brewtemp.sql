-- Create script for PostgresSQL

DROP TABLE mash_temps;
CREATE TABLE mash_temps (
	id		serial PRIMARY KEY,
	time	timestamp,
	temp	numeric
);


