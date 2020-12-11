// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h"

#include "MyCharacter.h"
#include "Target.h"
#include "AIPawn.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Red, OtherActor->GetName());
	//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, OtherActor->StaticClass()->GetName());

	if (OtherActor->IsA<ATarget>())
	{ // Hit the target
		if (GetLocalRole() == ROLE_Authority)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, "Bullet hit target");

			// Gain score for hitting the target
			auto MyCharacter = Cast<AMyCharacter>(this->Shooter);

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
	} 
	else if (OtherActor->IsA<AAIPawn>())
	{ // Hit the AI
		auto pawn = Cast<AAIPawn>(OtherActor);
		pawn->GotHit(Hit.BoneName);
	}
	else if (OtherActor->IsA<AMyCharacter>())
	{ // Hit another player
		if (GetLocalRole() == ROLE_Authority)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 20, FColor::Yellow, "Server hit another player!!!!!!");
			UKismetSystemLibrary::PrintText(GetWorld(), FText::FromString("Server hit another player!!!"));
			auto PlayerGotHit = Cast<AMyCharacter>(OtherActor);
			PlayerGotHit->GotDamage();

			if (!PlayerGotHit->Alive)
			{
				auto PlayerShooter = Cast<AMyCharacter>(this->Shooter);
				PlayerShooter->KillOne();
			}
		}
	}
	else
	{ // Not hit the target, finish the game
		this->NoHit();
		//TEnumAsByte<EQuitPreference::Type> QuitPreference;
		//UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), QuitPreference, false);
	}
	this->Destroy();
}