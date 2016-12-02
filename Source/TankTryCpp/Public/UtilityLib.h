// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilityLib.generated.h"

/**
 *
 */
UCLASS()
class TANKTRYCPP_API UUtilityLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "Uti")
	static FVector GetRandLocAroundPlayer(FVector target, FVector orbitor, float distance);

};
