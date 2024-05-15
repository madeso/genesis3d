#pragma once

/*   Actor

	This object is designed to support character animation.
	There are two basic objects to deal with.

	Actor Definition (ActorDef)
		A ActorDef embodies the geometry (polygon, and bone information),
		and a library of motions that can be applied to that geometry.

	Actor
		A Actor is an instance of an actor definition.  The definition is used for
		the geometry, but all additional settings, such as the bone pose, lighting information,
		and cuing information is unique for a Actor.

	There are two ways to use an Actor.
	Direct Control
		One method is to directly control the skeleton configuration.  Use _SetPose() to set its
		skeleton using a Motion animation.  The pose is positioned in world space relative to the
		transform given in SetPose().  Whenever a new skeleton pose is required, call _SetPose()
		to reposition the skeleton for a new point in time.

		More complex positioning can be achieved by blending more than one animation.  Use
		_BlendPose() after a _SetPose() to blend the second Motion into the first.  Additional
		blends can be applied by additional _BlendPose() calls.  Each blend is performed on the
		the existing skeleton (the results of any previous blends).
	Cuing
		Another method is to 'cue' up motions that are applied with parameterized blending over time.
		A cued motion takes effect 'now' in time.  The Actor advances in time and repositions itself
		according to its currently cued motions with a call to _AnimationStep().  AnimationStep()
		redefines what the actor thinks 'now' is.  This causes historical cues to be forgotten, and
		motions that are no longer valid are cleaned up.  AnimationTestStep() can be used to position
		the actor for potential queries with its currently cued motions at some arbitrary future time
		- relative to the last AnimationTestStep() call.  AnimationNudge() applies a given transform
		'instantly' to the current actor's cue list.  This is usefull for moving the actor as a
		result of a collision with another object.

	If a motion contains joint information that does not exactly match the Actor's skeleton
	joints, only the joints that match by name are applied.  So a Motion can be applied to
	a portion of the Actor, or a Motion that has more joint information than the skeleton can
	be applied and the extra joint information is ignored.

	Examples of this:  If the Actor is a biped and has no tail, but the motion is for a
	biped with a tail, the Motion can be applied, but the tail information will be ignored.
	Also if there is a Motion for only a left arm, it can be applied and it will only affect
	the left arm of the Actor, and consequently its left hand and fingers, but no other
	bones that are not children of the affected bones will be changed.


*/

#include "genesis.h"

#include "extbox.h"
#include "bitmap.h"

#include "motion.h"

#include "body.h"

enum class BlendingType
{
	/// Treats the blending amount as a linear value
	LINEAR,

	/// Applies a parametric smoothing curve to the blending amount
	///  so that a linear change in BlendAmount parameters will
	///  result in a smooth (non-linear) change in blending.
	HERMITE
};

struct Actor;
struct ActorDef;

/// an instance of an actor
struct Actor
{

	/// Create an Actor instance associated with the given Actor Definition
	Actor(ActorDef *ActorDefinition);

	/// Give the Actor Definition a Body.  Actor becomes responsible for its destruction.
	/// sets up default materials as referenced by the Body.
	bool SetBody(ActorDef *ActorDefinition, Body *geBodyGeometry);

	/// Adds a Motion to the Actor Definition's library.  The ActorDefinition becomes responsible for its destruction.
	/// returns the library index to the new Motion.
	bool AddMotion(ActorDef *ActorDefinition, Motion *M, int *Index);

	/// Destroy an Actor.
	~Actor();

	bool IsValid() const;

	/// Returns the Actor Definition
	ActorDef *GetActorDef();

	/// Selects a blending type.  BlendingType only affects the meaning of the
	/// BlendAmount parameter for the blend functions.  Can be changed anytime.
	void SetBlendingType(BlendingType BlendingType);

	/// Returns the number of materials for an instance of an actor.
	int GetMaterialCount();

	/// Returns the current material for an instance of an actor
	bool GetMaterial(int MaterialIndex, Bitmap **Bitmap, float *Red, float *Green, float *Blue);

	/// Allows a material to be overriden in an actor instance
	bool SetMaterial(Actor *Actor, int MaterialIndex, Bitmap *Bitmap, float Red, float Green, float Blue);

	/// Gets the current transform for a single bone in A.  (actor space->world space transform)
	/// with a NULL BoneName, this returns the current 'root' transform
	bool GetBoneTransform(const char *BoneName, XForm *Transform);

	/// Gets the extent box (axial-aligned bounding box) for a given bone (for the current pose)
	/// if BoneName is NULL, gets the a general bounding box from the body of the actor if it has been set.
	bool GetBoneExtBox(const char *BoneName, ExtBox *ExtBox);

