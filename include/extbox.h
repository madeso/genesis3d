#pragma once

#include "vec3d.h"

/// Axial aligned bounding box (extent box)
struct ExtBox
{
	vec3f Min;
	vec3f Max;

	/// Set the values in a box
	void Set(float X1, float Y1, float Z1,
			 float X2, float Y2, float Z2);

	/// Test a box for validity ( non NULL and max >= min )
	bool IsValid() const;

	/// Set box Min and Max to the passed point
	void SetToPoint(const vec3f *Point);

	/// Extend a box to encompass the passed point
	void ExtendToEnclose(const vec3f *Point);

	/// Return result of box intersection.
	/// If no intersection, returns false and bResult is not modified.
	/// If intersection, returns true and fills bResult (if not NULL)
	/// with the intersected box,
	/// bResult may be one of b1 or b2.
	bool Intersection(const ExtBox *B2, ExtBox *Result) const;

	/// computes union of b1 and b2 and returns in bResult.
	void Union(const ExtBox *B2, ExtBox *Result) const;

	bool ContainsPoint(const vec3f *Point) const;

	void GetTranslation(vec3f *pCenter) const;
	void SetTranslation(const vec3f *pCenter);
	void Translate(float DX, float DY, float DZ);

	void GetScaling(vec3f *pScale) const;
	void SetScaling(const vec3f *pScale);
	void Scale(float DX, float DY, float DZ);

	///  Creates a box that encloses the entire area of a box that moves along linear path
	void LinearSweep(const vec3f *StartPoint, const vec3f *EndPoint, ExtBox *EnclosingBox) const;

	/// Collides a ray with box B.  The ray is directed, from Start to End.
	///   Only returns a ray hitting the outside of the box.
	///     on success, true is returned, and
	///       if T is non-NULL, T is returned as 0..1 where 0 is a collision at Start, and 1 is a collision at End
	///       if Normal is non-NULL, Normal is the surface normal of the box where the collision occured.
	bool RayCollision(const vec3f *Start, const vec3f *End, float *T, vec3f *Normal) const;
};
