// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "SpecCamera.h"


// Sets default values
ASpecCamera::ASpecCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpecCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpecCamera::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASpecCamera::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

