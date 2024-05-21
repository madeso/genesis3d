#pragma once

/// Motion interface.
///	This object is a list of named Path objects
struct Motion
{
	Motion(bool ManageNames);

	Path *GetPath(int Index) const;
	int GetPathCount() const;

	bool SetName(std::string Name);
	std::string GetName() const;

	/// gets time of first key and time of last key (as if motion did not loop)
	/// if there are no paths in the motion: returns false and times are not set
	/// otherwise returns true
	///
	/// For a compound motion, GetTimeExtents will return the extents of the scaled submotions.
	/// For a single motion, no scaling is applied.
	bool GetTimeExtents(float *StartTime, float *EndTime) const;

	/// Inserts the new event and corresponding string.
	/// Only one event is allowed per time key.
	bool InsertEvent(float tKey, std::string String);

	/// Deletes the event
	bool DeleteEvent(float tKey);

	/// For searching or querying the array for events between two times
	/// times are compaired [StartTime,EndTime), '[' is inclusive, ')' is
	/// non-inclusive.  This prepares the GetNextEvent() function.
	/// @param StartTime Inclusive search start
	/// @param EndTime Non-inclusive search stop
	void SetupEventIterator(float StartTime, float EndTime);

	/// Iterates from StartTime to EndTime as setup in SetupEventIterator()
	/// and for each event between these times [StartTime,EndTime)
	/// this function will return Time and EventString returned for that event
	/// and the iterator will be positioned for the next search.  When there
	/// are no more events in the range, this function will return false (Time
	/// will be 0 and ppEventString will be empty).
	/// @param pTime Return time, if found
	/// @param ppEventString Return data, if found
	bool GetNextEvent(float *pTime, std::string *ppEventString);

	/// returns the time associated with the first and last events
	/// returns false if there are no events (and Times are not set)
	bool GetEventExtents(float *FirstEventTime, float *LastEventTime) const;
};
