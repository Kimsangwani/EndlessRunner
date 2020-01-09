// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
private:

	virtual void BeginPlay() override;

public:

	AMenuGameModeBase();

protected:
	UFUNCTION(BlueprintCallable, Category = "UMG")
	void MoveLevel(FName Level);

	UFUNCTION(BlueprintCallable, Category = "UMG")
	void QuitGame();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<class UUserWidget> HUDWidgetClass;
	class UUserWidget* HUDWidget;

	APlayerController* Controller;


};
