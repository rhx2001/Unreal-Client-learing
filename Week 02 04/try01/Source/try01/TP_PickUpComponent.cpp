// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_PickUpComponent.h"
#include "TP_WeaponComponent.h"
#include "GunActor.h"
#include "MyPlayerState.h"

UTP_PickUpComponent::UTP_PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UTP_PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpComponent::OnSphereBeginOverlap);
}

void UTP_PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	Atry01Character* Character = Cast<Atry01Character>(OtherActor);
	if(Character != nullptr )
	{
		if (!Character->GetPlayerState<AMyPlayerState>()->bTakeGun)
		{
			Character->GetPlayerState<AMyPlayerState>()->bTakeGun = true;
			OnPickUp.Broadcast(Character);
			HandlePickUp(Character);

		}
		// Unregister from the Overlap Event so it is no longer triggered
		//OnComponentBeginOverlap.RemoveAll(this);
	}
}

void UTP_PickUpComponent::HandlePickUp_Implementation(Atry01Character* Character)
{
	AddWidgetToViewPort();
	Character->MyGun = Cast<AGunActor>(GetOwner());
}

void UTP_PickUpComponent::AddWidgetToViewPort_Implementation()
{
	AGunActor* Gun = Cast<AGunActor>(GetOwner());
	if (Gun && Gun->MyUserAmmoClass)
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (PlayerController && PlayerController->IsLocalController())
			{
				// 创建 UMG Widget 实例
				Gun->MyUserAmmoInstance = CreateWidget<UUserWidget>(GetWorld(), Gun->MyUserAmmoClass);
				if (Gun->MyUserAmmoInstance)
				{
					// 将 Widget 添加到视口
					Gun->MyUserAmmoInstance->AddToViewport();
					UMyWeaponWidgets* AmmoState = Cast<UMyWeaponWidgets>(Gun->MyUserAmmoInstance);
					AmmoState->UpdateAmmos(Gun->LoadAmmos, Gun->AmmosNum);
				}
			}
			// Notify that the actor is being picked up
		}
}