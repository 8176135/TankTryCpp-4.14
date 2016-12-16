// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "VirusCalc.generated.h"

USTRUCT()
struct FVirusPart
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	

	bool dead = false;
	bool blocked = false;
	//bool growing = false;
//	float growAmount() const { return _growAmount; }
	//void growAmount(const float& growAmount) { _growAmount = growAmount; }
	//float currentSize = 0;
	//FVector* currentSize;
	float numOfFullSizedNeighbours;
	UStaticMeshComponent* meshComponent;
	bool fading = false;
	float lifetime;
	float fadingTime;
	//FORCEINLINE bool IsDead() const	{ return _dead; }
private:


public:
	FVirusPart() {}
	FVirusPart(FVector realLoc, bool isBlocked, UStaticMeshComponent* meshComp, float startingTimeToLive, float inFadeTime)
	{
		Location = realLoc;
		meshComponent = meshComp;
		blocked = isBlocked;
		lifetime = startingTimeToLive;
		fadingTime = inFadeTime;
	}


};

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
	virtual void Tick(float DeltaSeconds) override;

	void ForceUpdate();
	void SpawningVirus(FVector attackLoc);
	void QueueVirusesToSpawn(FVector attackerLoc);
	void KillVirus(FVector deadLoc);
	void CheckForNeighbours(FVector BlockToCheck, bool invert);
	
	UFUNCTION()
		void OverlapBegins();

	//FVirusWholeXYZ allVirusData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BoxSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float timerLength = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float spawnSpeed = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float totalLifetime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fadingTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float dmgPerSecond = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TEnumAsByte<ECollisionChannel>> ObstacleQueryChannels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> virusDmgTp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* fadingMaterial;

	USceneComponent* sceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* boxMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* billboardMarker;


	TMap<FVector, FVirusPart> allVirusData;
private:
	FVector currentLocation = FVector(0, 0, 0);
	TArray<FVector> attackLocations;
	FTimerHandle spawningTimer;
	const FVector allDirections[6] = { FVector(1, 0, 0) , FVector(-1, 0, 0) ,FVector(0, 1, 0) ,FVector(0, -1, 0),FVector(0, 0, 1),FVector(0, 0, -1) };
	//VirusSpawningParms
	FCollisionObjectQueryParams vOQP;
	FCollisionShape vColShape;
private:
	void SpawningTimeElapsed();
	void GetWorldLocation(FVector boxPos);
};

