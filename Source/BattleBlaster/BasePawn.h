// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BasePawn.generated.h"

class AProjectile;

UCLASS()
class BATTLEBLASTER_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComp;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> TurretMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> ProjectileSpawnPoint;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category="Particle Effects")
	TObjectPtr<UNiagaraSystem> DeathParticles;
	
	UPROPERTY(EditAnywhere, Category="Sound Effects")
	TObjectPtr<USoundBase> DeathSound;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> DeathCameraShakeClass;
	
	void RotateTurret(FVector LookAtTarget);

	virtual void HandleDestruction();
	
	void Fire();
};
