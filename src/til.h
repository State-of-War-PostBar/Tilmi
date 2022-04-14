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

#ifndef TILMI_TIL_H
#define TILMI_TIL_H

#include <pch.h>

// ==========================================  Description of til and tmi file  ==========================================

//              Don't try to read it too hard, I'm making a better documentation.   -- Taxerap

// Til file is basically a [set] of many til blocks.
// Each til block is a 32 * 32 monochrome bitmap.
//
// How data is stored in til:
//
// |  Offset  |      Size      |  Usage  |
// |       0  |         4      |  The total size of data after this number. A value of 0 marks end of til
// |       4  |         2      |  Width of current block, always 32 in til     <---------------------------
// |       6  |         2      |  Height of current block, always 32 in til                               |
// |       8  |        12      |  A bunch of .tsp constants, unknown usage                                |
// |      20  | 4 * map height | Offset to the data of the scanline at that height divided by 2           |
//                               This offset starts counting at -------------------------------------------
//                               Use this offset to find the beginning of scanline data.
//
// The scanline data uses some sort of alternating run-length encoding, identical to tsp files.
// Only difference is that all of til's data are monochrome (1 or 0).
// The first integer marks the total alternation of the scanline.
// The second integer marks if the first pixel is invisible. (1 for covering units, 0 for not covering)
// Then the RLE begins. 
//
// For example:
//
// 1 1 32   - One alternation, first pixel is invisible. 32 invisible pixels.
// 1 0 32   - One alternation, first pixel is visible. 32 visible pixels.
// 2 1 24 8 - Two alternation, first pixel is invisible. 24 invisible pixels, 8 visible pixels.
//
// Notice that SOW uses 1 for invisible pixel, 0 for visible pixel.
// But we use black (0) pixel for invisible and white (1) for visible when dealing with bmp images.
// So we'll have to reverse that when converting bmp into til.

// Theoretically, it is possible to set the size of one til block to fill the entire srf, and use a single index in tmi.
// Haven't test it yet...

// =======================================================================================================================

/// Pixels of a 32 * 32 til block
typedef struct
{
    unsigned char pixels[32][32];
} TilPixels;

///
/// Compare two til blocks to check if they are equal. Used to feed into compare function of containers.
///
/// \param lhs First til block
/// \param rhs Second til block
///
/// \return True if they are equal
///
bool
CompareTilBlock( const TilPixels *lhs, const TilPixels *rhs );

///
/// Compress pixels to til data. This function set a pointer to a static buffer containing til data
/// as the second argument.
///
/// \param block 32 * 32 block of pixels
/// \param output Address of pointer to receive output buffer address
///
/// \return The actual size of compressed til data in bytes
///
size_t
PixelsToTilData( const TilPixels *block, unsigned char **output );

///
/// Decompress til data to pixels.
///
/// \param til_data A til data block. Must be at least 4 bytes
/// \param output Pointer to a pixels buffer for output
///
void
TilDataToPixels( const unsigned char *til_data, TilPixels *output );

#endif // !TILMI_TIL_H
