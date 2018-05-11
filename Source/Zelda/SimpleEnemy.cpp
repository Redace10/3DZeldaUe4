// Fill out your copyright notice in the Description page of Project Settings.

#include "SimpleEnemy.h"

// Sets default values
ASimpleEnemy::ASimpleEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Health = DefaultHealth;
    CanBeHit = true;
    AttackDamage = DefaultAttackDamage;
    SlashSword = false;
    Dead = false;
}

// Called when the game starts or when spawned
void ASimpleEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimpleEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASimpleEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ASimpleEnemy::GetHealth() const
{
    return Health;
}

float ASimpleEnemy::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
    // Call the base class - this will tell us how much damage to apply
    const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    if (ActualDamage > 0.f)
    {
        Health -= ActualDamage;
        // If the damage depletes our health set our lifespan to zero - which will destroy the actor
        if (Health <= 0.f)
        {
            Dead = true;
            CanBeHit = false;
        }
    }
    return ActualDamage;
}  

bool ASimpleEnemy::GetCanBeHit() const
{
    return CanBeHit;
}

void ASimpleEnemy::SetCanBeHit(bool State)
{
    CanBeHit = State;
}

void ASimpleEnemy::SetDeadState(bool State)
{
    Dead = State;
}

bool ASimpleEnemy::GetDeadState()
{
    return Dead;
}

void ASimpleEnemy::InflictDamage(AActor* ImpactActor)
{
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    // Create a damage event
    TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
    FDamageEvent DamageEvent(ValidDamageTypeClass);
        
    ImpactActor->TakeDamage(AttackDamage, DamageEvent, PlayerController, this);
}

bool ASimpleEnemy::GetSlashSword() const
{
    return SlashSword;
}

void ASimpleEnemy::SetSlashSword(bool IsSlashed)
{
    SlashSword = IsSlashed;
}

void ASimpleEnemy::ChangeStartingHealthTo(float NewHealth)
{
    Health = NewHealth;
}

void ASimpleEnemy::SetAttackDamageTo(float Damage)
{
    AttackDamage = Damage;
}
