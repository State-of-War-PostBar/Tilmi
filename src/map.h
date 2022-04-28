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

#ifndef TILMI_MAP_H
#define TILMI_MAP_H

#include <pch.h>

/// Header of .map file. Constants.
extern const uint8_t TILMI_MAP_HEADER[5];

/// Two 32-bit word with unknown usage. DK said it's the position of the camera when game starts.
/// SOW seems to be changing them when loading. Not a concern of our program anyways, so all null.
extern const uint8_t TILMI_MAP_FILLER[8];

/// Signature for a tile that allow all entities to cross.
extern const uint8_t TILMI_MAP_ALL_CLEAR[15];

/// Signature for a tile that allow no entity to cross. Use as map border.
extern const uint8_t TILMI_MAP_ALL_REJECT[15];

/// After all the tiles are defined, we need 5 bytes of null to mark the end of file.
/// Just use 5 bytes from TILMI_MAP_FILLER.

#endif // !TILMI_MAP_H
