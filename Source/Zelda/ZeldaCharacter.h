// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZeldaCharacter.generated.h"

UCLASS(config=Game)
class AZeldaCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
    
    const float SlashDelayTime = 0.75f;
    
    // movement variables to avoid overlaps
    bool CanJump;
    bool CanWalk;
    
    float WalkSpeed;
    float SprintSpeed;
    
    float AttackDamage;
    const float DefaultAttackDamage = 1;
    bool CanBeHit;
    bool Dead;
    
    float MaxHealth;
    bool BlockWithShield;
    bool IsHit;
    
    
public:
	AZeldaCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character Movement")
    float SprintSpeedMultiplier;
    
    UFUNCTION(BlueprintPure, Category="HUD")
    float GetHealth() const;
    
    UFUNCTION(BlueprintPure, Category="HUD")
    float GetMaxHealth() const;
    
    UFUNCTION(BlueprintCallable, Category="HUD")
    void AddHealth(float Amount);
    
    UFUNCTION(BlueprintPure, Category="HUD")
    float GetMagic() const;
    
    UFUNCTION(BlueprintPure, Category="Attack")
    bool GetSlashSword() const;
    
    UFUNCTION(BlueprintPure, Category="Attack")
    bool GetBlockWithShield() const;
    
    UFUNCTION(BlueprintPure, Category="Attack")
    float GetAttackDamage() const;
    
    UFUNCTION(BlueprintPure, Category="Attack")
    bool GetCanBeHit() const;
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void SetCanBeHit(bool State);
    
    UFUNCTION(BlueprintPure, Category="Attack")
    bool GetIsHit() const;
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void SetIsHit(bool State);
    
    UFUNCTION(BlueprintCallable, Category="Death")
    void SetDeadState(bool State);
    
    UFUNCTION(BlueprintPure, Category="Death")
    bool GetDeadState();
    
    UFUNCTION(BlueprintCallable, Category="Damage")
    float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
    
    UFUNCTION(BlueprintCallable, Category="Attack")
    void InflictDamage(AActor* ImpactActor);

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
    
    void Sprint();
    
    void StopSprinting();
    
    void DoJump();
    
    void DoStopJumping();
    
    void SlashAttack();
    
    void ShieldBlock();
    
    void StopShieldBlock();
    

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
    float Health;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
    float Magic;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
    bool SlashSword;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

