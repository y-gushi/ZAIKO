#include "newExcelInput.h"
#include "CheckStyle.h"

//#include <crtdbg.h>//メモリリーク用

#define BUFSIZE  256

excelRead::excelRead()
{
}

excelRead::~excelRead()
{

}

char* SJIStoUTF8(char* szSJIS, char* bufUTF8, int size) {
    wchar_t bufUnicode[BUFSIZE];
    int lenUnicode = MultiByteToWideChar(CP_ACP, 0, szSJIS, strlen(szSJIS) + 1, bufUnicode, BUFSIZE);
    WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenUnicode, bufUTF8, size, NULL, NULL);
    return bufUTF8;
}

std::wstring multi_to_wide_capi(std::string const& src)
{
    std::size_t converted{};
    std::vector<wchar_t> dest(src.size(), L'\0');
    if (::_mbstowcs_s_l(&converted, dest.data(), dest.size(), src.data(), _TRUNCATE, ::_create_locale(LC_ALL, "jpn")) != 0) {
        throw std::system_error{ errno, std::system_category() };
    }
    dest.resize(std::char_traits<wchar_t>::length(dest.data()));
    dest.shrink_to_fit();
    return std::wstring(dest.begin(), dest.end());
}

std::string wide_to_utf8_cppapi(std::wstring const& src)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(src);
}

std::string multi_to_utf8_cppapi(std::string const& src)
{
    auto const wide = multi_to_wide_capi(src);
    return wide_to_utf8_cppapi(wide);
}

styles* excelRead::searchshipplace(char* pl) {
    char search[5] = { 0 };
    int len = 0;
    char ms[]="magaseek";//8
    char be[] = "bee";//3
    char zo[] = "zozo";//4
    char sb[] = "smarby";//6
    char sl[] = "shoplist";//8

    char mss[9] = { 0 };
    char bes[4] = { 0 };
    char zos[5] = { 0 };
    char sbs[7] = { 0 };
    char sls[9] = { 0 };
    //bee 1012
    //magaseek 710
    //smarby 1038
    //zozo 711
    //shoplist 715

    char bS[] = "1012";
    char msS[] = "710";
    char smS[] = "1038";
    char zoS[] = "711";
    char slS[] = "715";

    char zocel[] = "309";
    char slicel[] = "422";
    char magcel[] = "424";
    char smacel[] = "310";
    char becel[] = "1074";

    int result = 0;

    while (pl[len] != '\0') {
        for (int i = 0; i < 8 - 1; i++) {
            mss[i] = mss[i + 1];
            if (i < 6 - 1)
                sbs[i] = sbs[i + 1];
            if (i < 4 - 1)
                zos[i] = zos[i + 1];
            if (i < 3 - 1)
                bes[i] = bes[i + 1];
        }
        mss[7] = sbs[5] = zos[3] = bes[2] = pl[len];
        mss[8] = sbs[6] = zos[4] = bes[3] = '\0';

        result = strncmp(mss, ms, 8);
        if (result == 0) {
            char* sty = (char*)malloc(4);
            strcpy_s(sty,4, msS);
            char* csty = (char*)malloc(4);
            strcpy_s(csty, 4, magcel);
            styles* St = (styles*)malloc(sizeof(styles));
            St->daystyle = sty;
            St->celstyle = csty;
            return St;
        }

        result = strncmp(mss, sl, 8);
        if (result == 0) {
            char* sty = (char*)malloc(4);
            strcpy_s(sty,4, slS);
            char* csty = (char*)malloc(4);
            strcpy_s(csty, 4, slicel);
            styles* St = (styles*)malloc(sizeof(styles));
            St->daystyle = sty;
            St->celstyle = csty;
            return St;
        }

        result = strncmp(sbs, sb, 6);
        if (result == 0) {
            char* sty = (char*)malloc(5);
            strcpy_s(sty,5, smS);
            char* csty = (char*)malloc(4);
            strcpy_s(csty, 4, smacel);
            styles* St = (styles*)malloc(sizeof(styles));
            St->daystyle = sty;
            St->celstyle = csty;
            return St;
        }

        result = strncmp(zos, zo, 4);
        if (result == 0) {
            char* sty = (char*)malloc(4);
            strcpy_s(sty, 4,zoS);
            char* csty = (char*)malloc(4);
            strcpy_s(csty, 4, zocel);
            styles* St = (styles*)malloc(sizeof(styles));
            St->daystyle = sty;
            St->celstyle = csty;
            return St;
        }

        result = strncmp(bes, be, 3);
        if (result == 0) {
            char* sty = (char*)malloc(5);
            strcpy_s(sty, 5,bS);
            char* csty = (char*)malloc(5);
            strcpy_s(csty, 5, becel);
            styles* St = (styles*)malloc(sizeof(styles));
            St->daystyle = sty;
            St->celstyle = csty;
            return St;
        }
        len++;
    }

    char* sty = (char*)malloc(5);
    strcpy_s(sty, 5, bS);
    char* csty = (char*)malloc(5);
    strcpy_s(csty, 5, becel);
    styles* St = (styles*)malloc(sizeof(styles));
    St->daystyle = sty;
    St->celstyle = csty;
    return St;
}

