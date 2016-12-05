// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "CppFunctionList.h"


//UCppFunctionList::UCppFunctionList(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//
//}

//void UCppFunctionList::EditDisplaySize(UPARAM(ref) UDecalComponent *hello, float sizeToSetTo)
//{
//	if (IsValid(hello))
//	{
//		hello->FadeScreenSize = sizeToSetTo;
//	}
//}

//void UCppFunctionList::SpawnDecalCorrectly(UMaterialInterface *inDecal, FVector decalSize, USceneComponent* parentComp, FVector location, FRotator rotation)
//{
//	UGameplayStatics::SpawnDecalAttached(inDecal, decalSize, parentComp, NAME_None, location, rotation, EAttachLocation::KeepWorldPosition, 60);
//}
//
//void UCppFunctionList::Hahaha()
//{
//	
//}

void UCppFunctionList::PrintVector(FVector input)
{
	UE_LOG(LogTemp, Log, TEXT("X: %f, Y: %f, Z: %f"), input.X, input.Y, input.Z);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("X: %f, Y: %f, Z: %f"), input.X, input.Y, input.Z));
}
void UCppFunctionList::PrintRotator(FRotator input)
{
	UE_LOG(LogTemp, Log, TEXT("X: %f, Y: %f, Z: %f"), input.Roll, input.Pitch, input.Yaw);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("X: %f, Y: %f, Z: %f"), input.Roll, input.Pitch, input.Yaw));
}
void UCppFunctionList::PrintString(FString txt)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *(txt));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, (txt));
}

void UCppFunctionList::PrintBool(bool inBool)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), inBool ? "True" : "False");
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, FString::Printf(TEXT("%s"), inBool ? "True" : "False"));
}

float UCppFunctionList::CosineGraph(float Ampitude, float waveLength, float X)
{
	float cosineAngle = FMath::Cos((PI / waveLength) * X);
	return (cosineAngle * Ampitude + FMath::Abs(Ampitude));
}

FVector UCppFunctionList::FindEmptyOrbitRadiusPos(ADonNavigationManager* navManager, FVector target, FVector orbitor, float distance)
{
	FVector direction = (orbitor - target).GetSafeNormal();
	if (direction == FVector::ZeroVector)
	{
		return FVector();
	}
	FRotator startingRot = direction.ToOrientationRotator();
	for (int i = 0; i < 360; i += 3)
	{
		FRotator actualRot = startingRot.Add(0, i, 0);

		FVector currentTarget = target + (actualRot.Vector() * distance);
		FDonNavigationVoxel* checkData = navManager->VolumeAt(currentTarget);
		if (checkData->CanNavigate())
		{
			return currentTarget;
		}
		else
		{
			//PrintString(TEXT("WTF"));
		}
	}
	return FVector();
}