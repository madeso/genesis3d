#pragma once

#include "pixelformat.h"
#include "vfile.h"

struct BitmapInfo
{
	int Width;
	int Height;
	int Stride; // stride is in *pixels* ; it is the step to the next line : Stride >= Width
	PixelFormat Format;
	int MinimumMip; //*including* minimumMip == 0 often
	int MaximumMip; //*including* maximumMip == nummips-1
	bool HasColorKey;
	uint32 ColorKey; // meaningless unless HasColorKey ; the ColorKey is a Pixel in Format
	BitmapPalette *Palette;
};

struct BitmapPalette
{
	BitmapPalette *Create(PixelFormat Format, int Size);

	BitmapPalette *CreateCopy(const BitmapPalette *Palette);

	BitmapPalette *CreateFromFile(File *F);

	/// does GetPalette, and if NULL, then
	/// it create an optimal palette for a
	///	non-palettized bitmap
	///	(this is a create, you must destroy later!)
	/// put Slow == TRUE for higher quality & slower
	BitmapPalette *CreateFromBitmap(Bitmap *Bmp, bool Slow);

	bool SortColors(BitmapPalette *P, bool Slower);

	bool CreateRef(BitmapPalette *Palette);

	bool Destroy(BitmapPalette **ppPalette);

	bool WriteToFile(const BitmapPalette *Palette, File *F);

	bool SetFormat(BitmapPalette *Palette, PixelFormat Format);

	bool Copy(const BitmapPalette *Src, BitmapPalette *Target);

	/// get the info as if it were a bitmap; Into->Height == 1
	bool GetInfo(const BitmapPalette *P, BitmapInfo *Into);

	/// pFormat & pSize are optional
	bool Lock(BitmapPalette *Palette, void **pBits, PixelFormat *pFormat, int *pSize);

	/// palette unlock does NOT notify the bitmap that the palette has changed.
	/// call Bitmap_SetPalette() with the same palette pointer
	/// 	to tell the bitmap that it must to some processing
	/// (don't worry, it won't duplicate it or copy it onto itself)
	bool UnLock(BitmapPalette *Palette);

	/// does Lock/UnLock for you
	/// From and Into are arrays of Colors*gePixelFormat_BytesPerPel bytes
	bool GetData(const BitmapPalette *P, void *Into, PixelFormat Format, int Colors);

	/// does Lock/UnLock for you
	/// From and Into are arrays of Colors*gePixelFormat_BytesPerPel bytes
	bool SetData(BitmapPalette *P, const void *From, PixelFormat Format, int Colors);

	/// Set does Lock/Unlock for you ; these are slow! do not use these to work on all the colors!
	bool SetEntryColor(BitmapPalette *P, int Color, int R, int G, int B, int A);

	/// Get does Lock/Unlock for you ; these are slow! do not use these to work on all the colors!
	bool GetEntryColor(const BitmapPalette *P, int Color, int *R, int *G, int *B, int *A);

	bool SetEntry(BitmapPalette *P, int Color, uint32 Pixel);
	bool GetEntry(const BitmapPalette *P, int Color, uint32 *Pixel);
};

/**
 *
 * Before you can use a geBitmap in any module, you must add the geBitmap to that module first.  For example, if you were planning on using geEngine_DrawBitmap, you must first add the geBitmap to the geEngine module (see geEngine_AddBitmap).   Please note that adding a geBitmap to a geEngine module, does not allow you to use the geBitmap in other modules.  You must add the geBitmap to each and every module that you plan on using the geBitmap with.  Also note that you cannot add a geBitmap to a geEngine, while in the middle of a drawing frame (in between calls to geEngine_BeginFrame, and geEngine_EndFrame)
 *
 * if Bitmap is a lock for read, functions that modify it return failure
 * if Bitmap is a lock for write, functions that modify it attempt to
 *	modify the owner of the lock
 *
 * warning : if you lock multiple mips for write, and then modify one of the mips
 *		(such as via SetPalette) it may affect the owner and all sibling mips!
 *		doing different SetPalettes with different palettes on different locked mips
 *		has undefined behavior!
 *
 **/
struct Bitmap
{
	static Bitmap *Create(int Width, int Height, int MipCount, PixelFormat Format);
	void CreateRef();

	static Bitmap *CreateFromInfo(const BitmapInfo *pInfo);

	static Bitmap *CreateFromFile(File *F);
	static Bitmap *CreateFromFileName(const File *BaseFS, std::string Name);
	bool WriteToFile(File *F) const;

	/// BaseFS is not really const if it is a virtual file;
	///  it *is* const if it is a dos directory
	bool WriteToFileName(const File *BaseFS, std::string Name) const;

