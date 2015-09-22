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


	UFUNCTION(BlueprintCallable, Category = PlatformCamera)
	void OnTurnRound(FVector cur_forward);
	
	//重写此函数，防止原有的控制逻辑影响当前的控制逻辑
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime);

	virtual void UpdatePlatformCameraLerp(bool bDoPlatformLerp, float DeltaTime);

private:
	bool m_is_forward_left;
	bool m_is_cur_owner_fowrard_left;

	bool m_is_attach_follow_point;
};
