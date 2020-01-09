// Fill out your copyright notice in the Description page of Project Settings.


#include "RunnerMovementComponent.h"
#include "Runner.h"
#include "Engine/World.h"

void URunnerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}


}


