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

#include "til.h"

// The first two pair of integers are width and height of the til data.
// Followed by three pairs of til data with unknown usage, starting at offset 0x08.
// They are always 0, 0, 32, 32, 0, 0. Seems like having to do with tsp.
static const uint16_t TIL_TSP_CONSTANT[8] = { 32, 32, 0, 0, 32, 32, 0, 0 };

bool
CompareTilBlock( const TilPixels *lhs, const TilPixels *rhs )
{
    return RtlEqualMemory(lhs, rhs, sizeof(TilPixels));
}

size_t
PixelsToTilData( const TilPixels *block, unsigned char **output )
{
    // Static memory for til block
    // The size is the largest size possible for a 32 * 32 block in til form
    static unsigned char til_data[4 + 2 * 2 + 3 * 2 * 2 + 32 * 4 + 32 * (2 + 2 + 32 * 2)];

    // Pointer for scanlines
    unsigned char *ptr = til_data + 4;

    // Add til's tsp constants
    CopyMemory(ptr, TIL_TSP_CONSTANT, 8 * 2);

    // Scanline offsets data block
    unsigned char *width_offsets_ptr = ptr + 8 * 2;

    // Skip to actual data section
    ptr += 8 * 2 + 32 * 4;

    // Accumulated written data length of the til block (excluding 4 bytes of length header)
    uint32_t bytes_written = 2 * 2 + 3 * 2 * 2 + 32 * 4;

    // Iterate through [height] times for each scanline
    for (size_t h = 0; h < 32; h++)
    {
        // 32 bytes array for current scanline
        const unsigned char *scanline_pixels = block->pixels[h];

        //  Number of commands in the scanline
        uint16_t num_commands = 1;

        // First command of of the scanline
        // 0 = Visible (black), 1 = Invisible (white)
        uint16_t first_command;

        // Current processing command
        // False = visible, true = invisible
        bool current_command;

        // Total pixels a command covers
        uint16_t command_pixels = 1;

        // First pixel of the scanline
        unsigned char first_pixel = scanline_pixels[0];

        // Pointer for writing actual data after command information
        unsigned char *data_ptr = ptr + 4;

        // Pointer for beginning of scanline information
        unsigned char *scanline_begin_ptr = ptr;

        if (first_pixel)
        {
            // First pixel is visible
            first_command = 0;
            current_command = false;
        }
        else
        {
            // First pixel is invisible
            first_command = 1;
            current_command = true;
        }

        CopyMemory(scanline_begin_ptr + 2, &first_command, 2);
        bytes_written += 4;

        // Read the scanline
        for (size_t w = 1; w < 32; w++)
        {
            // Current pixel
            unsigned char current_pixel = scanline_pixels[w];

            // Current pixel is visible
            if (current_pixel)
            {
                if (!current_command)
                    // Current command is also visible, add another pixel
                    command_pixels++;
                else
                {
                    // Current command is invisible, so this is a new command
                    // Record the old command and start a new one
                    CopyMemory(data_ptr, &command_pixels, 2);
                    data_ptr += 2;
                    bytes_written += 2;

                    command_pixels = 1;
                    num_commands++;
                }

                current_command = false;
            }
            // Current pixel is invisible
            else
            {
                if (current_command)
                    // Current command is also invisible, add another pixel
                    command_pixels++;
                else
                {
                    CopyMemory(data_ptr, &command_pixels, 2);
                    data_ptr += 2;
                    bytes_written += 2;

                    command_pixels = 1;
                    num_commands++;
                }

                current_command = true;
            }
        }

        // Leftover pixels
        if (command_pixels)
        {
            CopyMemory(data_ptr, &command_pixels, 2);
            bytes_written += 2;
        }

        // Write how many commands this scanline has
        CopyMemory(scanline_begin_ptr, &num_commands, 2);

        // Write current scanline's offset
        CopyMemory(width_offsets_ptr + 4 * h, &(uint32_t){ (scanline_begin_ptr - til_data - 4) / 2 }, 4);

        // Move pointer to next scanline
        ptr = data_ptr + 2;
    }

    // Write the size of current til block
    CopyMemory(til_data, &bytes_written, 4);

    // Give the second argument the address of buffer
    *output = til_data;
    return bytes_written + 4;
}

void
TilDataToPixels( const unsigned char *til_data, TilPixels *output )
{
    const uint32_t *til_data_size = (uint32_t *) til_data;

    // Least possible size of a til block is 336 bytes
    if (*til_data_size < 336)
    {
        FillMemory(output, sizeof(TilPixels), 0);
        return;
    }

    const uint32_t *width_offsets = (uint32_t *)(til_data + 20);

    for (size_t h = 0; h < 32; h++)
    {
        // Pointer to current scanline data
        const uint16_t *current_scanline_ptr = (uint16_t *)(til_data + 2 * width_offsets[h] + 4);

        // Number of commands in current scanline
        uint16_t num_commands = *current_scanline_ptr++;

        // First command of current scanline
        // 0 = visible, 1 = invisible (we reverse it in our program)
        uint16_t first_command = *current_scanline_ptr++;
        first_command = !first_command;

        // Current command processing
        uint16_t current_command = first_command;

        // Total pixels covered of current scanline
        uint16_t accu_pixels = 0;

        for (size_t p = 0; p < num_commands; p++)
        {
            // Pixels that current command cover
            uint16_t command_pixels = *current_scanline_ptr++;

            FillMemory(&(output->pixels[h][accu_pixels]), command_pixels, current_command);

            accu_pixels += command_pixels;
            current_command = !current_command;
        }
    }
}
