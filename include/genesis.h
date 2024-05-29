#pragma once

// The master header for Genesis

#include <string>

#include "vec3d.h"
#include "xform3d.h"
#include "getypes.h"
#include "extbox.h"
#include "vfile.h"
#include "bitmap.h"
#include "actor.h"
#include "path.h"
#include "motion.h"

using HWND = int;

struct Engine
{
	Engine(HWND hWnd, std::string AppName, std::string DriverDirectory);
	~Engine();

	bool AddWorld(World *World);
	bool RemoveWorld(World *World);
	bool AddBitmap(Bitmap *Bitmap);
	bool RemoveBitmap(Bitmap *Bitmap);
	DriverSystem *GetDriverSystem();
	bool SetDriverAndMode(Driver *Driver, DriverMode *DriverMode);
	bool ShutdownDriver();
	bool BeginFrame(Camera *Camera, bool ClearScreen);
	bool EndFrame();
	bool RenderWorld(World *World, Camera *Camera, float Time);
	bool Printf(int32 x, int32 y, std::string String, ...);

	/// RenderPoly : if Texture is null, we Gouraud shade
	void RenderPoly(const TransformedLitVertex *Points, int NumPoints, const Bitmap *Texture, Poly_Type Flags) const;
	void RenderPolyArray(const TransformedLitVertex **pPoints, int *pNumPoints, int NumPolys, const Bitmap *Texture, Poly_Type Flags) const;

	/// DrawBitmap & RenderPoly : must Engine_AddBitmap first!
	bool DrawBitmap(const Bitmap *Bitmap, const Rect *Source, uint32 x, uint32 y) const;

	void FillRect(const Rect *rect, const Rgba *Color);

	bool SetGamma(float Gamma);
	bool GetGamma(float *Gamma);

	/// Enables/disables distance fogging.
	/// @param r,g,b is the color of the fog
	/// @param Start is how far out from the camera is not fogged - this is where the fog begins
	/// @param End is how far out from the camera where the fog fully obscures things
	bool SetFogEnable(bool Enable, float r, float g, float b, float Start, float End);

	bool ScreenShot(std::string FileName);
	void EnableFrameRateCounter(bool Enabled);
	bool Activate(bool bActive);
	bool UpdateWindow();
};

struct DriverSystem
{
	Driver *GetNextDriver(Driver *Start);
};

struct Driver
{
	DriverMode *GetNextMode(DriverMode *Start);
	bool GetName(std::string *Name);
};

struct DriverMode
{
	bool GetName(std::string *Name);
	bool GetWidthHeight(int32 *Width, int32 *Height);
};

struct SoundSystem
{
	static SoundSystem *Create(HWND hWnd);
	void Destroy();
	bool SetMasterVolume(float Volume);

	SoundDef *LoadSound(File *file);
};

struct SoundDef
{
	void Free();
	Sound *Play(float Volume, float Pan, float Frequency, bool Loop);
};

struct Sound
{
	bool StopSound();
	bool ModifySound(float Volume, float Pan, float Frequency);
	bool SoundIsPlaying();
};

struct World
{
	World(File *file);
	~World();

	// sound
	void GetSound3dConfig(const XForm *CameraTransform, const vec3f *SoundPos, float Min, float Ds, float *Volume, float *Pan, float *Frequency) const;
	void GetSound3dConfigIgnoreObstructions(const XForm *MXForm, const vec3f *SndPos, float Min, float Ds, float *Volume, float *Pan, float *Frequency) const;

	// World Actors
	bool RemoveActor(Actor *Actor);
	bool AddActor(Actor *Actor, ActorFlag Flags, uint32 UserFlags);
	bool SetActorFlags(Actor *Actor, ActorFlag Flags);

	// World Bitmaps
	bool AddBitmap(Bitmap *Bitmap);
	bool RemoveBitmap(Bitmap *Bitmap);
	bool HasBitmap(const Bitmap *Bitmap) const;
	Bitmap *GetBitmapByName(std::string BitmapName);
	bool BitmapIsVisible(const Bitmap *Bitmap);

	// World BModels
	WorldModel *GetNextModel(WorldModel *Start);

	// World Lights
	Light *AddLight();
	void RemoveLight(Light *Light);
	bool SetLTypeTable(int32 LType, std::string Table);

	// World fog
	Fog *AddFog();
	bool RemoveFog(Fog *Fog);

	// World Classes/Entities
	EntitySet *GetEntitySet(std::string ClassName);