Items* excelRead::datawrite(char* plfn, int pllen, char* orderfn, int orderlen, wchar_t* intxto, wchar_t* intxts, wchar_t* intxttwo, wchar_t* intxtthree, wchar_t* intxtfour)
{
    // メモリリーク検出
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    char* mfile = writename(orderfn, orderlen); //テスト書き出し
    
    styles* setstyle = nullptr;//スタイルの設定

    char *inMainstr;

    //ワイド文字列(WCHAR*)をマルチバイト文字列(char*)に変換
    char wStrC[200] = { 0 };
    size_t wLen = 0;
    errno_t err = 0;

    char* inSubstr = nullptr;
    char* inTwostr = nullptr;
    char* inThreestr = nullptr;
    char* inFourstr = nullptr;
    UINT8 strend = '\0';

    //ロケール指定
    setlocale(LC_ALL, "japanese");

    //変換
    err = wcstombs_s(&wLen, wStrC, 100, intxto, _TRUNCATE);
    
    inMainstr = (char*)malloc(200);
    
    inMainstr=SJIStoUTF8(wStrC, inMainstr, 100);//shift-jis utf8変換
    searchItemNum* onetstr = new searchItemNum(nullptr, nullptr);

    if (lstrlenW(intxts) > 0) {
        //変換
        err = wcstombs_s(&wLen, wStrC, 100, intxts, _TRUNCATE);
        inSubstr = (char*)malloc(200);//サブ文字 1上　char変換
        inSubstr=SJIStoUTF8(wStrC, inSubstr, 100);
    }
    else {
        UINT8* cstr= onetstr->strtalloc(&strend);
        inSubstr = (char*)cstr;
    }
    if (lstrlenW(intxttwo) > 0) {
        //変換
        err = wcstombs_s(&wLen, wStrC, 50, intxttwo, _TRUNCATE);
        inTwostr = (char*)malloc(100);//サブ文字 1上　char変換
        inTwostr = SJIStoUTF8(wStrC, inTwostr, 50);
    }
    else {
        inTwostr = (char*)onetstr->strtalloc(&strend);
    }
    if (lstrlenW(intxtthree) > 0) {
        //変換
        err = wcstombs_s(&wLen, wStrC, 50, intxtthree, _TRUNCATE);
        inThreestr = (char*)malloc(100);//サブ文字 1上　char変換
        inThreestr = SJIStoUTF8(wStrC, inThreestr, 50);
    }
    else {
        inThreestr = (char*)onetstr->strtalloc(&strend);
    }
    if (lstrlenW(intxtfour) > 0) {
        //変換
        err = wcstombs_s(&wLen, wStrC, 50, intxtfour, _TRUNCATE);
        inFourstr = (char*)malloc(100);//サブ文字 1上　char変換
        inFourstr = SJIStoUTF8(wStrC, inFourstr, 50);
    }
    else {
        inFourstr = (char*)onetstr->strtalloc(&strend);
    }
    delete onetstr;

    //文字連結検索用
    char sumstr[255] = { 0 };
    int sumstlen = 0;
    int substlen = 0;
    if (lstrlenW(intxto) > 0) {
        while (inMainstr[substlen] != '\0') {
            sumstr[sumstlen] = inMainstr[substlen];
            substlen++; sumstlen++;
        }
    }
    substlen = 0;
    if (lstrlenW(intxts) > 0) {
        while (inSubstr[substlen] != '\0') {
            sumstr[sumstlen] = inSubstr[substlen];
            substlen++; sumstlen++;
        }
    }
    substlen = 0;
    if (lstrlenW(intxttwo) > 0) {
        while (inTwostr[substlen] != '\0') {
                sumstr[sumstlen] = inTwostr[substlen];
                substlen++; sumstlen++;
        }
    }
    substlen = 0;
    if (lstrlenW(intxtthree) > 0) {
        while (inThreestr[substlen] != '\0') {
            sumstr[sumstlen] = inThreestr[substlen];
            substlen++; sumstlen++;
        }
    }
    substlen = 0;
    if (lstrlenW(intxtfour) > 0) {
        while (inFourstr[substlen] != '\0') {
            sumstr[sumstlen] = inFourstr[substlen];
            substlen++; sumstlen++;
        }
        substlen = 0;
    }
    //setlocale(LC_ALL, "");//ロケール既定値に戻す

    //ショップ検索
    setstyle = searchshipplace(sumstr);
    //char* styleset = searchshipplace(sumstr);
    /*-----------------------------
    入力文字チェック
    -------------------------------*/

    /*-----------------------
     シェアー文字列読み込み
     -----------------------*/
        
    char shares[] = "sharedStrings.xml";//ファイル位置　ファイル名部分一致検索　sharedStrings.xml


    std::ifstream PLR(plfn, std::ios::in | std::ios::binary);
    if (!PLR) {
        const char ef[] = "file open error";
        size_t fnlen = 0;
        while (plfn[fnlen] != '\0')
            fnlen++;
        fnlen++;
        Items* erroritem = (Items*)malloc(sizeof(Items));
        erroritem->itn = (UINT8*)malloc(16);
        erroritem->col = (UINT8*)malloc(fnlen);
        //strcpy_s((char*)erroritem->itn,16,ef);
        //strcpy_s((char*)erroritem->col, fnlen, (const char*)plfn);
        memcpy(erroritem->itn, ef, 16);
        memcpy(erroritem->col, plfn, fnlen);
        erroritem->next = nullptr;

        return erroritem;
    }

    HeaderRead* hr = new HeaderRead(plfn);
    CenterDerect* cddata = nullptr;//セントラルディレクトのデータ
    hr->endread(&PLR);//終端コードの読み込み
    hr->freeER();

    DeflateDecode* decShare = new DeflateDecode(&PLR);//sharestring ファイルの保存用
    //share セントラル取得
    while (hr->filenum < hr->ER->centralsum) {//cddata mallocなし
        cddata = hr->centeroneread(hr->readpos, hr->ER->size, hr->ER->centralsum, shares, &PLR);
        if (cddata) {
            break;
        }
        hr->freeheader();
    }
    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr->localread(cddata->localheader,&PLR);//sharesstringsの読み込み
        decShare->dataread(hr->LH->pos, cddata->nonsize);
    }

    shareRandD* sharray=new shareRandD(decShare->ReadV, decShare->readlen);//share string read to array

    sharray->getSicount();//get si count

    sharray->ReadShare();//文字列読み込み デコードデータ削除OK

    delete decShare;

     /*-----------------------
    パッキングリストシート読み込み
    -----------------------*/

    char sheetname[] = "worksheets/sheet1.xml";//ファイル位置　ファイル名部分一致検索
    DeflateDecode* decsheet = new DeflateDecode(&PLR);

    hr->filenum = 0;//レコード数初期化
    hr->readpos = hr->ER->position;
    cddata = nullptr;
    while (hr->filenum < hr->ER->centralsum) {
        cddata = hr->centeroneread(hr->readpos, hr->ER->size, hr->ER->centralsum, sheetname, &PLR);//セントラルディレクトのデータ
        if (cddata) {
            break;
        }
        hr->freeheader();
    }
    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr->localread(cddata->localheader, &PLR);//sharesstringsの読み込み
        decsheet->dataread(hr->LH->pos, cddata->nonsize);
    }

    Ctags* ms=new Ctags(decsheet->ReadV, decsheet->readlen, sharray);

    ms->sheetread();

    delete decsheet;//デコードデータ削除

    shipinfo* sg=new shipinfo(ms->rows);//シートデータ参照　freeなし

    sg->GetItems();//mallocなし　シートとセット

    PLR.close();

    //--------------------//
    //ファイル書き出し用
    //--------------------//

    _Post_ _Notnull_ FILE* cdf;
    fopen_s(&cdf, onetimefn, "wb");
    if (!cdf) {
        const char ef[] = "file open error";
        size_t fnlen = 0;
        while (onetimefn[fnlen] != '\0')
            fnlen++;
        fnlen++;
        Items* erroritem = (Items*)malloc(sizeof(Items));
        erroritem->itn = (UINT8*)malloc(16);
        erroritem->col = (UINT8*)malloc(fnlen);
        memcpy(erroritem->itn, ef, 16);
        memcpy(erroritem->col, onetimefn, fnlen);
        erroritem->next = nullptr;

        return erroritem;
    }

    _Post_ _Notnull_ FILE* fpr;
    fopen_s(&fpr, mfile, "wb");
    if (!fpr) {
        const char ef[] = "file open error";
        size_t fnlen = 0;
        while (mfile[fnlen] != '\0')
            fnlen++;
        fnlen++;
        Items* erroritem = (Items*)malloc(sizeof(Items));
        erroritem->itn = (UINT8*)malloc(16);
        erroritem->col = (UINT8*)malloc(fnlen);
        memcpy(erroritem->itn, ef, 16);
        memcpy(erroritem->col, plfn, fnlen);
        erroritem->next = nullptr;

        return erroritem;
    }

    LHmake dd;
    dd.getday();//時間の取得
    zipwrite zip(dd.times);//zipデータを作る
    HeaderWrite hw;//ローカルヘッダの書き込み
    UINT32 localSize = 0;
    UINT32 CDp = 0;

    /*-----------------------

   スタイルシート読み込み

   -----------------------*/
    
    char stylefn[] = "xl/styles.xml";

    std::ifstream Zr(orderfn, std::ios::in | std::ios::binary);

    if (!Zr) {
        const char ef[] = "file open error";
        size_t fnlen = 0;
        while (orderfn[fnlen] != '\0')
            fnlen++;
        fnlen++;
        Items* erroritem = (Items*)malloc(sizeof(Items));
        erroritem->itn = (UINT8*)malloc(16);
        erroritem->col = (UINT8*)malloc(fnlen);
        memcpy(erroritem->itn, ef, 16);
        memcpy(erroritem->col, plfn, fnlen);
        erroritem->next = nullptr;

        return erroritem;
    }

    HeaderRead* hr2 = new HeaderRead(orderfn);
    hr2->endread(&Zr);//終端コードの読み込み
    
    
    DeflateDecode* Sdeco = new DeflateDecode(&Zr);

    while (hr2->filenum < hr2->ER->centralsum) {
        cddata = hr2->centeroneread(hr2->readpos, hr2->ER->size, hr2->ER->centralnum, stylefn, &Zr);
        if (cddata)
            break;
        hr2->freeheader();
    }
    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr2->localread(cddata->localheader, &Zr);//sharesstringsの読み込み
        Sdeco->dataread(hr2->LH->pos, cddata->nonsize);
    }
    else {
        const char ef[] = "syle search error";
        size_t fnlen = 0;
        while (mfile[fnlen] != '\0')
            fnlen++;
        fnlen++;
        Items* erroritem = (Items*)malloc(sizeof(Items));
        erroritem->itn = (UINT8*)malloc(19);
        erroritem->col = (UINT8*)malloc(fnlen);
        memcpy(erroritem->itn, ef, 19);
        memcpy(erroritem->col, plfn, fnlen);
        erroritem->next = nullptr;

        return erroritem;
    }
    
    checkstyle* sr = new checkstyle();

    sr->readstyle(Sdeco->ReadV, Sdeco->readlen);

    UINT32 styleleng = Sdeco->readlen;//style 解凍データ長

    UINT8 teststyle[] = "bee";
    sr->configstyle(teststyle);//ショップ別スタイルセット

    char* styleset = nullptr;
    UINT32 memsiz = UINT32(sr->stylelen) + 1;
    styleset = (char*)malloc(memsiz);
    strcpy_s(styleset, memsiz, (const char*)sr->style);//style copy

    sr->styledatawrite(styleleng);

    delete Sdeco;
    

    //--------------style 圧縮--------------//
    
    encoding* styen = new encoding;//sharestring 圧縮
    styen->compress(sr->wd, sr->wdlen);
    crc CRCstyle;
    CRCstyle.rescrc();
    CRCstyle.mcrc(sr->wd, sr->wdlen);//crc 計算

    if (cddata) {
        localSize = hw.localwrite(fpr, dd.times, CRCstyle.crc32, styen->datalen, sr->wdlen, cddata->filenameleng, cddata->filename, cddata->version, cddata->bitflag, cddata->method);
        CDp = (styen->datalen) + localSize;//ローカルデータサイズ

        //セントラル情報の書き換え//
        cddata->crc = CRCstyle.crc32;
        cddata->localheader = zip.writeposition;
        cddata->day = (dd.times) & 0xFFFF;
        cddata->time = (dd.times >> 16) & 0xFFFF;
        cddata->size = styen->datalen;
        cddata->nonsize = (UINT32)sr->wdlen;//内容変更したら　更新必要

        hw.centralwrite(cdf, *cddata);

        //hr2->freeheader();
    }

    zip.writeposition += CDp;//データ書き込み位置更新

    styen->write(fpr);//圧縮データの書き込み

    delete styen;//share圧縮データ削除
    
    delete sr;//style 削除
    

    /*-----------------------
    shareシート読み込み
    -----------------------*/
    //char styleset[] = "1452";
    decShare = new DeflateDecode(&Zr);

    hr2->readpos = hr2->ER->position;

    while (hr2->filenum < hr2->ER->centralsum) {
        cddata = hr2->centeroneread(hr2->readpos, hr2->ER->size, hr2->ER->centralnum, shares, &Zr);
        if (cddata)
            break;
        hr2->freeheader();
    }
    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr2->localread(cddata->localheader, &Zr);//sharesstringsの読み込み
        decShare->dataread(hr2->LH->pos, cddata->nonsize);
        hr2->freeLH();
    }

    for (int i = 0; i < sharray->mycount; i++) {//シェアー文字列　メモリ解放
        if (sharray->sis[i])
            sharray->Sitablefree(sharray->sis[i]);
    }
        
    delete sharray;
    delete hr;
    
    sharray = new shareRandD(decShare->ReadV, decShare->readlen);//share 再初期化 検索用に呼び出し
    sharray->getSicount();//get si count
    sharray->ReadShare();//文字列読み込み

    //delete decShare;
    //share 検索　あったら番号取得

    int ma = 0;
    //UINT8** instrFlag = (UINT8**)malloc(txtnum);

    sharray->searchSi(inSubstr, inTwostr,inThreestr,inFourstr);//マッチした文字列のSi番号取得　なければnullptr
        
    //シェアー書き込み
    UINT8* sharedata = nullptr;//share書き込み　データ
        
    sharedata= sharray->writeshare((UINT8*)inMainstr, strlen(inMainstr), inFourstr, inThreestr, inTwostr, inSubstr);//share文字列書き込み share data更新
    UINT64 shrelength = sharray->writeleng;
    free(inMainstr);
    free(inSubstr);
    free(inTwostr);
    free(inThreestr);
    free(inFourstr);

    /*--------------------------
    share data書き込み　圧縮
    ---------------------------*/

    encoding* shenc = new encoding;//sharestring 圧縮
    shenc->compress(sharedata, shrelength);
    free(sharedata);

    if (cddata) {
        localSize = hw.localwrite(fpr, dd.times, sharray->buckcrc, shenc->datalen, sharray->writeleng, cddata->filenameleng, cddata->filename, cddata->version, cddata->bitflag, cddata->method);
        CDp = (shenc->datalen) + localSize;//ローカルデータサイズ

        //セントラル情報の書き換え//
        cddata->crc = sharray->Crc.crc32;
        cddata->localheader = zip.writeposition;
        cddata->day = (dd.times) & 0xFFFF;
        cddata->time = (dd.times >> 16) & 0xFFFF;
        cddata->size = shenc->datalen;
        cddata->nonsize = sharray->writeleng;//内容変更したら　更新必要

        hw.centralwrite(cdf, *cddata);
        hr2->freeheader();
    }

    zip.writeposition += CDp;//データ書き込み位置更新

    shenc->write(fpr);//圧縮データの書き込み

    delete decShare;//シェアー解凍データ削除
    delete shenc;//share圧縮データ削除
  
     /*-----------------------
    発注到着シート読み込み
    -----------------------*/
 
    DeflateDecode* Hdeco;
    char sheet[] = "worksheets/sheet";
    const char sharefn[] = "xl/sharedStrings.xml";
    bool t = false;
    Ctags* mh;//発注到着　cell データ読み込み
    searchItemNum* sI=nullptr;//品番検索　＆　書き込み
    CDdataes* slideCDdata = hr2->saveCD;//ファイル名検索用

    hr2->readpos = hr2->ER->position;//読み込み位置初期化
    hr2->filenum = 0;//レコード数初期化
    int result = 0;

    //品番、カラーエラー用
    MatchColrs* matchs = nullptr;
    MatchColrs* matchsroot = nullptr;

    //テスト用
    //const char *tesstr[3] = { "22249","22249","22249" };

    while (hr2->filenum < hr2->ER->centralsum) {
        //ファイル名 sheet 部分一致検索
        cddata = hr2->centeroneread(hr2->readpos, hr2->ER->size, hr2->ER->centralnum, sheet, &Zr);
        if (cddata) {

            hr2->localread(cddata->localheader, &Zr);//"worksheets/sheet"に一致するファイルの中身検索

            Hdeco = new DeflateDecode(&Zr);//解凍
            Hdeco->dataread(hr2->LH->pos, cddata->nonsize);//解凍　データ読み込み

            mh = new Ctags(Hdeco->ReadV, Hdeco->readlen, sharray);//シートデータ読み込み
            mh->sheetread();

            sI = new searchItemNum(sg->its, mh);
            //t = sI->searchitemNumber(sharray->uniqstr, sharray->inputsinum[3], sharray->inputsinum[2], sharray->inputsinum[1], sharray->inputsinum[0], setstyle->daystyle,setstyle->celstyle);//品番検索　＆　セルデータ追加　シェアー消去（入れる場合は引数に）
            t = sI->searchitemNumber(sharray->uniqstr, sharray->inputsinum[3], sharray->inputsinum[2], sharray->inputsinum[1], sharray->inputsinum[0], (char*)styleset, setstyle->celstyle);

            if (t)
            {//一致品番あった場合
                if (sI->rootMat) {//一致アイテムの保存
                    while (sI->rootMat) {
                        matchs = sI->addmatches(matchs, sI->rootMat->itemnum, sI->rootMat->color);//si コピー mallocなし
                        sI->rootMat = sI->rootMat->next;
                    }
                }

                mh->writesheetdata();//シートデータ書き込み
                crc CRC;
                CRC.rescrc();
                CRC.mcrc(mh->wd, mh->p);//crc 計算

                encoding* enc = new encoding;//圧縮
                enc->compress(mh->wd, mh->p);//データ圧縮

                cddata->zokusei = 0x00;
                cddata->gaibuzokusei = 0x00;
                cddata->bitflag = 0x06;
                localSize = hw.localwrite(fpr, dd.times, CRC.crc32, enc->datalen & 0xFFFFFFFF, mh->p & 0xFFFFFFFF, cddata->filenameleng, cddata->filename, cddata->version, cddata->bitflag, cddata->method);//ローカルヘッダー書き込み
                CDp = (enc->datalen) + localSize;//ローカルデータサイズ
                cddata->crc = CRC.crc32;//セントラル情報の書き換え
                cddata->localheader = zip.writeposition;
                cddata->time = (dd.times) & 0xFFFF;
                cddata->day = (dd.times >> 16) & 0xFFFF;
                cddata->size = enc->datalen;
                cddata->nonsize = mh->p & 0xFFFFFFFF;//内容変更したら　更新必要

                zip.writeposition += CDp;//データ書き込み位置更新

                enc->write(fpr);//圧縮データの書き込み

                //cddata一旦書き込み
                hw.centralwrite(cdf, *cddata);

                delete enc;
            }
            else {
                //cddata一旦書き込み
                UINT32 LHposstock = zip.writeposition;//ローカルヘッダーの位置更新用
                zip.LoclheadAndDatacopy(cddata->localheader, fpr, orderfn);//ローカルヘッダー検索＆書き込み
                cddata->localheader = LHposstock;//ローカルヘッダー相対位置のみ変更
                hw.centralwrite(cdf, *cddata);
            }
            mh->Rowtablefree();
            mh->selectfree();
            mh->panefree();
            mh->colfree();
            hr2->freeLH();
            delete Hdeco;//デコードデータ　削除
            delete mh;
            delete sI;
        }
        else {
            result = strcmp(hr2->scd->filename, sharefn);
            int styresul= strcmp(hr2->scd->filename, stylefn);

            if (result != 0 && styresul != 0) {//styleseet 入れる場合-> && styresul != 0
                //cddata一旦書き込み
                UINT32 LHposstock = zip.writeposition;//ローカルヘッダーの位置更新用
                zip.LoclheadAndDatacopy(hr2->scd->localheader, fpr, orderfn);//ローカルヘッダー検索＆書き込み
                hr2->scd->localheader = LHposstock;//ローカルヘッダー相対位置のみ変更
                hw.centralwrite(cdf, *hr2->scd);
            }
        }
        hr2->freeheader();
    }
    //hr2->freetxt(inptxt);//入力文字列の削除

    //free(styleset);
    free(setstyle->daystyle);
    free(setstyle->celstyle);
    free(setstyle);

    Items* errorItem = nullptr;

    sI = new searchItemNum(nullptr, nullptr);

    matchsroot = matchs;
    unitC un;
    int matchor = 1;
    int matchcol = 1;

    //シートがない品番、カラーを比較
    if (matchs) {
        while (sg->its) {
            sg->its->col = un.slipNum(sg->its->col);
            while (matchs) {
                matchor = strcmp((const char*)sg->its->itn, (const char*)matchs->itemnum);
                if (matchor == 0) {//品番、いろ一致入力
                    matchcol = strcmp((const char*)sg->its->col, (const char*)matchs->color);
                    if (matchcol == 0) {
                        //std::cout << "保存　一致データあり" << std::endl;
                        break;
                    }
                }
                else {
                    matchcol = 1;
                }
                matchs = matchs->next;
            }
            if (matchor == 0 && matchcol == 0) {//品番、いろ一致入力
            }
            else {
                //エラー　シートなし                
                UINT8* iTeM = sI->strtalloc(sg->its->itn);
                UINT8* iCoL = sI->strtalloc(sg->its->col);
                Items* itms = (Items*)malloc(sizeof(Items));
                itms->itn = iTeM;
                itms->col = iCoL;

                errorItem = sI->addItems(errorItem, itms);
                //char* shiftj = sI->CharChenge(sg->its->col);
            }
            matchs = matchsroot;//初期化
            sg->its = sg->its->next;
        }
    }
    else {
        std::cout << "sheet no error" << std::endl;
    }
    sI->freerootmacht(matchsroot);

    for (int i = 0; i < sharray->mycount; i++) {//シェアー文字列　メモリ解放
        if (sharray->sis[i])
            sharray->Sitablefree(sharray->sis[i]);
    }

    delete sharray;
    delete sg;//アイテム　文字データ シートとセット削除
    delete ms;//シート　セル削除 PLシートデータ

    hw.eocdwrite(cdf, hr2->ER->discnum, hr2->ER->disccentral, hr2->ER->centralnum, hr2->ER->centralsum, zip.writeposition, hw.cdsize);

    if (cdf)
        fclose(cdf);

    std::ifstream fin(onetimefn, std::ios::in | std::ios::binary);
    if (!fin) {
        std::cout << "not file open" << std::endl;
    }
    char rd = 0;

    if (fpr) {
        while (!fin.eof()) {
            fin.read((char*)&rd, sizeof(char));
            if (!fin.eof())
                fwrite(&rd, sizeof(char), 1, fpr);
        }
    }

    
    delete hr2;

    if(fin)
        fin.close();

    remove(onetimefn);

    if (fpr)
        fclose(fpr);
        
    if(Zr)
        Zr.close();
    
    return errorItem;
}
//LP... ->char
char* excelRead::convchar(wchar_t* a) {

    size_t origsize = wcslen(a) + 1;
    size_t convertedChars = 0;

    const size_t newsize = origsize * 2;
    // The new string will contain a converted copy of the original
    // string plus the type of string appended to it.
    char* nstring = new char[newsize];

    // Put a copy of the converted string into nstring
    wcstombs_s(&convertedChars, nstring, newsize, a, _TRUNCATE);
    // append the type of string to the new string.
    //_mbscat_s((unsigned char*)nstring, newsize + strConcatsize, (unsigned char*)strConcat);

    return nstring;
}


