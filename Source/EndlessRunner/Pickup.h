// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class ENDLESSRUNNER_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	class UStaticMeshComponent* Pickup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	class USoundWave* Effect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	class UBoxComponent* BoxComponent;

	virtual void PickupDestory();
};
