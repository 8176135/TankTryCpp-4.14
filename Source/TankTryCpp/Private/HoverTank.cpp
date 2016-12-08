// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "HoverTank.h"
#include "CppFunctionList.h"


// Sets default values
AHoverTank::AHoverTank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHoverTank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHoverTank::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AHoverTank::SetupPlayerInputComponent(class UInputComponent* inInputComponent)
{
	Super::SetupPlayerInputComponent(inInputComponent);

}

void AHoverTank::EditDisplaySize(UPARAM(ref) UDecalComponent *hello, float sizeToSetTo)
{
	if (IsValid(hello))
	{
		hello->FadeScreenSize = sizeToSetTo;
		
	}
}

//FVector AHoverTank::GetClosestLocAroundTarget(ADonNavigationManager* navManager,FVector target, FVector orbitor, float distance)
//{
//	return UCppFunctionList::FindEmptyOrbitRadiusPos(navManager, target, orbitor, distance);
//}

