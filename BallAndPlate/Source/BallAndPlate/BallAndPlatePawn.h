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
		USceneComponent* OurVisibleComponent1; // ball 
	UPROPERTY(EditAnywhere)
		USceneComponent* OurVisibleComponent2; // plate 
	UPROPERTY(EditAnywhere)
		AActor* kolX;
	UPROPERTY(EditAnywhere)
		AActor* kolY;
	UPROPERTY(EditAnywhere)
		AActor* kolXReference;
	UPROPERTY(EditAnywhere)
		AActor* kolYReference;

	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights1;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights2;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights3;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights4;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights5;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights6;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights7;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights8;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights9;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights10;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights11;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights12;
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights13;
	

	
	// Input functions
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);


	// Input variables
	FVector CurrentVelocity;
	FVector rot;
	bool bGrowing;
	
};
