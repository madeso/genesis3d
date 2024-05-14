#pragma once

///	Collision defines (for geWorld_Collision)
enum class CollisionFlag
{
	MESHES = (1 << 0),
	MODELS = (1 << 1),
	ACTORS = (1 << 2),
	NO_SUB_MODELS = (1 << 3),
	ALL = (GE_COLLIDE_MESHES | GE_COLLIDE_MODELS | GE_COLLIDE_ACTORS)
};

/// Actor flags (geWorld_AddActor)
enum class ActorFlag
{
	/// Render in normal views
	RENDER_NORMAL = (1 << 0),
	/// Render in mirror views
	RENDER_MIRRORS = (1 << 1),
	/// Render always, skipping all visibility tests
	RENDER_ALWAYS = (1 << 2),
	/// Collide when calling geWorld_Collision
	COLLIDE = (1 << 3)
};

//
//	Model flags (geWorld_ModelSetFlags)
//
enum class ModelFlag
{
	/// Render in normal views
	RENDER_NORMAL = (1 << 0),
	/// Render in mirror views
	RENDER_MIRRORS = (1 << 1),
	/// Render always, skipping all visibility tests
	RENDER_ALWAYS = (1 << 2),
	/// Collide when calling geWorld_Collision
	COLLIDE = (1 << 3)
};

struct Rgba
{
	float r, g, b, a;
};

struct Rect
{
	int32 Left;
	int32 Right;
	int32 Top;
	int32 Bottom;
};

/// Lit vertex
struct LitVertex
{
	// FIXME:  Convert 3d X,Y,Z to vec3f
	float X, Y, Z; // 3d vertex
	float u, v;	   // Uv's
	// FIXME:  Convert r,g,b,a to Rgba
	float r, g, b, a; // color
};

/// Transformed Lit vertex
struct TransformedLitVertex
{
	float x, y, z;	  // screen points
	float u, v;		  // Uv's
	float r, g, b, a; // color
};
