// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/TimelineComponent.h"
#include "Runner.generated.h"

UCLASS()
class ENDLESSRUNNER_API ARunner : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARunner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	class UStaticMeshComponent* Runner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setup")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Floats")
	float MoveSpeed;

	FVector CurrentVelocity;

protected:

	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* fCurve;

	UFUNCTION()
	void MoveToLane(float Value);

	UFUNCTION()
	void SetLane();

private:

	//Movea and Jump
	void MoveRight();
	void MoveLeft();
	void DownJump();

	void Jump();
	bool IsJumping();

	int32 Lane;
	int32 NewLane;
	float LaneY[3];

	const int32 RIGHT_LANE = 2;
	const int32 LEFT_LANE = 0;
	const float MIN_SPEED = 500.0f;
	const float MAX_SPEED = 2000.0f;
	float JumpSpeed = 600.0f;

	//Pickup
	const int32 MAGNET = 0x00000001;
	const int32 PowerJump = 0x00000002;
	int32 Buff;

	FTimerHandle ActiveBuffHandle;
	void UnActiveMagnetBuff();
	void UnActivePowerJumpBuff();

	//GameContol
	void GamePause();

	//Slide Control
	FVector2D previousTouchLocation;
	FVector2D newTouchLocation;

	void OnTouchBegin(ETouchIndex::Type touchType, FVector newTouchLocation);
	void OnTouching();

	class APlayerController* PlayerController;

public:

	class AEndlessRunnerGameModeBase* GameMode;

	class UTimelineComponent* MyTimeline;
	FOnTimelineFloat InterpFunction{};
	FOnTimelineEvent TimelineFinished{};

	void IncreaseMoveSpeed();

	void Death();

	void SetBuff(int32 _Buff);
	int32 GetBuff();

	void PowerUpJumpBoots();
	void ResetJumpVelocity();

};
