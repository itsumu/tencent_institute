// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "Net/UnrealNetwork.h"
#include"Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyCharacter::AMyCharacter(): Health(MaxHealth), Score(0), KillCount(0), DeathCount(0), Alive(true)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = GetCapsuleComponent();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//this->SetOwner(GetController());
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Property replication
void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCharacter, Health);
	DOREPLIFETIME(AMyCharacter, Score);
	DOREPLIFETIME(AMyCharacter, KillCount);
	DOREPLIFETIME(AMyCharacter, DeathCount);
	DOREPLIFETIME(AMyCharacter, Alive);
}

void AMyCharacter::GainScore() 
{
	this->Score += 10;
}

float AMyCharacter::TakeDamage(float DamageTaken)
{
	float HealthBefore = this->Health;
	
	UKismetSystemLibrary::PrintText(GetWorld(), FText::FromString("TakeDamage~"));

	this->Health = FMath::Clamp(this->Health - DamageTaken, 0.f, MaxHealth);

	if (this->Health < 0.01f)
	{
		Die();
	}
	return HealthBefore - this->Health;
}

void AMyCharacter::Die()
{
	UWorld* world = GetWorld();

	auto respawn = [this, world]()
	{
		this->Alive = true;
		this->Health = this->MaxHealth;
		if (this->ReviveEffect != nullptr)
		{
			//this->ReviveEffect->ActivateSystem();
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ReviveEffect, GetActorTransform());
		}
	};

	FTimerHandle UnusedHandle;
	float TimeDelay = 2.0f;
	GetWorldTimerManager().SetTimer(UnusedHandle, respawn, TimeDelay, false);
	++(this->DeathCount);
	this->Alive = false;
}

void AMyCharacter::KillOne()
{
	++(this->KillCount);
}


void AMyCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	if (OtherActor->GetClass()->IsChildOf<AGun>())
	{  // Pick up gun if overlap with a gun
		auto NewGun = Cast<AGun>(OtherActor);

		// Drop the old gun
		AGun* OldGun = this->EquippedGun;
		OldGun->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true));
		OldGun->SetActorLocationAndRotation(NewGun->GetActorLocation(), NewGun->GetActorQuat());

		// Attach new gun to the hand grip
		this->EquippedGun = NewGun;
		NewGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget, true), "GripPoint");
	}

}