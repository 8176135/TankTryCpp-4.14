// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"
#include "HoverTankController.h"

AHoverTankController::AHoverTankController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//SetActorTickEnabled(true);
}

void AHoverTankController::BeginPlay()
{
	FActorSpawnParameters spawnParms = FActorSpawnParameters();
	spawnParms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	transitionCamera = GetWorld()->SpawnActor<ASpecCamera>(specCamToSpawn, FVector(0, 0, 0), FRotator(0, 0, 0), spawnParms);
	transitionCamera->EEHandler->TransFinDele.BindUFunction(this, "MovementIsCompleted");
	ABaseTurret* startingTurret = GetWorld()->SpawnActor<ABaseTurret>(unitToSpawn, FVector(-272, 769, 116), FRotator(0, 0, 0), spawnParms);
	allTurretsSpawned.AddUnique(startingTurret);
	Possess(startingTurret);
	tankState = Cast<ATankStateCpp>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);
	Super::BeginPlay();
}

void AHoverTankController::Tick(float DeltaTime)
{
	if (IsValid(controlledPawn) && tankState)
	{
		FHitResult hitRes;
		if (GetWorld()->LineTraceSingleByChannel(hitRes, controlledPawn->eyeCam->GetComponentLocation(),
			controlledPawn->eyeCam->GetComponentLocation() + controlledPawn->eyeCam->GetForwardVector() * 10000, ECollisionChannel::ECC_Visibility, FCollisionQueryParams(NAME_None, true, controlledPawn)))
		{
			tankState->AimLocation = hitRes.Location;
		}
		for (int i = 0; i < allTurretsSpawned.Num(); ++i)
		{
			if (!IsValid(allTurretsSpawned[i]))
			{
				allTurretsSpawned.RemoveAt(i);
				i--;
				continue;
			}
			if (allTurretsSpawned[i] != controlledPawn && allTurretsSpawned[i] != possesionElict)
			{
				FRotator deltaRot = (allTurretsSpawned[i]->mainTurretSke->GetSocketLocation("Turret") - tankState->AimLocation).Rotation();
				FRotator correctRotation = FMath::RInterpTo(FRotator(allTurretsSpawned[i]->turretTruePitch, allTurretsSpawned[i]->turretYaw, 0), deltaRot, DeltaTime, 5);
				allTurretsSpawned[i]->turretYaw = correctRotation.Yaw;
				allTurretsSpawned[i]->turretTruePitch = correctRotation.Pitch;
			}
		}
	}
	Super::Tick(DeltaTime);
}

void AHoverTankController::SetPawn(APawn* inPawn)
{
	if (IsValid(inPawn))
	{
		if (inPawn->IsA(ABaseTurret::StaticClass()))
		{
			controlledPawn = Cast<ABaseTurret>(inPawn);
		}
		else
		{
			allowJump = false;
		}
	}
	Super::SetPawn(inPawn);
}

void AHoverTankController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("LookUp", this, &AHoverTankController::PitchCamera);
	InputComponent->BindAxis("Turn", this, &AHoverTankController::YawCamera);

	InputComponent->BindAction("Respawn", IE_Pressed, this, &AHoverTankController::RequestRespawn);
	InputComponent->BindAction("JumpTurret", IE_Pressed, this, &AHoverTankController::JumpTurret);
	InputComponent->BindAction("BuildTurret", IE_Pressed, this, &AHoverTankController::BuildTurret);
	InputComponent->BindAction("Fire", IE_Pressed, this, &AHoverTankController::BeginFiring);
	InputComponent->BindAction("Fire", IE_Released, this, &AHoverTankController::EndFiring);
}

void AHoverTankController::BeginFiring()
{
	if (allowJump)
	{
		for (ABaseTurret* turret : allTurretsSpawned)
		{
			turret->BeginFiring();
		}
	}
}

void AHoverTankController::EndFiring()
{
	if (allowJump)
	{
		for (ABaseTurret* turret : allTurretsSpawned)
		{
			turret->StopFiring();
		}
	}
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

void AHoverTankController::JumpTurret()
{
	if (!allowJump)
	{
		UCppFunctionList::PrintString("Still Jumping");
		return;
	}
	FVector startingLoc = controlledPawn->eyeCam->GetComponentLocation();
	FHitResult hitRes;
	FCollisionQueryParams colQ = FCollisionQueryParams();
	colQ.AddIgnoredActor(controlledPawn);
	bool hit = GetWorld()->LineTraceSingleByChannel(hitRes, startingLoc, controlledPawn->eyeCam->GetForwardVector() * 2000 + startingLoc, ECollisionChannel::ECC_Visibility, colQ);
	if (hit && hitRes.IsValidBlockingHit() && IsValid(hitRes.GetActor()) && hitRes.GetActor()->ActorHasTag("Turret"))
	{
		ABaseTurret* candidate = Cast<ABaseTurret>(hitRes.GetActor());
		if (candidate != NULL)
		{
			for (ABaseTurret* turret : allTurretsSpawned)
			{
				turret->StopFiring();
			}
			possesionElict = candidate;
			transitionCamera->SetActorLocationAndRotation(controlledPawn->eyeCam->GetComponentLocation(), controlledPawn->eyeCam->GetComponentRotation());
			Possess(transitionCamera);
			transitionCamera->StartTransition(candidate->eyeCam->GetComponentLocation(), candidate->eyeCam->GetComponentRotation());
			allowJump = false;
		}
		else
		{
			UCppFunctionList::PrintString("WHY ARE U STEALING MY TURRETS!!!");
		}
	}
	else
	{
		UCppFunctionList::PrintString("NANANANA");
	}
}

void AHoverTankController::BuildTurret()
{
	ABaseTurret* newTurret = GetWorld()->SpawnActor<ABaseTurret>(unitToSpawn, tankState->AimLocation, FRotator(0), FActorSpawnParameters());
	if (IsValid(newTurret))
	{
		allTurretsSpawned.AddUnique(newTurret);
	}
}

void AHoverTankController::MovementIsCompleted()
{
	allowJump = true;
	if (IsValid(possesionElict))
	{
		Possess(possesionElict);
	}
	possesionElict = nullptr;
}

//void AHoverTankController::MoveForward(float AxisValue)
//{
//	if (IsValid(controlledPawn))
//	{
//		controlledPawn->AddMovementInput(controlledPawn->eyeCam->GetForwardVector() * FVector(1, 1, 0) * AxisValue);
//	}
//}
//
//void AHoverTankController::MoveRight(float AxisValue)
//{
//	if (IsValid(controlledPawn))
//	{
//		controlledPawn->AddMovementInput(controlledPawn->eyeCam->GetRightVector() * FVector(1, 1, 0) * (AxisValue));
//	}
//}

void AHoverTankController::PitchCamera(float AxisValue)
{
	if (IsValid(controlledPawn))
	{
		controlledPawn->turretTruePitch = FMath::ClampAngle(controlledPawn->turretTruePitch + AxisValue, -60.0f, 60.0f);
	}

}

void AHoverTankController::YawCamera(float AxisValue)
{
	if (IsValid(controlledPawn))
	{
		controlledPawn->turretYaw = controlledPawn->turretYaw + AxisValue;
	}
}