	/// @param Mins  Mins of object (in object-space).  This CAN be NULL. If you are just testing a point, then use NULL (it's faster!!!).
	/// @param Maxs  Maxs of object (in object-space).  This CAN be NULL. If you are just testing a point, then use NULL (it's faster!!!).
	/// @param Front  Front of line (in world-space)
	/// @param Back  Back of line (in world-space)
	/// @param Contents  Contents to collide with (use GE_CONTENTS_SOLID_CLIP for default)
	/// @param CollideFlags  To mask out certain object types (GE_COLLIDE_ALL, etc...)
	/// @param UserFlags  To mask out actors (refer to geActor_SetUserFlags)
	/// @param CollisionCB  A callback to allow user to reject collisions with certain objects)
	/// @param Context  User data passed through above callback
	/// @param Col  Structure filled with info about what was collided with
	bool Collision(const vec3f *Mins, const vec3f *Maxs, const vec3f *Front, const vec3f *Back, ContentsFlag Contents, CollisionFlag CollideFlags, uint32 UserFlags, CollisionCB *CollisionCB, void *Context, Collision *Col);
	bool GetContents(const vec3f *Pos, const vec3f *Mins, const vec3f *Maxs, CollisionFlag Flags, uint32 UserFlags, CollisionCB *CollisionCB, void *Context, Contents *Contents);

	// World Polys
	Poly *AddPolyOnce(LitVertex *Verts, int32 NumVerts, Bitmap *Bitmap, Poly_Type Type, uint32 RenderFlags, float Scale);
	Poly *AddPoly(LitVertex *Verts, int32 NumVerts, Bitmap *Bitmap, Poly_Type Type, uint32 RenderFlags, float Scale);

	void RemovePoly(Poly *Poly);

	// World visibility
	bool GetLeaf(const vec3f *Pos, int32 *Leaf) const;
	bool MightSeeLeaf(int32 Leaf) const;

	/// Checks to see if Leaf1 can see Leaf2
	/// Currently VisFlags is not used yet.  It could be used for checking against areas, etc...
	/// Eventually you could also pass in a VisObject, that is manipulated with a camera...
	bool LeafMightSeeLeaf(int32 Leaf1, int32 Leaf2, uint32 VisFlags) const;

	bool IsActorPotentiallyVisible(const Actor *Actor, const Camera *Camera) const;
};

struct WorldModel
{
	bool SetXForm(const XForm *XForm);
	bool GetXForm(XForm *XForm) const;
	bool Open(bool Open);
	bool GetRotationalCenter(vec3f *Center) const;
	bool GetBBox(vec3f *Mins, vec3f *Maxs);
	Motion *GetMotion();
	void *GetUserData() const;
	void SetUserData(void *UserData);
	void SetFlags(ModelFlag ModelFlags);
	ModelFlag GetFlags();

	// World collision
	bool ModelCollision(const XForm *DXForm, Collision *Collision);
	bool TestModelMove(const XForm *DXForm, const vec3f *Mins, const vec3f *Maxs, const vec3f *In, vec3f *Out);
};

struct Entity
{
	void *GetUserData();
	std::string GetName() const;
};

struct EntitySet
{
	Entity *GetNextEntity(Entity *Entity);
};

/** The Camera object is responsible for handling projection and transformation support for rendering a scene from a given viewpoint.
 * The geCamera object manages the relationship between world space, camera space, and screen space. It is responsible for converting between these coordinate systems. It also comprises field of view information used in rendering a 3D scene to the screen.
 *
 * Space | Description
 * -------------|-------------------------------
 * World space: |	World space is a 3D right handed coordinate system. Most of the Genesis3D APIs operate on parameters given in world space. Collision APIs and other world APIs that yield locational information all give their results in world space.
 * Camera space:| 	Camera space is an intermediate 3D coordinate system that the engine uses in the process of projecting geometry to screen space.
 * Screen space:| 	Screen space is a left handed coordinate system.
 * */
struct Camera
{
	/** Creates a camera.
	 * You can use this to render multiple views to the screen at a time by creating multiple cameras with different screen rects.
	 * In between `Engine::BeginFrame` and `Engine::EndFrame`, you can render the world multiple times, through different cameras.
	 * If you render the world through two different cameras that have different screen rects, you will get two different views of
	 * the world rendered to the screen in a single frame.
	 * @param fov This parameter allows you to configure the field of view of the camera. A setting of 2.0 corresponds to a 90 degree field of view.
	 * @param rect 	Screen space rectangle that the camera will project to. This parameter allows to select regions on the screen will be rendered to if you render using this camera.
	 * @see Engine::BeginFrame
	 * @see Engine::EndFrame
	 */
	Camera(float fov, const Rect *rect);

