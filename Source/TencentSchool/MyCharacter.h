// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Gun.h"

#include "MyCharacter.generated.h"

UCLASS()
class TENCENTSCHOOL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Property replication
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	const float MaxHealth = 1.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		float Health;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
		int Score;

	UPROPERTY(BlueprintReadWrite, Replicated)
		int KillCount;

	UPROPERTY(BlueprintReadWrite, Replicated)
		int DeathCount;

	UPROPERTY(BlueprintReadWrite, Replicated)
		bool Alive;

	UPROPERTY()
		UCapsuleComponent* Collider;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UParticleSystem* ReviveEffect;

	UPROPERTY(BlueprintReadWrite)
		AGun* EquippedGun;

	UFUNCTION()
		void GainScore();

	UFUNCTION(BlueprintCallable, Category = "Health")
		float TakeDamage(float DamageTaken = 0.2f);

	UFUNCTION(BlueprintCallable)
		void Die();

	UFUNCTION(BlueprintCallable)
		void KillOne();

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);
};
