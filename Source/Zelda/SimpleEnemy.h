// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SimpleEnemy.generated.h"

UCLASS()
class ZELDA_API ASimpleEnemy : public ACharacter
{
	GENERATED_BODY()
    
    bool CanBeHit;
    const float DefaultHealth = 10;
    float AttackDamage;
    const float DefaultAttackDamage = 0.5;
    bool Dead;

public:
	// Sets default values for this character's properties
	ASimpleEnemy();
    
    UFUNCTION(BlueprintPure, Category="EnemyHUD")
    float GetHealth() const;
    
    UFUNCTION(BlueprintPure, Category="EnemyHUD")
    bool GetCanBeHit() const;
    
    UFUNCTION(BlueprintCallable, Category="EnemyHUD")
    void SetCanBeHit(bool State);
    
    UFUNCTION(BlueprintCallable, Category="Death")
    void SetDeadState(bool State);
    
    UFUNCTION(BlueprintPure, Category="Death")
    bool GetDeadState();
    
    UFUNCTION(BlueprintCallable, Category="Damage")
    float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void InflictDamage(AActor* ImpactActor);
    
    UFUNCTION(BlueprintPure, Category="Attack")
    bool GetSlashSword() const;
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void SetSlashSword(bool IsSlashed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
    bool SlashSword;
    
    void ChangeStartingHealthTo(float NewHealth);
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void SetAttackDamageTo(float Damage);
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
    float Health;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
