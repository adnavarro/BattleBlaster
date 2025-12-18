// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
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
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		//Tank->HandleDestruction();
		UE_LOG(LogTemp, Display, TEXT("Game Over! You Lose!"));
	}
	else
	{
		if (auto DeadTower = Cast<ATower>(DeadActor))
		{
			//DeadTower->HandleDestruction();
			DeadTower->Destroy();
			UE_LOG(LogTemp, Display, TEXT("Tower Destroyed!"));
			if (--TowerCount == 0)
			{
				UE_LOG(LogTemp, Display, TEXT("Game Over! You Win!"));
			}
		}
	}
}
