/********************************************************************************
** auth�� God Of Pen
** desc�� ����Creature״̬���Ľڵ�ͼ��
** Ver.:  V1.0.0
*********************************************************************************/
#include "Engine.h"
#include "EdGraph/EdGraph.h"

#include "CreatureStateMachineGraph.generated.h"
#pragma once
UCLASS()
class  UCreatureStateMachineGraph :public UEdGraph{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY()
	class	UCreatureAnimStateMachine* ParentStateMachine;
	UPROPERTY()
	class	UCreatureAnimStateNode* DefaultRootNode;

	virtual void Serialize(FArchive& Ar) override;
	//���뵱ǰ״̬��ͼ�е����нڵ�
	void CompileNodes();
	void CreateDefaultStateNode();
private:
	UPROPERTY()
	TArray<UCreatureAnimStateNode*> StateNodeList;
};
