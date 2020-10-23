// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Target.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//FScriptDelegate DelegateHit;
	//DelegateHit.BindUFunction(this, "OnHit");
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	Collider = this->FindComponentByClass<UCapsuleComponent>();
	Collider->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor->GetClass()->GetSuperClass() == ATarget::StaticClass()) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, "ABullet OnHit---------");
		}
	}
}