// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleEnemy.h"
#include "RobotEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ZELDA_API ARobotEnemy : public ASimpleEnemy
{
	GENERATED_BODY()
	
	const float BossHealth = 30;
    const float NormalAttack = 0.5;
    const float SpecialAttack = 1;
    bool Right;
	bool Left;
    bool Front;
    bool Back;
    bool IsHit;
    bool IsNormalAttack;
    bool IsSpecialAttack;
    bool IsHealing;
    
public:
    
    ARobotEnemy();
    
    UFUNCTION(BlueprintPure, Category="Hit")
    bool GetRight() const;
    
    UFUNCTION(BlueprintCallable, Category="Hit")
    void SetRight(bool State);
    
    UFUNCTION(BlueprintPure, Category="Hit")
    bool GetLeft() const;
    
    UFUNCTION(BlueprintCallable, Category="Hit")
    void SetLeft(bool State);
    
    UFUNCTION(BlueprintPure, Category="Hit")
    bool GetFront() const;
    
    UFUNCTION(BlueprintCallable, Category="Hit")
    void SetFront(bool State);
    
    UFUNCTION(BlueprintPure, Category="Hit")
    bool GetBack() const;
    
    UFUNCTION(BlueprintCallable, Category="Hit")
    void SetBack(bool State);
    
    UFUNCTION(BlueprintPure, Category="Attack")
    bool GetIsHit() const;
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void SetIsHit(bool State);
    
    UFUNCTION(BlueprintPure, Category="Attack")
    float GetNormalAttack() const;
    
    UFUNCTION(BlueprintPure, Category="Attack")
    float GetSpecialAttack() const;
    
    UFUNCTION(BlueprintPure, Category="Attack")
    bool GetIsNormalAttack() const;
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void SetIsNormalAttack(bool State);
    
    UFUNCTION(BlueprintPure, Category="Attack")
    bool GetIsSpecialAttack() const;
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void SetIsSpecialAttack(bool State);
    
    UFUNCTION(BlueprintPure, Category="Attack")
    bool GetIsHealing() const;
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void SetIsHealing(bool State);
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void IncreaseHealth();
	
};
