// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EndlessRunnerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API UEndlessRunnerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UEndlessRunnerSaveGame();

	UPROPERTY(VisibleAnywhere, Category = "Setup" )
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
	int32 SaveIndex;

	UPROPERTY(VisibleAnywhere, Category = "Setup")
	int32 HighScore;


};
