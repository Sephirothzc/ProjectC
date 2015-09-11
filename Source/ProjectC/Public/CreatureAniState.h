// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CreatureAniState.generated.h"

UCLASS(ClassGroup = AniState, abstract, Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTC_API UCreatureAniState : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCreatureAniState();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CreatureAniState")
	virtual bool EnterState();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CreatureAniState")
	virtual bool ExitState();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "CreatureAniState")
	virtual bool UpdateState(float delta_seconds);
	
};
