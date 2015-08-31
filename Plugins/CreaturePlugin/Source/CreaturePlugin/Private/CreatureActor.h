// UE4 Procedural Mesh Generation from the Epic Wiki (https://wiki.unrealengine.com/Procedural_Mesh_Generation)

#pragma once

#include "GameFramework/Actor.h"
#include "CreatureModule.h"
#include <map>

#include "CustomProceduralMeshComponent.h"
#include "CreatureActor.generated.h"

/**
 * 
 */

struct FCreatureBoneData
{
	FVector point1;
	FVector point2;
	FTransform xform;
	FTransform startXform;
	FTransform endXform;
	FString name;
};

USTRUCT()
struct FAnimationNotifyFrameNode {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Components|Creature")
	FString AnimationName;
	
	UPROPERTY(EditAnywhere, Category = "Components|Creature")
	TArray<int32> NotifyFrame;
};

// Blueprint event delegates event declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreatureAnimationStartEvent, float, frame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreatureAnimationEndEvent, float, frame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreatureAnimationNotifyEvent, float, frame);

UCLASS(Blueprintable)
class ACreatureActor : public AActor
{
	GENERATED_UCLASS_BODY()

protected:
	TArray<FProceduralMeshTriangle> draw_triangles;

	std::shared_ptr<CreatureModule::CreatureManager> creature_manager;

	TArray<FCreatureBoneData> bone_data;

	TArray<uint8> region_alphas;

	TMap<FString, uint8> region_alpha_map;

	TArray<FString> region_custom_order;

	FString absolute_creature_filename;

	bool should_play, is_looping;

	bool play_start_done, play_end_done;

	bool is_disabled;

	bool is_driven;

	bool is_ready_play;

	FCriticalSection  msg_lock;

	TArray<int32> register_notify_frame_queue;
	TMap< FString, TArray<int32> > register_notify_frame_map;

	void UpdateCreatureRender();



	void FillBoneData();

	void ParseEvents(float deltaTime);

	void InitStandardValues();

	void ProcessRenderRegions(TArray<FProceduralMeshTriangle>& draw_tris);

public:
	ACreatureActor();

	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	bool InitCreatureRender();

	// Allow viewing/changing the Material ot the procedural Mesh in editor (if placed in a level at construction)
	UPROPERTY(VisibleAnywhere, Category=Materials)
	UCustomProceduralMeshComponent* creature_mesh;

	/* Removed, put this in if you need a default capsule root collider
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	UCapsuleComponent * rootCollider;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	FString creature_filename;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	float animation_speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	float bone_data_size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	float creature_bounds_scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	FVector creature_bounds_offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	bool creature_debug_draw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	float bone_data_length_factor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	float region_overlap_z_delta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	bool smooth_transitions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
	FString start_animation_name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components|Creature")
	float animation_frame;

	UPROPERTY(BlueprintAssignable, Category = "Components|Creature")
	FCreatureAnimationStartEvent CreatureAnimationStartEvent;

	UPROPERTY(BlueprintAssignable, Category = "Components|Creature")
	FCreatureAnimationEndEvent CreatureAnimationEndEvent;
	
	UPROPERTY(BlueprintAssignable, Category = "Components|Creature")
	FCreatureAnimationNotifyEvent CreatureAnimationNotifyEvent;

	UPROPERTY(EditAnywhere, Category = "Components|Creature")
	TArray<FAnimationNotifyFrameNode> register_notify_frame_node_array;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent);
#endif

	virtual void OnConstruction(const FTransform & Transform);

	// Loads a data packet from a file
	static void LoadDataPacket(const std::string& filename_in);

	// Loads an animation from a file
	static void LoadAnimation(const std::string& filename_in, const std::string& name_in);

	// Loads the creature character from a file
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void LoadCreature(const FString& filename_in);

	// Adds a loaded animation onto the creature character
	bool AddLoadedAnimation(const std::string& filename_in, const std::string& name_in);

	// Returns the CreatureManager associated with this actor
	CreatureModule::CreatureManager * GetCreatureManager();

	// Blueprint version of setting the active animation name
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void SetBluePrintActiveAnimation(FString name_in);

	// Blueprint version of setting the blended active animation name
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void SetBluePrintBlendActiveAnimation(FString name_in, float factor);

	// Blueprint version of setting a custom time range for a given animation
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void SetBluePrintAnimationCustomTimeRange(FString name_in, int32 start_time, int32 end_time);

	// Blueprint function that returns the transform given a bone name, position_slide_factor
	// determines how far left or right the transform is placed. The default value of 0 places it
	// in the center of the bone, positve values places it to the right, negative to the left
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	FTransform GetBluePrintBoneXform(FString name_in, bool world_transform, float position_slide_factor);

	// BLueprint function that returns whether a given input point is colliding with any of the bones
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	bool IsBluePrintBonesCollide(FVector test_point, float bone_size);

	// Blueprint function that decides whether the animation will loop or not
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void SetBluePrintAnimationLoop(bool flag_in);

	// Blueprint function that decides whether to play the animation or not
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void SetBluePrintAnimationPlay(bool flag_in);

	// Blueprint function that plays the animation from the start
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void SetBluePrintAnimationPlayFromStart();

	// Blueprint function that resets the animation to the start time
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void SetBluePrintAnimationResetToStart();

	// Blueprint function that returns the current animation frame
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")	
	float GetBluePrintAnimationFrame();

	// Blueprint function that sets the alpha(opacity value) of a region
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void SetBluePrintRegionAlpha(FString region_name_in, uint8 alpha_in);

	// Blueprint function that sets up a custom z order for the various regions
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void SetBluePrintRegionCustomOrder(TArray<FString> order_in);

	// Blueprint function that clears the custom z order for the various regions
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void ClearBluePrintRegionCustomOrder();

	// Sets the an active animation by name
	void SetActiveAnimation(const std::string& name_in);

	// Sets the active animation by smoothly blending, factor is a range of ( 0 < factor < 1 )
	void SetAutoBlendActiveAnimation(const std::string& name_in, float factor);

	// Blueprint function that turns on/turns off internal updates of this object
	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void SetIsDisabled(bool flag_in);

	UFUNCTION(BlueprintCallable, Category = "Components|Creature")
	void SetSetMeshMaterial(int32 ElementIndex, UMaterialInterface* InMaterial);
	
	void SetDriven(bool flag_in);

	bool GetIsReadyPlay() const;

	// Update callback
	virtual void Tick(float DeltaTime) override;

	// Called on startup
	virtual void BeginPlay();

	void GenerateTriangle(TArray<FProceduralMeshTriangle>& OutTriangles);
};