Items* excelRead::addItems(Items* base, Items* itm)
{
    if (!base) {
        base = (Items*)malloc(sizeof(Items));
        if (base) {
            base = itm;
            base->next = nullptr;
        }
    }
    else {
        base->next = addItems(base->next, itm);
    }

    return base;
}

char* excelRead::writename(char* wfn, int len)
{
    char* fn = (char*)malloc(255);
    char c[] = "Centraldata";//11
    int i = 0;
    int pathlen = 0;
    int slashnum = 0;
    int centerlen = 0;

    if (fn) {
        while (wfn[i] != '.' && i < len) {
            fn[i] = wfn[i];
            if (slashnum < 2) {
                onetimefn[centerlen] = wfn[centerlen];
                centerlen++;
            }
            i++; slashnum++;
        }
        pathlen = i;
        while (wfn[pathlen] != '/' && pathlen > 0) {
            pathlen--;
        }

        for (int j = 0; j < 11; j++) {
            onetimefn[centerlen] = c[centerlen];
            centerlen++;
        }

        onetimefn[centerlen] = '\0';

        fn[i] = '2';// .記入
        i++;
        fn[i] = wfn[i - 1];// .記入
        i++;

        fn[i] = 'x'; i++;
        fn[i] = 'l'; i++;
        fn[i] = 's'; i++;
        fn[i] = 'x'; i++;
        fn[i] = '\0'; i++;
    }

    fpath = (char*)malloc(pathlen + 11 + 1);

    for (int ind = 0; ind < pathlen; ind++) {
        fpath[ind] = wfn[ind];
    }

    for (int fnt = 0; fnt < 11; fnt++) {
        fpath[pathlen + fnt] = c[fnt];
    }

    pathlen = pathlen + 11;
    fpath[pathlen] = '\0';

    return fn;
}
