// zaiko.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "ZAIKO.h"
#include <windows.h>
#include "charTchar.h"
#include <gdiplus.h>
#include<CommCtrl.h>
#include "newExcelInput.h"
#include "SearchItemNUm.h"

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "comctl32.lib")
using namespace Gdiplus;

#define MAX_LOADSTRING 100
#define BUTTON1 100
#define BUTTON2 200
#define BUTTON3 300

#define ID_PROGRESS 100
#define ID_BUTTON1  101
#define ID_BUTTON2  102
#define ID_STATIC   103

//ウインドウを中央にする為にX座標(LEFT)とY座標(TOP)算出するマクロ
#define ScreenCenterX(Width) ((0)<((int)((GetSystemMetrics(SM_CXSCREEN)- Width) / 2)) ? ((int)((GetSystemMetrics(SM_CXSCREEN)- Width) / 2)) :(0))
#define ScreenCenterY(Height) ((0)<((int)((GetSystemMetrics(SM_CYSCREEN)- Height) / 2)) ? ((int)((GetSystemMetrics(SM_CYSCREEN)- Height) / 2)) :(0))

TCHAR fn[300];
TCHAR fnt[300];
TCHAR inipl[] = { _T("../Desktop") };
OPENFILENAME o;
charchange ci;
charchange cs;
excelRead ex;

char cfn[255] = { 0 };

//gdi
GdiplusStartupInput gdiSI;
ULONG_PTR           gdiToken;

char szClassName[] = "gdiplus01";    //ウィンドウクラス


void OnPaint(HDC hdc)
{
    Graphics MyGraphics(hdc);

    Pen Mypen1(Color(255, 0, 0, 255), 5), Mypen2(Color(100, 255, 0, 0), 40);
    MyGraphics.DrawLine(&Mypen1, 20, 20, 200, 20);
    MyGraphics.DrawLine(&Mypen2, 20, 50, 200, 50);
    MyGraphics.DrawLine(&Mypen1, 20, 80, 200, 80);
    return;
}

VOID Example_DrawImage3(HDC hdc)

{
    Graphics graphics(hdc);

    // Create an Image object.
    //Image image(L"C:/Users/Bee/Desktop/ship.png");
    //Image myPNG(L"C:/Users/Bee/Desktop/輸送船アイコン.png");

    // Create an array of PointF objects that specify the destination of the image.
    PointF destPoints[3] = {
    PointF(30.0f, 30.0f),
    PointF(250.0f, 50.0f),
    PointF(175.0f, 120.0f) };

    PointF* pdestPoints = destPoints;

    // Draw the image.
    //graphics.DrawImage(&myPNG, 30, 30);
}

//エディットボックスのハンドル
HWND hEdit;
HWND subhEdit;
HWND subtwo_hEdit;
HWND subthree_hEdit;
HWND subfour_hEdit;

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// ファイル名を指定するコモンダイアログを表示
BOOL GetFileName(HWND hWnd, TCHAR* fname, int sz, TCHAR* initDir = 0);
HWND Button1, hButton2, hButton3;

// ファイル名を指定するコモンダイアログを表示

