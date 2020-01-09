// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"


AMenuGameModeBase::AMenuGameModeBase()
{

}

void AMenuGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Controller = GetWorld()->GetFirstPlayerController();

	FInputModeUIOnly InputMode;
	Controller->SetInputMode(InputMode);
	Controller->bShowMouseCursor = true;

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
}

void AMenuGameModeBase::MoveLevel(FName Level)
{
	UGameplayStatics::OpenLevel(this, Level, true);
}

void AMenuGameModeBase::QuitGame()
{
	FGenericPlatformMisc::RequestExit(false);
}
