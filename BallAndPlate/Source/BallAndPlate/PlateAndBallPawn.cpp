// Fill out your copyright notice in the Description page of Project Settings.

#include "BallAndPlate.h"
#include "PlateAndBallPawn.h"


// Sets default values
APlateAndBallPawn::APlateAndBallPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlateAndBallPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlateAndBallPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void APlateAndBallPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

