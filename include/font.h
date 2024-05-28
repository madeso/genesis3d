#pragma once

#include "genesis.h"
#include "bitmap.h"

/// **************************
/// to use this API:

/// 2.  CreateFont().

/// 3.  (Optionally) AddCharacters().
/// 3A. Otherwise, IF you intend to use DrawText(), call AddBitmapBuffer().

/// 4.  Between Engine_BeginFrame() and Engine_EndFrame(), and after Engine_RenderWorld(),
///       DrawText(). You may call DrawTextToBitmap() anytime, though.

/// 5.  When finished, Destroy().

/// these are bit flags for Font::DrawText(). Currently only _WORDWRAP is implemented, and without
/// it, the function will still wrap, just not on word boundaries.
/// Note that these will fail for non ascii fonts.
enum class FontFlag
{
    /// wrap to fit inside the drawing rect
    WRAP = 0x00000001,
    /// wrap on word boundaries
    WORDWRAP = 0x00000002,
    /// returns number of characters that fit in drawing rectangle, WITHOUT drawing anything.
    JUST_RETURN_FIT = 0x00000004,
    JUSTIFY_RIGHT = 0x00000008,
    JUSTIFY_CENTER = 0x00000010,
};

/// An instance of a font.
/// Bitmapped font support interface.
/// This implementation supports any TrueType fonts provided by Windows
struct Font
{
    //***************************************************************************************
    /// Creates a font, and returns a pointer to it.
    /// Pass in the string name of the TrueType font (case sensitive), and the height in pixels.
    /// The new font set has NO actual characters in it at first.  You must add characters to it with the AddCharacters() function before you can use the font.
    /// All fonts start out with a grayscale palette, with the range 0 to 128.
    /// @param fontNameString - char pointer to a string containing the case sensitive name of the font.
    /// @param fontSize - the pixel height of the requested font.
    /// @return pointer to the newly created font if successful, NULL if not.
    static Font *Create(std::string fontNameString, const int fontSize, const int fontWeight, const bool antialiased);

    void CreateRef();

    /// Destroys a font.
    ~Font();

    //***************************************************************************************
    /// Adds a set of characters to the font defined by the ascii range passed in
    /// (leastIndex and mostIndex, inclusive).
    /// This is the function that actually uses the
    /// Win32 GetGlyphOutline() function to draw the character onto a Bitmap, which can be
    /// blitted to the screen.
    /// @param font - pointer to the font to add characters to.
    /// @param e - pointer to a valid Engine.
    /// @param leastIndex and mostIndex - the ASCII range of characters to add.
    /// @param cellBuffer - an allocated hunk of ram to temproarily store the character image
    /// @param bufferSize - length of the above buffer
    /// @return true if success, false if  failure
    bool AddCharacters(unsigned char leastIndex, unsigned char mostIndex);

    /// destroys any valid "scratch-pad" buffer attached to the Font.
    /// you'll rarely need to call this function; it's called by Destroy() anyway.
    /// Calling this function with a Font that has no initialized buffer doesn't
    /// hurt anything.
    /// @param font - pointer to the Font.
    void DestroyBitmapBuffer();

    /// Adds a Bitmap to the Font, to be used as a temporary "scratch-pad".  This is
    /// required for using DrawText() when no characters have been added.
    /// You don't need to call this function IF you AddCharacters() to this Font.
    /// You call this function for each Font you need to use.  Font's don't share buffers.
    /// if you call this function on a Font that already has a valid buffer, the buffer is
    /// destroyed, and replaced by the new one.
    /// @param font - pointer to the Font to add a buffer to.
    /// @param width and height - the size of the buffer to create.  Make sure this size is >= /// the biggest rectagle of text you'll want to write to the screen using this Font and DrawText().
    /// @return true if success, false if  failure
    bool AddBitmapBuffer(const uint32 width, const uint32 height);

