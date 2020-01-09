// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Components/TimelineComponent.h"
#include "Coin.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API ACoin : public APickup
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACoin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	class USphereComponent* AttractedComponent;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveVector* fAttractedToRunner;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBeginOverlapAttracted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	virtual void PickupDestory() override;

	const int32 COIN_SCORE = 10;
	const int32 MAGNET_TIME = 15;

	class UTimelineComponent* AttractedTimeline;
	FOnTimelineVector AttractedInterp{};
	//FOnTimelineEvent TimelineFinished{};

	UFUNCTION()
	void AttractedToRunner(FVector Slide);

};
