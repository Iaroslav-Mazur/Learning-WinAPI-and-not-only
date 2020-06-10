#include <iostream>
#include <Windows.h>

INT_PTR Dlgproc(
    HWND hWndDlg,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        return TRUE;
        break;
    case WM_COMMAND:
        switch (wParam)
        {
        case IDOK:
            EndDialog(hWndDlg, 0);
            return TRUE;
        }
        break;
    }
}

int Winmain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    DialogBoxA(hInstance, "Best Dialog Box", NULL, (DLGPROC)Dlgproc);
    
    return 0;
}