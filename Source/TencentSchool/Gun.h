// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Rotator.h"

#include "Gun.generated.h"

UCLASS()
class TENCENTSCHOOL_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		ACharacter* Shooter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator MuzzleRotation;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, category = "MyEvent")
		void StartFire(ACharacter* NewShooter);
};
