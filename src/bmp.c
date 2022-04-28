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

#include "bmp.h"

void
BytesToBits( const unsigned char bytes[static 32], unsigned char bits[static 4] )
{
    for (unsigned int i = 0; i < 4; i++)
        for (unsigned int j = 0; j < 8; j++)
            bits[i] |= bytes[8 * i + j] ? (1 << (7 - j)) : 0;
}

void
BitsToBytes( const unsigned char bits[static 4], unsigned char bytes[static 32] )
{
    for (unsigned int i = 0; i < 4; i++)
        for (unsigned int j = 0; j < 8; j++)
            bytes[i * 8 + j] = !!(bits[i] & (1 << (7 - j)));
}
