#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),

	EWT_MAX UMETA(DisplayName = "DefaultMax")
};

FORCEINLINE FString GetWeaponTypeString(EWeaponType WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponType::EWT_AssaultRifle:
		return FString(TEXT("Assault Rifle"));
	case EWeaponType::EWT_RocketLauncher:
		return FString(TEXT("Rocket Launcher"));
	default:
		return FString();
	}
}
