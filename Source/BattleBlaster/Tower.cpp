// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"
#include "Tank.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsInFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
	
}

void ATower::CheckFireCondition()
{
	if (Tank.IsValid() && IsInFireRange()) 
	{
		Fire();
	}
}

bool ATower::IsInFireRange()
{
	if (Tank.IsValid() && Tank->bPlayerIsAlive)
	{
		float DistanceToTank = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		return (DistanceToTank <= FireRange);
	}
	return false;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
	UE_LOG(LogTemp, Display, TEXT("Tower Destroyed!"));
}
