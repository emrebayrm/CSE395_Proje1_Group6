// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PlateAndBallActor.generated.h"

UCLASS()
class BALLANDPLATE_API APlateAndBallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlateAndBallActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere)
		USceneComponent* plate;
	UPROPERTY(EditAnywhere)
		USceneComponent* ball;

};
