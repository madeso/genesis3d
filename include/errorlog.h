#pragma once

typedef enum
{
	/// Driver not supported
	INVALID_DRIVER_HANDLE,
	/// Mode not supported
	INVALID_MODE_HANDLE,
	/// Could not init Driver
	DRIVER_INIT_FAILED,
	/// Driver init failure
	DRIVER_ALLREADY_INITIALIZED,
	/// File open error for driver
	DRIVER_NOT_FOUND,
	/// Driver shutdown failure
	DRIVER_NOT_INITIALIZED,
	/// Wrong driver version, or bad driver
	INVALID_DRIVER,
	DRIVER_BEGIN_SCENE_FAILED,
	DRIVER_END_SCENE_FAILED,
	CREATE_SOUND_MANAGER_FAILED,
	CREATE_SOUND_BUFFER_FAILED,
	DS_ERROR,
	INVALID_WAV,
	NO_PERF_FREQ,
	FILE_OPEN_ERROR,
	FILE_READ_ERROR,
	FILE_WRITE_ERROR,
	PALETTE_LOAD_FAILURE,
	GBSP_LOAD_FAILURE,
	INVALID_PARMS,
	INVALID_CAMERA,
	RENDER_WORLD_FAILED,
	BEGIN_WORLD_FAILED,
	END_WORLD_FAILED,
	BEGIN_MODELS_FAILED,
	END_MODELS_FAILED,
	BEGIN_MESHES_FAILED,
	END_MESHES_FAILED,
	RENDER_MESH_FAILED,
	BAD_LMAP_EXTENTS,
	INVALID_TEXTURE,
	REGISTER_WORLD_TEXTURE_FAILED,
	REGISTER_LIGHTMAPS_FAILED,
	REGISTER_WORLD_PALETTE_FAILED,
	REGISTER_MISC_TEXTURE_FAILED,
	INVALID_MESH_FILE,
	LOAD_BITMAP_FAILED,
	MAX_MESH_DEFS,
	MESH_MAX_NODES,
	INVALID_MESH_MATERIAL,
	MAX_MESH_MATERIALS,
	MAX_MESH_CLIP_PLANES,
	RENDERQ_OVERFLOW,
	INVALID_LTYPE,
	MAX_ENTITIES,
	GET_ENTITY_DATA_ERROR,
	INVALID_ENTITY_FIELD_TYPE,
	MODEL_NOT_FOUND,
	MODEL_NOT_IN_ENTITY,
	MAX_TEXTURES,
	MAX_DECALS,
	MAX_VERTS,
	OUT_OF_MEMORY,
	INVALID_BSP_TAG,
	INVALID_BSP_VERSION,
	ERROR_READING_BSP_CHUNK,
	/// failure to create a path (memory allocation failed)
	PATH_CREATE_ENOMEM,
	/// failure to insert a rotation keyframe
	PATH_INSERT_R_KEYFRAME,
	/// failure to insert a translation keyframe
	PATH_INSERT_T_KEYFRAME,
	/// failure to delete a rotation keyframe
	PATH_DELETE_R_KEYFRAME,
	/// failure to delete a translation keyframe
	PATH_DELETE_T_KEYFRAME,
	/// failure to read from file
	PATH_FILE_READ,
	/// tried to create path from file with wrong/bad version
	PATH_FILE_VERSION,
	/// failure to parse file (unexpected format problem)
	PATH_FILE_PARSE,
	/// failure to read from file
	PATH_FILE_WRITE,
	/// failure to create (memory allocation failed)
	MOTION_CREATE_ENOMEM,
	/// failure to add path into motion (memory allocation failed)
	MOTION_ADDPATH_ENOMEM,
	/// failure to add path into motion (path creation failed)
	MOTION_ADDPATH_PATH,
	/// failure to add path into motion due to name conflict
	MOTION_ADDPATH_BAD_NAME,
	/// failure to insert event (memory allocation failed or duplicate key)
	MOTION_INSERT_EVENT,
	/// failure to insert event
	MOTION_DELETE_EVENT,
	/// failure to read from file
	MOTION_FILE_READ,
	/// failure to write to file
	MOTION_FILE_WRITE,
	/// failure to parse file (unexpected format problem)
	MOTION_FILE_PARSE,
	/// failure to insert into list because of existing identical key
	TKARRAY_INSERT_IDENTICAL,
	/// failure to insert into list because of memory allocation failure
	TKARRAY_INSERT_ENOMEM,
	/// failure to delete from list because key was not found
	TKARRAY_DELETE_NOT_FOUND,
	/// failure to create TKArray object (out of memroy)
	TKARRAY_CREATE,
	/// TKArray object can't be added to - it's list is as big as it can get
	TKARRAY_TOO_BIG,
	/// insertion to VKArray failed
	VKARRAY_INSERT,
	/// insertion to QKArray failed
	QKARRAY_INSERT,
	/// Motion object failed to create (memory allocation failed)
	POSE_CREATE_ENOMEM,
	/// Motion_AddJoint failed to allocate/reallocate memory for new joint
	POSE_ADDJOINT_ENOMEM,
	/// failure to create TKEvents object (memory allocation failed)
	TKEVENTS_CREATE_ENOMEM,
	/// failure to delete from list because key was not found
	TKEVENTS_DELETE_NOT_FOUND,
	/// failure to insert into list because of memory allocation failure
	TKEVENTS_INSERT_ENOMEM,
	/// failure to insert into list
	TKEVENTS_INSERT,
	/// failure to read from data file
	TKEVENTS_FILE_READ,
	/// failure to write to data file
	TKEVENTS_FILE_WRITE,
	/// failure to read tkevents object: file has wrong version
	TKEVENTS_FILE_VERSION,
	/// failure to parse file (unexpected format problem)
	TKEVENTS_FILE_PARSE,
	/// failure to create, insert, or append (memory allocation failed)
	STRBLOCK_ENOMEM,
	/// string too long to insert or append
	STRBLOCK_STRLEN,
	/// failure to read from data file
	STRBLOCK_FILE_READ,
	/// failure to write to data file
	STRBLOCK_FILE_WRITE,
	/// failure to parse reading from input file (unexpected format problem)
	STRBLOCK_FILE_PARSE,
	/// failure to create, or add (memory allocation failed)
	BODY_ENOMEM,
	/// failure to parse reading from input file (unexpected format problem)
	BODY_FILE_PARSE,
	/// failure to read from data file
	BODY_FILE_READ,
	/// failure to write to data file
	BODY_FILE_WRITE,
	/// XFArray object failed to return array, or array size doesn't match bone count
	BODY_BONEXFARRAY,
	/// failure to create. (memory allocation failure)
	XFARRAY_ENOMEM,
	/// failure to create. (memory allocation failure)
	PUPPET_ENOMEM,
	/// failure to render.
	PUPPET_RENDER,
	/// failure to create: associated body has no materials.
	PUPPET_NO_MATERIALS,
	/// failure to load texture
	PUPPET_LOAD_TEXTURE,
	/// failure to create or add to. (memory allocation/reallocation failure)
	TEXPOOL_ENOMEM,
	/// failure to add to pool, pool is too large.
	TEXPOOL_TOO_BIG,
	/// failure to load texture into pool
	TEXPOOL_LOAD_TEXTURE,
	/// texture pool destroyed without first freeing all it's shared textures
	TEXPOOL_TEXTURE_NOT_FREE,
	/// failure to create. (memory allocation failure)
	ACTOR_ENOMEM,
	/// failure to prepare actor for rendering (bad Body or allocation failure)
	ACTOR_RENDER_PREP,
	/// failure to render.  failure to get geometry from Body
	ACTOR_RENDER_FAILED,
	/// failure to add motion. too many.
	ACTOR_TOO_MANY_MOTIONS,
	/// failure to read from data file.
	ACTOR_FILE_READ,
	/// failure to parse reading from input file(unexpected format problem)
	ACTOR_FILE_PARSE,
	/// failure to write to data file.
	ACTOR_FILE_WRITE,
	/// Bad model motion file (for bsp files)
	RR_INVALID_MODEL_MOTION_FILE,
	/// Chunk size does not match structure size of kind
	RR_BAD_BSP_FILE_CHUNK_SIZE,
} ErrorIDEnumType;

