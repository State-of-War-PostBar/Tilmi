/*************************************************************************************************
*                                                                                                *
*                                           [ Til-mi ]                                           *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                     A tool for creating State of War's til and tmi files.                      *
*               (ɔ) 2022 - 2024 State of War Baidu PostBar, some rights reserved.                *
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

#include "map.h"

const uint8_t TILMI_MAP_HEADER[5] = { 0x04, 0x56, 0x45, 0x52, 0x37 };
const uint8_t TILMI_MAP_FILLER[8] = { 0 };
const uint8_t TILMI_MAP_ALL_CLEAR[15] = { 0 };
const uint8_t TILMI_MAP_ALL_REJECT[15] = { 0xfd, 0xff, 0xff, 0x00, 0xfd, 0xff, 0xff, 0x00 };
