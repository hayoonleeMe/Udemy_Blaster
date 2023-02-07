// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/PlayerController.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDShield(float Shield, float MaxShield);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDAnnouncementCountdown(float CountdownTime);
	void SetHUDGrenades(int32 Grenades);
	void SetHUDWeaponTypeText(const EWeaponType& WeaponType);
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	void UpdateElimmedText(ESlateVisibility VisibilityChange);

	virtual float GetServerTime();				// Synced with server world clock
	virtual void ReceivedPlayer() override;		// Sync with server clock as soon as possible (Called after this PlayerController's viewport/net connection is associated with this player controller)
	void OnMatchStateSet(FName State);
	void HandleMatchHasStarted();
	void HandleCooldown();

	float SingleTripTime = 0.f;
	
protected:
	virtual void BeginPlay() override;
	void SetHUDTime();
	void PollInit();

	/*
	 * Sync time between client and server
	 */

	// Requests the current server time, passing in the client's time when the request was sent
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	// Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f;	// difference between client and server time

	UPROPERTY(EditAnywhere, Category = Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;
	void CheckTimeSync(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();

	UFUNCTION(Client, Reliable)
	void ClientJoinMidGame(FName StateOfMatch, float Warmup, float Match, float Cooldown, float StartingTime);

	void HighPingWarning();
	void StopHighPingWarning();
	void CheckPing(float DeltaTime);
	
private:
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	UPROPERTY()
	class ABlasterGameMode* BlasterGameMode;

	UPROPERTY(ReplicatedUsing = OnRep_ElimmedTextVisibility)
	ESlateVisibility ElimmedTextVisibility;

	UFUNCTION()
	virtual void OnRep_ElimmedTextVisibility();
	void SetHUDElimmedTextVisibility();

	float LevelStartingTime = 0.f;
	float MatchTime = 0.f;			// Game Mode 에서 얻어오도록 변경
	float WarmupTime = 0.f;
	float CooldownTime = 0.f;
	uint32 CountdownInt = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY()
	class UCharacterOverlay* CharacterOverlay;

	float HUDHealth;
	bool bInitializeHealth = false;
	float HUDMaxHealth;
	float HUDShield;
	bool bInitializeShield = false;
    float HUDMaxShield;
	float HUDScore;
	bool bInitializeScore = false;
	int32 HUDDefeats;
	bool bInitializeDefeats = false;
	int32 HUDGrenades;
	bool bInitializeGrenades = false;
	int32 HUDWeaponAmmo;
	bool bInitializeWeaponAmmo = false;
	int32 HUDCarriedAmmo;
	bool bInitializeCarriedAmmo = false;
	EWeaponType HUDWeaponType;
	bool bInitializeWeaponType = false;

	float HighPingRunningTime = 0.f;

	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.f;

	float PingAnimationRunningTime = 0.f;

	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20.f;

	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50.f;
};