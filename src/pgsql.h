#ifndef PGSQL_H
#define PHSQL_H

#include<pqxx/pqxx>

class PgsqlDatabase {
public:
	PgsqlDatabase();
	~PgsqlDatabase() = default;

	bool isConnected();
	bool insertMeasurement(const int temp);

private:
	pqxx::connection conn;
	const std::string insertSql = "INSERT INTO mash_temps(time, temp) VALUES(now(), $1)";

};

#endif
