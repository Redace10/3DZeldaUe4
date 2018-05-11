// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ZeldaCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/MovementComponent.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AZeldaCharacter

AZeldaCharacter::AZeldaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
    
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
    
    SprintSpeedMultiplier = 2.0f;
    
    SprintSpeed = GetCharacterMovement()->MaxWalkSpeed * 2;
    WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
    
    MaxHealth = 5;
    Health = MaxHealth;
    Magic = 1;
    SlashSword = false;
    
    // movement variables to avoid overlaps
    CanJump = true;
    CanWalk = true;
    AttackDamage = DefaultAttackDamage;
    CanBeHit = true;
    Dead = false;
    BlockWithShield = false;
    IsHit = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AZeldaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AZeldaCharacter::DoJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AZeldaCharacter::DoStopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AZeldaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AZeldaCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AZeldaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AZeldaCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AZeldaCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AZeldaCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AZeldaCharacter::OnResetVR);
    
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AZeldaCharacter::Sprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AZeldaCharacter::StopSprinting);
    
    PlayerInputComponent->BindAction("SlashAttack", IE_Pressed, this, &AZeldaCharacter::SlashAttack);
    
    PlayerInputComponent->BindAction("ShieldBlock", IE_Pressed, this, &AZeldaCharacter::ShieldBlock);
    
    PlayerInputComponent->BindAction("ShieldBlock", IE_Released, this, &AZeldaCharacter::StopShieldBlock);
}


void AZeldaCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AZeldaCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
    Jump();
}

void AZeldaCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
    StopJumping();
}

void AZeldaCharacter::DoJump()
{
    if (CanJump) {
        Jump();
    }
}

void AZeldaCharacter::DoStopJumping()
{
    StopJumping();
}

void AZeldaCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AZeldaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AZeldaCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && CanWalk)
	{
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
	}
}

void AZeldaCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && CanWalk)
	{
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        
        // get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
	}
}

void AZeldaCharacter::Sprint() {
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AZeldaCharacter::StopSprinting() {
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

float AZeldaCharacter::GetHealth() const
{
    return Health;
}

float AZeldaCharacter::GetMaxHealth() const
{
    return MaxHealth;
}

void AZeldaCharacter::AddHealth(float Amount) {
    if (Health < MaxHealth) {
        Health += Amount;
    }
}

float AZeldaCharacter::GetMagic() const
{
    return Magic;
}

bool AZeldaCharacter::GetSlashSword() const
{
    return SlashSword;
}

bool AZeldaCharacter::GetBlockWithShield() const
{
    return BlockWithShield;
}

void AZeldaCharacter::SlashAttack()
{
    if (SlashSword == false && GetCharacterMovement()->IsMovingOnGround() && BlockWithShield == false) {
        SlashSword = true;
        //GetCharacterMovement()->StopMovementImmediately();
        //CanWalk = false;
        CanJump = false;
        FTimerHandle handle;
        GetWorldTimerManager().SetTimer(handle, [this]() {
            SlashSword = false;
            //CanWalk = true;
            CanJump = true;
        }, SlashDelayTime, false);
    }
}

void AZeldaCharacter::ShieldBlock()
{
    if (SlashSword == false && GetCharacterMovement()->IsMovingOnGround()) {
        BlockWithShield = true;
        GetCharacterMovement()->StopMovementImmediately();
        CanWalk = false;
    }
}

void AZeldaCharacter::StopShieldBlock()
{
    BlockWithShield = false;
    CanWalk = true;
}

float AZeldaCharacter::GetAttackDamage() const
{
    return AttackDamage;
}

float AZeldaCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
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

bool AZeldaCharacter::GetCanBeHit() const
{
    return CanBeHit;
}

void AZeldaCharacter::SetCanBeHit(bool State)
{
    CanBeHit = State;
}

bool AZeldaCharacter::GetIsHit() const
{
    return IsHit;
}

void AZeldaCharacter::SetIsHit(bool State)
{
    IsHit = State;
    if (IsHit) {
        GetCharacterMovement()->StopMovementImmediately();
        CanWalk = false;
    } else {
        CanWalk = true;
    }
}

void AZeldaCharacter::SetDeadState(bool State)
{
    Dead = State;
}

bool AZeldaCharacter::GetDeadState()
{
    return Dead;
}

void AZeldaCharacter::InflictDamage(AActor* ImpactActor)
{
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController != nullptr)
    {
        // Create a damage event
        TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
        FDamageEvent DamageEvent(ValidDamageTypeClass);
        
        ImpactActor->TakeDamage(AttackDamage, DamageEvent, PlayerController, this);
    }
}
