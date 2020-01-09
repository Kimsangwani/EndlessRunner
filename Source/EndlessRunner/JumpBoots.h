// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "JumpBoots.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AJumpBoots : public APickup
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

public:

	AJumpBoots();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	virtual void PickupDestory() override;
	const int32 PowerJump = 0x00000002;

};
