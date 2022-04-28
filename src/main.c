/*************************************************************************************************
*                                                                                                *
*                                           [ Til-mi ]                                           *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                     A tool for creating State of War's til and tmi files.                      *
*               (ɔ) 2017 - 2022 State of War Baidu PostBar, some rights reserved.                *
*                                                                                                *
*             Tilmi is a free software. You can freely do whatever you want with it              *
*     under the JUST DON'T BOTHER ME PUBLIC LICENSE (hereinafter referred to as the license)     *
*                  published by mhtvsSFrpHdE <https://github.com/mhtvsSFrpHdE>.                  *
*                                                                                                *
*               Tilmi is created, intended to be useful, but without any warranty.               *
*                      For more information, please forward to the license.                      *
*                                                                                                *
*                 You should have received a copy of the license along with the                  *
*                        source code of this program. If not, please see                         *
*               <https://github.com/State-of-War-PostBar/Tilmi/blob/main/LICENSE>.               *
*                                                                                                *
*      For more information about the project and us, please visit our Github repository at      *
*                        <https://github.com/State-of-War-PostBar/Tilmi>.                        *
*                                                                                                *
**************************************************************************************************/

#include <pch.h>

#include "bmp.h"
#include "lang.h"
#include "map.h"
#include "til.h"
#include "vector.h"

#include <cderr.h>
#include <commdlg.h>
#include <shlwapi.h>
#include <strsafe.h>

static HINSTANCE program_instance;

static TilmiLanguage chosen_language;

static int res_width, res_height;

static HANDLE icon_sow;
static HANDLE cursor_arrow;

static HFONT ui_font[TILMI_LANG_ENUM_END];

LRESULT
CALLBACK
LanguageWindowProc( HWND, UINT, WPARAM, LPARAM );

LRESULT
CALLBACK
MainWindowProc( HWND, UINT, WPARAM, LPARAM );

int
WINAPI
wWinMain( HINSTANCE instance, HINSTANCE prev_instance, PWSTR cmd_line, int cmd_show )
{
    program_instance = instance;

    res_width = GetSystemMetrics(SM_CXSCREEN);
    res_height = GetSystemMetrics(SM_CYSCREEN);

    icon_sow = LoadImage(program_instance, MAKEINTRESOURCE(1), IMAGE_ICON, 0, 0, 0);
    cursor_arrow = LoadCursor(NULL, IDC_ARROW);

    ui_font[TILMI_EN_US] = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 0, 0, 0, 0, TEXT("Segoe UI"));
    ui_font[TILMI_ZH_SP] = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, GB2312_CHARSET, 0, 0, 0, 0, TEXT("Microsoft Yahei"));

    WNDCLASS language_window_class =
    {
        .hbrBackground = (HBRUSH) COLOR_WINDOW,
        .hCursor = cursor_arrow,
        .hIcon = (HICON) icon_sow,

        .lpfnWndProc = LanguageWindowProc,
        .lpszClassName = TEXT("LanguageWindow")
    };
    RegisterClass(&language_window_class);

    CreateWindow(TEXT("LanguageWindow"),
                 TEXT("Tilmi"),
                 WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                 (res_width - 300) / 2, (res_height - 175) / 2,
                 300, 175,
                 NULL,
                 NULL,
                 program_instance,
                 NULL);

    MSG window_message = { 0 };
    while (GetMessage(&window_message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&window_message);
        DispatchMessage(&window_message);
    }

    // We usually don't need to UnregisterClass...
    DeleteObject(ui_font[TILMI_ZH_SP]);
    DeleteObject(ui_font[TILMI_EN_US]);
    DeleteObject(cursor_arrow);
    DeleteObject(icon_sow);

    return 0;
}

#define Commandables enum

