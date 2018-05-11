// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotEnemy.h"

ARobotEnemy::ARobotEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    ChangeStartingHealthTo(BossHealth);
    IsNormalAttack = false;
    IsSpecialAttack = false;
    IsHealing = false;
}

bool ARobotEnemy::GetRight() const
{
    return Right;
}

void ARobotEnemy::SetRight(bool State)
{
    Right = State;
}

bool ARobotEnemy::GetLeft() const
{
    return Left;
}

void ARobotEnemy::SetLeft(bool State)
{
    Left = State;
}

bool ARobotEnemy::GetFront() const
{
    return Front;
}

void ARobotEnemy::SetFront(bool State)
{
    Front = State;
}

bool ARobotEnemy::GetBack() const
{
    return Back;
}

void ARobotEnemy::SetBack(bool State)
{
    Back = State;
}

void ARobotEnemy::SetIsHit(bool State)
{
    IsHit = State;
}

bool ARobotEnemy::GetIsHit() const
{
    return IsHit;
}

float ARobotEnemy::GetNormalAttack() const
{
    return NormalAttack;
}

float ARobotEnemy::GetSpecialAttack() const
{
    return SpecialAttack;
}

void ARobotEnemy::SetIsNormalAttack(bool State)
{
    IsNormalAttack = State;
}

bool ARobotEnemy::GetIsNormalAttack() const
{
    return IsNormalAttack;
}

void ARobotEnemy::SetIsSpecialAttack(bool State)
{
    IsSpecialAttack = State;
}

bool ARobotEnemy::GetIsSpecialAttack() const
{
    return IsSpecialAttack;
}

void ARobotEnemy::SetIsHealing(bool State)
{
    IsHealing = State;
}

bool ARobotEnemy::GetIsHealing() const
{
    return IsHealing;
}

void ARobotEnemy::IncreaseHealth()
{
    if (GetHealth() <= BossHealth) {
        ChangeStartingHealthTo(GetHealth() + 1);
    }
}
