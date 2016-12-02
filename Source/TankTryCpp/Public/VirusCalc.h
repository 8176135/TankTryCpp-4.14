// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "VirusCalc.generated.h"

UCLASS()
class TANKTRYCPP_API AVirusCalc : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVirusCalc();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
};

USTRUCT()
struct FVirusPart
{
	GENERATED_BODY()

	bool dead = false;
	bool blocked = false;
	float currentSize = 0;
};
