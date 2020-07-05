#include "charTchar.h"

charchange::charchange()
{
    filename = nullptr;
    len = 0;
}

void charchange::changechar(TCHAR* tc)
{
    //TCHAR	pszTchar[] = { _T("abcdあいう") };
    //char* pszChar;
#ifdef UNICODE
    {
        //charに必要な文字数の取得
        len = ::WideCharToMultiByte(CP_THREAD_ACP, 0, tc, -1, NULL, 0, NULL, NULL);
        filename = new char[len];
        if (filename)
        {
            //変換
            len = ::WideCharToMultiByte(CP_THREAD_ACP, 0, tc, (int)::wcslen(tc) + 1, filename, len, NULL, NULL);
            if (len == 0)
            {
                filename = NULL;
            }
        }
    }
#else
    {
        size_t	nLen;

        nLen = ::_tcslen(tc) + 1;
        filename = new char[nLen];
        if (filename)
            ::strcpy_s(filename, nLen * sizeof(char), tc);

        len = nLen;
    }
#endif
    //
    //ここでchar*型のpszCharを任意の処理で利用
    //	//処理が終わったらdelete
}