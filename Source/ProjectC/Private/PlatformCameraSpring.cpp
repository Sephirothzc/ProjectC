// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectC.h"
#include "PlatformCameraSpring.h"

void UPlatformCameraSpring::OnTurnRound(FVector cur_forward)
{
	m_cur_forward = cur_forward;
}


void UPlatformCameraSpring::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime)
{
	UpdatePlatformCameraLerp(bEnablePlatformCameraLerp, DeltaTime);
}

void UPlatformCameraSpring::UpdatePlatformCameraLerp(bool bDoPlatformLerp, float DeltaTime)
{
	FRotator DesiredRot = GetComponentRotation();

	FVector ArmOrigin = GetComponentLocation() + TargetOffset;

	FVector DesiredLoc = ArmOrigin;

	if (bEnablePlatformCameraLerp)
	{
		if (m_cur_forward.X > 0.f)
		{
			
		}
		else if (m_cur_forward.X < 0.f) {
			DesiredLoc = PreviousDesiredLoc;
		}
	}

	FVector LimitLocation = PreviousDesiredLoc + CameraChangeLimit;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bDrawPlatformCameraLerpMarkers)
	{
		DrawDebugSphere(GetWorld(), ArmOrigin, 5.f, 8, FColor::Green);
		//DrawDebugSphere(GetWorld(), DesiredLoc, 5.f, 8, FColor::Yellow);

		DrawDebugSphere(GetWorld(), LimitLocation, 5.f, 8, FColor::Red);
		

		//const FVector ToOrigin = ArmOrigin - DesiredLoc;
		//DrawDebugDirectionalArrow(GetWorld(), DesiredLoc, DesiredLoc + ToOrigin * 0.5f, 7.5f, bClampedDist ? FColor::Red : FColor::Green);
		//DrawDebugDirectionalArrow(GetWorld(), DesiredLoc + ToOrigin * 0.5f, ArmOrigin, 7.5f, bClampedDist ? FColor::Red : FColor::Green);
	}
#endif
	
	PreviousDesiredLoc = DesiredLoc;

	// Now offset camera position back along our rotation
	DesiredLoc -= DesiredRot.Vector() * TargetArmLength;
	// Add socket offset in local space
	DesiredLoc += FRotationMatrix(DesiredRot).TransformVector(SocketOffset);


	// Form a transform for new world transform for camera
	FTransform WorldCamTM(DesiredRot, DesiredLoc);
	// Convert to relative to component
	FTransform RelCamTM = WorldCamTM.GetRelativeTransform(ComponentToWorld);

	// Update socket location/rotation
	RelativeSocketLocation = RelCamTM.GetLocation();
	RelativeSocketRotation = RelCamTM.GetRotation();

	UpdateChildTransforms();
}

