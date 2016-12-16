// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "TankStateCpp.h"
#include "CppFunctionList.h"
#include "VirusCalc.h"

//#include "Virus"


// Sets default values
AVirusCalc::AVirusCalc()
{
	//VariableSetup
	PrimaryActorTick.bCanEverTick = true;
	FAttachmentTransformRules tempATR(EAttachmentRule::KeepRelative, false);
	vOQP = FCollisionObjectQueryParams();
	for (auto collisionChannel : ObstacleQueryChannels)
		vOQP.AddObjectTypesToQuery(collisionChannel);


	//ComponentSetup
	sceneComp = CreateDefaultSubobject<USceneComponent>("TheRootOfAllEvil");
	billboardMarker = CreateDefaultSubobject<UBillboardComponent>(FName("Billboard Marker"));
	SetRootComponent(sceneComp);
	billboardMarker->AttachToComponent(RootComponent, tempATR);
}

// Called when the game starts or when spawned
void AVirusCalc::BeginPlay()
{
	vColShape = FCollisionShape::MakeBox(FVector(BoxSize / 2 - 1, BoxSize / 2 - 1, BoxSize / 2 - 1));

	Super::BeginPlay();

	attackLocations.Add(FVector(0, 0, 0));
	GetWorld()->GetTimerManager().SetTimer(spawningTimer, this, &AVirusCalc::SpawningTimeElapsed, timerLength, false);

}

// Called every frame
void AVirusCalc::Tick(float DeltaTime)
{
	for (auto data : allVirusData)
	{
		if (data.Value.dead)
		{
			continue;
		}
		if (data.Value.meshComponent->RelativeScale3D != FVector(1))
		{
			FVector newScale = UCppFunctionList::Clamp3Numbers((DeltaTime * data.Value.numOfFullSizedNeighbours * FVector(spawnSpeed) + data.Value.meshComponent->RelativeScale3D), 1, 1, 1);
			data.Value.meshComponent->SetRelativeScale3D(newScale);
			if (!data.Value.blocked && newScale == FVector(1))
			{
				CheckForNeighbours(data.Key, true);
				QueueVirusesToSpawn(data.Key);
			}
		}
		else
		{
			allVirusData[data.Key].lifetime -= DeltaTime;
			if (data.Value.lifetime <= 0)
			{
				KillVirus(data.Key);
			}
			else if (data.Value.lifetime < data.Value.fadingTime)
			{
				if (!data.Value.fading)
				{
					allVirusData[data.Key].fading = true;
					data.Value.meshComponent->SetMaterial(0, fadingMaterial);
				}
				data.Value.meshComponent->SetScalarParameterValueOnMaterials(FName("Transparency"), data.Value.lifetime / data.Value.fadingTime);
			}
		}
	}
	TArray<AActor*> allOverlappers;
	GetOverlappingActors(allOverlappers);
	if (allOverlappers.Num() > 0)
	{
		for (AActor* actor : allOverlappers)
		{
			if (IsValid(actor))
			{
				APawn* player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
				if (IsValid(player) && actor->GetUniqueID() == player->GetUniqueID())
				{
					UGameplayStatics::ApplyDamage(player, DeltaTime * dmgPerSecond, NULL, this, virusDmgTp);
					UCppFunctionList::PrintString(FString::Printf(TEXT("3refjewoifjewfiewj %f"), Cast<ATankStateCpp>(player->PlayerState)->Health));
				}
			}
		}
	}
	Super::Tick(DeltaTime);
}

void AVirusCalc::SpawningVirus(FVector curLoc)
{
	FAttachmentTransformRules tempATR(EAttachmentRule::KeepRelative, false);
	FVector realLoc = FVector((curLoc.X * BoxSize) + GetActorLocation().X, (curLoc.Y * BoxSize) + GetActorLocation().Y, (curLoc.Z * BoxSize) + GetActorLocation().Z);

	TArray<FOverlapResult> overLapResults;
	GetWorld()->OverlapMultiByObjectType(overLapResults, realLoc, FQuat::Identity, vOQP, vColShape, FCollisionQueryParams(FName("VirusSpreading"), false));
	UStaticMeshComponent* newStaticMeshComp = NewObject<UStaticMeshComponent>(this, FName(*FString::Printf(TEXT("Virus Part - %f:%f:%f"), curLoc.X, curLoc.Y, curLoc.Z))); //ConstructObject<UStaticMeshComponent>(, this, FName(*FString::Printf(TEXT("Virus Part - %f:%f:%f"), curLoc.X, curLoc.Y, curLoc.Z)));
	newStaticMeshComp->RegisterComponent();
	newStaticMeshComp->AttachToComponent(RootComponent, tempATR, NAME_None);
	newStaticMeshComp->SetRelativeScale3D(FVector(0, 0, 0));
	newStaticMeshComp->SetWorldLocation(realLoc);
	newStaticMeshComp->SetStaticMesh(boxMesh);
	newStaticMeshComp->SetCollisionProfileName(FName("VirusCol"));
	allVirusData.Add(curLoc, FVirusPart(realLoc, overLapResults.Num() == 0 ? false : true, newStaticMeshComp, totalLifetime, fadingTime));
	CheckForNeighbours(curLoc, false);
}

