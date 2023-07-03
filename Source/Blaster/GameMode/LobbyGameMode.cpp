// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UMultiplayerSessionsSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		// if Subsystem is not valid, executes halt
		check(Subsystem);

		if (NumberOfPlayers == Subsystem->DesiredNumPublicConnections)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				bUseSeamlessTravel = true;

				FString MatchType = Subsystem->DesiredMatchType;
				if (MatchType == "FreeForAll")
				{
					World->ServerTravel(FString(TEXT("/Game/Maps/BlasterMap?listen")));
				}
				else if (MatchType == "Teams")
				{
					World->ServerTravel(FString(TEXT("/Game/Maps/Teams?listen")));
				}
				else if (MatchType == "CaptureTheFlag")
				{
					World->ServerTravel(FString(TEXT("/Game/Maps/CaptureTheFlag?listen")));
				}
			}
		}
	} 
}