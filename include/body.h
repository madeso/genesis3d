#pragma once

#include "xform3d.h"
#include "vfile.h"
#include "bitmap.h"

#define GE_BODY_NUMBER_OF_LOD (4) // 0 is highest detail
#define GE_BODY_NO_PARENT_BONE (-1)
#define GE_BODY_HIGHEST_LOD (0)

#define GE_BODY_ROOT (-1) // for specifying 'root' bounding box.

/// Actor body interface.
/**
 This object is for managing the data associated with a skeletal-based mesh, a 'body'.
 This object holds the geometry for the body and the list of materials needed.
*/
struct Body
{
	Body();
	~Body();
	Body(File *pFile);

	bool IsValid() const;

	bool GetGeometryStats(int lod, int *Vertices, int *Faces, int *Normals) const;

	bool AddFace(const vec3f *Vertex1, const vec3f *Normal1, float U1, float V1, int BoneIndex1, const vec3f *Vertex2, const vec3f *Normal2, float U2, float V2, int BoneIndex2, const vec3f *Vertex3, const vec3f *Normal3, float U3, float V3, int BoneIndex3, int MaterialIndex);

	/// Bitmap is added to body.  It's reference count is increased.  Caller still owns a pointer
	/// to the bitmap, and is responsible for destroying it.
	bool AddMaterial(std::string MaterialName, Bitmap *Bitmap, float Red, float Green, float Blue, int *MaterialIndex);

	/// returned bitmap is a pointer to the bitmap in the body's list.  It may not be destroyed.
	/// if caller would like to 'own' a copy of that bitmap pointer, it should call geBitmap_CreateRef()
	bool GetMaterial(int MaterialIndex, std::string *MaterialName, Bitmap **Bitmap, float *Red, float *Green, float *Blue) const;

	/// Bitmap is set into the body.  It's reference count is increased.  Caller still owns a pointer
	/// to the bitmap, and is responsible for destroying it.
	bool SetMaterial(int MaterialIndex, Bitmap *Bitmap, float Red, float Green, float Blue);

	int GetMaterialCount() const;

	bool AddBone(int ParentBoneIndex, std::string BoneName, const XForm *AttachmentMatrix, int *BoneIndex);

	bool ComputeLevelsOfDetail(int Levels);

	int GetBoneCount() const;

	void GetBone(int BoneIndex, std::string *BoneName, XForm *Attachment, int *ParentBoneIndex) const;

	int32 GetBoneNameChecksum() const;

	/// @param BoneIndex use GE_BODY_ROOT for specifing 'root' bounding box.
	void SetBoundingBox(int BoneIndex, const vec3f *MinimumBoxCorner, const vec3f *MaximumBoxCorner);

	/// @param BoneIndex use GE_BODY_ROOT for specifing 'root' bounding box.
	bool GetBoundingBox(int BoneIndex, vec3f *MinimumBoxCorner, vec3f *MaximumBoxCorner) const;

	bool GetBoneByName(std::string BoneName, int *pBoneIndex, XForm *Attachment, int *pParentBoneIndex) const;

	bool WriteToFile(File *pFile) const;
};
