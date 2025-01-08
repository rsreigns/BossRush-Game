// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChargeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "ERobotStates.h"
#include "Navigation/PathFollowingComponent.h"
#include "Animations/RobotAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"



void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool bCanSeePlayer = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("CanSeePlayer"));

	if (!bCanSeePlayer)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (!bIsFinished) { return; }
	if (bCanCharge) { return; }

	//ChargeAtPlayer();
}


EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ControllerRef = OwnerComp.GetAIOwner();
	CharacterRef = ControllerRef->GetCharacter();
	RobotAnim = Cast<URobotAnimInstance>(
		CharacterRef->GetMesh()->GetAnimInstance()
	);


	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(
		TEXT("CurrentState"),
		static_cast<uint8>(ERobotStates::Charge)
	);

	RobotAnim->bIsCharging = true;

	

	if ( APawn* PlayerRef = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor"))); PlayerRef)
	{
		FVector PlayerLocation = PlayerRef->GetActorLocation();

		OwnerComp.GetBlackboardComponent()
			->SetValueAsEnum(
				TEXT("CurrentState"),
				static_cast<uint8>(ERobotStates::Charge)
			);

		FAIMoveRequest MoveRequest{ PlayerLocation };
		MoveRequest.SetUsePathfinding(true);
		ControllerRef->MoveTo(MoveRequest);

		OriginalWalkSpeed = CharacterRef->GetCharacterMovement()->MaxWalkSpeed;
		CharacterRef->GetCharacterMovement()->MaxWalkSpeed = ChargeAttackSpeed;

		bIsFinished = false;
		bCanCharge = false;

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

void UBTT_ChargeAttack::ChargeAtPlayer()
{
	
	APawn* PlayerRef{
		GetWorld()->GetFirstPlayerController()->GetPawn()
	};

	FVector PlayerLocation{ PlayerRef->GetActorLocation() };

	FAIMoveRequest MoveRequest{ PlayerLocation };
	MoveRequest.SetUsePathfinding(true);

	ControllerRef->MoveTo(MoveRequest);
	ControllerRef->SetFocus(PlayerRef);

	ControllerRef->ReceiveMoveCompleted.AddUnique(MoveCompletedDelegate);

	OriginalWalkSpeed = CharacterRef->GetCharacterMovement()
		->MaxWalkSpeed;

	CharacterRef->GetCharacterMovement()
		->MaxWalkSpeed = ChargeAttackSpeed;



}


void UBTT_ChargeAttack::FinishAttackTask()
{
	UE_LOG(LogTemp, Warning, TEXT("Task Finished!"));
}

void UBTT_ChargeAttack::StartCoolDown()
{
	bCanCharge = false;
	GetWorld()->GetTimerManager().SetTimer(ChargeCooldownTimerHandle, this, &UBTT_ChargeAttack::EndCooldown, ChargeCooldownDuration, false);
}

void UBTT_ChargeAttack::EndCooldown()
{
	bCanCharge = true;
}