void AVirusCalc::QueueVirusesToSpawn(FVector attackerLoc)
{
	for (FVector direction : allDirections)
	{
		FVector checkingLocation = attackerLoc + direction;
		if (!allVirusData.Contains(checkingLocation))
		{
			SpawningVirus(checkingLocation);
		}
	}
}

void AVirusCalc::KillVirus(FVector deadLoc)
{
	allVirusData[deadLoc].lifetime = 0;
	allVirusData[deadLoc].meshComponent->SetVisibility(false);
	allVirusData[deadLoc].meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	allVirusData[deadLoc].dead = true;
}

void AVirusCalc::CheckForNeighbours(FVector blockToCheck, bool invert)
{
	if (!allVirusData.Contains(blockToCheck))
	{
		UCppFunctionList::PrintString("Neighbour not in list");
		return;
	}
	if (!invert)
	{
		allVirusData[blockToCheck].numOfFullSizedNeighbours = 0;
		for (FVector direction : allDirections)
		{
			FVector checkingLocation = blockToCheck + direction;
			if (allVirusData.Contains(checkingLocation) && !allVirusData[checkingLocation].dead
				&& !allVirusData[checkingLocation].blocked && allVirusData[checkingLocation].meshComponent->RelativeScale3D == FVector(1))
			{
				allVirusData[blockToCheck].numOfFullSizedNeighbours++;
			}
		}
	}
	else if (allVirusData[blockToCheck].meshComponent->RelativeScale3D == FVector(1))
	{
		for (FVector direction : allDirections)
		{
			FVector checkingLocation = blockToCheck + direction;
			if (allVirusData.Contains(checkingLocation) && !allVirusData[checkingLocation].dead)
			{
				allVirusData[checkingLocation].numOfFullSizedNeighbours++;
			}
		}
	}
	else
	{
		UCppFunctionList::PrintString("Passed a block that is not full for an inverse check");
		return;
	}
}

void AVirusCalc::OverlapBegins()
{

}

void AVirusCalc::ForceUpdate()
{
	for (auto data : allVirusData)
	{
		if (!data.Value.dead)
		{
			if (data.Value.meshComponent->RelativeScale3D != FVector(1))
			{
				allVirusData[data.Key].numOfFullSizedNeighbours = 0;
				for (FVector direction : allDirections)
				{
					FVector checkingLocation = data.Key + direction;
					if (allVirusData.Contains(checkingLocation) && !allVirusData[checkingLocation].dead
						&& !allVirusData[checkingLocation].blocked && allVirusData[checkingLocation].meshComponent->RelativeScale3D == FVector(1))
					{
						allVirusData[data.Key].numOfFullSizedNeighbours++;
					}
				}
			}
			else if (!data.Value.blocked)
			{
				QueueVirusesToSpawn(data.Key);
			}
		}
	}
}
void AVirusCalc::SpawningTimeElapsed()
{
	SpawningVirus(FVector(0, 0, 0));
	allVirusData[FVector(0, 0, 0)].numOfFullSizedNeighbours++;
	//if (attackLocations.Num() > 0)
	//{
	//	if (allVirusData.Contains(attackLocations[0]) && !allVirusData[attackLocations[0]].dead)
	//	{
	//		attackLocations.RemoveAt(0);
	//		SpawningTimeElapsed();
	//		return;
	//	}
	//	FVector curLoc = attackLocations[0];
	//	attackLocations.RemoveAt(0);
	//	
	//	//if (overLapResults.Num() == 0)
	//	//{
	//	//	for (FVector direction : allDirections)
	//	//	{
	//	//		attackLocations.Add(curLoc + direction);
	//	//	}
	//	//}
	//	//if (attackLocations.Num() > 0)
	//	//{
	//	//	int influence = 0;
	//	//	for (FVector direction : allDirections)
	//	//	{
	//	//		if (allVirusData.Contains(attackLocations[0] + direction) && !allVirusData[attackLocations[0] + direction].blocked && !allVirusData[attackLocations[0] + direction].dead)
	//	//		{
	//	//			influence++;
	//	//		}
	//	//	}
	//	//	if (influence == 0)
	//	//	{
	//	//		GetWorldTimerManager().ClearTimer(spawningTimer);
	//	//		return;
	//	//	}
	//	//	GetWorldTimerManager().SetTimer(spawningTimer, this, &AVirusCalc::SpawningTimeElapsed, timerLength / influence, true);
	//	//}
	//}
}
