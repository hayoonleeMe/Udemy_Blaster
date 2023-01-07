// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/PlayerController.h"
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
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float CountdownTime);
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	void UpdateElimmedText(ESlateVisibility VisibilityChange);
	void UpdateHUDWeaponType(ESlateVisibility Visibility, const FString& WeaponTypeString = FString());
	
protected:
	virtual void BeginPlay() override;
	void SetHUDTime();

private:
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	UPROPERTY(ReplicatedUsing = OnRep_ElimmedTextVisibility)
	ESlateVisibility ElimmedTextVisibility;

	UFUNCTION()
	virtual void OnRep_ElimmedTextVisibility();
	void SetHUDElimmedTextVisibility();

	float MatchTime = 120.f;
	float CountdownInt = 0.f;
};
