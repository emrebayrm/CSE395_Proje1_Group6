// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "BallAndPlatePawn.generated.h"

UCLASS()
class BALLANDPLATE_API ABallAndPlatePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallAndPlatePawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere)
		USceneComponent* OurVisibleComponent2;
	UPROPERTY(EditAnywhere)
		USceneComponent* OurVisibleComponent1;
	/*UPROPERTY(EditAnywhere)
		UCameraComponent* OurCamera;*/

	// Input functions
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	void StartGrowing();
	void StopGrowing();

	// Input variables
	FVector CurrentVelocity;
	FVector rot;
	bool bGrowing;
	
};
