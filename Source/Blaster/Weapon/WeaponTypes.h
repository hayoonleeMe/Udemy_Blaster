#pragma once

#define TRACE_LENGTH 80000.f

#define CUSTOM_DEPTH_PURPLE 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),
	EWT_RocketLauncher UMETA(DisplayName = "Rocket Launcher"),
	EWT_Pistol UMETA(DisplayName = "Pistol"),
	EWT_SubmachineGun UMETA(DisplayName = "Submachine Gun"),
	EWT_Shotgun UMETA(DisplayName = "Shotgun"),
	EWT_SniperRifle UMETA(DisplayName = "Sniper Rifle"),
	EWT_GrenadeLauncher UMETA(DisplayName = "Grenade Launcher"),

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
	case EWeaponType::EWT_Pistol:
		return FString(TEXT("Pistol"));
	case EWeaponType::EWT_SubmachineGun:
		return FString(TEXT("Submachine Gun"));
	case EWeaponType::EWT_Shotgun:
		return FString(TEXT("Shotgun"));
	case EWeaponType::EWT_SniperRifle:
		return FString(TEXT("Sniper Rifle"));
	case EWeaponType::EWT_GrenadeLauncher:
		return FString(TEXT("Grenade Launcher"));
	default:
		return FString();
	}
}
