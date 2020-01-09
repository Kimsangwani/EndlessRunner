// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpBoots.h"
#include "Runner.h"
#include "EndlessRunnerGameModeBase.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


AJumpBoots::AJumpBoots()
{

}

void AJumpBoots::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AJumpBoots::OnBeginOverlap);

	Pickup->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));
	Pickup->AddRelativeLocation(FVector(0.0f, 50.0f, 0.0f));
}

void AJumpBoots::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJumpBoots::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunner* Runner = Cast<ARunner>(OtherActor);
	if (Runner)
	{
		Runner->PowerUpJumpBoots();

		if (Effect)
		{
			UGameplayStatics::PlaySound2D(this, Effect);
		}

		Runner->SetBuff(PowerJump);
		PickupDestory();
	}
}

void AJumpBoots::PickupDestory()
{
	Super::PickupDestory();

}