// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "try01Character.h"
#include "try01Projectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GunActor.h"
#include "MyWeaponWidgets.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/FirstPerson/Blueprints/Weapons")); // 替换为实际路径

}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		AGunActor* Gun = Cast<AGunActor>(GetOwner());
		if (Gun)
		{

			if (Gun->LoadAmmos > 0) 
			{
				if (World != nullptr)
				{
					(*this).HandleFire();
				}
			}
			else 
			{
				return;
			}
		}
	}
	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::HandleFire_Implementation(){
	UE_LOG(LogTemp, Warning, TEXT("Fire"));
	AGunActor* Gun = Cast<AGunActor>(GetOwner());
	if (Gun)
	{
		if (Gun->LoadAmmos > 0) {
			UWorld* const World = GetWorld();
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			// Spawn the projectile at the muzzle
			Atry01Projectile* SpawnedProjectile = World->SpawnActor<Atry01Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (SpawnedProjectile)
			{
				SpawnedProjectile->SetOwner(this->GetOwner()->GetOwner());
				Gun->LoadAmmos--;
				Gun->OnRep_HandleAmmos();
			}
		}
	}
}

bool UTP_WeaponComponent::AttachWeapon(Atry01Character* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::Reload);
		}
	}

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_WeaponComponent::Reload_Implementation() {
	AGunActor* Gun = Cast<AGunActor>(GetOwner());
	if (Gun) 
	{
		uint32 AmmosMaxNum = Gun->AmmosMaxNum, AmmosNum = Gun->AmmosNum, LoadAmmos = Gun->LoadAmmos;
		if (AmmosMaxNum < AmmosNum + LoadAmmos)
		{
			AmmosNum += LoadAmmos;
			AmmosNum -= AmmosMaxNum;
			LoadAmmos = AmmosMaxNum;
		}
		else
		{
			LoadAmmos += AmmosNum;
			AmmosNum = 0;
		}
		Gun->AmmosNum = AmmosNum;
		Gun->LoadAmmos = LoadAmmos;

	}
}

