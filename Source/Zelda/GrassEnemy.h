// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleEnemy.h"
#include "GrassEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ZELDA_API AGrassEnemy : public ASimpleEnemy
{
	GENERATED_BODY()
    bool IsHowl;
    const float EnemyHealth = 5;
    
public:
    AGrassEnemy();
    
    UFUNCTION(BlueprintPure, Category="Attack")
    bool GetIsHowl() const;
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void SetIsHowl(bool HowlState);
	
	
};