    /// This is the function you put between Engine::BeginFrame() and Engine::EndFrame(), the function that draws text to the screen.
    /// IF the font has NO characters in it (added by AddCharacters() ) then a different, more windows-intensive way is used to draw out the characters.
    /// Assuming you've added characters to the font, characters which have NOT been added
    /// WILL cause an assert if you try to draw them.
    /// Only GE_FONTSET_WORDWRAP is meaningfull right now.  Using any other flags will cause
    /// an assert.
    /// As stated above, you can use an entirely different way of creating a string, by
    /// making a font with no characters in it.  This
    /// jumps through Windows DIB hoops, and draws the text in a non-anti-aliased, but
    /// (hopefully) more unicode-tolerant way (DrawText() ).
    /// @param textString - pointer to the text string to output to the screen.
    /// @param rect - screen rectangle to place the text within.
    /// @param Color - RGB color the text should be.
    /// @param flags - a bitfield of GE_FONT_ values.
    /// @param clipRect - pointer to a screen rectangle to clip the text to.  MAY BE NULL, in which  case the text is only clipped by the boundaries of the screen.
    /// @return true if success, false if  failure
    bool DrawText(std::string textString, const Rect *rect, const Rgba *Color, FontFlag flags, const Rect *clipRect);

    /// This is the function you put between Engine::BeginFrame() and Engine::EndFrame(), the function that draws text to the screen.
    /// IF the font has NO characters in it (added by AddCharacters() ) then a different, more windows-intensive way is used to draw out the characters.
    /// Assuming you've added characters to the font, characters which have NOT been added
    /// WILL cause an assert if you try to draw them.
    /// Only GE_FONTSET_WORDWRAP is meaningfull right now.  Using any other flags will cause
    /// an assert.
    /// As stated above, you can use an entirely different way of creating a string, by
    /// making a font with no characters in it.  This
    /// jumps through Windows DIB hoops, and draws the text in a non-anti-aliased, but
    /// (hopefully) more unicode-tolerant way (DrawText() ).
    /// The Color argument is will be used to modify the existing palette of the targetBitmap
    /// passed in.  Therefore, you won't be able to _DrawTextToBitmap() a red piece of text,
    /// then a green piece, then a blue piece.  You'll end up with three lines of blue text.
    /// @param textString - pointer to the text string to output to the screen.
    /// @param rect - screen rectangle to place the text within.
    /// @param Color - RGB color the text should be.
    /// @param flags - a bitfield of GE_FONT_ values.
    /// @param clipRect - pointer to a screen rectangle to clip the text to.  MAY BE NULL, in which case the text is only clipped by the boundaries of the screen.
    /// @param targetBitmap - pointer to a target bitmap to draw the text into.  MAY NOT BE NULL, and MUST BE GE_PIXELFORMAT_8BIT.
    /// @return true if success, false if  failure
    bool DrawTextToBitmap(std::string textString, const Rect *rect, const Rgba *Color, FontFlag flags, const Rect *clipRect, Bitmap *targetBitmap);

    /// These two functions return the pixel width and height of the string passed in.
    /// these two functions assume no text wrapping!
    /// @param textString the text string to output to the screen.
    /// @return success: a positive value in pixels.  IF the text passed in contains characters
    ///          which haven't been added to the font yet, BUT other characters HAVE
    ///          been added, the function asserts.
    ///         failure: -1.
    int32 GetStringPixelWidth(std::string textString);

    /// These two functions return the pixel width and height of the string passed in.
    /// these two functions assume no text wrapping!
    /// @param textString the text string to output to the screen.
    /// @return success: a positive value in pixels.  IF the text passed in contains characters
    ///          which haven't been added to the font yet, BUT other characters HAVE
    ///          been added, the function asserts.
    ///         failure: -1.
    int32 GetStringPixelHeight(std::string textString);

    /// This function returns a pointer to the drawing buffer contained by the font.
    /// @return a valid pointer to a Bitmap, OR NULL, signifying that the buffer wasn't initialized.
    Bitmap *GetBuffer();

    bool GetCharMap(uint8 character, Rect *rect, Bitmap **targetBitmap, int32 *fullWidth, int32 *fullHeight, int32 *offsetX, int32 *offsetY);

    void EnableAntialiasing(const bool anti);
    bool IsAntialiased();
};