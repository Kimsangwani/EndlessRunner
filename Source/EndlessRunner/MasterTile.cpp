// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterTile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Runner.h"
#include "EndlessRunnerGameModeBase.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "MasterObstacle.h"
#include "Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "HighObstacle.h"

// Sets default values
AMasterTile::AMasterTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = MasterTile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MasterTile"));

	TileDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TileDirection"));
	TileDirection->SetupAttachment(MasterTile);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(TileDirection);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftLane"));
	LeftLane->SetupAttachment(MasterTile);
	MiddleLane = CreateDefaultSubobject<UArrowComponent>(TEXT("MiddleLane"));
	MiddleLane->SetupAttachment(MasterTile);
	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("RightLane"));
	RightLane->SetupAttachment(MasterTile);
}

// Called when the game starts or when spawned
void AMasterTile::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMasterTile::OnBeginOverlap);

	HighObstacleCount = 0;

	for (int32 i = 1; i < 4; i++)
	{
		SpawnObstacle(i);
	}	

	//3Lane HighObstacle
	DuplicatePreventionHighObstacle();
}

// Called every frame
void AMasterTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMasterTile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunner* Runner = Cast<ARunner>(OtherActor);
	if (Runner)
	{
		AEndlessRunnerGameModeBase* GameMode = Cast<AEndlessRunnerGameModeBase>(GetWorld()->GetAuthGameMode());
		GameMode->SpawnTile();

		Runner->IncreaseMoveSpeed();

		UWorld* const World = GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(TileDestroyTimerHandle, this, &AMasterTile::TileDestroy, 1.f, false);
		}
	}
}

FVector AMasterTile::GetMeshBoxExtent()
{
	FVector BoxExtent = MasterTile->CalcBounds(MasterTile->GetComponentTransform()).BoxExtent;
	BoxExtent.Y = 0.0f;
	BoxExtent.Z = 0.0f;

	return BoxExtent;
}

void AMasterTile::TileDestroy()
{
	TArray<AActor*> FoundActors;
	this->GetAttachedActors(FoundActors);

	for (auto Actor : FoundActors)
	{
		AMasterObstacle* Obstacle = Cast<AMasterObstacle>(Actor);
		if (Obstacle)
		{
			Obstacle->Destroy();
		}

		APickup* Pickup = Cast<APickup>(Actor);
		if (Pickup)
		{
			Pickup->Destroy();
		}
	}

	Destroy();
}

void AMasterTile::SpawnObstacle(int32 LaneIndex)
{
	float Index = FMath::RandRange(0.0f, 1.0f);

	ARunner* Runner = Cast<ARunner>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!Runner)
	{
		return;
	}

	FVector Location = GetActorLocation();
	Location.Y = MasterTile->GetChildComponent(LaneIndex)->GetComponentLocation().Y;
	Location.Z = MasterTile->GetChildComponent(LaneIndex)->GetComponentLocation().Z;

	const FRotator Rotation = FRotator(0, 90.0f, 0);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	UWorld* const World = GetWorld();
	if (World)
	{
		if (Index <= 0.25f)
		{

		}
		else if (Index <= 0.5f)
		{
			if (MasterObstacleClass[0])
			{
				AMasterObstacle* Obstacle = World->SpawnActor<AMasterObstacle>(MasterObstacleClass[0], Location, Rotation, SpawnParams);
				if (Obstacle)
				{
					Obstacle->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
				}
			}
		}
		else if (Index <= 0.7f)
		{
			if (MasterObstacleClass[1])
			{
				AMasterObstacle* Obstacle = World->SpawnActor<AMasterObstacle>(MasterObstacleClass[1], Location, Rotation, SpawnParams);
				if (Obstacle)
				{
					HighObstacleCount++;
					Obstacle->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
				}
			}
		}
		else if (Index <= 0.95f)
		{
			if (PickupClass[0])
			{
				APickup* Coin = World->SpawnActor<APickup>(PickupClass[0], Location, Rotation, SpawnParams);
				if (Coin)
				{
					Coin->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
				}
			}
		}
		else if(Index <= 0.975f)
		{
			if (PickupClass[1])
			{
				APickup* Magnet = World->SpawnActor<APickup>(PickupClass[1], Location, Rotation, SpawnParams);
				if (Magnet)
				{
					Magnet->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
				}
			}
		}
		else
		{
			if (PickupClass[2])
			{
				APickup* JumpBoots = World->SpawnActor<APickup>(PickupClass[2], Location, Rotation, SpawnParams);
				if (JumpBoots)
				{
					JumpBoots->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
				}
			}
		}

	}
}

void AMasterTile::DuplicatePreventionHighObstacle()
{
	if (HighObstacleCount == 3)
	{
		TArray<AActor*> FoundActors;
		this->GetAttachedActors(FoundActors);

		for (auto Actor : FoundActors)
		{
			AHighObstacle* HighObstacle = Cast<AHighObstacle>(Actor);
			if (HighObstacle)
			{
				HighObstacles.Add(HighObstacle);
			}
		}

		int32 Index = FMath::RandRange(0, 2);
		HighObstacles[Index]->Destroy();
	}
}