	/// Gets the non-axial-aligned bounding box for a given bone (for the current pose)
	///  The box is specified by a corner, and
	///  a non-normalized orientation transform.  Add DX,DY,DZ components
	///  of the orientation to get other corners of the box
	/// if BoneName is NULL, gets the a general bounding box from the body of the actor if it has been set.
	bool GetBoneBoundingBox(const char *BoneName,
							vec3f *Corner,
							vec3f *DX,
							vec3f *DY,
							vec3f *DZ);

	/// Gets the current axial-aligned bounding box for an actor's bone configuration
	/// takes all bones into account
	bool GetDynamicExtBox(ExtBox *ExtBox);

	/// Gets an assigned general non changing bounding box from the actor
	bool GetExtBox(ExtBox *ExtBox);

	/// Sets an assigned general non changing bounding box from the actor
	bool SetExtBox(const ExtBox *ExtBox,
				   const char *CenterBoxOnThisNamedBone); // NULL uses root position of actor

	/// Gets the rendering hint bounding box from the actor
	///   if the RenderHintExtBox is disabled, Enabled is false, and the box returned has zero dimensions,
	///   centered at the root position of the actor.  If the RenderHintExtBox is enabled, Enabled is
	///   true, and the box returned is the one set with _SetRenderHintExtBox, offset by the
	///   bone position of the bone named in _SetRenderHintExtBox().
	bool GetRenderHintExtBox(ExtBox *Box, bool *Enabled);

	/// Sets a rendering hint bounding box from the actor.  Increases performance by
	///   enabling the rendering of the actor to occur only if the box is visible.
	///   If the box is not visible, a detailed analysis of the actor's current geometry is avoided.
	///   This does allow errors to occur:
	///   If the actor has a bit of geometry that extends outside this box for some
	///   animation, that extended geometry may not be drawn, if the box if off-screen.
	///   If the render hint box is not set, the engine will make no conservative assumptions
	///   about the visibility of an actor - it will always be drawn if any portion of it is
	///   visible.
	///   To attach the box to the 'root' bone, pass NULL for CenterBoxOnThisNamedBone
	///   For disabling the hint box: (disabled is default) pass Box with zero mins and maxs
	bool SetRenderHintExtBox(const ExtBox *Box,
							 const char *CenterBoxOnThisNamedBone);

	/// Returns the pointer which was set with SetUserData.  NULL if not set.
	void *GetUserData();

	/// Sets the actors user data pointer to the given value.  For clients only.
	void SetUserData(void *UserData);

	/// Poses the actor in its default pose
	/// Transform is where to position the root for this pose.
	///  if Transform is NULL, the root for the pose is assumed to be the root of the actor.
	void ClearPose(const XForm *Transform);

	/// Poses the actor using given motion M at a time offset of Time
	/// Transform is where to position the root for this pose.
	///  if Transform is NULL, the root for the pose is assumed to be the root of the actor.
	void SetPose(const Motion *Motion, float Time, const XForm *Transform);

	/// Blends the current pose of the Actor with
	///  a new pose using motion M at a time offset of Time
	/// A BlendAmount of 0 will result in the existing pose, A BlendAmount of 1 will
	/// result in the new pose from M.  The BlendingType set by _SetBlendingType() determines
	/// the blending function between 0 and 1
	/// Transform is where to position the root for this pose.
	///  if Transform is NULL, the root for the pose is assumed to be the root of the actor.
	void BlendPose(const Motion *Motion, float Time,
				   const XForm *Transform, float BlendAmount);

	bool GetBoneAttachment(const char *BoneName, XForm *Transform);
	bool SetBoneAttachment(const char *BoneName, XForm *Transform);

	bool SetLightingOptions(bool UseFillLight,					// true or false
							const vec3f *FillLightNormal,		// normalized vector
							float FillLightRed,					// 0 .. 255
							float FillLightGreen,				// 0 .. 255
							float FillLightBlue,				// 0 .. 255
							float AmbientLightRed,				// 0 .. 255
							float AmbientLightGreen,			// 0 .. 255
							float AmbientLightBlue,				// 0 .. 255
							bool AmbientLightFromFloor,			// true or false
							int MaximumDynamicLightsToUse,		// 0 for none
							const char *LightReferenceBoneName, // NULL for root
							bool PerBoneLighting);
	/// if true, then dynamic lighting attenuation and direction is computed
	/// for each bone.  if false, then the computations are relative to the
	/// single bone named by the LightReferenceBoneName

	bool GetLightingOptions(bool *UseFillLight,				// true or false
							vec3f *FillLightNormal,			// normalized vector
							float *FillLightRed,			// 0 .. 255
							float *FillLightGreen,			// 0 .. 255
							float *FillLightBlue,			// 0 .. 255
							float *AmbientLightRed,			// 0 .. 255
							float *AmbientLightGreen,		// 0 .. 255
							float *AmbientLightBlue,		// 0 .. 255
							bool *UseAmbientLightFromFloor, // true or false
							int *MaximumDynamicLightsToUse,
							const char **LightReferenceBoneName,
							bool *PerBoneLighting); // NULL for root

