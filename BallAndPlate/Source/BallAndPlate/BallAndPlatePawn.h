// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Networking.h"
#include "BallAndPlatePawn.generated.h"
 
#define PORT 9999
#define PACKET_SIZE 20

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
	
	void setUpLights();

	inline float map(long x, long in_min, long in_max, long out_min, long out_max)
	{
		return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
	}

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ABallAndPlatePawn();


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
	UPROPERTY(EditAnywhere)
		TArray<APointLight*> UprightLights14;
	

	TArray<TArray<APointLight*>> lights;
	
	// Input functions
	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);


	// Input variables
	FVector CurrentVelocity;
	FVector rot;
	bool bGrowing;
	void UpdateLights();

	

	//////////// Socket ////////////////
	FSocket* ListenerSocket;
	FSocket* ConnectionSocket;
	FIPv4Endpoint RemoteAddressForConnection;
	FTimerHandle timeHandler;


	void ConnectToServer();

	void Connect(
		const FString& YourChosenSocketName,
		const FIPv4Address& ip,
		int32 port
		);

	void readValueFromSocket();
	void checkConnection();
	void sendValueToSocket();



	bool didConnect;
	FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);
	
};
