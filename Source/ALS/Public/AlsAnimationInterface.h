// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Utility/AlsGameplayTags.h"
#include "AlsAnimationInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UAlsAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALS_API IAlsAnimationInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	UFUNCTION(BlueprintCallable)
	virtual void AddRecoilWhileShooting(
		FGameplayTag OverlayModeTag,
		float Strength=0.0f,
		float DelayTime=0.05f,
		float TimeBeforeStart=0.0f
		)=0;
};
