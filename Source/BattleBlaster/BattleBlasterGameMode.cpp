// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "Tank.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	
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
	
	TArray<AActor*> OutTowers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(),OutTowers);
	TowerCount = OutTowers.Num();
#if WITH_EDITOR
	UE_LOG(LogTemp, Display, TEXT("Tower Count: %d"), TowerCount);
#endif
}
