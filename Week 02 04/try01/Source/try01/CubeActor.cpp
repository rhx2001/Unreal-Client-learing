// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeActor.h"
#include "try01Projectile.h"
#include "Net/UnrealNetwork.h"
#include "MyPlayerState.h"
#include "try01GameMode.h"
#include "MyUserWidget.h"
#include "try01Character.h"
#include "MyWeaponWidgets.h"
#include "GunActor.h"

// Sets default values
ACubeActor::ACubeActor()
{


	bReplicates = true;
	bAlwaysRelevant = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ACubeActor::OnMeshHit);
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetNotifyRigidBodyCollision(true);
	RootComponent = StaticMeshComponent;

}

void ACubeActor::OnRep_ColorUpdated()
{

	if (DynamicMaterial) {
		DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), this->CurrentColor);
	}
}

// Called when the game starts or when spawned
void ACubeActor::BeginPlay()
{

	Super::BeginPlay();

	MeshScale = FVector(1.0f, 1.0f, 1.0f);
	BeHitTimes = 0;
	
	Atry01GameMode* GameMode = Cast<Atry01GameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode && GameMode->CurrentImportantCubes< GameMode->MaxImportantCubes && HasAuthority())
	{
		// 使用随机数决定是否设置为important
		if (FMath::RandBool())  // 随机决定
		{
			Important = true;
			GameMode->CurrentImportantCubes++;  // 增加important Cube数量
		}
	}


	if (StaticMeshComponent && StaticMeshComponent->GetMaterial(0)) {
		DynamicMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	}


	
}

// Called every frame
void ACubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurrentScale = StaticMeshComponent->GetComponentScale();
	FVector NewScale = FMath::VInterpTo(CurrentScale, MeshScale, DeltaTime, ScaleSpeed);
	StaticMeshComponent->SetWorldScale3D(NewScale);
	if (Important && HasAuthority()) {
		float Time = GetWorld()->GetTimeSeconds();
		FLinearColor NewColor = FLinearColor(
			FMath::Sin(Time) * 0.5f + 0.5f,  // Red
			FMath::Cos(Time) * 0.5f + 0.5f,  // Green
			FMath::Sin(Time * 0.5f) * 0.5f + 0.5f,  // Blue
			1.0f  // Alpha
		);

		if (!CurrentColor.Equals(NewColor)) {  // 避免不必要的复制
			this->CurrentColor = NewColor;
			OnRep_ColorUpdated();  // 手动调用来更新颜色
		}
	}
}

void ACubeActor::OnMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& Hit){
	if (OtherActor && OtherActor) {
		Atry01Character* OwningCharacter = Cast<Atry01Character>(OtherActor->GetOwner());
		if (OwningCharacter)
		{
			AGunActor* GunActor = OwningCharacter->MyGun;
			if (GunActor)
			{
				UMyWeaponWidgets* CrossHair = Cast<UMyWeaponWidgets>(GunActor->MyUserAmmoInstance);
				if (CrossHair)
				{
					CrossHair->AmmoOnHit();
					//ClientShowAmmoOnHit(CrossHair); // 通知 Client
				}
			}
		}
	}
	if (OtherActor && OtherActor->IsA(Atry01Projectile::StaticClass()) && HasAuthority()) {

		AddScale(OtherActor);
	}

}


void ACubeActor::AddScale_Implementation(AActor* OtherActor) 
{
	BeHitTimes++;
	if (BeHitTimes == HitTimes) 
	{
		AActor* OwnerActor = OtherActor->GetOwner();
		if(OwnerActor && HasAuthority())
		{
			APawn* OwnerPawn = Cast<APawn>(OwnerActor);
			if (OwnerPawn)
			{
				
				AMyPlayerState* OwnerPlayerState = Cast<AMyPlayerState>(OwnerPawn->GetPlayerState());
				if (OwnerPlayerState) 
				{
					if(Important)
					{
						OwnerPlayerState->PlayerScore += CubeScore * 2;
					}
					else 
					{
						OwnerPlayerState->PlayerScore += CubeScore;
					}
					if (HasAuthority())
					{
						OwnerPlayerState->OnRep_UpdateGrade();
					}
					UE_LOG(LogTemp, Warning, TEXT("Player %s Score updated to: %d"), *OwnerPlayerState->GetPlayerName(), OwnerPlayerState->PlayerScore);
					
				}
			}
		}
		Destroy();
	}
	OtherActor->Destroy();
	MeshScale -= FVector(TargetScalePara);
}

void ACubeActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACubeActor, CurrentColor);
	DOREPLIFETIME(ACubeActor, Important);
}