	/// returns whether Bmp was actually destroyed : not success/failure
	bool Destroy();

	/// LockForWrite returns data in Info's format
	bool GetInfo(BitmapInfo *Info, BitmapInfo *SecondaryInfo) const;

	bool Blit(int SrcPositionX, int SrcPositionY, Bitmap *Dst, int DstPositionX, int DstPositionY, int SizeX, int SizeY) const;

	/// don't use this with Src == Dst, use UpdateMips instead !
	bool BlitMip(int SrcMip, Bitmap *Dst, int DstMip) const;

	bool BlitBitmap(Bitmap *Dst) const;

	/// blits the largest mip from Src that fits in Dst
	bool BlitBestMip(Bitmap *Dst) const;

	/// a non-exclusive lock
	/// not really const, stores lock-count, but *data* is const
	/// will do a format conversion!
	bool LockForRead(Bitmap **Target, int MinimumMip, int MaximumMip, PixelFormat Format, bool RespectColorKey, uint32 ColorKey) const;

	/// lock for read in a format that gaurantee no conversions
	/// then do GetInfo on the locks to see what you have!
	bool LockForReadNative(Bitmap **Target, int MinimumMip, int MaximumMip) const;

	/// an exclusive lock
	bool LockForWrite(Bitmap **Target, int MinimumMip, int MaximumMip);

	/// Format must be one of the two returned in GetInfo !!
	bool LockForWriteFormat(Bitmap **Target, int MinimumMip, int MaximumMip, PixelFormat Format);

	/// must be done on All locked mips
	bool UnLock();

	/// _SetFormat may cause you to lose color information!
	/// SetFormat does a conversion!
	/// if NewFormat is palettized and Palette is NULL, we create a palette for the bitmap!
	bool SetFormat(PixelFormat NewFormat, bool RespectColorKey, uint32 ColorKey, const BitmapPalette *Palette);

	/// the Min version keeps colorkey & palette from the old format
	bool SetFormatMin(PixelFormat NewFormat);

	/// SetColorKey discards old colorkey information!
	///	does not do a conversion (changes the colorkey in the current data
	/// if 'Smart' is on, we don't set HasColorKey to true unless it is actually used!
	bool SetColorKey(bool HasColorKey, uint32 ColorKey, bool Smart);

	/// tells you the average color; computes it and caches it out
	bool GetAverageColor(int *pR, int *pG, int *pB) const;

	BitmapPalette *GetPalette(const Bitmap *Bmp);

	/// _SetPal tries to _CreateRef your Palette, so no copy occurs & palettes may be shared
	/// you may _Destroy() palette after using it to set (though its bits may not be freed)
	///	(hence Palette is *not* const)
	/// Warning : SetPalette on any mip changes the palette of ALL mips !
	/// see Palette note at _UnLock
	/// _SetPal destroys the bitmap's original palette and refs the new one,
	/// so if you setpal with the bitmap's palette, there is no net change in ref counts (good!)
	bool SetPalette(const BitmapPalette *Palette);

	/// returns true if bitmap has *any* type of alpha
	bool HasAlpha() const;

	Bitmap *GetAlpha() const;

	/// we Ref the AlphaBmp, so you may destroy it after calling Set()
	/// it may be NULL
	/// there's only one Alpha per bitmap (for the top Mip) right now
	bool SetAlpha(const Bitmap *AlphaBmp);

	/// this Gamma does not change the *original* (system/secondary) bits
	///	it only affects the appearance when drawn
	/// note : if you write to the gamma corrected bits, you must gamma correct manually if you
	///	wish to fit in smoothly with the previous data
	/// warning : if you use this function with many different gammas, performance will suffer!
	///	use one global gamma for all bitmaps!  try to let the engine manage gamma for you,
	///	via Engine_SetGamma !
	bool SetGammaCorrection(float Gamma, bool Apply);

	bool SetPreferredFormat(Bitmap *Bmp, PixelFormat Format);
	PixelFormat GetPreferredFormat() const;

	/// works only on a Lock()
	void *GetBits();

	/// rebuilds mips; *tries* to be smart & not overwrite manually-fixed mips
	/// RefreshMips does *not* build mips that don't exist
	bool RefreshMips();

	/// will create the target if it doesn't exist;
	/// will overwrite manually-fixed mips!
	bool UpdateMips(int SourceMip, int TargetMip);

	/// creates or destroys to match the new count
	bool SetMipCount(int Count);

	/// Destroy all mips (except the first)! use with care! this is not polite!
	bool ClearMips();

	/// Shortcuts
	int Width() const;
	int Height() const;
	uint32 MipBytes(int mip) const;
};
