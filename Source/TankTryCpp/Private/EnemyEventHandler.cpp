// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "EnemyEventHandler.h"


// Sets default values
AEnemyEventHandler::AEnemyEventHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyEventHandler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyEventHandler::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

