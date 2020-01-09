// Fill out your copyright notice in the Description page of Project Settings.


#include "Runner.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "EndlessRunnerGameModeBase.h"
#include "MasterTile.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine/World.h"

// Sets default values
ARunner::ARunner()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = Runner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Runner"));
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Runner);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	LaneY[0] = -300.0f;
	LaneY[1] = 0.0f;
	LaneY[2] = 300.0f;

	MyTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	InterpFunction.BindUFunction(this, FName("MoveToLane"));
	TimelineFinished.BindUFunction(this, FName("SetLane"));

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ARunner::BeginPlay()
{
	Super::BeginPlay();

	Runner->SetSimulatePhysics(true);
	GameMode = Cast<AEndlessRunnerGameModeBase>(GetWorld()->GetAuthGameMode());

	PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	MoveSpeed = 500.f;

	//Timeline
	if (fCurve)
	{
		MyTimeline->AddInterpFloat(fCurve, InterpFunction, FName("LaneY"));
		MyTimeline->SetTimelineFinishedFunc(TimelineFinished);

		Lane = 1;
		NewLane = 1;

		MyTimeline->SetLooping(false);
		MyTimeline->SetIgnoreTimeDilation(true);
	}
}

// Called every frame
void ARunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Runner Move
	CurrentVelocity.X = MoveSpeed;
	if (!CurrentVelocity.IsZero())
	{
		FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
		SetActorLocation(NewLocation);
	}
}

// Called to bind functionality to input
void ARunner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARunner::MoveRight);
	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARunner::MoveLeft);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunner::Jump);
	PlayerInputComponent->BindAction("DownJump", IE_Pressed, this, &ARunner::DownJump);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ARunner::GamePause);

	InputComponent->BindTouch(IE_Pressed, this, &ARunner::OnTouchBegin);
}

void ARunner::MoveToLane(float Value)
{
	FVector Location = GetActorLocation();
	float LocationY = FMath::Lerp(LaneY[Lane], LaneY[NewLane], Value);
	SetActorLocation(FVector(Location.X, LocationY, Location.Z));
}

void ARunner::SetLane()
{
	Lane = NewLane;
}

void ARunner::MoveLeft()
{
	NewLane = FMath::Clamp(Lane - 1, LEFT_LANE, RIGHT_LANE);
	GameMode->AddScore(-1);

	MyTimeline->PlayFromStart();
}

void ARunner::DownJump()
{
	Runner->AddImpulse(FVector(0.0f, 0.0f, JumpSpeed * -2.0f), "", true);
}

void ARunner::MoveRight()
{
	NewLane = FMath::Clamp(Lane + 1, LEFT_LANE, RIGHT_LANE);
	GameMode->AddScore(-1);

	MyTimeline->PlayFromStart();
}

void ARunner::Jump()
{
	if(GetVelocity().Z < 0.0f)
	{
		Runner->AddImpulse(FVector(0.0f, 0.0f, JumpSpeed), "", true);
	}
}

bool ARunner::IsJumping()
{


	return false;
}

void ARunner::GamePause()
{
	GameMode->GamePause();
}

void ARunner::OnTouchBegin(ETouchIndex::Type touchType, FVector newTouchLocation)
{
	bool IsPressed;
	PlayerController->GetInputTouchState(ETouchIndex::Touch1, previousTouchLocation.X, previousTouchLocation.Y, IsPressed);
}

void ARunner::OnTouching()
{
	bool IsPressed;
	PlayerController->GetInputTouchState(ETouchIndex::Touch1, previousTouchLocation.X, previousTouchLocation.Y, IsPressed);

	if (IsPressed)
	{
		FVector2D DeltaTouchLocation = newTouchLocation - previousTouchLocation;

		DeltaTouchLocation.X *= -1.0f;

		FVector DeltaOffset = FVector(DeltaTouchLocation.X, 0.0f, DeltaTouchLocation.Y);
		
		this->AddActorWorldOffset(DeltaOffset);

		previousTouchLocation = newTouchLocation;
	}
}

void ARunner::IncreaseMoveSpeed()
{
	MoveSpeed = FMath::Clamp(MoveSpeed + 10.0f, MIN_SPEED, MAX_SPEED);
}

void ARunner::Death()
{
	MoveSpeed = 0;

	GameMode->SaveScore();
	GameMode->EndGame();
}

void ARunner::SetBuff(int32 _Buff)
{
	Buff = _Buff;

	if (GetWorld())
	{
		if (_Buff == MAGNET)
		{
			GetWorld()->GetTimerManager().SetTimer(ActiveBuffHandle, this, &ARunner::UnActiveMagnetBuff, 10.0f, true);
		}
		else if (_Buff == PowerJump)
		{
			PowerUpJumpBoots();
			GetWorld()->GetTimerManager().SetTimer(ActiveBuffHandle, this, &ARunner::PowerUpJumpBoots, 5.0f, true);
		}
	}
}

int32 ARunner::GetBuff()
{
	return Buff;
}

void ARunner::UnActiveMagnetBuff()
{
	Buff -= MAGNET;
}

void ARunner::UnActivePowerJumpBuff()
{
	Buff -= PowerJump;
}

void ARunner::PowerUpJumpBoots()
{
	JumpSpeed = 1000.0f;

	if (GetWorld())
	{	
		GetWorld()->GetTimerManager().SetTimer(ActiveBuffHandle, this, &ARunner::ResetJumpVelocity, 5.0f, true);
	}
}

void ARunner::ResetJumpVelocity()
{
	JumpSpeed = 500.0f;
}
