// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Runner.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "EndlessRunnerGameModeBase.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"

ACoin::ACoin()
{
	AttractedComponent = CreateDefaultSubobject<USphereComponent>(TEXT("AttractedComponent"));
	AttractedComponent->SetupAttachment(RootComponent);

	AttractedTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	AttractedInterp.BindUFunction(this, FName("AttractedToRunner"));
	//TimelineFinished.BindUFunction(this, FName("SetLane"));
}

void ACoin::BeginPlay()
{
	Super::BeginPlay();

	Pickup->AddRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnBeginOverlap);

	AttractedComponent->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnBeginOverlapAttracted);

	if (fAttractedToRunner)
	{
		AttractedTimeline->AddInterpVector(fAttractedToRunner, AttractedInterp, FName("Slide"));

		AttractedTimeline->SetLooping(false);
		AttractedTimeline->SetIgnoreTimeDilation(false);
	}
}

void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Pickup->AddWorldRotation(FRotator(0.0f, 1.0f, 0.0f));
}

void ACoin::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunner* Runner = Cast<ARunner>(OtherActor);
	if (Runner)
	{
		AEndlessRunnerGameModeBase* GameMode = Runner->GameMode;
		if (GameMode)
		{
			GameMode->AddScore(COIN_SCORE);
			GameMode->AddCoin(1);
		}

		if (Effect)
		{
			UGameplayStatics::PlaySound2D(this, Effect);
		}

		PickupDestory();
	}
}

void ACoin::OnBeginOverlapAttracted(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunner* Runner = Cast<ARunner>(OtherActor);
	if (Runner)
	{
		if (Runner->GetBuff() == 1)
		{
			AttractedTimeline->PlayFromStart();
		}
	}
}

void ACoin::PickupDestory()
{
	Super::PickupDestory();
}

void ACoin::AttractedToRunner(FVector Slide)
{
	FVector Location = GetActorLocation();

	ARunner* Runner = Cast<ARunner>(GetWorld()->GetFirstPlayerController()->GetPawn());
	FVector RunnerLocation = Runner->GetActorLocation();

	FVector ToLocation = FMath::VInterpTo(Location, RunnerLocation, GetWorld()->DeltaTimeSeconds, MAGNET_TIME);

	SetActorLocation(Slide + ToLocation);
}
