#include "Tank.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ATank::ATank()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (auto SubSystem = 
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				SubSystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	
	SetPlayerEnabled(false);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		RotateTurret(HitResult.ImpactPoint);
#if WITH_EDITOR
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.f, 12, FColor::Red);
#endif
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::TurnInput);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::Fire);
	}
}

void ATank::MoveInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	
	FVector DeltaLocation = FVector(0.f, 0.f, 0.f);
	DeltaLocation.X = Speed * InputValue * GetWorld()->GetDeltaSeconds();
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::TurnInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FRotator DeltaRotation = FRotator(0.f, 0.f, 0.f);
	DeltaRotation.Yaw = InputValue * TurnRate * GetWorld()->GetDeltaSeconds();
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	bPlayerIsAlive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetPlayerEnabled(false);
}

void ATank::SetPlayerEnabled(bool bIsPlayerEnabled)
{
	if (PlayerController)
	{
		if (bIsPlayerEnabled)
		{
			PlayerController->SetShowMouseCursor(true);
			EnableInput(PlayerController);
		}
		else
		{
			PlayerController->SetShowMouseCursor(false);
			DisableInput(PlayerController);
		}
	}
}
