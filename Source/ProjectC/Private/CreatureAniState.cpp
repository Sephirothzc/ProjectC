// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectC.h"
#include "CreatureAniState.h"


// Sets default values for this component's properties
UCreatureAniState::UCreatureAniState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCreatureAniState::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCreatureAniState::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

