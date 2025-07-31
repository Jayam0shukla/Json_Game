// Copyright Epic Games, Inc. All Rights Reserved.

#include "Variant_Shooter/ShooterProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "JsonBoxActor.h"

AShooterProjectile::AShooterProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the collision component and assign it as the root
	RootComponent = CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Component"));
	CollisionComponent->SetSphereRadius(16.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	// Create the projectile movement component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bShouldBounce = true;

	// Set default damage type
	HitDamageType = UDamageType::StaticClass();
}

void AShooterProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Ignore the pawn that fired this projectile
	CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void AShooterProjectile::NotifyHit(
	UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (bHit) return;
	bHit = true;

	// AI perception noise
	MakeNoise(NoiseLoudness, GetInstigator(), GetActorLocation(), NoiseRange, NoiseTag);

	// Apply physics impulse
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * PhysicsForce, Hit.ImpactPoint);
	}

	// Damage characters
	if (ACharacter* HitCharacter = Cast<ACharacter>(Other))
	{
		if (HitCharacter != GetOwner())
		{
			DamageCharacter(HitCharacter, Hit);
		}
	}

	// Damage dynamic boxes
	if (AJsonBoxActor* BoxActor = Cast<AJsonBoxActor>(Other))
	{
		BoxActor->ReceiveDamage(1, GetInstigator()); // Change 1 to any damage value
	}

	// Disable further collision
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Blueprint extension hook
	BP_OnProjectileHit(Hit);
}

void AShooterProjectile::DamageCharacter(ACharacter* HitCharacter, const FHitResult& Hit)
{
	UGameplayStatics::ApplyDamage(
		HitCharacter,
		HitDamage,
		GetInstigator()->GetController(),
		this,
		HitDamageType
	);
}