	void SetScale(float ScaleX, float ScaleY, float ScaleZ);

	bool SetShadow(bool DoShadow,
				   float Radius,
				   const Bitmap *ShadowMap,
				   const char *BoneName);

	///  Animation Cuing API:
	/// high level Actor animation:  The principle is that motions can be applied to an actor
	/// and the actor will keep track of which motions are currently appropriate.  Call
	///	_AnimationStep() to compute a new pose for an elapsed time interval.  The new pose
	///  will take into account all motions that are 'currently' cued up to be set or blended.

	/// cue up a new motion.  The motion begins at the current time.  The motion can be
	/// blended in or out over time and time scaled.  If the return value is false, the
	/// animation was not cued up (failure implies Actor is incompletely initialized).
	bool AnimationCue(
		Motion *Motion,				   // motion to Cue
		float TimeScaleFactor,		   // time scale to apply to cued motion
		float TimeIntoMotion,		   // time offset to begin motion with (Not TimeScaled)
		float BlendTime,			   // time to apply a blend.
		float BlendFromAmount,		   // blend value at current time
		float BlendToAmount,		   // blend value after BlendTime time has elapsed
		const XForm *MotionTransform); // local transform to adjust motion by (NULL implies NO transform)

	/// removes the last animation cue that was cued up.  Can be called repeatedly to successively
	/// remove older and older cues.  Returns true when a cue was removed, false if there
	/// are no cues to remove.
	bool AnimationRemoveLastCue(Actor *A);

	/// applies a time step to actor A.  re-poses the actor according to all currently applicable
	/// Animation Cues. (failure implies Actor is incompletely initialized)
	bool AnimationStep(float DeltaTime);

	/// applies a 'temporary' time step to actor A.  re-poses the actor according to all
	/// currently appliciable cues.  (failure implies Actor is incompletely initialized)
	/// DeltaTime is always relative to the the last AnimationStep()
	bool AnimationTestStep(float DeltaTime);

	/// optimized version of AnimationStep.  Limits calculations to the bone named BoneName, and it's
	/// parents.  BoneName will be correctly computed, but the other bones will be wrong.  This is usefull for
	/// moving and animating an actor that is not actually visible.  Rendering and queries will be 'optimized'
	/// until the actor is given any pose or animation that doesn't go through AnimationStepBoneOptimized() or
	///  AnimationTestStepBoneOptimized().  BoneName can be NULL to compute only 'root' bone.
	bool AnimationStepBoneOptimized(float DeltaTime, const char *BoneName);

	/// optimized version of AnimationTestStep.  Limits calculations to the bone named BoneName, and it's
	/// parents.  BoneName will be correctly computed, but the other bones will be wrong.  This is usefull for
	/// moving and animating an actor that is not actually visible.  Rendering and queries will be 'optimized'
	/// until the actor is given any pose or animation that doesn't go through AnimationStepBoneOptimized() or
	///  AnimationTestStepBoneOptimized().  BoneName can be NULL to compute only 'root' bone.
	bool AnimationTestStepBoneOptimized(float DeltaTime, const char *BoneName);

	/// applies an 'immediate' offset to the animated actor
	bool AnimationNudge(XForm *Offset);

	/// Return data, if return value is true
	bool GetAnimationEvent(const char **ppEventString);
};

/// the deinition of an actor's geometry/bone structure
struct ActorDef
{
	/// Create an 'empty' Actor Definition.
	ActorDef();

	/// Create an Actor Definition from a file image.
	ActorDef(File *pFile);

	/// Destroy a ActorDef (its Body and its geMotions)  Actors that rely on this definition become invalid.
	~ActorDef();

	bool IsValid() const;

	/// Writes an existing Actor to a file image.  Returns true on success, false on failure.
	bool WriteToFile(File *pFile);

	/// Returns a Body pointer
	Body *GetBody();

	/// Returns true if the actor definition has a bone named 'Name'
	bool HasBoneNamed(const char *Name);

	/// Returns the number of geMotions in the geActors Motion library.
	int GetMotionCount() const;

	/// Returns a Motion pointer from the geActors Motion library
	///   This is an aliased pointer - Not a copy.  Changes to this motion will be reflected
	///   in the actor.  Destroying this return motion will confuse the actor.
	/// Index must be in range [0..GetMotionCount-1]
	Motion *GetMotionByIndex(int Index) const;

	/// Returns a Motion pointer from the geActors Motion library
	///   This is an aliased pointer - Not a copy.  Changes to this motion will be reflected
	///   in the actor.  Destroying this return motion will confuse the actor.
	/// if there is no motion that matches the given name, the return value will be NULL
	Motion *GetMotionByName(const char *Name) const;

	/// Returns a motion name given an ActorDef and a motion index.
	const char *GetMotionName(int Index) const;
};
