// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "LaserOrbCpp.h"
#include "CppFunctionList.h"

// Sets default values
ALaserOrbCpp::ALaserOrbCpp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	EEHandler = CreateDefaultSubobject<UEnemyEventHandler>(TEXT("EnemyEventHandlerFirst"));
	//OurMovementComponent = CreateDefaultSubobject<ULaserSentryPMC>(TEXT("LaserSentryCustomMC"));
	//OurMovementComponent->UpdatedComponent = RootComponent;
	//firePoint->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ALaserOrbCpp::BeginPlay()
{
	TankState = Cast<ATankStateCpp>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);
	Super::BeginPlay();
}

// Called every frame
void ALaserOrbCpp::Tick(float deltaTime)
{
	//currentDodgeTime += deltaTime;

	
	//OurMovementComponent->AddInputVector(GetActorRightVector() * speed);
	//if (IsValid(AiTarget))
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("Test2222: %f"), TankState->Health);
	//	FVector deltaVector = (this->GetActorLocation() - AiTarget->GetActorLocation());
	//	trueFaceDirection = deltaVector.GetSafeNormal().Rotation();
	//	trueFaceDirection.Yaw += 180;
	//	trueFaceDirection.Pitch *= -1;
	//	interpFaceDirection = FMath::RInterpTo(this->GetActorRotation(), trueFaceDirection, deltaTime,  rotateSpeed);
	//	//SetActorRotation(currentDirection);

	//	float currentDistance = deltaVector.Size();
	//	if (!FMath::IsNearlyEqual(currentDistance, optimalDistance, optimalRange))
	//	{
	//		if (currentDistance > optimalDistance)
	//		{
	//			WrongPlace(false);
	//		}
	//		else
	//		{
	//			WrongPlace(true);
	//		}
	//	}
	//	if (!FMath::IsNearlyEqual(this->GetActorLocation().Z, optimalHeight, optimalRange))
	//	{
	//		if (this->GetActorLocation().Z > optimalHeight)
	//		{
	//			WrongHeight(false);
	//		}
	//		else
	//		{
	//			WrongHeight(true);
	//		}
	//	}
	//	//UE_LOG(LogTemp, Warning, TEXT("Test2222: %f"), currentDistance);
	//}
	//else
	//{
	//	LookForNewTarget();
	//}
	Super::Tick(deltaTime);
}

float ALaserOrbCpp::TakeDamage(float damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	FHitResult hitRes;
	FVector impulseDir;
	DamageEvent.GetBestHitInfo(this, DamageCauser, hitRes, impulseDir);
	
	health -= damage;

	if (health <= 0)
	{
		SetLifeSpan(0.1f);
		DeathEfx();
	}
	else
	{
		if (EEHandler->HurtDele.IsBound())
		{
			EEHandler->HurtDele.Execute(5);
		}
		else
		{
			UCppFunctionList::PrintString(TEXT("Delegate not bound..?"));
		}

	}

	return Super::TakeDamage(damage, DamageEvent, EventInstigator, DamageCauser);
}

void ALaserOrbCpp::Shoot(AActor* target)
{
	FHitResult laserFireRes;
	bool isHit = GetWorld()->LineTraceSingleByObjectType(laserFireRes, GetActorLocation() + GetActorForwardVector() * 40, target->GetActorLocation(),
		laserBlockingObjects, FCollisionQueryParams(FName(EName::NAME_None), false, target));
	if (!isHit)
	{
		float distance = (target->GetActorLocation() - GetActorLocation()).Size();
		float damage = UCppFunctionList::CosineGraph(damagePerShot, damageFalloff, distance);
		FVector hitDir = (laserFireRes.TraceEnd - laserFireRes.TraceStart).GetSafeNormal();
		//UCppFunctionList::PrintString(TEXT("IsHitFine"));
		UGameplayStatics::ApplyPointDamage(target, damagePerShot, hitDir, laserFireRes, nullptr, this, damageTypeGiven);
	}
	LaserEffects();
	////FHitResult laserFireRes;

	////bool isHit = GetWorld()->LineTraceSingleByObjectType(laserFireRes, GetActorLocation() + GetActorForwardVector() * 40, AiTarget->GetActorLocation(), laserBlockingObjects, FCollisionQueryParams(FName(EName::NAME_None, false)));
	//if (laserFireRes.Actor.IsValid() && laserFireRes.Actor.Get() == AiTarget)
	//{
	//	float distance = (this->GetActorLocation() - target->GetActorLocation()).Size();

	//	float damage = UCppFunctionList::CosineGraph(damagePerShot, damageFalloff, distance);

	//	FVector hitDir = (laserFireRes.Location - laserFireRes.TraceStart).GetSafeNormal() * -1;
	//	UGameplayStatics::ApplyPointDamage(AiTarget, damagePerShot, hitDir, laserFireRes, nullptr, this, damageTypeGiven);
	//	LaserEffects();
	//}
	//else
	//{
	//	isTargetVisible = false;
	//}
}

//UPawnMovementComponent* ALaserOrbCpp::GetMovementComponent() const
//{
//	return OurMovementComponent;
//}

//void ALaserOrbCpp::TakingDamage(float Damage, FVector hitFrom)
//{
//	
//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("Workz"));
//	if (FMath::FRand() < Damage / dodgeChanceToDmgRatio && currentDodgeTime > dodgeCooldown)
//	{
//		currentDodgeTime = 0;
//		if (FMath::RandBool())
//		{
//			StartDodging(hitFrom.GetSafeNormal().RotateAngleAxis(90, FVector::UpVector));
//		}
//		else
//		{
//			StartDodging(hitFrom.GetSafeNormal().RotateAngleAxis(-90, FVector::UpVector));
//		}
//	}
//}

