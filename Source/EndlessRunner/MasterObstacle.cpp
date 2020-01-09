// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterObstacle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Runner.h"

// Sets default values
AMasterObstacle::AMasterObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = Obstacle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Obstacle"));

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMasterObstacle::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMasterObstacle::OnBeginOverlap);
}

void AMasterObstacle::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARunner* Runner = Cast<ARunner>(OtherActor);
	if (Runner)
	{
		Runner->Death();
		BoxComponent->SetGenerateOverlapEvents(false);
	}
}

float AMasterObstacle::GetMeshZLenght()
{
	FVector BoxExtent = Obstacle->CalcBounds(Obstacle->GetComponentTransform()).BoxExtent;

	//(LogTemp, Warning, TEXT("%f"), BoxExtent.Z);

	return BoxExtent.Z;
}

// Called every frame
void AMasterObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

