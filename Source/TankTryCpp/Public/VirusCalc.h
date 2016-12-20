// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HoverTank.h"
#include "DonAINavigation/Classes/DonNavigationManager.h"
#include "VirusCalc.generated.h"

USTRUCT()
struct FVirusPart
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	

	bool dead = false;
	bool blocked = false;
	float numOfFullSizedNeighbours;
	UStaticMeshComponent* meshComponent;
	UPostProcessComponent* PPComponent;
	UBoxComponent* PPVolume;

	bool fading = false;

	float lifetime;
	float fadingTime;
	//FORCEINLINE bool IsDead() const	{ return _dead; }
private:


public:
	FVirusPart() {}
	FVirusPart(FVector realLoc, bool isBlocked, UStaticMeshComponent* meshComp, UPostProcessComponent* PPComp , UBoxComponent* boxComp,float startingTimeToLive, float inFadeTime)
	{
		Location = realLoc;
		meshComponent = meshComp;
		blocked = isBlocked;
		lifetime = startingTimeToLive;
		fadingTime = inFadeTime;
		PPComponent = PPComp;
		PPVolume = boxComp;
	}


};

struct PPXValueStorage
{
public: 
	float WhiteTint = 0;
	float LensFlareIntensity = 0;
	float BloomIntensity = 0;
	inline void UpdateValues(float deltaTime)
	{
		WhiteTint += deltaTime;
		LensFlareIntensity += deltaTime;
		BloomIntensity += deltaTime;
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
		void OverlapBegins(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void OverlapEnds(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* sparksPS;

	USceneComponent* sceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh* boxMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBillboardComponent* billboardMarker;

	TMap<FVector, FVirusPart> allVirusData;

	UFUNCTION()
		void ClearVirusNavBlock(FVector blockToClear);
private:
	FVector currentLocation = FVector(0, 0, 0);
	TArray<FVector> attackLocations;
	FTimerHandle spawningTimer;
	const FVector allDirections[6] = { FVector(1, 0, 0) , FVector(-1, 0, 0) ,FVector(0, 1, 0) ,FVector(0, -1, 0),FVector(0, 0, 1),FVector(0, 0, -1) };
	//VirusSpawningParms
	FCollisionObjectQueryParams vOQP;
	FCollisionShape vColShape;
	AHoverTank* player;

	PPXValueStorage storage;
	FPostProcessSettings unifiedPPSettings;
	int numOfCompBeingOverlapped = 0;

	ADonNavigationManager* navManager;
private:
	void SpawningTimeElapsed();
	void GetWorldLocation(FVector boxPos);
};