	/// Destroys a camera
	~Camera();

	void SetZScale(float ZScale);
	float GetZScale() const;
	/// sets a far clipping plane.  The world and objects aren't drawn if they lie beyond ZFar.
	/// Zfar is the distance out from the camera.
	/// Polygons crossing the line are not nesessarily clipped exactly to the line.
	void SetFarClipPlane(bool Enable, float ZFar);
	void GetFarClipPlane(bool *Enable, float *ZFar) const;

	/// @brief Sets the field of view and screen rect on a camera
	/// @param Fov New field of view setting for the camera
	/// @param rect New screen rect for the camera
	/// @see  See Camera() for a discussion of the FOV and Rect parameters.
	void SetAttributes(float Fov, const Rect *rect);

	/// Gets the current screen rect of a camera
	Rect GetClippingRect() const;

	/// @brief Generates a unit vector in the direction of the camera, looking towards the given screen X and Y coordinates.
	/// This API is useful for selecting objects in world space via screen space coordinates. Given a set of screen coordinates, use this API to get a normalized vector in the direction of that screen point. Scale the vector by an arbitrary amount, and use this to calculate an end point from the current camera translation. Take the current camera translation, and that end point, and use geWorld_Collision to collide against objects. This will allow you to accurately point to objects in 3D space using mouse coordinates.
	/// @param ScreenX X coordinate in screen space
	/// @param ScreenY Y coordinate in screen space
	/// @returns Output unit vector
	vec3f ScreenPointToWorld(int32 ScreenX, int32 ScreenY) const;

	/// Projects a point in camera space to a point in screen space.
	/// See the discussion in the overview on world space vs. camera space vs. screen space.
	/// @param PointInCameraSpace Point to project
	/// @returns Point projected into screen space
	vec3f Project(const vec3f &PointInCameraSpace) const;
	void Transform(const vec3f *WorldSpacePoint, vec3f *CameraSpacePoint) const;
	void TransformArray(const vec3f *WorldSpacePointPtr, vec3f *CameraSpacePointPtr, int count) const;
	void TransformAndProject(const vec3f *Point, vec3f *ProjectedPoint) const;
	void TransformAndProjectArray(const vec3f *WorldSpacePointPtr, vec3f *ProjectedSpacePointPtr, int count) const;
	void TransformAndProjectL(const LitVertex *Point, TransformedLitVertex *ProjectedPoint) const;
	void TransformAndProjectLArray(const LitVertex *WorldSpacePointPtr, TransformedLitVertex *ProjectedSpacePointPtr, int count) const;
	bool SetWorldSpaceXForm(const XForm *XForm);
	bool SetWorldSpaceVisXForm(const XForm *XForm);
	const XForm *GetWorldSpaceXForm() const;
	const XForm *GetWorldSpaceVisXForm() const;
	bool ConvertWorldSpaceToCameraSpace(XForm *CXForm) const;
};

struct Poly
{
	bool GetLVertex(int32 Index, LitVertex *LVert);
	bool SetLVertex(int32 Index, const LitVertex *LVert);
};

struct Light
{
	bool SetLightAttributes(const vec3f *Pos, const Rgba *RGBA, float Radius, bool CastShadow);
};

struct Fog
{
	bool Fog_SetAttributes(const vec3f *Pos, Rgba *Color, float LightBrightness, float VolumeBrightness, float VolumeRadius);
};

struct Mesh
{
};

// Polys
enum class Poly_Type
{
	GE_TEXTURED_POLY,
	GE_GOURAUD_POLY,
	GE_TEXTURED_POINT
};

// Poly Fx flags
#define GE_RENDER_DO_NOT_OCCLUDE_OTHERS (1 << 0) // Poly will not occlude others
#define GE_RENDER_DO_NOT_OCCLUDE_SELF (1 << 1)	 // Renders under any condition.  Useful for halos, etc...
#define GE_RENDER_BACKFACED (1 << 2)			 // Poly should be backfaced from the Camera's Pov
#define GE_RENDER_DEPTH_SORT_BF (1 << 3)		 // Sorts relative to camera position, from back to front
#define GE_RENDER_CLAMP_UV (1 << 4)				 // Clamp UV's in both directions

// World Add flags
#define GE_WORLD_RENDER (1 << 0)
#define GE_WORLD_COLLIDE (1 << 1)

