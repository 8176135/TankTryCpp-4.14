// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "HoverTank.h"
#include "TankStateCpp.h"
#include "HoverTankController.generated.h"

/**
 *
 */
UCLASS()
class TANKTRYCPP_API AHoverTankController : public APlayerController
{
	GENERATED_BODY()
public:
	AHoverTankController(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetPawn(APawn* inPawn) override;
	virtual void SetupInputComponent() override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	AHoverTank* controlledPawn;
	ATankStateCpp* tankState;
};
