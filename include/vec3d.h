#pragma once

struct vec3f
{
	float X, Y, Z;

	float GetElement(int Index);
	void Set(float X, float Y, float Z);
	void Get(float *X, float *Y, float *Z) const;
	float DotProduct(const vec3f *V2) const;
	void CrossProduct(const vec3f *V2, vec3f *VResult) const;
	bool Compare(const vec3f *V2, float tolarance) const;
	float Normalize();
	bool IsNormalized() const;
	void Scale(float Scale, vec3f *VDst) const;
	float Length() const;
	float LengthSquared() const;
	void Subtract(const vec3f *V2, vec3f *V1MinusV2) const;
	void Add(const vec3f *V2, vec3f *VSum) const;
	void Clear();
	void Inverse();
	void MA(float Scale, const vec3f *V2, vec3f *V1PlusV2Scaled);
	void AddScaled(const vec3f *V2, float Scale, vec3f *V1PlusV2Scaled) const;
	float DistanceBetween(const vec3f *V2); // returns length of V1-V const2
	bool IsValid() const;
};
