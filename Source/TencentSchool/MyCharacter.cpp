// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
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

void AMyCharacter::GainScore() 
{
	this->Score += 10;
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