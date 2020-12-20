#pragma once

//#include "External/sqlite3/sqlite3.h"
#include "../../External/sqlite3/sqlite3.h"

class SQLiteDB
{
public:
	~SQLiteDB();

	void Connect(const char* path);

private:
	sqlite3* m_db = nullptr;
};