enum class ContentsFlag
{
	/// Solid (Visible)
	GE_CONTENTS_SOLID = 1 << 0,
	/// Window (Visible)
	GE_CONTENTS_WINDOW = 1 << 1,
	/// Empty but Visible (water, lava, etc...)
	GE_CONTENTS_EMPTY = 1 << 2,

	/// Vis will see through it
	GE_CONTENTS_TRANSLUCENT = 1 << 3,
	/// Wavy (Visible)
	GE_CONTENTS_WAVY = 1 << 4,
	/// Won't be included in vis oclusion
	GE_CONTENTS_DETAIL = 1 << 5,

	/// Structural but not visible
	GE_CONTENTS_CLIP = 1 << 6,
	/// Primary splitter (Non-Visible)
	GE_CONTENTS_HINT = 1 << 7,
	/// Area seperator leaf (Non-Visible)
	GE_CONTENTS_AREA = 1 << 8,

	GE_CONTENTS_FLOCKING = 1 << 9,
	GE_CONTENTS_SHEET = 1 << 10,
	/// No brush lives in this leaf
	GE_CONTENTS_AIR = 1 << 11,
	GE_RESERVED4 = 1 << 12,
	GE_RESERVED5 = 1 << 13,
	GE_RESERVED6 = 1 << 14,
	GE_RESERVED7 = 1 << 15,

	// 16-31 reserved for user contents
	GE_CONTENTS_USER1 = 1 << 16,
	GE_CONTENTS_USER2 = 1 << 17,
	GE_CONTENTS_USER3 = 1 << 18,
	GE_CONTENTS_USER4 = 1 << 19,
	GE_CONTENTS_USER5 = 1 << 20,
	GE_CONTENTS_USER6 = 1 << 21,
	GE_CONTENTS_USER7 = 1 << 22,
	GE_CONTENTS_USER8 = 1 << 23,
	GE_CONTENTS_USER9 = 1 << 24,
	GE_CONTENTS_USER10 = 1 << 25,
	GE_CONTENTS_USER11 = 1 << 26,
	GE_CONTENTS_USER12 = 1 << 27,
	GE_CONTENTS_USER13 = 1 << 28,
	GE_CONTENTS_USER14 = 1 << 29,
	GE_CONTENTS_USER15 = 1 << 30,
	GE_CONTENTS_USER16 = 1 << 31,
	// 16-31 reserved for user contents

	// These contents are all solid types
	GE_CONTENTS_SOLID_CLIP = GE_CONTENTS_SOLID | GE_CONTENTS_WINDOW | GE_CONTENTS_CLIP,
	GE_CONTENTS_CANNOT_OCCUPY = GE_CONTENTS_SOLID_CLIP,

	// These contents are all visible types
	GE_VISIBLE_CONTENTS = GE_CONTENTS_SOLID | GE_CONTENTS_EMPTY | GE_CONTENTS_WINDOW | GE_CONTENTS_WAVY
};

struct Contents
{
	Mesh *Mesh;
	WorldModel *Model;
	Actor *Actor;
	ContentsFlag Contents;
};

typedef bool CollisionCB(WorldModel *Model, Actor *Actor, void *Context);

struct Plane
{
	/// Orientation of plane
	vec3f Normal;
	/// Distance from origin
	float Dist;
};

struct Collision
{
	/// Pointer to what model was hit (if any)
	WorldModel *Model;
	/// Pointer to what mesh was hit (if any)
	Mesh *Mesh;
	/// Pointer to what actor was hit (if any)
	Actor *Actor;
	/// Impact Point
	vec3f Impact;
	/// Percent from 0 to 1.0, how far along the line for the impact point
	float Ratio;
	/// Impact Plane
	Plane Plane;
};

/// They are merely means of overriding ways the engine normally renders primitives, etc...
/// Debug purposes only.
enum class RenderState
{
	// Z Writes
	ZWRITE,
	// Z Compares
	ZCMP,
	// Bilinear filtering
	BILINEAR,
	// Anti-Aliasing
	ANTI_ALIAS,
	// Normal, Gouraud only, Lines only, etc
	POLYMODE
};

enum class RenderFlag
{
	OFF = 0,
	ON = 1
};

/// A method to override how polys are drawn for debugging purposes...
enum class PolyMode
{
	/// Draw as is
	NORMAL = 1,
	/// Gouraud only
	GOURAUD = 2,
	/// Outlines only
	LINES = 3,
};
