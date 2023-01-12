// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileRocket.h"
#include "Kismet/GameplayStatics.h"

AProjectileRocket::AProjectileRocket()
{
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket Mesh"));
	RocketMesh->SetupAttachment(RootComponent);
	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectileRocket::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                              FVector NormalImpulse, const FHitResult& Hit)
{
	// Instigator : 액터의 피해를 담당하는 APawn, Projectile 을 Weapon 에서 생성할 때 Weapon 의 Owner 를 Projectile 의 Instigator 로 설정하여 생성함
	APawn* FiringPawn = GetInstigator();
	if (FiringPawn)
	{
		AController* FiringController = FiringPawn->GetController();
		if (FiringController)
		{
			// DamageInnerRadius ~ DamageOuterRadius 의 범위 내에 데미지를 가한다.  DamageInnerRadius 로 갈수록 데미지는 MinimumDamage 에서 BaseDamage 에 가까워진다.
			UGameplayStatics::ApplyRadialDamageWithFalloff(this, Damage, 10.f, GetActorLocation(), 200.f, 500.f, 1.f, UDamageType::StaticClass(), TArray<AActor*>(), this, FiringController);
		}
	}
	
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}
