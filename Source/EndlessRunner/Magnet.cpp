// Fill out your copyright notice in the Description page of Project Settings.


#include "Magnet.h"
#include "Runner.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "EndlessRunnerGameModeBase.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"

AMagnet::AMagnet()
{

}

void AMagnet::BeginPlay()
{
	Super::BeginPlay();

	Pickup->AddRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	Pickup->SetWorldRotation(FRotator(0.0f, 0.0f, -90.0f));

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMagnet::OnBeginOverlap);
}

void AMagnet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagnet::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunner* Runner = Cast<ARunner>(OtherActor);
	if (Runner)
	{
		if (Effect)
		{
			//UGameplayStatics::PlaySound2D(this, Effect);
		}

		Runner->SetBuff(MAGNET);

		PickupDestory();
	}
}

void AMagnet::PickupDestory()
{
	Super::PickupDestory();
}