BOOL GetFileName(HWND hWnd, TCHAR* fname, int sz, TCHAR* initDir) {
    fname[0] = _T('\0');
    ZeroMemory(&o, sizeof(o));
    o.lStructSize = sizeof(o);              //      構造体サイズ
    o.hwndOwner = hWnd;                             //      親ウィンドウのハンドル
    //o.lpstrInitialDir = initDir;    //      初期フォルダー
    o.lpstrFile = fname;                    //      取得したファイル名を保存するバッファ
    o.nMaxFile = sz;                                //      取得したファイル名を保存するバッファサイズ
    o.lpstrFilter = _TEXT("xlsxファイル(*.xlsx)\0*.xlsx\0") _TEXT("全てのファイル(*.*)\0*.*\0");
    o.lpstrDefExt = _TEXT("xlsx");
    o.lpstrTitle = _TEXT("テキストファイルを指定");
    o.nFilterIndex = 1;
    return GetOpenFileName(&o);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。
    GdiplusStartup(&gdiToken, &gdiSI, NULL);


    /* GDI+初期化 */
    /*
    HCURSOR hCursor = LoadCursor(NULL, IDC_ARROW);
    HBRUSH hBrush = (HBRUSH)(COLOR_WINDOW + 1);
    WNDCLASS wcl = { 0, WindowProc, 0, 0, hInst, NULL, hCursor, hBrush, NULL, "mh" };
    DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    if (!RegisterClass(&wcl) ||
        !CreateWindowEx(0, "mh", "gdiplus test", style, 50, 50, 270, 520, NULL, NULL, hInst, NULL))
        return FALSE;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    */

    /* GDI+終了 */
    //GdiplusShutdown(lpToken);

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ZAIKO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ZAIKO));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;


    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_ZAIKO));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ZAIKO);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));



    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    Items* deco = (Items*)malloc(sizeof(Items));
    deco = nullptr;
    int i = 0; int j = 0; int totalcount = 0;
    TCHAR buf[300];
    TCHAR bufb[300];
    char errorSheet[3000] = "シートがない品番：0";

    int newsize = 0;
    wchar_t* wcstring = nullptr;
    size_t convertedChars = 0;
    searchItemNum chan = searchItemNum(nullptr, nullptr);

    HFONT hFnt;     //フォントのハンドル

    //テキスト受け取り
    LPTSTR strTex = NULL;
    LPTSTR strSub = NULL;
    LPTSTR strSub_two = NULL;
    LPTSTR strSub_three = NULL;
    LPTSTR strSub_four = NULL;

    Items* err = nullptr;

    switch (message)
    {
    case WM_CREATE:
    {
        Button1 = CreateWindow(TEXT("BUTTON"),
            TEXT("パッキングリスト 選択"), WS_CHILD | WS_VISIBLE,
            450, 10, 200, 23, hWnd, (HMENU)BUTTON1, hInst, NULL);
        hButton2 = CreateWindow(TEXT("BUTTON"),
            TEXT("在庫状況ファイル 選択"), WS_CHILD | WS_VISIBLE,
            450, 43, 200, 23, hWnd, (HMENU)BUTTON2, hInst, NULL);
        hButton3 = CreateWindow(TEXT("BUTTON"),
            TEXT("書き込み実行"), WS_CHILD | WS_VISIBLE,
            450, 86, 200, 23, hWnd, (HMENU)BUTTON3, hInst, NULL);

        //エディットボックスの定義
        hEdit = CreateWindow(
            _T("EDIT"),             //ウィンドウクラス名
            _T("メイン文字"),                   //キャプション
            WS_CHILD | WS_VISIBLE | WS_BORDER |
            WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | ES_MULTILINE,         //スタイル指定
            10, 10,                  //位置 ｘ、ｙ
            300, 100,                //幅、高さ
            hWnd,                   //親ウィンドウ
            (HMENU)1,               // メニューハンドルまたは子ウィンドウID
            hInst,                  //インスタンスハンドル
            NULL);                  //その他の作成データ

        subhEdit = CreateWindow(
            _T("EDIT"),             //ウィンドウクラス名
            _T("セル上にしたい文字を上から入力"),                   //キャプション
            WS_CHILD | WS_VISIBLE | WS_BORDER |
            WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | ES_MULTILINE,         //スタイル指定
            10, 110,                  //位置 ｘ、ｙ
            300, 50,                //幅、高さ
            hWnd,                   //親ウィンドウ
            (HMENU)2,               // メニューハンドルまたは子ウィンドウID
            hInst,                  //インスタンスハンドル
            NULL);                  //その他の作成データ

        subtwo_hEdit = CreateWindow(
            _T("EDIT"),             //ウィンドウクラス名
            NULL,                   //キャプション
            WS_CHILD | WS_VISIBLE | WS_BORDER |
            WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | ES_MULTILINE,         //スタイル指定
            10, 170,                  //位置 ｘ、ｙ
            300, 50,                //幅、高さ
            hWnd,                   //親ウィンドウ
            (HMENU)3,               // メニューハンドルまたは子ウィンドウID
            hInst,                  //インスタンスハンドル
            NULL);                  //その他の作成データ

        subthree_hEdit = CreateWindow(
            _T("EDIT"),             //ウィンドウクラス名
            NULL,                   //キャプション
            WS_CHILD | WS_VISIBLE | WS_BORDER |
            WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | ES_MULTILINE,         //スタイル指定
            10, 230,                  //位置 ｘ、ｙ
            300, 50,                //幅、高さ
            hWnd,                   //親ウィンドウ
            (HMENU)4,               // メニューハンドルまたは子ウィンドウID
            hInst,                  //インスタンスハンドル
            NULL);                  //その他の作成データ

        subfour_hEdit = CreateWindow(
            _T("EDIT"),             //ウィンドウクラス名
            NULL,                   //キャプション
            WS_CHILD | WS_VISIBLE | WS_BORDER |
            WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL |
            ES_LEFT | ES_MULTILINE,         //スタイル指定
            10, 290,                  //位置 ｘ、ｙ
            300, 50,                //幅、高さ
            hWnd,                   //親ウィンドウ
            (HMENU)5,               // メニューハンドルまたは子ウィンドウID
            hInst,                  //インスタンスハンドル
            NULL);                  //その他の作成データ

    //テキストエディットのフォント作成
        hFnt = CreateFont(14, 0, 0, 0,
            FW_NORMAL, FALSE, FALSE, 0,
            SHIFTJIS_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, FIXED_PITCH, _T("System"));
        //テキストエディットのフォント変更のメッセージを送信
        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFnt, MAKELPARAM(FALSE, 0));
        SendMessage(subhEdit, WM_SETFONT, (WPARAM)hFnt, MAKELPARAM(FALSE, 0));

        break;
    }

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 選択されたメニューの解析:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case BUTTON1:
            GetFileName(hWnd, fn, 300, inipl);
            
            cs.changechar(o.lpstrFile);
            cs.filename[cs.len] = '\0';
