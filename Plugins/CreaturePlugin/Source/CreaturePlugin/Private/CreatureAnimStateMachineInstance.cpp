
#include "CreaturePluginPCH.h"
#include "CreatureAnimStateMachineInstance.h"
#include "CreatureAnimState.h"
#include "CreatureAnimStateMachine.h"
#include "CreatureAnimTransition.h"

UCreatureAnimStateMachineInstance::UCreatureAnimStateMachineInstance()
{

}

void UCreatureAnimStateMachineInstance::SetCondition(FString ConditionName, bool Flag)
{
	FName conditionAsName(*ConditionName);

	m_currentConditionValues.Add(conditionAsName, Flag);

	if (CurrentState)
	{
		CurrentState->CheckCondition(this);
	}
}

bool UCreatureAnimStateMachineInstance::GetCondition(FString ConditionName) const
{
	FName conditionAsName(*ConditionName);
	const bool *value = m_currentConditionValues.Find(conditionAsName);
	return (value) ? *value : false;
}

void UCreatureAnimStateMachineInstance::SetCurrentState(UCreatureAnimState *state)
{
	if (CurrentState)
	{
		CurrentState->EndState(this);
	}

	CurrentState = state;

	if (CurrentState)
	{
		CurrentState->BeginState(this);
	}
}

UCreatureMeshComponent * UCreatureAnimStateMachineInstance::GetOwningMeshComponent() const
{
	return Cast<UCreatureMeshComponent>(GetOuter());
}

void UCreatureAnimStateMachineInstance::InitInstance(UCreatureAnimStateMachine* forStateMachine)
{
	check(forStateMachine);
	TargetStateMachine = forStateMachine;

	//�󶨶�����ʼ���β�¼���MeshComponent����֧��AnimStart/AnimEndת��
	UCreatureMeshComponent *owningComponent = GetOwningMeshComponent();
	if (owningComponent !=nullptr)
	{
		owningComponent->CreatureAnimationStartEvent.AddDynamic(this, &UCreatureAnimStateMachineInstance::OnAnimStart);
		owningComponent->CreatureAnimationEndEvent.AddDynamic(this, &UCreatureAnimStateMachineInstance::OnAnimEnd);
	}

	//��ʱʹ�ã�ֱ�ӴӸ��ڵ�������һ���ڵ�
	SetCurrentState(forStateMachine->RootState->TransitionList[0]->TargetState);
}

void UCreatureAnimStateMachineInstance::OnAnimStart(float frame)
{
	//SetCondition(FString(TEXT("AnimationStart")),true);
	//SetCondition(FString(TEXT("AnimationEnd")), false);
}

void UCreatureAnimStateMachineInstance::OnAnimEnd(float frame)
{
	//SetCondition(FString(TEXT("AnimationStart")), false);
	//SetCondition(FString(TEXT("AnimationEnd")), true);
	CurrentState->AnimationEnd(this);
}