LRESULT
CALLBACK
LanguageWindowProc( HWND handle, UINT message, WPARAM wp, LPARAM lp )
{
    Commandables
    {
        BTN_EN_US = 1,
        BTN_ZH_SP
    };

    static HWND button_en, button_zh;

    switch (message)
    {
        case WM_CREATE:

            // Create window controls
            button_en = CreateWindow(TEXT("BUTTON"),
                                     TEXT("English"),
                                     WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                     45, 10,
                                     200, 50,
                                     handle,
                                     (HMENU) BTN_EN_US,
                                     (HINSTANCE) GetWindowLongPtr(handle, GWLP_HINSTANCE),
                                     NULL);
            button_zh = CreateWindow(TEXT("BUTTON"),
                                     TEXT("简体中文"),
                                     WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                                     45, 75,
                                     200, 50,
                                     handle,
                                     (HMENU) BTN_ZH_SP,
                                     (HINSTANCE) GetWindowLongPtr(handle, GWLP_HINSTANCE),
                                     NULL);

            // Set controls' font
            SendMessage(button_en, WM_SETFONT, (WPARAM) ui_font[TILMI_EN_US], TRUE);
            SendMessage(button_zh, WM_SETFONT, (WPARAM) ui_font[TILMI_ZH_SP], TRUE);
            return 0;

        case WM_COMMAND:
            chosen_language = LOWORD(wp);
            LoadTilmiStrings(program_instance, chosen_language);

            WNDCLASS main_window_class =
            {
                .hbrBackground = (HBRUSH) COLOR_WINDOW,
                .hCursor = cursor_arrow,
                .hIcon = (HICON) icon_sow,

                .lpfnWndProc = MainWindowProc,
                .lpszClassName = TEXT("MainWindow")
            };
            RegisterClass(&main_window_class);

            CreateWindow(TEXT("MainWindow"),
                         TEXT("Tilmi"),
                         WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                         (res_width - 500) / 2, (res_height - 760) / 2,
                         500, 760,
                         NULL,
                         NULL,
                         program_instance,
                         NULL);
            DestroyWindow(handle);
            return 0;

        case WM_DESTROY:
            if (!chosen_language)
                PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(handle, message, wp, lp);
}

LRESULT
CALLBACK
MainWindowProc( HWND handle, UINT message, WPARAM wp, LPARAM lp )
{
#define TILMI_ERROR_CLEANUP( error_msg, clean_tag )                                                                       \
        {                                                                                                                 \
            MessageBox(handle, GetTilmiString(error_msg), GetTilmiString(TILMI_STR_MBTITLE_ERROR), MB_OK | MB_ICONERROR); \
            success = false;                                                                                              \
            goto clean_tag;                                                                                               \
        }

#define TILMI_CHECK_FP_TOOLONG_CLEANUP( clean_tag )                                                                                          \
        {                                                                                                                                    \
            if (CommDlgExtendedError() == FNERR_BUFFERTOOSMALL)                                                                              \
                MessageBox(handle, GetTilmiString(TILMI_STR_ERR_FP_TOOLONG), GetTilmiString(TILMI_STR_MBTITLE_ERROR), MB_OK | MB_ICONERROR); \
            success = false;                                                                                                                 \
            goto clean_tag;                                                                                                                  \
        }

    Commandables
    {
        BTN_MODE1_GENERATE = 1,

        BTN_MODE2_BMP_SELECT,
        BTN_MODE2_CONVERT,

        BTN_MODE3_TIL_SELECT,
        BTN_MODE3_TMI_SELECT,
        BTN_MODE3_CONVERT,

        BTN_MODE4_GENERATE,

        BTN_EXIT
    };

    static HWND groupbox_mode1;
    static HWND edit_mode1_map_width, edit_mode1_map_height;
    static HWND button_mode1_generate;

    static HWND groupbox_mode2;
    static HWND edit_mode2_bmp;
    static HWND button_mode2_bmp_select, button_mode2_convert;

    static HWND groupbox_mode3;
    static HWND edit_mode3_til, edit_mode3_tmi;
    static HWND button_mode3_til_select, button_mode3_tmi_select;
    static HWND button_mode3_convert;

    static HWND groupbox_mode4;
    static HWND edit_mode4_map_width, edit_mode4_map_height;
    static HWND button_mode4_generate;

    static HWND button_exit;

    switch (message)
    {
        case WM_CREATE:

            // Create window controls
            groupbox_mode1 = CreateWindow(TEXT("BUTTON"),
                                          GetTilmiString(TILMI_STR_MODE1_GROUPBOX),
                                          WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                                          10, 40, 465, 100, handle, 0, program_instance, NULL);

            edit_mode1_map_width = CreateWindow(TEXT("EDIT"),
                                                TEXT(""),
                                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER,
                                                120, 85, 50, 24, handle, 0, program_instance, NULL);
            edit_mode1_map_height = CreateWindow(TEXT("EDIT"),
                                                 TEXT(""),
                                                 WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER,
                                                 300, 85, 50, 24, handle, 0, program_instance, NULL);
            button_mode1_generate = CreateWindow(TEXT("BUTTON"),
                                                 GetTilmiString(TILMI_STR_MODE1N4_BTN_GENERATE),
                                                 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                                 360, 85, 100, 24, handle, (HMENU) BTN_MODE1_GENERATE, program_instance, NULL);

            groupbox_mode2 = CreateWindow(TEXT("BUTTON"),
                                          GetTilmiString(TILMI_STR_MODE2_GROUPBOX),
                                          WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                                          10, 150, 465, 180, handle, 0, program_instance, NULL);

            edit_mode2_bmp = CreateWindow(TEXT("EDIT"),
                                          TEXT(""),
                                          WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
                                          30, 224, 350, 24, handle, 0, program_instance, NULL);
            button_mode2_bmp_select = CreateWindow(TEXT("BUTTON"),
                                               TEXT("..."),
                                               WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                               400, 224, 50, 24, handle, (HMENU) BTN_MODE2_BMP_SELECT, program_instance, NULL);
            button_mode2_convert = CreateWindow(TEXT("BUTTON"),
                                               GetTilmiString(TILMI_STR_MODE2N3_BTN_CONVERT),
                                               WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                               170, 270, 150, 40, handle, (HMENU) BTN_MODE2_CONVERT, program_instance, NULL);

            groupbox_mode3 = CreateWindow(TEXT("BUTTON"),
                                          GetTilmiString(TILMI_STR_MODE3_GROUPBOX),
                                          WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                                          10, 340, 465, 220, handle, 0, program_instance, NULL);
            edit_mode3_til = CreateWindow(TEXT("EDIT"),
                                          TEXT(""),
                                          WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
                                          30, 404, 350, 24, handle, 0, program_instance, NULL);
            button_mode3_til_select = CreateWindow(TEXT("BUTTON"),
                                      TEXT("..."),
                                      WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                      400, 404, 50, 24, handle, (HMENU) BTN_MODE3_TIL_SELECT, program_instance, NULL);
            edit_mode3_tmi = CreateWindow(TEXT("EDIT"),
                                          TEXT(""),
                                          WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
                                          30, 472, 350, 24, handle, 0, program_instance, NULL);
            button_mode3_tmi_select = CreateWindow(TEXT("BUTTON"),
                                      TEXT("..."),
                                      WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                      400, 472, 50, 24, handle, (HMENU) BTN_MODE3_TMI_SELECT, program_instance, NULL);
            button_mode3_convert = CreateWindow(TEXT("BUTTON"),
                                               GetTilmiString(TILMI_STR_MODE2N3_BTN_CONVERT),
                                               WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                               170, 507, 150, 40, handle, (HMENU) BTN_MODE3_CONVERT, program_instance, NULL);

            groupbox_mode4 = CreateWindow(TEXT("BUTTON"),
                                          GetTilmiString(TILMI_STR_MODE4_GROUPBOX),
                                          WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                                          10, 570, 465, 100, handle, 0, program_instance, NULL);
            edit_mode4_map_width = CreateWindow(TEXT("EDIT"),
                                                TEXT(""),
                                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER,
                                                120, 620, 50, 24, handle, 0, program_instance, NULL);
            edit_mode4_map_height = CreateWindow(TEXT("EDIT"),
                                                TEXT(""),
                                                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_CENTER | ES_NUMBER,
                                                300, 620, 50, 24, handle, 0, program_instance, NULL);
            button_mode4_generate = CreateWindow(TEXT("BUTTON"),
                                                GetTilmiString(TILMI_STR_MODE1N4_BTN_GENERATE),
                                                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                                360, 620, 100, 24, handle, (HMENU) BTN_MODE4_GENERATE, program_instance, NULL);


            button_exit = CreateWindow(TEXT("BUTTON"),
                                       GetTilmiString(TILMI_STR_BTN_EXIT),
                                       WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                                       370, 680, 105, 30, handle, (HMENU) BTN_EXIT, program_instance, NULL);

            // Set controls' font
            SendMessage(groupbox_mode1, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(edit_mode1_map_width, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(edit_mode1_map_height, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(button_mode1_generate, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);

            SendMessage(groupbox_mode2, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(edit_mode2_bmp, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(button_mode2_bmp_select, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(button_mode2_convert, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);

            SendMessage(groupbox_mode3, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(edit_mode3_til, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(button_mode3_til_select, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(edit_mode3_tmi, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(button_mode3_tmi_select, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(button_mode3_convert, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);

            SendMessage(groupbox_mode4, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(edit_mode4_map_width, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(edit_mode4_map_height, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            SendMessage(button_mode4_generate, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);

            SendMessage(button_exit, WM_SETFONT, (WPARAM) ui_font[chosen_language], TRUE);
            return 0;

        case WM_PAINT:

            // Draw labels
            PAINTSTRUCT ps = { 0 };
            HDC dc = BeginPaint(handle, &ps);

            HGDIOBJ old_font = SelectObject(dc, (HGDIOBJ) ui_font[chosen_language]);
            SetBkMode(dc, TRANSPARENT);

            SetTextColor(dc, RGB(0, 123, 201));
            DrawText(dc, GetTilmiString(TILMI_STR_TXT_NOTICE), -1, &(RECT){ 10, 10, 490, 30 }, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            SetTextColor(dc, RGB(0, 0, 0));
            DrawText(dc, GetTilmiString(TILMI_STR_MODE1N4_TXT_MAP_WIDTH), -1, &(RECT){ 30, 85, 150, 109 }, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            DrawText(dc, GetTilmiString(TILMI_STR_MODE1N4_TXT_MAP_HEIGHT), -1, &(RECT){ 200, 85, 320, 109 }, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            DrawText(dc, GetTilmiString(TILMI_STR_MODE2_BMP_FILEPATH), -1, &(RECT){ 30, 190, 300, 214 }, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            DrawText(dc, GetTilmiString(TILMI_STR_MODE3_TXT_TIL_FILEPATH), -1, &(RECT){ 30, 370, 300, 394 }, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            DrawText(dc, GetTilmiString(TILMI_STR_MODE3_TXT_TMI_FILEPATH), -1, &(RECT){ 30, 438, 300, 462 }, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            DrawText(dc, GetTilmiString(TILMI_STR_MODE1N4_TXT_MAP_WIDTH), -1, &(RECT){ 30, 620, 150, 644 }, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            DrawText(dc, GetTilmiString(TILMI_STR_MODE1N4_TXT_MAP_HEIGHT), -1, &(RECT){ 200, 620, 320, 644 }, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            DrawText(dc, GetTilmiString(TILMI_STR_TXT_VERSION), -1, &(RECT){ 10, 680, 490, 710 }, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            SelectObject(dc, old_font);

            EndPaint(handle, &ps);
            return 0;

        case WM_COMMAND:
            switch(LOWORD(wp))
            {
                case BTN_MODE1_GENERATE:
                {
                    bool success = true;

                    TCHAR map_width_str[5], map_height_str[5];
                    GetWindowText(edit_mode1_map_width, map_width_str, 4);
                    GetWindowText(edit_mode1_map_height, map_height_str, 4);

                    int map_width = StrToInt(map_width_str);
                    int map_height = StrToInt(map_height_str);

                    // Map dimensions need to be 1 ~ 127
                    if (map_width < 1 || map_width > 127 || map_height < 1 || map_height > 127)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_INVALID_MAP_WIDTH_HEIGHT, mode1_clean_end);

                    TCHAR file_name[256] = { '\0' };

                    OPENFILENAME ofn =
                    {
                        .lStructSize = sizeof(OPENFILENAME),
                        .hwndOwner = handle,
                        .hInstance = NULL,
                        .lpstrFilter = GetTilmiString(TILMI_STR_FILTER_TIL),
                        .nFilterIndex = 0,
                        .lpstrCustomFilter = NULL,
                        .nMaxCustFilter = 0,
                        .nMaxFile = 256,
                        .lpstrFile = file_name,
                        .lpstrFileTitle = NULL,
                        .lpstrInitialDir = NULL,
                        .lpstrTitle = GetTilmiString(TILMI_STR_TXT_SELECT_TIL_OUT),
                        .lpstrDefExt = TEXT("til"),
                        .Flags = OFN_OVERWRITEPROMPT
                    };

                    if (!GetSaveFileName(&ofn))
                        TILMI_CHECK_FP_TOOLONG_CLEANUP(mode1_clean_end);

                    HANDLE til_out = CreateFile(file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (til_out == INVALID_HANDLE_VALUE)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_TIL_OUT, mode1_clean_end);

                    TCHAR *dot = StrRChr(file_name, NULL, TEXT('.'));
                    *(dot) = TEXT('\0');
                    StringCbCat(dot, 5 * sizeof(TCHAR), TEXT(".tmi"));

                    HANDLE tmi_out = CreateFile(file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (tmi_out == INVALID_HANDLE_VALUE)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_TMI_OUT, mode1_clean_til);

                    // Create a til block with all pixels being visible
                    TilPixels block;
                    FillMemory(&(block.pixels), 32 * 32, 1);

                    // Compress that block to til data
                    // It is also possible to make it as a constant array of bytes, but that's too tedious
                    unsigned char *compressed_block = NULL;
                    size_t til_block_size = PixelsToTilData(&block, &compressed_block);

                    // Windows requires us to take the size written/read when using ReadFile/WriteFile
                    DWORD size_io = 0;
                    WriteFile(til_out, compressed_block, til_block_size, &size_io, NULL);
                    WriteFile(til_out, &(uint32_t){ 0 }, 4, &size_io, NULL);

                    WriteFile(tmi_out, &(uint16_t){ map_width }, 2, &size_io, NULL);
                    WriteFile(tmi_out, &(uint16_t){ map_height }, 2, &size_io, NULL);

                    // Because we only have 1 til block, the index of all tmi block is 0, which is that til block
                    char *fill = (char *) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, map_width * map_height * 2);
                    WriteFile(tmi_out, fill, map_width * map_height * 2, &size_io, NULL);

                    // Clean up
                    HeapFree(GetProcessHeap(), 0, fill);
                    CloseHandle(tmi_out);
                mode1_clean_til:
                    CloseHandle(til_out);
                mode1_clean_end:

                    if (success)
                        MessageBox(handle, GetTilmiString(TILMI_STR_TXT_DONE), GetTilmiString(TILMI_STR_MBTITLE_OK), MB_OK);
                    return 0;
                }

                case BTN_MODE2_BMP_SELECT:
                {
                    bool success = true;
                    TCHAR file_name[256] = { TEXT('\0') };

                    OPENFILENAME ofn =
                    {
                        .lStructSize = sizeof(OPENFILENAME),
                        .hwndOwner = handle,
                        .hInstance = NULL,
                        .lpstrFilter = GetTilmiString(TILMI_STR_FILTER_BMP),
                        .nFilterIndex = 0,
                        .lpstrCustomFilter = NULL,
                        .nMaxCustFilter = 0,
                        .nMaxFile = 256,
                        .lpstrFile = file_name,
                        .lpstrFileTitle = NULL,
                        .lpstrInitialDir = NULL,
                        .lpstrTitle = GetTilmiString(TILMI_STR_TXT_SELECT_BMP_IN),
                        .lpstrDefExt = TEXT("bmp")
                    };

                    if (!GetOpenFileName(&ofn))
                        TILMI_CHECK_FP_TOOLONG_CLEANUP(mode2_bmp_select_end);

                mode2_bmp_select_end:

                    if (success)
                        SetWindowText(edit_mode2_bmp, file_name);
                    return 0;
                }

                case BTN_MODE2_CONVERT:
                {
                    bool success = true;
                    TCHAR file_name[256] = { TEXT('\0') };

                    GetWindowText(edit_mode2_bmp, file_name, 256);

                    HANDLE bmp_in = CreateFile(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (bmp_in == INVALID_HANDLE_VALUE)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_BMP_IN, mode2_clean_end);

                    LARGE_INTEGER bmp_filesize;
                    GetFileSizeEx(bmp_in, &bmp_filesize);

                    if (bmp_filesize.QuadPart < 62)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_BMP_INVALID, mode2_clean_bmp);

                    HANDLE bmp_map = CreateFileMapping(bmp_in, NULL, PAGE_READONLY, 0, 0, NULL);
                    unsigned char *bmp_data = (unsigned char *) MapViewOfFile(bmp_map, FILE_MAP_READ, 0, 0, bmp_filesize.QuadPart);
                    if (!bmp_data)
                        // That's not something our program can handle...
                        ExitProcess(1);

                    BITMAPFILEHEADER *bmp_file_header = (BITMAPFILEHEADER *) bmp_data;
                    BITMAPINFOHEADER *bmp_info = (BITMAPINFOHEADER *) (bmp_data + 14);

                    // Only accepts 1 bit monochrome (for now?)
                    if (bmp_info->biBitCount != 1)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_BMP_NOT_MONOCHROME, mode2_clean_bmp_map);

                    int32_t image_width = bmp_info->biWidth;
                    int32_t image_height = bmp_info->biHeight;

                    // Check if the dimensions of the bmp are aligned to 32
                    if (!image_width || !image_height || image_width % 32 || image_height % 32)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_BMP_NOT_32_ALIGN, mode2_clean_bmp_map);

                    int16_t map_width = image_width / 32;
                    int16_t map_height = image_height / 32;

                    if (map_width < 1 || map_width > 127 || map_height < 1 || map_height > 127)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_INVALID_MAP_WIDTH_HEIGHT, mode2_clean_bmp_map);

                    unsigned char *bmp_bits = bmp_data + bmp_file_header->bfOffBits;

                    TCHAR out_file_name[256] = { TEXT('\0') };
                    OPENFILENAME ofn =
                    {
                        .lStructSize = sizeof(OPENFILENAME),
                        .hwndOwner = handle,
                        .hInstance = NULL,
                        .lpstrFilter = GetTilmiString(TILMI_STR_FILTER_TIL),
                        .nFilterIndex = 0,
                        .lpstrCustomFilter = NULL,
                        .nMaxCustFilter = 0,
                        .nMaxFile = 256,
                        .lpstrFile = out_file_name,
                        .lpstrFileTitle = NULL,
                        .lpstrInitialDir = NULL,
                        .lpstrTitle = GetTilmiString(TILMI_STR_TXT_SELECT_TIL_OUT),
                        .lpstrDefExt = TEXT("til"),
                        .Flags = OFN_OVERWRITEPROMPT
                    };

                    if (!GetSaveFileName(&ofn))
                        TILMI_CHECK_FP_TOOLONG_CLEANUP(mode2_clean_bmp_map);

                    HANDLE til_out = CreateFile(out_file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (til_out == INVALID_HANDLE_VALUE)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_TIL_OUT, mode2_clean_bmp_map);

                    TCHAR *dot = StrRChr(out_file_name, NULL, TEXT('.'));
                    *(dot) = TEXT('\0');
                    StringCbCat(dot, 5 * sizeof(TCHAR), TEXT(".tmi"));

                    HANDLE tmi_out = CreateFile(out_file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (tmi_out == INVALID_HANDLE_VALUE)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_TMI_OUT, mode2_clean_til);

                    DWORD size_io = 0;
                    WriteFile(tmi_out, &map_width, 2, &size_io, NULL);
                    WriteFile(tmi_out, &map_height, 2, &size_io, NULL);

                    Vector unique_tils = Vector_Create(sizeof(TilPixels));

                    // Although NOT NECESSARY, it is recommanded to set the first til block to be all visible
                    TilPixels block_all_visible;
                    FillMemory(&(block_all_visible.pixels), 32 * 32, 1);
                    Vector_Push(&unique_tils, &block_all_visible);

                    unsigned char *til_all_visible = NULL;
                    size_t til_all_visible_filesize = PixelsToTilData(&block_all_visible, &til_all_visible);
                    WriteFile(til_out, til_all_visible, til_all_visible_filesize, &size_io, NULL);

                    // 32 pixels is 32 bits = 4 bytes
                    // 32 * 32 pixels is 128 bytes
                    size_t length_per_width = 4 * map_width;

                    // Bmp's bottom-left origin
                    unsigned char *bmp_origin = bmp_bits + (image_height > 0 ? (length_per_width * (image_height - 1)) : 0);

                    // Iterate through each 32 * 32 block of the bmp
                    // If we find an unique block, record it in til and mark a new tmi index
                    // if we find an already exist block, record that index in tmi
                    for (size_t h = 0; h < map_height; h++)
                        for (size_t w = 0; w < map_width; w++)
                        {
                            unsigned char *current_block =
                            image_height < 0 ?
                                bmp_origin + length_per_width * h * 32 + w * 4
                            :
                                bmp_origin - length_per_width * h * 32 + w * 4;

                            TilPixels current_block_pixels = { 0 };
                            for (unsigned char hp = 0; hp < 32; hp++)
                            {
                                unsigned char *current_line =
                                image_height < 0 ?
                                    current_block + length_per_width * hp
                                :
                                    current_block - length_per_width * hp;

                                unsigned char current_line_pixels[4] = { 0 };
                                CopyMemory(current_line_pixels, current_line, 4);
                                BitsToBytes(current_line_pixels, current_block_pixels.pixels[hp]);
                            }

                            // Check if til block already exists
                            char *old_til = (char *) Vector_Find(&unique_tils, &current_block_pixels, (VectorCmpFunc) CompareTilBlock);
                            if (!old_til)
                            {
                                // If it does not, write til to vector
                                // and write the new index to tmi, new block to til
                                Vector_Push(&unique_tils, &current_block_pixels);

                                WriteFile(tmi_out, &(uint16_t){ unique_tils.length - 1 }, 2, &size_io, NULL);

                                unsigned char *til_result = NULL;
                                size_t til_block_filesize = PixelsToTilData(&current_block_pixels, &til_result);
                                WriteFile(til_out, til_result, til_block_filesize, &size_io, NULL);
                            }
                            else
                            {
                                // If it does, write the old index to tmi
                                uint16_t index = (old_til - (char *)(unique_tils.ptr)) / sizeof(TilPixels);
                                WriteFile(tmi_out, &index, 2, &size_io, NULL);
                            }
                        }

                    Vector_Destroy(&unique_tils);

                    WriteFile(til_out, &(uint32_t){ 0 }, 4, &size_io, NULL);

                    // Clean up
                    CloseHandle(tmi_out);
                mode2_clean_til:
                    CloseHandle(til_out);
                mode2_clean_bmp_map:
                    UnmapViewOfFile(bmp_data);
                    CloseHandle(bmp_map);
                mode2_clean_bmp:
                    CloseHandle(bmp_in);
                mode2_clean_end:

                    if (success)
                        MessageBox(handle, GetTilmiString(TILMI_STR_TXT_DONE), GetTilmiString(TILMI_STR_MBTITLE_OK), MB_OK);
                    return 0;
                }

                case BTN_MODE3_TIL_SELECT:
                {
                    bool success = true;
                    TCHAR file_name[256] = { '\0' };

                    OPENFILENAME ofn =
                    {
                        .lStructSize = sizeof(OPENFILENAME),
                        .hwndOwner = handle,
                        .hInstance = NULL,
                        .lpstrFilter = GetTilmiString(TILMI_STR_FILTER_TIL),
                        .nFilterIndex = 0,
                        .lpstrCustomFilter = NULL,
                        .nMaxCustFilter = 0,
                        .nMaxFile = 256,
                        .lpstrFile = file_name,
                        .lpstrFileTitle = NULL,
                        .lpstrInitialDir = NULL,
                        .lpstrTitle = GetTilmiString(TILMI_STR_TXT_SELECT_TIL_IN),
                        .lpstrDefExt = TEXT("til")
                    };

                    if (!GetOpenFileName(&ofn))
                        TILMI_CHECK_FP_TOOLONG_CLEANUP(mode3_til_select_end);

                mode3_til_select_end:

                    if (success)
                        SetWindowText(edit_mode3_til, file_name);
                    return 0;
                }

                case BTN_MODE3_TMI_SELECT:
                {
                    bool success = true;
                    TCHAR file_name[256] = { '\0' };

                    OPENFILENAME ofn =
                    {
                        .lStructSize = sizeof(OPENFILENAME),
                        .hwndOwner = handle,
                        .hInstance = NULL,
                        .lpstrFilter = GetTilmiString(TILMI_STR_FILTER_TMI),
                        .nFilterIndex = 0,
                        .lpstrCustomFilter = NULL,
                        .nMaxCustFilter = 0,
                        .nMaxFile = 256,
                        .lpstrFile = file_name,
                        .lpstrFileTitle = NULL,
                        .lpstrInitialDir = NULL,
                        .lpstrTitle = GetTilmiString(TILMI_STR_TXT_SELECT_TMI_IN),
                        .lpstrDefExt = TEXT("tmi")
                    };

                    if (!GetOpenFileName(&ofn))
                        TILMI_CHECK_FP_TOOLONG_CLEANUP(mode3_tmi_select_end);

                mode3_tmi_select_end:

                    if (success)
                        SetWindowText(edit_mode3_tmi, file_name);
                    return 0;
                }

                case BTN_MODE3_CONVERT:
                {
                    bool success = true;
                    TCHAR file_name[256] = { TEXT('\0') };

                    GetWindowText(edit_mode3_til, file_name, 256);

                    HANDLE til_in = CreateFile(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (til_in == INVALID_HANDLE_VALUE)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_TIL_IN, mode3_clean_end);

                    GetWindowText(edit_mode3_tmi, file_name, 256);

                    HANDLE tmi_in = CreateFile(file_name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (tmi_in == INVALID_HANDLE_VALUE)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_TMI_IN, mode3_clean_til);

                    OPENFILENAME ofn =
                    {
                        .lStructSize = sizeof(OPENFILENAME),
                        .hwndOwner = handle,
                        .hInstance = NULL,
                        .lpstrFilter = GetTilmiString(TILMI_STR_FILTER_BMP),
                        .nFilterIndex = 0,
                        .lpstrCustomFilter = NULL,
                        .nMaxCustFilter = 0,
                        .nMaxFile = 256,
                        .lpstrFile = file_name,
                        .lpstrFileTitle = NULL,
                        .lpstrInitialDir = NULL,
                        .lpstrTitle = GetTilmiString(TILMI_STR_TXT_SELECT_BMP_OUT),
                        .lpstrDefExt = TEXT("bmp"),
                        .Flags = OFN_OVERWRITEPROMPT
                    };

                    if (!GetSaveFileName(&ofn))
                        TILMI_CHECK_FP_TOOLONG_CLEANUP(mode3_clean_tmi);

                    HANDLE bmp_out = CreateFile(file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (bmp_out == INVALID_HANDLE_VALUE)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_BMP_OUT, mode3_clean_tmi);

                    LARGE_INTEGER til_filesize;
                    GetFileSizeEx(til_in, &til_filesize);

                    HANDLE til_map = CreateFileMapping(til_in, NULL, PAGE_READONLY, 0, 0, NULL);
                    const unsigned char *til_data = (unsigned char *) MapViewOfFile(til_map, FILE_MAP_READ, 0, 0, til_filesize.QuadPart);
                    if (!til_data)
                        // That's not something our program can handle...
                        ExitProcess(1);

                    const unsigned char *til_data_ptr = til_data;
                    Vector unique_tils = Vector_Create(sizeof(TilPixels));

                    // Record all til blocks
                    while (true)
                    {
                        uint32_t *til_block_size = (uint32_t *) til_data_ptr;
                        if (!til_block_size || !(*til_block_size))
                            break;
                        TilPixels til_block;
                        TilDataToPixels(til_data_ptr, &til_block);

                        Vector_Push(&unique_tils, &til_block);
                        til_data_ptr += *til_block_size + 4;
                    }

                    int16_t map_width = 0;
                    int16_t map_height = 0;

                    DWORD size_io = 0;
                    ReadFile(tmi_in, &map_width, 2, &size_io, NULL);
                    ReadFile(tmi_in, &map_height, 2, &size_io, NULL);

                    // Check if map size makes sense
                    if (!map_width || !map_height || map_width > 127 || map_height > 127)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_INVALID_MAP_WIDTH_HEIGHT, mode3_clean_til_map);

                    int32_t image_width = map_width * 32;
                    int32_t image_height = map_height * 32;

                    BITMAPFILEHEADER bmp_file_header =
                    {
                        .bfType = 0x4d42,
                        .bfSize = 62 * image_width * image_height / 8,
                        .bfReserved1 = 0,
                        .bfReserved2 = 0,
                        .bfOffBits = 62
                    };
                    BITMAPINFOHEADER bmp_info =
                    {
                        .biSize = sizeof(BITMAPINFOHEADER),
                        .biWidth = image_width,
                        .biHeight = image_height,
                        .biPlanes = 1,
                        .biBitCount = 1,
                        .biCompression = BI_RGB,
                        .biSizeImage = image_width * image_height / 8,
                        .biXPelsPerMeter = 0,
                        .biYPelsPerMeter = 0,
                        .biClrUsed = 0,
                        .biClrImportant = 0
                    };
                    // Not sure what these data of bmp does... but they don't affect the result
                    unsigned char fill[] = { 0, 0, 0, 0, 0xff, 0xff, 0xff, 0 };

                    WriteFile(bmp_out, &bmp_file_header, sizeof(BITMAPFILEHEADER), &size_io, NULL);
                    WriteFile(bmp_out, &bmp_info, sizeof(BITMAPINFOHEADER), &size_io, NULL);
                    WriteFile(bmp_out, fill, 8, &size_io, NULL);
                    unsigned char *bmp_bits = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, image_width * image_height / 8);

                    uint32_t length_per_width = image_width / 8;

                    // We only output positive height bmp
                    // That is, origin at botton left
                    unsigned char *bmp_origin = bmp_bits + length_per_width * (image_height - 1);

                    for (size_t h = 0; h < map_height; h++)
                        for (size_t w = 0; w < map_width; w++)
                        {
                            uint16_t tmi_index;
                            ReadFile(tmi_in, &tmi_index, 2, &size_io, NULL);
                            if (tmi_index > unique_tils.length - 1)
                                TILMI_ERROR_CLEANUP(TILMI_STR_ERR_TMI_NOT_TIL_PAIR, mode3_clean_heap);

                            TilPixels *til_block = Vector_PtrAt(&unique_tils, tmi_index);
                            for (size_t p = 0; p < 32; p++)
                            {
                                unsigned char *bmp_block = bmp_origin - h * 32 * length_per_width - p * length_per_width + w * 4;
                                BytesToBits(til_block->pixels[p], bmp_block);
                            }
                        }

                    WriteFile(bmp_out, bmp_bits, image_width * image_height / 8, &size_io, NULL);

                    // Clean up
                mode3_clean_heap:
                    HeapFree(GetProcessHeap(), 0, bmp_bits);
                mode3_clean_til_map:
                    UnmapViewOfFile(til_data);
                    CloseHandle(til_map);
                    CloseHandle(bmp_out);
                mode3_clean_tmi:
                    CloseHandle(tmi_in);
                mode3_clean_til:
                    CloseHandle(til_in);
                mode3_clean_end:

                    if (success)
                        MessageBox(handle, GetTilmiString(TILMI_STR_TXT_DONE), GetTilmiString(TILMI_STR_MBTITLE_OK), MB_OK);
                    return 0;
                }

                case BTN_MODE4_GENERATE:
                {
                    bool success = true;

                    TCHAR map_width_str[5], map_height_str[5];
                    GetWindowText(edit_mode4_map_width, map_width_str, 4);
                    GetWindowText(edit_mode4_map_height, map_height_str, 4);

                    int32_t map_width = StrToInt(map_width_str);
                    int32_t map_height = StrToInt(map_height_str);

                    // Map dimensions need to be 1 ~ 127
                    if (map_width < 1 || map_width > 127 || map_height < 1 || map_height > 127)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_INVALID_MAP_WIDTH_HEIGHT, mode1_clean_end);

                    TCHAR file_name[256] = { '\0' };

                    OPENFILENAME ofn =
                    {
                        .lStructSize = sizeof(OPENFILENAME),
                        .hwndOwner = handle,
                        .hInstance = NULL,
                        .lpstrFilter = GetTilmiString(TILMI_STR_FILTER_MAP),
                        .nFilterIndex = 0,
                        .lpstrCustomFilter = NULL,
                        .nMaxCustFilter = 0,
                        .nMaxFile = 256,
                        .lpstrFile = file_name,
                        .lpstrFileTitle = NULL,
                        .lpstrInitialDir = NULL,
                        .lpstrTitle = GetTilmiString(TILMI_STR_TXT_SELECT_MAP_OUT),
                        .lpstrDefExt = TEXT("map"),
                        .Flags = OFN_OVERWRITEPROMPT
                    };

                    if (!GetSaveFileName(&ofn))
                        TILMI_CHECK_FP_TOOLONG_CLEANUP(mode4_clean_end);

                    HANDLE map_out = CreateFile(file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (map_out == INVALID_HANDLE_VALUE)
                        TILMI_ERROR_CLEANUP(TILMI_STR_ERR_MAP_OUT, mode4_clean_end);

                    DWORD size_io = 0;

                    // .map header
                    WriteFile(map_out, TILMI_MAP_HEADER, 5, &size_io, NULL);
                    WriteFile(map_out, TILMI_MAP_FILLER, 8, &size_io, NULL);

                    // Width and height of map
                    WriteFile(map_out, &map_width, 4, &size_io, NULL);
                    WriteFile(map_out, &map_height, 4, &size_io, NULL);

                    for (uint8_t h = 0; h < map_height; h++)
                        for (uint8_t w = 0; w < map_width; w++)
                        {
                            // Write current tile information
                            WriteFile(map_out, &h, 1, &size_io, NULL);
                            WriteFile(map_out, &w, 1, &size_io, NULL);
                            WriteFile(map_out, (h == 0 || h == map_height - 1 || w == 0 || w == map_width - 1) ? TILMI_MAP_ALL_REJECT : TILMI_MAP_ALL_CLEAR, 15, &size_io, NULL);
                        }

                    // 5 bytes of EOF mark
                    WriteFile(map_out, TILMI_MAP_FILLER, 5, &size_io, NULL);
                    CloseHandle(map_out);

                mode4_clean_end:

                    if (success)
                        MessageBox(handle, GetTilmiString(TILMI_STR_TXT_DONE), GetTilmiString(TILMI_STR_MBTITLE_OK), MB_OK);
                    return 0;
                }

                case BTN_EXIT:
                    DestroyWindow(handle);
                    return 0;
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(handle, message, wp, lp);

#undef TILMI_FP_TOOLONG_CLEANUP
#undef TILMI_ERROR_CLEANUP
}

#undef CommandControls
