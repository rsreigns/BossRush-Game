// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_ChargeAttack.generated.h"


UCLASS()
class BOSSRUSH_API UBTT_ChargeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
	AAIController* ControllerRef;

	ACharacter* CharacterRef;

	class URobotAnimInstance* RobotAnim;

	FScriptDelegate MoveCompletedDelegate;

	float OriginalWalkSpeed;

	UPROPERTY(EditAnywhere)
	float ChargeAttackSpeed{ 2000.0f };

	bool bIsFinished{ false };
	

protected:
	virtual void TickTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds
	) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge Attack")
	float ChargeCooldownDuration = 2.0f;

public:


	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory
	)override;

	void ChargeAtPlayer();

	

	UFUNCTION()
	void FinishAttackTask();

private:
	FTimerHandle ChargeCooldownTimerHandle;
	bool bCanCharge = false;

	void StartCoolDown();
	void EndCooldown();
	
};
