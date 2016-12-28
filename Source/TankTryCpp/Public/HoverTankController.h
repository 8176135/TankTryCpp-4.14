// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "HoverTank.h"
#include "TankStateCpp.h"
#include "BaseTurret.h"
#include "SpecCamera.h"
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

	//Input Axis
	//void MoveForward(float AxisValue);
	//void MoveRight(float AxisValue);
	void PitchCamera(float AxisValue);
	void YawCamera(float AxisValue);

	//Input Actions
	void BeginFiring();
	void EndFiring();
	void RequestRespawn();
	void JumpTurret();
	void BuildTurret();

	UFUNCTION()
		void MovementIsCompleted();

	ABaseTurret* controlledPawn;
	ASpecCamera* transitionCamera;
	ATankStateCpp* tankState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ABaseTurret> unitToSpawn;

private:
	bool allowJump = true;
	ABaseTurret* possesionElict;
};