enum class ErrorClassType
{
	MEMORY_RESOURCE,
	DISPLAY_RESOURCE,
	SOUND_RESOURCE,
	SYSTEM_RESOURCE,
	INTERNAL_RESOURCE,

	FILEIO_OPEN,
	FILEIO_CLOSE,
	FILEIO_READ,
	FILEIO_WRITE,
	FILEIO_FORMAT,
	FILEIO_VERSION,

	LIST_FULL,
	DATA_FORMAT,
	SEARCH_FAILURE,
};

struct ErrorLog
{
	/// clears error history
	static void Clear(void);

	/// reports size of current error log
	static int Count(void);

	/// not intended to be used directly: use Error_Add or Error_AddString
	static void AddExplicit(ErrorClassType, std::string ErrorIDString, std::string ErrorFileString, int LineNumber, std::string UserString, std::string Context);

	/// adds text to the previous logged error
	static bool AppendString(std::string String);

	/// reports from the error log.
	/// history is 0 for most recent,  1.. for second most recent etc.
	/// returns true if report succeeded.  false if it failed.
	static bool Report(int History, ErrorClassType *Error, std::string *UserString);
};

/// 'Debug' version includes a textual error id, and the user string
/// 'Release' version does not include the textual error id, or the user string

/// logs an error.
#define Error_Add(Error, Context) ErrorLog::AddExplicit(Error, #Error, __FILE__, __LINE__, "", Context)

/// logs an error with additional identifing string.
#define Error_AddString(Error, String, Context) ErrorLog::AddExplicit(Error, #Error, __FILE__, __LINE__, String, Context)
