// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"
#include "HoverTankController.h"

AHoverTankController::AHoverTankController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//SetActorTickEnabled(true);
	tankState = Cast<ATankStateCpp>(PlayerState);

	
}

void AHoverTankController::BeginPlay()
{
	//-272 769 116
	//FVector(-272, 769, 116)
	FActorSpawnParameters spawnParms = FActorSpawnParameters();
	Possess(GetWorld()->SpawnActor<APawn>(unitToSpawn, FVector(-272, 769, 116), FRotator(0, 0, 0), spawnParms));
	Super::BeginPlay();
}

void AHoverTankController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHoverTankController::SetPawn(APawn* inPawn)
{
	if (IsValid(inPawn))
	{
		controlledPawn = Cast<AHoverTank>(inPawn);
	}
	Super::SetPawn(inPawn);
}

void AHoverTankController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &AHoverTankController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AHoverTankController::MoveRight);
	InputComponent->BindAxis("LookUp", this, &AHoverTankController::PitchCamera);
	InputComponent->BindAxis("Turn", this, &AHoverTankController::YawCamera);

	InputComponent->BindAction("Respawn", IE_Pressed, this, &AHoverTankController::RequestRespawn);
}

void AHoverTankController::RequestRespawn()
{
	if (IsValid(controlledPawn))
	{
		UCppFunctionList::PrintString("RespawnToBeImplemented");
	}
	else
	{
		UCppFunctionList::PrintString("Ur still alive");
	}
}

void AHoverTankController::MoveForward(float AxisValue)
{
	if (IsValid(controlledPawn))
	{
		controlledPawn->AddMovementInput(controlledPawn->eyeCam->GetForwardVector() * AxisValue);
	}
}

void AHoverTankController::MoveRight(float AxisValue)
{
	if (IsValid(controlledPawn))
	{
		controlledPawn->AddMovementInput(controlledPawn->eyeCam->GetRightVector() * (AxisValue));
	}
}

void AHoverTankController::PitchCamera(float AxisValue)
{
	controlledPawn->TruePitch = FMath::ClampAngle(controlledPawn->TruePitch - AxisValue, -60.0f, 60.0f);
}

void AHoverTankController::YawCamera(float AxisValue)
{
	controlledPawn->TurretYaw = controlledPawn->TurretYaw + AxisValue;
}