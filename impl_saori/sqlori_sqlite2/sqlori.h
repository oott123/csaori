#pragma once

#include <string>

struct sqlite;

class Sqlori {
private:
	sqlite *_db;
	std::string _result;

public:
	Sqlori(): _db(NULL) {}

	const std::string &result() { return _result; }

	static int callBack(void *callee, int size, char **values, char **fields);

	void clear() { _result = ""; }

	int open(const std::string &path);
	int exec(const std::string &command);
	int close();
};
