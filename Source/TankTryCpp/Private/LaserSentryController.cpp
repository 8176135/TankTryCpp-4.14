// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "LaserSentryController.h"
#include "CppFunctionList.h"
#include "LaserSentryCustomPFC.h"


ALaserSentryController::ALaserSentryController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<ULaserSentryCustomPFC>(TEXT("PathFollowingComponent")))
{
	SetActorTickEnabled(true);
}

void ALaserSentryController::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(shootingTimer, this, &ALaserSentryController::ShootingTimeElapsed, 60.0f / fireRate, true);
}

void ALaserSentryController::Tick(float deltaTime)
{
	if (IsValid(ControlledPawn))
	{
		//ControlledPawn->GetMovementComponent()->AddInputVector(FVector(1, 0, 0) * 100);
		if (IsValid(AiTarget))
		{
			FVector deltaVector = (ControlledPawn->GetActorLocation() - AiTarget->GetActorLocation());
			trueFaceDirection = deltaVector.GetSafeNormal().Rotation();
			trueFaceDirection.Yaw += 180;
			trueFaceDirection.Pitch *= -1;
			interpFaceDirection = FMath::RInterpTo(ControlledPawn->GetActorRotation(), trueFaceDirection, deltaTime, 4);
			ControlledPawn->SetActorRotation(interpFaceDirection);

		}
		//if ((GetPathFollowingComponent()->GetCurrentTargetLocation() - ControlledPawn->GetActorLocation()).Size() < 20)
		//{
		//	ControlledPawn->GetMovementComponent()->Velocity *= 0;
		//}
	}
	Super::Tick(deltaTime);
}

void ALaserSentryController::SetPawn(APawn* inPawn)
{
	ControlledPawn = inPawn;
	Super::SetPawn(inPawn);
}

void ALaserSentryController::ShootingTimeElapsed()
{
	//if (IsValid(AiTarget))
	//{
	//	FHitResult laserFireRes;
	//	bool isHit = GetWorld()->LineTraceSingleByObjectType(laserFireRes, ControlledPawn->GetActorLocation() + GetActorForwardVector() * 40, AiTarget->GetActorLocation(), laserBlockingObjects, FCollisionQueryParams(FName(EName::NAME_None, false)));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Shot Fired"));
	//}
}
