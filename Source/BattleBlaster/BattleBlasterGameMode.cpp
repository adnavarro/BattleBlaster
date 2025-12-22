// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleBlasterGameMode.h"

#include "BattleBlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "Tank.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> OutTowers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(),OutTowers);
	TowerCount = OutTowers.Num();
#if WITH_EDITOR
	UE_LOG(LogTemp, Display, TEXT("Tower Count: %d"), TowerCount);
#endif
	
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		if (ATank* FountTank = Cast<ATank>(PlayerPawn))
		{
			Tank = FountTank;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Game Mode: Tank Pawn not found!"));
		}
	}
	
	for (int32 i = 0; i < TowerCount; ++i)
	{
		if (ATower* Tower = Cast<ATower>(OutTowers[i]))
		{
			Tower->Tank = Tank;
#if WITH_EDITOR
			UE_LOG(LogTemp, Display, TEXT("%s Setting the tank variable"), *Tower->GetActorNameOrLabel());
#endif
		}
	}
	
	CountDownSeconds = CountDownDelay;
	GetWorldTimerManager().SetTimer(
		CountDownTimerHandle,
		this,
		&ABattleBlasterGameMode::OnCountDownTimerTimeout,
		1.f,
		true
	);
}

void ABattleBlasterGameMode::OnCountDownTimerTimeout()
{
	
	--CountDownSeconds;
	
	if (CountDownSeconds > 0)
	{
		UE_LOG(LogTemp, Display, TEXT("Game starts in: %d"), CountDownSeconds);
	}
	else if (CountDownSeconds == 0)
	{
		UE_LOG(LogTemp, Display, TEXT("GO!"));
		if (Tank.IsValid())
		{
			Tank->SetPlayerEnabled(true);
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountDownTimerHandle);
	}
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	
	bool bIsGameOver = false;
	
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		bIsGameOver = true;
	}
	else
	{
		if (auto DeadTower = Cast<ATower>(DeadActor))
		{
			DeadTower->HandleDestruction();
			if (--TowerCount == 0)
			{
				bIsGameOver = true;
				bIsVictory = true;
			}
		}
	}
	
	if (bIsGameOver)
	{
		FString GameOverString;
		bIsVictory ? GameOverString = TEXT("You Win!") : GameOverString = TEXT("You Lose!");
		UE_LOG(LogTemp, Display, TEXT("%s"), *GameOverString);
		
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			this,
			&ABattleBlasterGameMode::OnGameOverTimerTimeout,
			GameOverDelay,
			false
		);
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UBattleBlasterGameInstance* BBGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		if (BBGameInstance)
		{
			if (bIsVictory)
			{
				BBGameInstance->LoadNextLevel();
			}
			else
			{
				BBGameInstance->RestartCurrentLevel();
			}
		}
	}
}

