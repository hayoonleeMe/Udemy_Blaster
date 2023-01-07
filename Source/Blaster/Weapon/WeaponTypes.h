#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName = "Assault Rifle"),

	EWT_MAX UMETA(DisplayName = "DefaultMax")
};

FORCEINLINE FString GetWeaponTypeString(EWeaponType WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponType::EWT_AssaultRifle:
		return FString(TEXT("Assault Rifle"));
	default:
		return FString();
	}
}
