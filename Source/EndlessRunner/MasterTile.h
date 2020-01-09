// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasterTile.generated.h"

UCLASS()
class ENDLESSRUNNER_API AMasterTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	class UStaticMeshComponent* MasterTile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	TArray<TSubclassOf<class AMasterObstacle>> MasterObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	TArray<TSubclassOf<class APickup>> PickupClass;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	class UArrowComponent* TileDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	class UArrowComponent* LeftLane;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	class UArrowComponent* MiddleLane;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	class UArrowComponent* RightLane;

	FVector GetMeshBoxExtent();

private:

	FTimerHandle TileDestroyTimerHandle;

	void TileDestroy();

	void SpawnObstacle(int32 LaneIndex);

	TArray<class AHighObstacle*> HighObstacles;
	int32 HighObstacleCount;
	void DuplicatePreventionHighObstacle();
};
