// Fill out your copyright notice in the Description page of Project Settings.

#include "GrassEnemy.h"

AGrassEnemy::AGrassEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    IsHowl = false;
    ChangeStartingHealthTo(EnemyHealth);
}

bool AGrassEnemy::GetIsHowl() const
{
    return IsHowl;
}

void AGrassEnemy::SetIsHowl(bool HowlState)
{
    IsHowl = HowlState;
}



