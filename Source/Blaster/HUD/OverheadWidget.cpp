// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	//ENetRole RemoteRole = InPawn->GetRemoteRole();
	ENetRole LocalRole = InPawn->GetLocalRole();
	FString Role;
	switch (LocalRole)
	{
	case ENetRole::ROLE_Authority:
		Role = FString(TEXT("Authority"));
		break;

	case ENetRole::ROLE_AutonomousProxy:
		Role = FString(TEXT("Autonomous Proxy"));
		break;

	case ENetRole::ROLE_SimulatedProxy:
		Role = FString(TEXT("Simulated Proxy"));
		break;

	case ENetRole::ROLE_None:
		Role = FString(TEXT("None"));
		break;
	}

	//FString RemoteRoleString = FString::Printf(TEXT("Remote Role : %s"), *Role);
	FString LocalRoleString = FString::Printf(TEXT("Local Role : %s"), *Role);
	SetDisplayText(LocalRoleString);
}

// 레벨을 이동하여 기존의 레벨이 제거되었을 때 호출되는 메소드
void UOverheadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}
