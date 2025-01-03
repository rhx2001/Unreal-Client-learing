// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunActor.generated.h"

UCLASS()
class TRY01_API AGunActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunActor();
	UPROPERTY(Replicated, EditAnywhere, ReplicatedUsing = OnRep_HandleAmmos)
	uint32 AmmosNum = 200;

	UPROPERTY(EditAnywhere)
	uint32 AmmosMaxNum = 20;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_HandleAmmos)
	uint32 LoadAmmos = 20;

	UPROPERTY()
	TSubclassOf<UUserWidget> MyUserAmmoClass;

	UPROPERTY()
	UUserWidget* MyUserAmmoInstance;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnRep_HandleAmmos();
	
	
};
