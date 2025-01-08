// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RobotAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BOSSRUSH_API URobotAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentSpeed{ 0.0 };

	UFUNCTION(BlueprintCallable)
	void UpdateSpeed();

public:

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bIsCharging{ false };
};
