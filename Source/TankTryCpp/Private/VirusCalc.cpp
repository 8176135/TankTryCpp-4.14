// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "VirusCalc.h"

//#include "Virus"


// Sets default values
AVirusCalc::AVirusCalc()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVirusCalc::BeginPlay()
{
	Super::BeginPlay();
	//FVirusPart somePart[500][500][500];
	
	//somePart[20][20][20].dead = true;
	//UE_LOG(LogTemp, Warning, TEXT("Your message"));
}

// Called every frame
void AVirusCalc::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

