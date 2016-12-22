// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Networking.h"
#include "BallAndPlatePawn.generated.h"

#define PORT 9999
#define PACKET_SIZE 1

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
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void setUpLights();

	inline float map(float x, float in_min, float in_max, float out_min, float out_max)
	{
		return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
	}

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ABallAndPlatePawn();

	UPROPERTY(EditAnywhere)
		USceneComponent* CollectibleComponent1; // ring
	UPROPERTY(EditAnywhere)
		USceneComponent* CollectibleComponent2; // ring
	UPROPERTY(EditAnywhere)
		USceneComponent* CollectibleComponent3; // ring

	/*FVector CollectibleComponent1Loaction;
	FVector CollectibleComponent2Loaction;
	FVector CollectibleComponent3Loaction;*/

	bool IsGame = true;

	void GameMode(int CollectibleComponent1LoactionX, int CollectibleComponent1LoactionY,
			      int CollectibleComponent2LoactionX, int CollectibleComponent2LoactionY,
		          int CollectibleComponent3LoactionX, int CollectibleComponent3LoactionY);

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
		AActor* wingX;
	UPROPERTY(EditAnywhere)
		AActor* wingY;


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
	FString handShakeSendMessage = TEXT("G");
	FString stringReceiveData;        // To convert binary to string.
	TArray<uint8> binaryReceivedData; // Socket reads binary values.


	void ConnectToServer();

	void Connect(
		const FString& YourChosenSocketName,
		const FIPv4Address& ip,
		int32 port
	);

	void readValueFromSocket();
	void checkConnection();
	void parseInput(FString& input);

	// Debug functions.
	void printCoordinates();
	void printYem();

	bool didConnect;
	FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);

	int ballXCoordinate = 0;
	int ballYCoordinate = 0;
	int motorXAngle = 0;
	int motorYAngle = 0;

	int yem1[2];
	int yem2[2];
	int yem3[2];
};
