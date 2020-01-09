// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRunnerGameModeBase.h"
#include "MasterTile.h"
#include "Engine/World.h"
#include "Components/ArrowComponent.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "EndlessRunnerSaveGame.h"
#include "Kismet/GameplayStatics.h"

AEndlessRunnerGameModeBase::AEndlessRunnerGameModeBase()
{
	Coin = 0;
	Score = 0;
	ScoreMultiplier = 1;
}

void AEndlessRunnerGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 10; i++)
	{
		SpawnTile();
	}

	Controller = GetWorld()->GetFirstPlayerController();

	FInputModeGameOnly InputMode;
	Controller->SetInputMode(InputMode);
	Controller->bShowMouseCursor = false;

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}
	
	DoesSaveGameExist();

	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(ScoreTimerHandle, this, &AEndlessRunnerGameModeBase::IncreaseScore, 0.5f, true);
	}
}

void AEndlessRunnerGameModeBase::IncreaseScore()
{
	SetScore(Score + (ScoreMultiplier * 1));
}

void AEndlessRunnerGameModeBase::LoadScore()
{
	UEndlessRunnerSaveGame* EndlessRunnerSaveGame = Cast<UEndlessRunnerSaveGame>(UGameplayStatics::CreateSaveGameObject(UEndlessRunnerSaveGame::StaticClass()));

	if (EndlessRunnerSaveGame)
	{
		EndlessRunnerSaveGame->SaveSlotName = "HighScoreSave";
		EndlessRunnerSaveGame->SaveIndex = 0;

		EndlessRunnerSaveGame = Cast<UEndlessRunnerSaveGame>(UGameplayStatics::LoadGameFromSlot(EndlessRunnerSaveGame->SaveSlotName, EndlessRunnerSaveGame->SaveIndex));

		HighScore = EndlessRunnerSaveGame->HighScore;	
	}
}

void AEndlessRunnerGameModeBase::SaveScore()
{
	UEndlessRunnerSaveGame* EndlessRunnerSaveGame = Cast<UEndlessRunnerSaveGame>(UGameplayStatics::CreateSaveGameObject(UEndlessRunnerSaveGame::StaticClass()));

	if (EndlessRunnerSaveGame)
	{
		EndlessRunnerSaveGame->SaveSlotName = "HighScoreSave";
		EndlessRunnerSaveGame->SaveIndex = 0;

		if (EndlessRunnerSaveGame->HighScore < GetScore())
		{
			EndlessRunnerSaveGame->HighScore = GetScore();	
		}	

		UGameplayStatics::SaveGameToSlot(EndlessRunnerSaveGame, EndlessRunnerSaveGame->SaveSlotName, EndlessRunnerSaveGame->SaveIndex);
	}
}

void AEndlessRunnerGameModeBase::DoesSaveGameExist()
{	
	LoadScore();	
}

void AEndlessRunnerGameModeBase::SpawnTile()
{
	if (MasterTileClass)
	{
		FVector Location = NextSpawnPoint.GetLocation();
		const FRotator Rotation = FRotator(0, 0, 0);

		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			AMasterTile* MasterTile = World->SpawnActor<AMasterTile>(MasterTileClass, Location, Rotation, SpawnParams);
			
			if (MasterTile)
			{
				UArrowComponent* TileDirection = MasterTile->TileDirection;
				FVector TileLocation = TileDirection->GetComponentLocation() + MasterTile->GetMeshBoxExtent();
				
				NextSpawnPoint.SetLocation(TileLocation);
			}
		}
	}
}

int32 AEndlessRunnerGameModeBase::GetScore() const
{
	return Score;
}

int32 AEndlessRunnerGameModeBase::GetCoin() const
{
	return Coin;
}

int32 AEndlessRunnerGameModeBase::GetHighScore()
{
	UEndlessRunnerSaveGame* EndlessRunnerSaveGame = Cast<UEndlessRunnerSaveGame>(UGameplayStatics::CreateSaveGameObject(UEndlessRunnerSaveGame::StaticClass()));

	if (EndlessRunnerSaveGame)
	{
		return EndlessRunnerSaveGame->HighScore;
	}

	return 0;
}

void AEndlessRunnerGameModeBase::GamePause()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().PauseTimer(ScoreTimerHandle);
	}

	if (GamePauseWidgetClass)
	{
		if (!GamePauseWidget)
		{
			GamePauseWidget = CreateWidget<UUserWidget>(GetWorld(), GamePauseWidgetClass);
		}

		if (!GamePauseWidget->IsVisible())
		{
			GamePauseWidget->AddToViewport();

			FInputModeGameAndUI InputMode;
			Controller->SetInputMode(InputMode);
			Controller->bShowMouseCursor = true;
			Controller->SetPause(true);
		}
		else
		{
			GamePauseWidget->RemoveFromParent();

			FInputModeGameOnly InputMode;
			Controller->SetInputMode(InputMode);
			Controller->bShowMouseCursor = false;
			Controller->SetPause(false);
		}
	}
}

void AEndlessRunnerGameModeBase::SetScore(int32 _Score)
{
	Score = _Score;
}

void AEndlessRunnerGameModeBase::AddScore(int32 _Score)
{
	Score += _Score;
}

void AEndlessRunnerGameModeBase::AddCoin(int32 _Coin)
{
	Coin += _Coin;
}

void AEndlessRunnerGameModeBase::SetHighScore(int32 _HighScore)
{
}

void AEndlessRunnerGameModeBase::SetNextSpawnPoint(FTransform Transform)
{
	NextSpawnPoint = Transform;
}

void AEndlessRunnerGameModeBase::MoveLevel(FName Level)
{
	UGameplayStatics::OpenLevel(this, Level, true);
}

void AEndlessRunnerGameModeBase::EndGame()
{
	UWorld* const World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(ScoreTimerHandle);
	}

	if (HUDWidget)
	{	
		HUDWidget->RemoveFromParent();

		if (GameOverWidgetClass)
		{
			GameOverWidget = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
			if (GameOverWidget)
			{
				GameOverWidget->AddToViewport();

				FInputModeUIOnly InputMode;
				Controller->SetInputMode(InputMode);
				Controller->bShowMouseCursor = true;
			}
		}
	}
}