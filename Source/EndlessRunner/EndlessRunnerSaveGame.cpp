// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRunnerSaveGame.h"


UEndlessRunnerSaveGame::UEndlessRunnerSaveGame()
{
	PlayerName = TEXT("Runner");;
	SaveSlotName = TEXT("HighScoreSave");
	SaveIndex = 0;
}