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

#include "lang.h"

static const TCHAR *tilmi_strings[TILMI_STR_ENUM_END];

void
LoadTilmiStrings( HINSTANCE instance, TilmiLanguage language )
{
    for (size_t i = 1; i < TILMI_STR_ENUM_END; i++)
        LoadString(instance, 100 * language + i, (LPTCH) &(tilmi_strings[i]), 0);
}

const TCHAR *const
GetTilmiString( TilmiStringID str )
{
    return tilmi_strings[str];
}
