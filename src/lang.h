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

#ifndef TILMI_LANG_H
#define TILMI_LANG_H

#include <pch.h>

/// Language codes
typedef enum
{
    TILMI_EN_US = 1,
    TILMI_ZH_SP,

    TILMI_LANG_END
} TilmiLanguage;

/// Index for string resources
typedef enum
{
    TILMI_STR_TXT_NOTICE = 1,

    TILMI_STR_MODE1_GROUPBOX,
    TILMI_STR_MODE1_TXT_MAP_WIDTH,
    TILMI_STR_MODE1_TXT_MAP_HEIGHT,
    TILMI_STR_MODE1_BTN_GENERATE,

    TILMI_STR_MODE2_GROUPBOX,
    TILMI_STR_MODE2_BMP_FILEPATH,
    TILMI_STR_MODE2AND3_BTN_CONVERT,

    TILMI_STR_MODE3_GROUPBOX,
    TILMI_STR_MODE3_TXT_TIL_FILEPATH,
    TILMI_STR_MODE3_TXT_TMI_FILEPATH,

    TILMI_STR_TXT_VERSION,
    TILMI_STR_BTN_EXIT,

    TILMI_STR_FILTER_TIL,
    TILMI_STR_FILTER_TMI,
    TILMI_STR_FILTER_BMP,

    TILMI_STR_TXT_SELECT_TIL_IN,
    TILMI_STR_TXT_SELECT_TIL_OUT,
    TILMI_STR_TXT_SELECT_TMI_IN,
    TILMI_STR_TXT_SELECT_TMI_OUT,
    TILMI_STR_TXT_SELECT_BMP_IN,
    TILMI_STR_TXT_SELECT_BMP_OUT,

    TILMI_STR_MBTITLE_OK,

    TILMI_STR_TXT_DONE,

    TILMI_STR_MBTITLE_ERROR,

    TILMI_STR_ERR_TIL_IN,
    TILMI_STR_ERR_TIL_OUT,
    TILMI_STR_ERR_TMI_IN,
    TILMI_STR_ERR_TMI_OUT,
    TILMI_STR_ERR_BMP_IN,
    TILMI_STR_ERR_BMP_OUT,
    TILMI_STR_ERR_FP_TOOLONG,

    TILMI_STR_ERR_INVALID_MAP_WIDTH_HEIGHT,

    TILMI_STR_ERR_BMP_INVALID,
    TILMI_STR_ERR_BMP_NOT_MONOCHROME,
    TILMI_STR_ERR_BMP_NOT_32_ALIGN,

    TILMI_STR_ERR_TMI_NOT_TIL_PAIR,

    TILMI_STR_ENUM_END
} TilmiStringID;

void
LoadTilmiStrings( HINSTANCE instance, TilmiLanguage language );

const TCHAR *const
GetTilmiString( TilmiStringID str );

#endif // !TILMI_LANG_H
