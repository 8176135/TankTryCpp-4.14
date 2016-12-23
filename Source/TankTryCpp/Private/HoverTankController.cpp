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
	
}

void AHoverTankController::Tick(float DeltaTime)
{

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

}

void AHoverTankController::MoveForward(float AxisValue)
{
	if (IsValid(controlledPawn))
	{
		controlledPawn->AddMovementInput(FVector(AxisValue, 0, 0));
	}
}

void AHoverTankController::MoveRight(float AxisValue)
{
	if (IsValid(controlledPawn))
	{
		controlledPawn->AddMovementInput(FVector(0, AxisValue, 0));
	}
}

void AHoverTankController::PitchCamera(float AxisValue)
{

}

void AHoverTankController::YawCamera(float AxisValue)
{

}
