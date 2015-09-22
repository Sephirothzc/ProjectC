// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectC.h"
#include "PlatformCameraSpring.h"

UPlatformCameraSpring::UPlatformCameraSpring(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (GetOwner() != nullptr) {
		if (GetOwner()->GetActorForwardVector().X > 0.f) {
			FVector owner_location = GetOwner()->GetActorLocation();
			m_is_forward_left = true;
		}
		else {
			m_is_forward_left = false;
		}
		m_is_cur_owner_fowrard_left = m_is_forward_left;
	}
	CameraChangeLimit = FVector(0.f, 0.f, 0.f);
	CameraFollowLimit = FVector(0.f, 0.f, 0.f);
	m_is_attach_follow_point = true;
}

void UPlatformCameraSpring::OnTurnRound(FVector cur_forward)
{
	if (cur_forward.X > 0.f) {
		m_is_cur_owner_fowrard_left = true;
	}
	else {
		m_is_cur_owner_fowrard_left = false;
	}
}


void UPlatformCameraSpring::UpdateDesiredArmLocation(bool bDoTrace, bool bDoLocationLag, bool bDoRotationLag, float DeltaTime)
{
	UpdatePlatformCameraLerp(bEnablePlatformCameraLerp, DeltaTime);
}

void UPlatformCameraSpring::UpdatePlatformCameraLerp(bool bDoPlatformLerp, float DeltaTime)
{
	FRotator DesiredRot = GetComponentRotation();

	FVector ArmOrigin = GetComponentLocation() + TargetOffset;

	FVector ActorOrigin = GetComponentLocation();

	FVector DesiredLoc = ArmOrigin;

	FVector FollowPoint;
	FVector LimitPoint = DesiredLoc + CameraChangeLimit;

	if (m_is_attach_follow_point) {
		FollowPoint = DesiredLoc + CameraFollowLimit;
	}
	else {
		FollowPoint = PreviousDesiredLoc + CameraFollowLimit;
	}

	if (bEnablePlatformCameraLerp) {
		if (m_is_cur_owner_fowrard_left == m_is_forward_left) {
			if (FollowPoint.X <= ActorOrigin.X + 1.f) {
				m_is_attach_follow_point = true;
			}
			else {
				m_is_attach_follow_point = false;
				DesiredLoc.X = PreviousDesiredLoc.X;
			}
		}
		else {
			if (LimitPoint.X > ActorOrigin.X + 1) {
				TargetOffset = -TargetOffset;
				m_is_forward_left = !m_is_forward_left;
			}
			else {
				m_is_attach_follow_point = false;
				DesiredLoc.X = PreviousDesiredLoc.X;
			}
		}
	}



#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bDrawPlatformCameraLerpMarkers)
	{
		DrawDebugSphere(GetWorld(), ActorOrigin, 5.f, 8, FColor::Green);
		DrawDebugSphere(GetWorld(), FollowPoint, 5.f, 8, FColor::Yellow);

		DrawDebugSphere(GetWorld(), LimitPoint, 5.f, 8, FColor::Red);
		

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

