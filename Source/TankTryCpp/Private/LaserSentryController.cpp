// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "LaserSentryController.h"
#include "CppFunctionList.h"
#include "LaserSentryCustomPFC.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"


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
	if (IsValid(AiTarget) && IsValid(ControlledPawn) && Blackboard->GetValueAsEnum("State") != 2)
	{
		FVector deltaVector = (AiTarget->GetActorLocation() - ControlledPawn->GetActorLocation());
		trueFaceDirection = deltaVector.GetSafeNormal().Rotation();
		//trueFaceDirection.Yaw += 180;
		//trueFaceDirection.Pitch *= -1;
		interpFaceDirection = FMath::RInterpTo(ControlledPawn->GetActorRotation(), trueFaceDirection, deltaTime, 4);
		ControlledPawn->SetActorRotation(interpFaceDirection);
		if (Blackboard->GetValueAsEnum("State") == 1)
		{
			ControlledPawn->AddMovementInput(FVector(0, 0, (optimalHeight - ControlledPawn->GetActorLocation().Z) / 300));
			ControlledPawn->AddMovementInput(deltaVector.GetSafeNormal() * (((deltaVector.Size() - optimalDistance)) / 200));
		}
	}
	Super::Tick(deltaTime);
}

void ALaserSentryController::SetPawn(APawn* inPawn)
{
	if (IsValid(inPawn))
	{
		UCppFunctionList::PrintString("Brah");
		ControlledPawn = Cast<ALaserOrbCpp>(inPawn);
		EEHandler = ControlledPawn->EEHandler;
		if ((*EEHandler).HurtDele.IsBound())
		{
			(*EEHandler).HurtDele.Unbind();
		}
		(*EEHandler).HurtDele.BindUFunction(this, FName("CtrlPawnIsHurt"));
	}
	Super::SetPawn(inPawn);
}

void ALaserSentryController::CtrlPawnIsHurt(float amountOfDmg)
{
	if (Blackboard->GetValueAsEnum("State") != 2 && FMath::FRand() < (amountOfDmg / 40))
	{
		Blackboard->SetValueAsEnum("State", 2);
	}
}

void ALaserSentryController::ShootingTimeElapsed()
{
	if (Blackboard->GetValueAsEnum(FName("State")) == 1 && IsValid(AiTarget) && IsValid(ControlledPawn))
	{
		ControlledPawn->Shoot(AiTarget);
		//EEHandler->HurtDele.Unbind();
		//FHitResult laserFireRes;
		//bool isHit = GetWorld()->LineTraceSingleByObjectType(laserFireRes, ControlledPawn->GetActorLocation() + GetActorForwardVector() * 40, AiTarget->GetActorLocation(), laserBlockingObjects, FCollisionQueryParams(FName(EName::NAME_None), false, AiTarget));
		//if (!isHit)
		//{
		//	float distance = (AiTarget->GetActorLocation() - ControlledPawn->GetActorLocation()).Size();
		//	float damage = UCppFunctionList::CosineGraph(damagePerShot, damageFalloff, distance);
		//	FVector hitDir = (laserFireRes.TraceEnd - laserFireRes.TraceStart).GetSafeNormal();
		//	UGameplayStatics::ApplyPointDamage(AiTarget, damagePerShot, hitDir, laserFireRes, nullptr, this, damageTypeGiven);
		//	ControlledPawn->LaserEffects();
		//}
	}
}
