// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blaster/BlasterComponents/LagCompensationComponent.h"

void AShotgun::FireShotgun(const TArray<FVector_NetQuantize>& HitTargets)
{
	AWeapon::Fire(FVector());
	
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		const FVector Start = SocketTransform.GetLocation();
		TMap<ABlasterCharacter*, uint32> HitMap;			// 각 Pellet이 맞춘 캐릭터마다 몇발 씩 맞았는지 저장하는 TMap
		TMap<ABlasterCharacter*, uint32> HeadShotHitMap;	// 각 Pellet이 맞춘 캐릭터마다 헤드샷을 몇발 씩 맞았는지 저장하는 TMap

		for (auto HitTarget : HitTargets)
		{
			FHitResult FireHit;
			WeaponTraceHit(Start, HitTarget, FireHit);

			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
			if (BlasterCharacter)
			{
				const bool bHeadShot = FireHit.BoneName.ToString() == FString("head");

				if (bHeadShot)
				{
					if (HeadShotHitMap.Contains(BlasterCharacter)) HeadShotHitMap[BlasterCharacter]++;
					else HeadShotHitMap.Emplace(BlasterCharacter, 1);
				}
				else
				{
					if (HitMap.Contains(BlasterCharacter)) HitMap[BlasterCharacter]++;
					else HitMap.Emplace(BlasterCharacter, 1);
				}

				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
				}

				if (HitSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, HitSound, FireHit.ImpactPoint, 0.5f, FMath::FRandRange(-0.5f, 0.5f));
				}
			}
		}

		TArray<ABlasterCharacter*> HitCharacters;
		// Maps Character hit to total damage
		TMap<ABlasterCharacter*, float> DamageMap;

		// Calculate body shot damage by multiplying times hit x Damage - stored in Damagemap
		for (auto HitPair : HitMap)
        {
        	if (HitPair.Key)
        	{
        		DamageMap.Emplace(HitPair.Key, HitPair.Value * Damage);

        		HitCharacters.AddUnique(HitPair.Key);
        	}
        }
		// Calculate head shot damage by multiplying times hit x HeadShotDamage - stored in Damagemap
		for (auto HeadShotHitPair : HeadShotHitMap)
		{
			if (HeadShotHitPair.Key)
			{
				if (DamageMap.Contains(HeadShotHitPair.Key))
				{
					DamageMap[HeadShotHitPair.Key] += HeadShotHitPair.Value * HeadShotDamage;
				}
				else
				{
					DamageMap.Emplace(HeadShotHitPair.Key, HeadShotHitPair.Value * HeadShotDamage);
				}

				HitCharacters.AddUnique(HeadShotHitPair.Key);
			}
		}

		// Loop through DamageMap to get total damage for each character
		for (auto DamagePair : DamageMap)
		{
			if (DamagePair.Key && InstigatorController)
			{
				bool bCauseAuthDamage = !bUseServerSideRewind || OwnerPawn->IsLocallyControlled();
				if (HasAuthority() && bCauseAuthDamage)
				{
					UGameplayStatics::ApplyDamage(DamagePair.Key, DamagePair.Value, InstigatorController, this, UDamageType::StaticClass());	
				}
			}
		}
		
		if (!HasAuthority() && bUseServerSideRewind)
		{
			BlasterOwnerCharacter = BlasterOwnerCharacter == nullptr ? Cast<ABlasterCharacter>(OwnerPawn) : BlasterOwnerCharacter;
			BlasterOwnerController = BlasterOwnerController == nullptr ? Cast<ABlasterPlayerController>(InstigatorController) : BlasterOwnerController;
			if (BlasterOwnerCharacter && BlasterOwnerController && BlasterOwnerCharacter->GetLagCompensation() && BlasterOwnerCharacter->IsLocallyControlled())
			{
				BlasterOwnerCharacter->GetLagCompensation()->ShotgunServerScoreRequest(HitCharacters, Start, HitTargets, BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime);
			}
		}
	}
}

void AShotgun::ShotgunTraceEndWithScatter(const FVector& HitTarget, TArray<FVector_NetQuantize>& HitTargets)
{
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket == nullptr) return;
	
	const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
	const FVector TraceStart = SocketTransform.GetLocation();
	
	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;

	for (uint32 i = 0; i < NumberOfPellets; i++)
	{
		const FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
		const FVector EndLoc = SphereCenter + RandVec;
		FVector ToEndLoc = EndLoc - TraceStart;
		ToEndLoc = TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size();
		
		HitTargets.Add(ToEndLoc);
	}
}
