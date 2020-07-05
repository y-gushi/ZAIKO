#pragma once
#include "deflate.h"
#include "Header.h"
#include "timeget.h"
#include "slidewindow.h"
#include "CRC.h"
#include "encode.h"
#include "decord.h"
#include "compressdata.h"
#include "SearchItemNUm.h"
#include "ShipDataRead.h"
#include "ZipFilewrite.h"
#include "shareRandW.h"
#include "RowColumn.h"

#include <codecvt>

#include <stdlib.h>
#include <string>

#include "atlbase.h"
#include "atlstr.h"
#include "comutil.h"

class excelRead {
public:
	excelRead();
	~excelRead();

	//スタイル数値
	const char magas[5] = "107";
	const char bees[5] = "769";
	const char zozos[5] = "431";
	const char smarbs[5] = "1127";
	const char shpls[5] = "1122";
	const char nomals[4] = "769";

	char* searchshipplace(char* pl);

	Items* datawrite(char* plfn, int pllen, char* orderfn, int orderlen, wchar_t* intxto, wchar_t* intxts, wchar_t* intxttwo, wchar_t* intxtthree, wchar_t* intxtfour);

	char* convchar(wchar_t* a);

	char* fpath = nullptr;

	char onetimefn[255] = { 0 };

	Items* addItems(Items* base, Items* itm);
	char* writename(char* wfn, int len);
};