Note: For systems eariler than Windows 10 to run this program you need UCRT.
(https://support.microsoft.com/en-us/topic/update-for-universal-c-runtime-in-windows-322bf30f-4735-bb94-3949-49f5c49f4732)

1.0 Update:
    - Fixed a bug that messes up negative height bmps
    - Added a mode to create .map templates

---------------------------------

What are til and tmi?

Mxx.til and Mxx.tmi files under the Data/Levels folder defines the invisible areas of units of the map.
Part of unit's sprites in such area will be invisible, like it "covers" unit.
For examples, trees, and the large buildings of M04, M10, M22...
Notice that this invisibility does not work on buildings and bottom parts of turrets.
This program helps create til/tmi files from visible images. There are 4 modes available for this program...

---------------------------------

Mode 1: Create empty til/tmi

Create a blank til/tmi template without any unit invisibility at all.
You need to specify the width and height of the map. Map's width and height need to be 1 ~ 127.
For more explanisions on map sizes, see the attached image.

Note: When outputing, the program asks you for til's file path, but will not ask you for tmi's path.
Tmi file will be created with the same file name of til, and will override existing same-named tmi file without prompting, if that exists.

---------------------------------

Mode 2: Create til/tmi with invisibilities

Convert a bmp to til/tmi.

How to create unit invisibilities:
Create an UNCOMPRESSED, 1 BIT MONORCHROME bmp image with resolution same as the srf of the map.
For original SOW, small maps are 2048 * 2048 and large maps are 3072 * 2560.
If you are using non-standard srf sizes, make the dimensions match 32 * the dimensions of the map size.
For example, a map with size 80 * 80 would need srf and til/tmi with size 2560 * 2560.
Make sure the dimensions of the bmp picture are multiples of 32, and no larger than 4064.
Then fill in the pixels you want units to be invisible. All the visible pixels should be white.
(You can create the image with any drawing software you want, then open the picture in Microsoft Piant and export it as "Monochrome Bitmap")
(Paint should be able to convert all kinds of rgba or grayscale images)
(Just don't use fancy brushes because the edges will be messed up when converting to monochrome)

For exmaple:
Open the background picture of the map with image editors that support layers. Create a transparent layer for the masking.
Use pencil or magic wand to circle around objects that will cover units/turrets, and fill in whatever color.
When done, export the masking layer to a png and open it with Microsoft Paint.
Export the image as "Monochrome bmp".
And then you can use it in this program.
Be sure to backup the original til/tmi.

Note: Same as mode 1, it does not ask you for tmi's file path and straight overrides existing tmi file.

---------------------------------

Mode 3: Reverse conversion

Convert til/tmi to bmp.
Just select the til and tmi file and start conversion.
It is recommended to convert the output image to other formats (like png) before editing.

Note: The program does not check til/tmi integrity, it only checks if the tmi matches the til.
If you feed in broken til/tmi and they somehow happened to match, the program will still run but produce garbage.

---------------------------------

Mode 4: Create empty map

Create a blank .map file with specific size.
Note that the edges of the map are all unusable.
See attached image for details on map sizes.

---------------------------------

If you have any questions ask me at discord!

Source repository <github.com/State-of-War-PostBar/Tilmi>
Made by Taxerap <github.com/Taxerap>
Thank NafMaxYang for bmp examples and testing
Thank -RedImitate.Foul- for Windows 11 & Windows 7 testing
