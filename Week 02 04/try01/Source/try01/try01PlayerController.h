// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "try01PlayerController.generated.h"


class UInputMappingContext;

/**
 *
 */
UCLASS()
class TRY01_API Atry01PlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReadyInputAction;



	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	void SetupInputComponent();


	// End Actor interface
public:
	UFUNCTION(Server, Reliable)
	void ServerReady();

	UFUNCTION(NetMulticast, Reliable)
	void SetPlayerEnabledState(bool bEnabled);

	void HandleReadyInput(const FInputActionValue& Value);
};
