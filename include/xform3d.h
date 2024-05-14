#pragma once

#include "vec3d.h"

/**3D transform interface.
  This is essentially a 'standard' 4x4 transform matrix, with the bottom row always 0,0,0,1.
	```
	| AX, AY, AZ, Translation.X |
	| BX, BY, BZ, Translation.Y |
	| CX, CY, CZ, Translation.Z |
	|  0,  0,  0,      1        |
	```
*/
struct XForm
{
	float AX, AY, AZ;  // e[0][0],e[0][1],e[0][2]
	float BX, BY, BZ;  // e[1][0],e[1][1],e[1][2]
	float CX, CY, CZ;  // e[2][0],e[2][1],e[2][2]
	vec3f Translation; // e[0][3],e[1][3],e[2][3]
					   //	  0,0,0,1		// e[3][0],e[3][1],e[3][2]

	/// copies Src to Dst.
	void Copy(XForm *Dst) const;

	/// returns true if M is 'valid'
	/// 'valid' means that M is non NULL, and there are no NAN's in the matrix.
	bool IsValid() const;

	/// returns true if M is orthonormal
	/// (if the rows and columns are all normalized (transform has no scaling or shearing)
	/// and is orthogonal (row1 cross row2 = row3 & col1 cross col2 = col3)
	/// * does not check for right-handed convention *
	bool IsOrthonormal() const;

	/// returns true if M is orthogonal
	/// (row1 cross row2 = row3 & col1 cross col2 = col3)
	/// * does not check for right-handed convention *
	bool IsOrthogonal() const;

	/// essentially removes scaling (or other distortions) from
	/// an orthogonal (or nearly orthogonal) matrix
	/// returns a right-handed matrix
	void Orthonormalize();

	/// sets M to an identity matrix (clears it)
	///  Results in a right-handed transform.
	void SetIdentity();

	/// sets up a transform that rotates RadianAngle about X axis
	/// all existing contents of M are replaced.
	///  Results in a right-handed transform.
	void SetXRotation(float RadianAngle);

	/// sets up a transform that rotates RadianAngle about Y axis
	/// all existing contents of M are replaced.
	///  Results in a right-handed transform.
	void SetYRotation(float RadianAngle);

	/// sets up a transform that rotates RadianAngle about Z axis
	/// all existing contents of M are replaced.
	///  Results in a right-handed transform.
	void SetZRotation(float RadianAngle);

	/// sets up a transform that translates x,y,z
	/// all existing contents of M are replaced.
	///  Results in a right-handed transform.
	void SetTranslation(float x, float y, float z);

	/// sets up a transform that scales by x,y,z
	/// all existing contents of M are replaced.
	///  Results in a right-handed transform.
	void SetScaling(float x, float y, float z);

	/// Rotates M by RadianAngle about X axis
	/// applies the rotation to the existing contents of M
	void RotateX(float RadianAngle);

	/// Rotates M by RadianAngle about Y axis
	/// applies the rotation to the existing contents of M
	void RotateY(float RadianAngle);

	/// Rotates M by RadianAngle about Z axis
	/// applies the rotation to the existing contents of M
	void RotateZ(float RadianAngle);

	/// Translates M by x,y,z
	/// applies the translation to the existing contents of M
	void Translate(float x, float y, float z);

	/// Scales M by x,y,z
	/// applies the scale to the existing contents of M
	void Scale(float x, float y, float z);

	/// MProduct = matrix multiply of M1*M2
	/// Concatenates the transformation in the M2 matrix onto the transformation in M1
	void Multiply(const XForm *M2, XForm *MProduct) const;

	/// Result is Matrix M * Vector V:  V Tranformed by M
	void Transform(const vec3f *V, vec3f *Result) const;

	void TransformArray(const vec3f *Source, vec3f *Dest, int Count) const;

	/// Result is Matrix M * Vector V:  V Rotated by M (no translation)
	void Rotate(const vec3f *V, vec3f *Result) const;

	/// Gets a vector that is 'left' in the frame of reference of M (facing -Z)
	/**
		"Left,Up,In" are just the basis vectors in the new coordinate space.
		You can get them by multiplying the unit bases into the transforms.
	*/

	void GetLeft(vec3f *Left) const;

	/// Gets a vector that is 'up' in the frame of reference of M (facing -Z)
	/**
		"Left,Up,In" are just the basis vectors in the new coordinate space.
		You can get them by multiplying the unit bases into the transforms.
	*/

	void GetUp(vec3f *Up) const;

	/// Gets a vector that is 'in' in the frame of reference of M (facing -Z)
	/**
		"Left,Up,In" are just the basis vectors in the new coordinate space.
		You can get them by multiplying the unit bases into the transforms.
	*/

	void GetIn(vec3f *In) const;

	/// Gets the Transpose transform of M   (M^T)
	/// Transpose of a matrix is the switch of the rows and columns
	/// The transpose is usefull because it is rapidly computed and is equal to the inverse
	/// transform for orthonormal transforms    [inverse is (M')  where M*M' = Identity ]
	void GetTranspose(XForm *MTranspose) const;

	/// applies the transpose transform of M to V.  Result = (M^T) * V
	void TransposeTransform(const vec3f *V, vec3f *Result) const;

	/// Finds Euler angles from M and puts them into Angles
	/**
		the Euler angles are subsequent rotations :
			by Angles->Z around the Z axis
			then by Angles->Y around the Y axis, in the newly rotate coordinates
			then by Angles->X around the X axis
	*/
	void GetEulerAngles(vec3f *Angles) const;

	/// Applies Euler angles to build M
	/**
		the Euler angles are subsequent rotations :
			by Angles->Z around the Z axis
			then by Angles->Y around the Y axis, in the newly rotate coordinates
			then by Angles->X around the X axis
	*/
	///  Results in a right-handed transform.
	void SetEulerAngles(const vec3f *Angles);

	/// Builds an XForm from orthonormal Left, Up and In vectors
	///  Results in a right-handed transform.
	void SetFromLeftUpIn(const vec3f *Left, const vec3f *Up, const vec3f *In);

	/// Mirrors a XForm3d about a plane
	void Mirror(const vec3f *PlaneNormal, float PlaneDist, XForm *Dest) const;
};

#define GEXFORM3D_MINIMUM_SCALE (0.00001f)
