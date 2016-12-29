// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"
#include "BaseTurret.h"


// Sets default values
ABaseTurret::ABaseTurret()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	colQuery = FCollisionQueryParams();
	colQuery.bTraceComplex = true;
	colQuery.AddIgnoredActor(this);

	objQuery = FCollisionObjectQueryParams();
	for (TEnumAsByte<ECollisionChannel> blocker : bulletBlockers)
	{
		objQuery.AddObjectTypesToQuery(blocker.GetValue());
	}

	Tags.Add("Turret");

	//Components
	sceneComp = CreateDefaultSubobject<USceneComponent>("BaseSceneComp");
	SetRootComponent(sceneComp);

	mainTurretSke = CreateDefaultSubobject<USkeletalMeshComponent>("MainTurretSkeleton");
	mainTurretSke->AttachToComponent(sceneComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	animationInst = mainTurretSke->GetAnimInstance();

	bulletPartic = CreateDefaultSubobject<UParticleSystemComponent>("BulletTracerParticle");
	bulletPartic->bAutoActivate = false;

	muzzleFlashPartic = CreateDefaultSubobject<UParticleSystemComponent>("MuzzleFlashParticle");
	muzzleFlashPartic->bAutoActivate = false;
	muzzleFlashPartic->SecondsBeforeInactive = 0.5;

	eyeCam = CreateDefaultSubobject<UCameraComponent>("EyeCamera");

	if (mainTurretSke->DoesSocketExist("EyeSocket"))
	{
		eyeCam->AttachToComponent(mainTurretSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "EyeSocket");
		eyeCam->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, -90, 0));
	}
}

// Called when the game starts or when spawned
void ABaseTurret::BeginPlay()
{
	if (mainTurretSke->DoesSocketExist("EyeSocket"))
	{
		eyeCam->AttachToComponent(mainTurretSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "EyeSocket");
		eyeCam->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, -90, 0));
	}
	if (mainTurretSke->DoesSocketExist("WrongEnd"))
	{
		bulletPartic->AttachToComponent(mainTurretSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "WrongEnd");
		muzzleFlashPartic->AttachToComponent(mainTurretSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "WrongEnd");
	}

	tankState = Cast<ATankStateCpp>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);

	animationInst = mainTurretSke->GetAnimInstance();
	GetWorldTimerManager().SetTimer(firingTimer, this, &ABaseTurret::FireGun, 60 / fireRPM, true);
	GetWorldTimerManager().PauseTimer(firingTimer);

	Super::BeginPlay();
}

// Called every frame
void ABaseTurret::Tick(float DeltaTime)
{
	spreadXVal = FMath::Clamp(spreadXVal - DeltaTime * PI * gunCooldownSpeed, 0.0f, PI / 2);
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseTurret::SetupPlayerInputComponent(class UInputComponent* inputComp)
{
	Super::SetupPlayerInputComponent(inputComp);
}

void ABaseTurret::PostEditChangeProperty(struct FPropertyChangedEvent& changedEvent)
{
	if (mainTurretSke->DoesSocketExist("EyeSocket"))
	{
		eyeCam->AttachToComponent(mainTurretSke, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "EyeSocket");
		eyeCam->SetRelativeLocationAndRotation(FVector(0, 0, 0), FRotator(0, -90, 0));
	}

	Super::PostEditChangeProperty(changedEvent);
}

void ABaseTurret::BeginFiring()
{
	GetWorldTimerManager().UnPauseTimer(firingTimer);
	timerPausePend = false;
	//if (timerPausePend)
	//{
	//	timerPausePend = false;
	//	return;
	//}
	if (animationInst)
	{
		animationInst->Montage_Play(gunFireMont, fireRPM / 60, EMontagePlayReturnType::Duration, 0);
	}
}

void ABaseTurret::StopFiring()
{
	timerPausePend = true;
}

void ABaseTurret::FireGun()
{
	if (timerPausePend)
	{
		GetWorldTimerManager().PauseTimer(firingTimer);
		animationInst->Montage_Stop(0.1f, gunFireMont);
		timerPausePend = false;
		return;
	}
	FVector startingLoc = mainTurretSke->GetSocketLocation("EyeSocket");

	FVector deltaVector = tankState->AimLocation - startingLoc;
	FRotator aimingRotation = deltaVector.Rotation();
	float spreadAmount = UCppFunctionList::CosineGraph(-maxInaccuracy, 1, spreadXVal);
	FVector actualBulletDir = UCppFunctionList::SpreadRotator(aimingRotation, spreadAmount).Vector();
	actualBulletDir *= 9000;
	FVector endingLoc = startingLoc + actualBulletDir;

	FHitResult hitRes;
	bool hit = GetWorld()->LineTraceSingleByObjectType(hitRes, startingLoc, endingLoc, objQuery, colQuery);
	if (hit && hitRes.IsValidBlockingHit())
	{
		if (IsValid(hitRes.GetActor()))
		{
			UGameplayStatics::ApplyPointDamage(hitRes.GetActor(), damagePerShot, hitRes.TraceStart - hitRes.Location, hitRes, GetController(), this, dmgType);
		}
		if (hitRes.GetComponent())
		{
			UGameplayStatics::SpawnDecalAttached(bulletHitDecal, FVector(5, FMath::FRandRange(8, 10.5f), FMath::FRandRange(8, 10.5f)),
				hitRes.GetComponent(), NAME_None, hitRes.Location, hitRes.Normal.Rotation().Add(0, 180, FMath::FRand() * 360), EAttachLocation::KeepWorldPosition, 60);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletHitPartic, hitRes.Location, hitRes.Normal.Rotation());
		}
		ParticleEfx(hitRes.Location);
	}
	else
	{
		ParticleEfx(endingLoc);
	}
	spreadXVal = FMath::Clamp(spreadXVal + gunHeatPerShot, 0.0f, PI / 2);
}

void ABaseTurret::PassAlongSomeStuff(ATankStateCpp* playerTankState)
{
	tankState = playerTankState;
}

void ABaseTurret::ParticleEfx(FVector trueAimLoc)
{
	muzzleFlashPartic->Activate(true);
	bulletPartic->SetWorldRotation((trueAimLoc - bulletPartic->GetComponentLocation()).Rotation());
	bulletPartic->Activate(true);
}
