#pragma once

#include "xform3d.h"
#include "quatern.h"
#include "vfile.h"

enum ChannelMask
{
	ROTATION_CHANNEL = 1,
	TRANSLATION_CHANNEL = 2,
	ALL_CHANNELS = PATH_ROTATION_CHANNEL | PATH_TRANSLATION_CHANNEL,
};

enum class PathInterpolation
{
	// linear blend for translation or rotation channel
	LINEAR = 0,
	// hermite cubic spline for translation channel
	HERMITE,
	// spherical-linear blend for rotation channel
	SLERP,
	// higher order blend for rotation channel 'G1' continuity
	SQUAD,
	// not supported yet.
	GE_PATH_INTEROPLATE_TRIPOD,
	// hermite cubic with zero derivative at keyframes ('easing' curve)
	HERMITE_ZERO_DERIV = 7
};

/// Time-indexed keyframe creation, maintenance, and sampling.
struct Path
{

	/// Create a ref
	Path(Path *P);

	/// creates new Path
	///  A looping path should have the same first & last point.  The path
	///  generator will choose arbitrarily between these points for a
	///  sample exactly at the end of the loop.
	/// @param TranslationInterpolation type of interpolation for translation channel
	/// @param RotationInterpolation type of interpolation for rotation channel
	/// @param Looped True if end of path is connected to head
	Path(PathInterpolation TranslationInterpolation, PathInterpolation RotationInterpolation, bool Looped);

	/// Create a copy
	Path(const Path *P);

	/// loads a file  (binary or ascii)
	Path(File *F);

	/// dumps formatted ascii to the file.
	bool WriteToFile(File *F) const;

	/// dumps a minimal binary image for fastest reading
	bool WriteToBinaryFile(File *F) const;

	/// inserts a keyframe at a specific time.
	bool InsertKeyframe(ChannelMask channelMask, float Time, const XForm *Matrix);

	/// deletes the nth keyframe
	bool DeleteKeyframe(int Index, ChannelMask channelMask);

	/// gets the time for the first and last keys in the path (ignoring looping)
	/// if there are no keys, return false and times are not set.
	/// returns true if there are keys.
	bool GetTimeExtents(float *StartTime, float *EndTime) const;

	/// retrieves keyframe[index], and it's time
	/// @param Index gets keyframe[index]
	/// @param Channel for this channel
	/// @param Time returns the time of the keyframe
	/// @param Matrix returns the matrix of the keyframe
	void GetKeyframe(int Index, ChannelMask Channel, float *Time, XForm *Matrix) const;

	/// retrieves count of keyframes for a specific channel
	int GetKeyframeCount(ChannelMask Channel) const;

	/// retrieves the index of the keyframe at a specific time for a specific channel
	int GetKeyframeIndex(ChannelMask Channel, float Time) const;

	/// sampling a path  (time based)
	/// returns a transform matrix sampled at 'Time'.
	/// p is not const because information is cached in p for next sample
	XForm Sample(float Time) const;

protected:
	/// returns a rotation and a translation for the path at 'Time'
	/// p is not const because information is cached in p for next sample
	void SampleChannels(float Time, Quat *Rotation, vec3f *Translation) const;

	/// slides all samples in path starting with StartingIndex down by TimeOffset
	bool OffsetTimes(int StartingIndex, ChannelMask channelMask, float TimeOffset);

	bool ModifyKeyframe(int Index, ChannelMask channelMask, const XForm *Matrix);
};
