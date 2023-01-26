// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blaster/BlasterComponents/CombatComponent.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HealRampUp(DeltaTime);
}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	bHealing = true;
	HealingRate = HealAmount / HealingTime;
	AmountToHeal += HealAmount;
}

void UBuffComponent::HealRampUp(float DeltaTime)
{
	if (!bHealing || Character == nullptr || Character->IsElimmed()) return;

	const float HealThisFrame = HealingRate * DeltaTime;
	Character->SetHealth(FMath::Clamp(Character->GetHealth() + HealThisFrame, 0.f, Character->GetMaxHealth()));
	Character->UpdateHUDHealth();
	AmountToHeal -= HealThisFrame;

	if (AmountToHeal <= 0.f /*|| Character->GetHealth() >= Character->GetMaxHealth() : 풀피가 되어도 지속시간이 끝날 때까지 효과는 지속시킬 것이다.*/)
	{
		bHealing = false;
		AmountToHeal = 0.f;
	}
}

void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UBuffComponent::SetInitialSpeeds(float BaseSpeed, float CrouchSpeed)
{
	InitialBaseSpeed = BaseSpeed;
	InitialCrouchSpeed = CrouchSpeed;
}

void UBuffComponent::SetInitialJumpVelocity(float Velocity)
{
	InitialJumpVelocity = Velocity;
}

void UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime)
{
	if (Character == nullptr) return;

	Character->GetWorldTimerManager().SetTimer(SpeedBuffTimer, this, &UBuffComponent::ResetSpeeds, BuffTime);

	MulticastSpeedBuff(BuffBaseSpeed, BuffCrouchSpeed);
}

void UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime)
{
	if (Character == nullptr) return;

	Character->GetWorldTimerManager().SetTimer(JumpBuffTimer, this, &UBuffComponent::ResetJump, BuffTime);

	MulticastJumpBuff(BuffJumpVelocity);
}

void UBuffComponent::ResetSpeeds()
{
	MulticastSpeedBuff(InitialBaseSpeed, InitialCrouchSpeed);
}

void UBuffComponent::ResetJump()
{
	MulticastJumpBuff(InitialJumpVelocity);
}

void UBuffComponent::MulticastSpeedBuff_Implementation(float BaseSpeed, float CrouchSpeed)
{
	if (Character && Character->GetCharacterMovement() && Character->GetCombat())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
		
		Character->GetCombat()->SetBaseWalkSpeed(BaseSpeed);
	}
}

void UBuffComponent::MulticastJumpBuff_Implementation(float JumpVelocity)
{
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	}
}