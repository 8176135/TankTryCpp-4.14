// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "LaserSentryController.generated.h"

/**
*
*/
UCLASS()
class TANKTRYCPP_API ALaserSentryController : public AAIController
{
	GENERATED_BODY()

public:
	ALaserSentryController(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetPawn(APawn* inPawn) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AiEssential)
		AActor *AiTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyEssential)
		float fireRate = 120;

private:
	FRotator interpFaceDirection;
	FRotator trueFaceDirection;

	FTimerHandle shootingTimer;
	void ShootingTimeElapsed();

	APawn* ControlledPawn;
};
