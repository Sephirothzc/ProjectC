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
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlatformCamera)
	bool bEnablePlatformCameraLerp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlatformCamera)
	bool bDrawPlatformCameraLerpMarkers;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlatformCamera, meta = (editcondition = "bEnablePlatformCameraLerp"))
	FVector CameraChangeLimit;

	UFUNCTION(BlueprintCallable, Category = PlatformCamera)
	void OnTurnRound(FVector cur_forward);
	
	//��д�˺�������ֹԭ�еĿ����߼�Ӱ�쵱ǰ�Ŀ����߼�
	virtual void UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime);

	virtual void UpdatePlatformCameraLerp(bool bDoPlatformLerp, float DeltaTime);

private:
	FVector m_cur_forward;
	
};
