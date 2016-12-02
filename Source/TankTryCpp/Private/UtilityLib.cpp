// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTryCpp.h"
#include "UtilityLib.h"
#include "CppFunctionList.h"

FVector UUtilityLib::GetRandLocAroundPlayer(FVector target, FVector orbitor, float distance)
{
	return UCppFunctionList::FindOrbitRadiusPos(target, orbitor, distance);
}
