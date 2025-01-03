// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeActor.generated.h"

UCLASS()
class TRY01_API ACubeActor : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ACubeActor();

	UPROPERTY(Category = StaticMeshActor, VisibleAnywhere, BlueprintReadWrite, meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|StaticMesh", AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	float ScaleSpeed = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	float TargetScalePara = 0.5f;

	FVector MeshScale;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	int32 HitTimes = 2;

	int32 BeHitTimes;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Replicated, Category = "Parameter")
	bool Important;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	uint32 CubeScore;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_ColorUpdated)
	FLinearColor CurrentColor;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);
	UFUNCTION(NetMulticast, Reliable)
	void AddScale(AActor* OtherActor);

	UFUNCTION()
	void OnRep_ColorUpdated();

private:
	UMaterialInstanceDynamic* DynamicMaterial;

};
