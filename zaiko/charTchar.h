#pragma once

#include <Windows.h>
#include <tchar.h>

class charchange {
public:
	char* filename;
	int len;
	charchange();
	void changechar(TCHAR* tc);
};
