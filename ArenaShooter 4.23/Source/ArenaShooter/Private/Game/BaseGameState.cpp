// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"

#include "BaseGameMode.h"
#include "BasePlayerController.h"
#include "BasePlayerState.h"
#include "UnrealNetwork.h"


void ABaseGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseGameState, _PlayerInfos);
	DOREPLIFETIME(ABaseGameState, _iMaxLobbySize);
}

///////////////////////////////////////////////

void ABaseGameState::UpdatePlayerList()
{
	// Clients only update their list locally, server replicates new player info
	if (Role != ROLE_Authority) { _OnPlayerInfoRefresh.Broadcast(); }

	// Since the gamemode only exists on the server, then client authority will return NULL :D
	ABaseGameMode* gameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != NULL)
	{
		// Clear the list, then re-populate it
		_PlayerInfos.Empty();

		// Add an FPlayerInfo for each connected player controller in the gamemode
		for (int i = 0; i < gameMode->GetConnectedPlayerControllers().Num(); i++)
		{
			FPlayerInfo	info;
			ABasePlayerState* playerState = Cast<ABasePlayerState>(gameMode->GetConnectedPlayerControllers()[i]->PlayerState);
			if (playerState != NULL)
			{
				info._PlayerName = playerState->GetPlayerInfo().GetPlayerName();
				info._PlayerTag = playerState->GetPlayerInfo().GetPlayerTag();
				info._bIsHost = playerState->GetPlayerInfo().IsHost();
				info._PlayerController = playerState->GetPlayerInfo().GetPlayerController();
			}
			_PlayerInfos.Add(info);
		}	
		
		// Update player list on all clients connected
		_OnPlayerInfoRefresh.Broadcast();
	}

	// Future-proofing (client authority will run this)
	else { return; }
}

///////////////////////////////////////////////

void ABaseGameState::SetMaxLobbySize(int Size)
{
	// Can only be called from the server
	if (Role != ROLE_Authority) { return; }

	_iMaxLobbySize = Size;
}

///////////////////////////////////////////////

void ABaseGameState::DisconnectClientFromLobby()
{

}
