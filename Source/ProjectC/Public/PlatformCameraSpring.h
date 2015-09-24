// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "PlatformCameraSpring.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Camera, meta = (BlueprintSpawnableComponent), hideCategories = (Mobility))
class PROJECTC_API UPlatformCameraSpring : public USpringArmComponent
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlatformCamera)
	bool bEnablePlatformCameraLerp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlatformCamera)
	bool bDrawPlatformCameraLerpMarkers;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlatformCamera, meta = (editcondition = "bEnablePlatformCameraLerp"))
	FVector CameraChangeLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlatformCamera, meta = (editcondition = "bEnablePlatformCameraLerp"))
	FVector CameraFollowLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlatformCamera, meta = (editcondition = "bEnablePlatformCameraLerp"))
	float CameraLerpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlatformCamera, meta = (editcondition = "bEnablePlatformCameraLerp"))
	int32 CameraLerpMaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlatformCamera, meta = (editcondition = "bEnablePlatformCameraLerp"))
	float CameraLerpTolerance;
	

	UFUNCTION(BlueprintCallable, Category = PlatformCamera)
	void OnTurnRound(FVector cur_forward);
	
	//重写此函数，防止原有的控制逻辑影响当前的控制逻辑
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime);

	virtual void UpdatePlatformCameraLerp(bool bDoPlatformLerp, float DeltaTime);

	virtual void BeginPlay();

private:
	enum PLATFORM_CAMERA_STATE {
		PLATFORM_CAMERA_STATE_NONE = 0,
		PLATFORM_CAMERA_STATE_LEFT,
		PLATFORM_CAMERA_STATE_RIGHT,
		PLATFORM_CAMERA_STATE_MOVE_LEFT,
		PLATFORM_CAMERA_STATE_MOVE_RIGHT,
		PLATFORM_CAMERA_STATE_MOVE_MAX,
	};

	void EnterCameraState(PLATFORM_CAMERA_STATE next_state);
	void ExitCameraState(PLATFORM_CAMERA_STATE cur_state);
	void ChangeCameraState(PLATFORM_CAMERA_STATE next_state);
	float CalcCameraPosX(const FVector& follow_point, const FVector& limit_point, float delta_seconds);

	PLATFORM_CAMERA_STATE m_cur_camera_state;
	bool m_is_forward_left;
	bool m_is_cur_owner_fowrard_left;

	bool m_is_attach_follow_point;

	bool m_camera_move_nearly_attach;

	FVector m_pre_camera_location;
};
