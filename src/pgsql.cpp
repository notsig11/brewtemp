#include"pgsql.h"
#include<iostream>

PgsqlDatabase::PgsqlDatabase()
				: conn("host=localhost dbname=brewtherm user=brewtherm password=mashtemps")
{
	conn.prepare("add_measurement", insertSql);
}

bool PgsqlDatabase::isConnected() {
	return conn.is_open();
}

bool PgsqlDatabase::insertMeasurement(const int temp) {
	if(!conn.is_open()) {
		return false;
	}

	pqxx::work txn(conn);
	try {
		txn.prepared("add_measurement")(temp / 1000.0).exec();
		txn.commit();
	}
	catch(const std::exception &e)
	{
		std::cout << e.what() << "\n";
		txn.abort();
		return false;
	}
	return true;
}
