// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

#include "Target.h"
#include "MyCharacter.h"

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

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) 
{
	if (OtherActor->GetClass()->GetSuperClass() == ATarget::StaticClass()) 
	{ // Hit the target
		if (GEngine) 
		{
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, "Bullet hit target");
		}

		// todo: Transfer to event trigger
		// Gain score for hitting the target
		ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (PlayerCharacter == nullptr) 
		{
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, "Player character not accessible");
			return;
		}
		AMyCharacter* MyCharacter = Cast<AMyCharacter>(PlayerCharacter);

		MyCharacter->GainScore();

		// Destroy target and spawn new one in a random range
		OtherActor->Destroy();

		FRotator Rotation(90, 0, 0);
		FVector Location(-300 + FMath::RandRange(-100, 100), 120 + FMath::RandRange(-100, 100), 280 + FMath::RandRange(-100, 100));

		AActor* pawn = GetWorld()->SpawnActor<ATarget>(this->NewTargetType, Location, Rotation);
		if (pawn == nullptr) 
		{
			GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, "New target not spawned");
		}
	}
	this->Destroy();
}