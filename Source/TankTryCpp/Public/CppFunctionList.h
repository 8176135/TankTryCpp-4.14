// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CppFunctionList.generated.h"
/**
 *
 */
class UCppFunctionList
{
		/*	UFUNCTION(BlueprintCallable, Category = "OnlyCpp")
			static void EditDisplaySize(UPARAM(ref) UDecalComponent *hello, float sizeToSetTo);
		UFUNCTION(BlueprintCallable, Category = "OnlyCpp")
			static void SpawnDecalCorrectly(UMaterialInterface* inDecal, FVector decalSize, USceneComponent* parentComp, FVector location, FRotator rotation);*/

			//UFUNCTION(BlueprintCallable, Category = doesthiswork)
			//	static void Hahaha();
public:
		static void PrintVector(FVector input);
	static void PrintRotator(FRotator input);
	static void PrintString(TCHAR* txt);
	static void PrintBool(bool inBool);
	static float CosineGraph(float Ampitude, float waveLength, float X);

	static FVector FindOrbitRadiusPos(FVector target, FVector orbitor, float distance);
	//static bool IsWithinDistance(float a, float b, float tolerance);
};