#ifdef UNICODE
            MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, cs.filename, cs.len, bufb, (sizeof bufb) / 2);//strlen(cc.filename)
            //もしくはmbstowcs(buf,str,(sizeof buf)/2);
#else
            strcpy(buf, cfn);
#endif
            MessageBox(hWnd, bufb, L"ファイル名", MB_OK);
            break;
        case BUTTON2:
            GetFileName(hWnd, fnt, 300, inipl);
            ci.changechar(o.lpstrFile);
            ci.filename[ci.len] = '\0';
#ifdef UNICODE
            MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, ci.filename, ci.len, buf, (sizeof buf) / 2);//strlen(cc.filename)
            //もしくはmbstowcs(buf,str,(sizeof buf)/2);
#else
            strcpy(buf, cfn);
#endif
            MessageBox(hWnd, buf, L"ファイル名", MB_OK);
            break;
        case BUTTON3:
            //テキスト取得
            strTex = (LPTSTR)calloc((GetWindowTextLength(hEdit) + 1), sizeof(TCHAR)); // 文字数 + 終端 null 文字分のメモリを確保
            strSub = (LPTSTR)calloc((GetWindowTextLength(subhEdit) + 1), sizeof(TCHAR));
            strSub_two = (LPTSTR)calloc((GetWindowTextLength(subtwo_hEdit) + 1), sizeof(TCHAR));
            strSub_three = (LPTSTR)calloc((GetWindowTextLength(subthree_hEdit) + 1), sizeof(TCHAR));
            strSub_four = (LPTSTR)calloc((GetWindowTextLength(subfour_hEdit) + 1), sizeof(TCHAR));

            if (strTex) // strText が NULL でなければ次の処理をする
            {
                GetWindowText(hEdit, strTex, (GetWindowTextLength(hEdit) + 1)); // エディットのテキストを取得。長さには終端 null 文字分を含めます。
               // MessageBox(hEdit, strTex, TEXT(""), MB_OK);
            }
            if (strSub) {
                GetWindowText(subhEdit, strSub, (GetWindowTextLength(subhEdit) + 1));//sub文字列取得
                //MessageBox(subhEdit, strSub, TEXT(""), MB_OK);
            }
            if (strSub_two) {
                GetWindowText(subtwo_hEdit, strSub_two, (GetWindowTextLength(subtwo_hEdit) + 1));//sub文字列取得
               // MessageBox(subtwo_hEdit, strSub_two, TEXT(""), MB_OK);
            }
            if (strSub_three) {
                GetWindowText(subthree_hEdit, strSub_three, (GetWindowTextLength(subthree_hEdit) + 1));//sub文字列取得
                //MessageBox(subthree_hEdit, strSub_three, TEXT(""), MB_OK);
            }
            if (strSub_four) {
                GetWindowText(subfour_hEdit, strSub_four, (GetWindowTextLength(subfour_hEdit) + 1));//sub文字列取得
                //MessageBox(subfour_hEdit, strSub_four, TEXT(""), MB_OK);
            }


            deco = ex.datawrite(cs.filename, cs.len, ci.filename, ci.len, strTex, strSub, strSub_two, strSub_three, strSub_four);

            err = deco;
            if (err) {//シート無品番連結
                while (err) {
                    i = 0; j = 0;
                    char* itm = chan.CharChenge(err->itn);
                    char* colo = chan.CharChenge(err->col);
                    while (itm[i] != '\0') {
                        errorSheet[totalcount + i] = itm[i];
                        i++;
                    }
                    totalcount += i;
                    errorSheet[totalcount] = ' '; totalcount++;

                    while (colo[j] != '\0') {
                        errorSheet[totalcount + j] = colo[j];
                        j++;
                    }
                    totalcount += j;
                    errorSheet[totalcount] = '\n'; totalcount++;
                    deco = err->next;
                    free(err->col);
                    free(err->itn);
                    free(err);
                    err = deco;
                }
                errorSheet[totalcount] = '\0';
                totalcount++;//終了文字分プラス
            }
            else {
                totalcount = 20;
            }


#ifdef UNICODE

            MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, errorSheet, totalcount, buf, (sizeof buf) / 2);//strlen(cc.filename)
           //もしくはmbstowcs(buf,str,(sizeof buf)/2);
#else
            strcpy(buf, d);
#endif
            MessageBox(hWnd, buf, L"入力できなかった品番・色", MB_OK);

            break;
        }
        return 0;
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        //OnPaint(hdc);
        Example_DrawImage3(hdc);
        // TODO: HDC を使用する描画コードをここに追加してください...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
    
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
