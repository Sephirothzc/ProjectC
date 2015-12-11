// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtraCameraPrivatePCH.h"

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

	CameraLerpSpeed = 5.f;
	CameraLerpMaxDistance = 50;
	CameraLerpTolerance = 5.f;

	m_cur_camera_state = PLATFORM_CAMERA_STATE_NONE;
}

void UPlatformCameraSpring::BeginPlay() {
	m_pre_camera_location = GetComponentLocation() + TargetOffset;
	ChangeCameraState(PLATFORM_CAMERA_STATE_RIGHT);
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
	if (bEnablePlatformCameraLerp) {
		UpdatePlatformCameraLerp(bEnablePlatformCameraLerp, DeltaTime);
	}
	else {
		Super::UpdateDesiredArmLocation(bDoTrace, bDoLocationLag, bDoRotationLag, DeltaTime);
	}
}

void UPlatformCameraSpring::UpdatePlatformCameraLerp(bool bDoPlatformLerp, float DeltaTime)
{
	FRotator DesiredRot = GetComponentRotation();
	FVector ArmOrigin = GetComponentLocation() + TargetOffset;
	FVector DesiredLoc = ArmOrigin;
	
	FVector LimitPoint = m_pre_camera_location + CameraChangeLimit;
	FVector FollowPoint = m_pre_camera_location + CameraFollowLimit;

	bool clamped_dist = false;
	if (bDoPlatformLerp) {
		DesiredLoc.X = CalcCameraPosX(FollowPoint, LimitPoint, DeltaTime);

		float FromOriginZ = DesiredLoc.Z - m_pre_camera_location.Z;
		if (FMath::Abs(FromOriginZ) > CameraLerpMaxDistance) {
			if (FromOriginZ < 0) {
				FromOriginZ = -CameraLerpMaxDistance;
			}
			else {
				FromOriginZ = CameraLerpMaxDistance;
			}
			DesiredLoc.Z = DesiredLoc.Z - FromOriginZ;
			clamped_dist = true;
		}
		else {
			DesiredLoc.Z = FMath::FInterpTo(m_pre_camera_location.Z, DesiredLoc.Z, DeltaTime, CameraLerpSpeed);
		}
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bDrawPlatformCameraLerpMarkers)
	{
		FVector ActorOrigin = GetComponentLocation();
		DrawDebugSphere(GetWorld(), ActorOrigin, 5.f, 8, FColor::Green);
		DrawDebugSphere(GetWorld(), FollowPoint, 5.f, 8, FColor::Yellow);
		DrawDebugSphere(GetWorld(), LimitPoint, 5.f, 8, FColor::Red);

		DrawDebugLine(GetWorld(), FVector(LimitPoint.X, LimitPoint.Y, -10000.f), FVector(LimitPoint.X, LimitPoint.Y, 10000.f), FColor::Red);
		DrawDebugLine(GetWorld(), FVector(FollowPoint.X, LimitPoint.Y, -10000.f), FVector(FollowPoint.X, LimitPoint.Y, 10000.f), FColor::Yellow);

		const FVector ToOrigin = FollowPoint - ActorOrigin;
		DrawDebugDirectionalArrow(GetWorld(), ActorOrigin, ActorOrigin + ToOrigin * 0.5f, 7.5f, clamped_dist ? FColor::Red : FColor::Green);
		DrawDebugDirectionalArrow(GetWorld(), ActorOrigin + ToOrigin * 0.5f, FollowPoint, 7.5f, clamped_dist ? FColor::Red : FColor::Green);
	}
#endif
	
	m_pre_camera_location = DesiredLoc;

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

void UPlatformCameraSpring::EnterCameraState(PLATFORM_CAMERA_STATE next_state) {
	switch (next_state)
	{
	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_LEFT:
		m_is_forward_left = false;
		break;

	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_RIGHT:
		m_is_forward_left = true;
		break;

	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_MOVE_LEFT:
		TargetOffset = -TargetOffset;
		CameraChangeLimit = -CameraChangeLimit;
		CameraFollowLimit = -CameraFollowLimit;
		m_camera_move_nearly_attach = false;
		break;

	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_MOVE_RIGHT:
		TargetOffset = -TargetOffset;
		CameraChangeLimit = -CameraChangeLimit;
		CameraFollowLimit = -CameraFollowLimit;
		m_camera_move_nearly_attach = false;
		break;

	default:
		break;
	}
	m_cur_camera_state = next_state;
}

void UPlatformCameraSpring::ExitCameraState(PLATFORM_CAMERA_STATE cur_state) {
	switch (cur_state)
	{
	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_LEFT:
		
		break;

	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_RIGHT:
		
		break;

	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_MOVE_LEFT:
		break;

	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_MOVE_RIGHT:
		break;

	default:
		break;
	}
}

void UPlatformCameraSpring::ChangeCameraState(PLATFORM_CAMERA_STATE next_state) {
	ExitCameraState(m_cur_camera_state);
	EnterCameraState(next_state);
}

float UPlatformCameraSpring::CalcCameraPosX(const FVector& follow_point, const FVector& limit_point, float delta_seconds) {
	float ActorLocationX = GetComponentLocation().X;
	float CameraLocationX = (GetComponentLocation() + TargetOffset).X;

	switch (m_cur_camera_state)
	{
	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_LEFT: 
	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_RIGHT: {
		bool follow_check = false;
		bool limit_check = false;
		if (m_is_forward_left) {
			follow_check = follow_point.X <= ActorLocationX;
			limit_check = limit_point.X > ActorLocationX;
		}
		else {
			follow_check = follow_point.X >= ActorLocationX;
			limit_check = limit_point.X < ActorLocationX;
		}
		if (m_is_cur_owner_fowrard_left == m_is_forward_left) {
			if (follow_check) {
				m_is_attach_follow_point = true;
			}
			else {
				m_is_attach_follow_point = false;
				CameraLocationX = m_pre_camera_location.X;
			}
		}
		else {
			if (limit_check) {
				if (m_is_forward_left) {
					ChangeCameraState(PLATFORM_CAMERA_STATE_MOVE_LEFT);
				}
				else {
					ChangeCameraState(PLATFORM_CAMERA_STATE_MOVE_RIGHT);
				}
				CameraLocationX = m_pre_camera_location.X;
			}
			else {
				m_is_attach_follow_point = false;
				CameraLocationX = m_pre_camera_location.X;
			}
		}
		break;
	}

	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_MOVE_LEFT:
	case UPlatformCameraSpring::PLATFORM_CAMERA_STATE_MOVE_RIGHT: {
		FVector target_location = GetComponentLocation() + TargetOffset;
		CameraLocationX = FMath::FInterpTo(m_pre_camera_location.X, target_location.X, delta_seconds, CameraLerpSpeed);

		const float FromOriginX = CameraLocationX - target_location.X;
		if (!m_camera_move_nearly_attach) {
			if (FMath::Abs(FromOriginX) < CameraLerpMaxDistance) {
				m_camera_move_nearly_attach = true;
			}
		}
		else {
			if (FMath::Abs(FromOriginX) > CameraLerpMaxDistance) {
				CameraLocationX = target_location.X + CameraLerpMaxDistance;
			}
		}

		if (FMath::Abs(target_location.X - CameraLocationX) < CameraLerpTolerance) {
			if (m_cur_camera_state == PLATFORM_CAMERA_STATE_MOVE_LEFT) {
				ChangeCameraState(PLATFORM_CAMERA_STATE_LEFT);
			}
			if (m_cur_camera_state == PLATFORM_CAMERA_STATE_MOVE_RIGHT) {
				ChangeCameraState(PLATFORM_CAMERA_STATE_RIGHT);
			}
		}

		break;
	}

	default:
		break;
	}

	return CameraLocationX;
}