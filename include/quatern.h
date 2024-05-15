#pragma once

#include "xform3d.h"

/// Quaternion mathematical system interface.
/**
The quatern module contains basic support for a quaternion object.

quaternions are an extension of complex numbers that allows an
expression for rotation that can be easily interpolated.  s are also
more numericaly stable for repeated rotations than matrices.


A quaternion is a 4 element 'vector'  `[w,x,y,z]` where:

```
q = w + xi + yj + zk
i*i = -1
j*j = -1
k*k = -1
i*j = -j*i = k
j*k = -k*j = i
k*i = -i*k = j
q' (conjugate) = w - xi - yj - zk
||q|| (magnitude) = sqrt(q*q') = sqrt(w*w + x*x + y*y + z*z)
unit quaternion ||q|| == 1; this implies  q' == qinverse
quaternions are associative (q1*q2)*q3 == q1*(q2*q3)
quaternions are not commutative  q1*q2 != q2*q1
qinverse (inverse (1/q) ) = q'/(q*q')
```

`q` can be expressed by `w + xi + yj + zk` or `[w,x,y,z]`
or as in this implementation `(s,v)` where `s=w`, and `v=[x,y,z]`

quaternions can represent a rotation.  The rotation is an angle `t`, around a
unit vector `u`.   `q=(s,v);  s= cos(t/2);   v= u*sin(t/2)`.

quaternions can apply the rotation to a point.  let the point be `p [px,py,pz]`,
and let `P` be a quaternion(`0,p`).  Protated = `q*P*qinverse`
( Protated = `q*P*q'` if `q` is a unit quaternion)

concatenation rotations is similar to matrix concatenation.  given two rotations
`q1` and `q2`,  to rotate by `q1`, then `q2`:  let `qc = (q2*q1)`, then the combined
rotation is given by `qc*P*qcinverse` (= `qc*P*qc'` if `q` is a unit quaternion)

multiplication:
```q1 = w1 + x1i + y1j + z1k
q2 = w2 + x2i + y2j + z2k
q1*q2 = q3 =
		(w1*w2 - x1*x2 - y1*y2 - z1*z2)     {w3}
		(w1*x2 + x1*w2 + y1*z2 - z1*y2)i	{x3}
		(w1*y2 - x1*z2 + y1*w2 + z1*x2)j    {y3}
		(w1*z2 + x1*y2 + y1*x2 + z1*w2)k	{z3}```

also,
```q1 = (s1,v1) = [s1,(x1,y1,z1)]
q2 = (s2,v2) = [s2,(x2,y2,z2)]
q1*q2 = q3	=	(s1*s2 - dot_product(v1,v2),			{s3}
				(s1*v2 + s2*v1 + cross_product(v1,v2))	{v3}```


interpolation - it is possible (and sometimes reasonable) to interpolate between
two quaternions by interpolating each component.  This does not quarantee a
resulting unit quaternion, and will result in an animation with non-linear
rotational velocity.

spherical interpolation: (slerp) treat the quaternions as vectors
find the angle between them (`w = arccos(q1 dot q2) `).
given `0<=t<=1,  q(t) = q1*(sin((1-t)*w)/sin(w) + q2 * sin(t*w)/sin(w)`.
since `q == -q`, care must be taken to rotate the proper way.

this implementation uses the notation `quaternion q = (quatS,quatV)`
	where `quatS` is a scalar, and `quatV` is a 3 element vector.
*/
struct Quat
{
	float W;
	float X, Y, Z;

	/// return true if Q is non null and for has no NAN's in its components
	bool IsValid() const;

	/// set quaternion components.  Doesn't normalize
	void Set(float W, float X, float Y, float Z);

	/// set quaternion components.  Doesn't normalize
	void SetVec3d(float W, const vec3f *V);

	/// set a quaternion from an axis and a rotation around the axis
	void SetFromAxisAngle(const vec3f *Axis, float Theta);

	/// gets an axis and angle of rotation around the axis from a quaternion
	/// returns true if there is an axis.
	/// returns false if there is no axis (and Axis is set to 0,0,0, and Theta is 0)
	bool GetAxisAngle(vec3f *Axis, float *Theta) const;

	/// get quaternion components into W,X,Y,Z
	void Get(float *W, float *X, float *Y, float *Z) const;

	/// get quaternion components into W and V
	void GetVec3d(float *W, vec3f *V) const;

	/// takes upper 3 by 3 portion of matrix (rotation sub matrix)
	/// and generates a quaternion
	static void FromMatrix(const XForm *RotationMatrix, Quat *QDest);

	/// takes a unit quaternion and makes RotationMatrixDest an equivelant rotation xform.
	/// (any translation in RotationMatrixDest will be list)
	void ToMatrix(XForm *RotationMatrixDest) const;

	/// spherical interpolation between q0 and q1.   0<=t<=1
	/// resulting quaternion is 'between' q0 and q1
	/// with t==0 being all q0, and t==1 being all q1.
	/// returns a quaternion with a positive W - always takes shortest route
	/// through the positive W domain.
	static void Slerp(const Quat *Q0, const Quat *Q1, float T, Quat *QT);

	/// spherical interpolation between q0 and q1.   0<=t<=1
	/// resulting quaternion is 'between' q0 and q1
	/// with t==0 being all q0, and t==1 being all q1.
	static void SlerpNotShortest(const Quat *Q0, const Quat *Q1, float T, Quat *QT);

	/// multiplies q1 * q2, and places the result in q.
	/// no failure. 	renormalization not automatic
	void Multiply(const Quat *Q2, Quat *QProduct) const;

	/// Rotates V by the quaternion Q, places the result in VRotated.
	void Rotate(const vec3f *V, vec3f *VRotated) const;

	/// returns true if q is a unit quaternion.  false otherwise.
	bool IsUnit() const;

	/// normalizes q to be a unit quaternion.  returns original magnitude of q
	float Normalize();

	/// copies quaternion QSrc into QDst
	void Copy(Quat *QDst) const;

	/// sets Q to be a quaternion with no rotation (like an identity matrix)
	void SetNoRotation();

	/// ln(Q) for unit quaternion only!
	void Ln(Quat *LnQ) const;

	/// exp(Q) for pure quaternion only!  (zero scalar part (W))
	void Exp(Quat *ExpQ) const;

	/// Q = Q * Scale  (result is not generally a unit quaternion!)
	void Scale(float Scale, Quat *QScaled) const;

	/// QSum = Q1 + Q2  (result is not generally a unit quaternion!)
	void Add(const Quat *Q2, Quat *QSum) const;

	/// QSum = Q1 - Q2  (result is not generally a unit quaternion!)
	void Subtract(const Quat *Q2, Quat *QSum) const;

	/// sets QInv to the inverse of Q.
	void Inverse(Quat *QInv) const;

	/// returns Magnitude of Q.
	float Magnitude() const;

	/// return true if quaternions differ elementwise by less than Tolerance.
	bool Compare(Quat *Q2, float Tolerance);
};
