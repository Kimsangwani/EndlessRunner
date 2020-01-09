// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Magnet.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AMagnet : public APickup
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMagnet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	virtual void PickupDestory() override;

	const int32 MAGNET = 0x00000001;

};
