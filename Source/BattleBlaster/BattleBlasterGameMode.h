// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleBlasterGameMode.generated.h"


class ATank;
/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere)
	float GameOverDelay = 3.f;
	
	UPROPERTY(VisibleAnywhere)
	bool bIsVictory = false;
	
	UPROPERTY(EditAnywhere)
	int32 CountDownDelay = 3;
	
	int32 CountDownSeconds;
	
	FTimerHandle CountDownTimerHandle;
	
	TWeakObjectPtr<ATank> Tank;
	int32 TowerCount;
	
	void ActorDied(AActor* DeadActor);
	
	void OnGameOverTimerTimeout();
	void OnCountDownTimerTimeout();
};
