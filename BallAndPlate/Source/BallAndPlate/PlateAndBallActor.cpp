// Fill out your copyright notice in the Description page of Project Settings.

#include "BallAndPlate.h"
#include "PlateAndBallActor.h"


// Sets default values
APlateAndBallActor::APlateAndBallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));


	plate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("plate"));
	ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ball"));

	plate->AttachTo(RootComponent);
	ball->AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void APlateAndBallActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlateAndBallActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

