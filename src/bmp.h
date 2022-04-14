/*************************************************************************************************
*                                                                                                *
*                                           [ Til-mi ]                                           *
*                                                                                                *
**************************************************************************************************
*                                                                                                *
*                     A tool for creating State of War's til and tmi files.                      *
*               (ɔ) 2017 - 2022 State of War Baidu Postbar, some rights reserved.                *
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

#ifndef TILMI_BMP_H
#define TILMI_BMP_H

#include <pch.h>

///
/// Convert 1-bit monochrome data to 8-bit
///
/// \param bytes 8-bit to convert from
/// \param bits  1-bit to convert to. Must be all 0
///
void
BytesToBits( const unsigned char bytes[static 32], unsigned char bits[static 4] );

///
/// Convert 8-bit monochrome data to 1-bit
///
/// \param bits  1-bit to convert from
/// \param bytes 8-bit to convert to
///
void
BitsToBytes( const unsigned char bits[static 4], unsigned char bytes[static 32] );

#endif // !TILMI_BMP_H
