// Copyright Epic Games, Inc. All Rights Reserved.

#include "try01Character.h"
#include "try01Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyGameInstance.h"
#include "Engine/LocalPlayer.h"
#include "MyUserWidget.h"
#include "Net/UnrealNetwork.h"
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// Atry01Character

Atry01Character::Atry01Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(false);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/FirstPerson/Blueprints/PlayerStateWidget")); // 替换为实际路径
	if (WidgetClass.Succeeded())
	{
		MyUserWidgetClass = WidgetClass.Class;
	}
}

void Atry01Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	GetUi();
}

//////////////////////////////////////////////////////////////////////////// Input

void Atry01Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Atry01Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &Atry01Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void Atry01Character::SetUi_Implementation()
{
	FString LevelName = FPackageName::GetShortName(GetWorld()->GetMapName());
	UE_LOG(LogTemplateCharacter, Error, TEXT("the level name is: %s"), *LevelName);
	if (MyUserWidgetClass)
	{
		// 创建 UMG Widget 实例
		MyUserWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MyUserWidgetClass);
		if (MyUserWidgetInstance)
		{
			// 将 Widget 添加到视口
			MyUserWidgetInstance->AddToViewport();
			UMyUserWidget* ScoreState = Cast<UMyUserWidget>(MyUserWidgetInstance);
			//ScoreState->UpdateUi(0);
		}
	}
}

void Atry01Character::GetUi_Implementation()
{
	SetUi();
}


void Atry01Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void Atry01Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void Atry01Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(Atry01Character, MyGun);
}