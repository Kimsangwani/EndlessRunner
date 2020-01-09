// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessRunnerGameModeBase.generated.h"

/**
 * 
 */

UCLASS()
class ENDLESSRUNNER_API AEndlessRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AEndlessRunnerGameModeBase();
	void EndGame();
	void SpawnTile();

	int ScoreMultiplier;

	UFUNCTION(BlueprintPure, Category = "UMG")
	int32 GetScore() const;
	UFUNCTION(BlueprintPure, Category = "UMG")
	int32 GetCoin() const;
	UFUNCTION(BlueprintPure, Category = "UMG")
	int32 GetHighScore();

	UFUNCTION(BlueprintCallable, Category = "UMG")
	void MoveLevel(FName Level);

	UFUNCTION(BlueprintCallable, Category = "UMG")
	void GamePause();

	void SetScore(int32 _Score);
	void AddScore(int32 _Score);

	void AddCoin(int32 _Coin);

	void SetHighScore(int32 _HighScore);

	void SetNextSpawnPoint(FTransform Transform);

	void SaveScore();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
	TSubclassOf<class AMasterTile> MasterTileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<class UUserWidget> HUDWidgetClass;
	class UUserWidget* HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	class UUserWidget* GameOverWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<class UUserWidget> GamePauseWidgetClass;
	class UUserWidget* GamePauseWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	int32 Score;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	int32 Coin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	int32 HighScore;

	FTransform NextSpawnPoint;

private:

	virtual void BeginPlay() override;

	APlayerController* Controller;

	FTimerHandle ScoreTimerHandle;
	void IncreaseScore();

	void LoadScore();

	void DoesSaveGameExist();
};
