// Fill out your copyright notice in the Description page of Project Settings.

#include "BallAndPlate.h"
#include "BallAndPlatePawn.h"
#include <thread>
#include <string>

float a = 0.0f;

float differenceX_Z;
float differenceY_Z;
FRotator rotation;
// Sets default values
ABallAndPlatePawn::ABallAndPlatePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	OurVisibleComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent1"));
	OurVisibleComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent2"));
	
	CollectibleComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectibleComponent1"));
	CollectibleComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectibleComponent2"));
	CollectibleComponent3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollectibleComponent3"));

	CollectibleComponent1->SetupAttachment(RootComponent);
	CollectibleComponent2->SetupAttachment(RootComponent);
	CollectibleComponent3->SetupAttachment(RootComponent);

	OurVisibleComponent1->SetupAttachment(RootComponent);// AttachTo(RootComponent);
	OurVisibleComponent2->SetupAttachment(RootComponent); // AttachTo(RootComponent);

}

// Called when the game starts or when spawned
void ABallAndPlatePawn::BeginPlay()
{
	Super::BeginPlay();
	differenceX_Z = kolXReference->GetActorLocation().Z - kolX->GetActorLocation().Z;
	differenceY_Z = kolYReference->GetActorLocation().Z - kolY->GetActorLocation().Z;
	rotation = FRotator(RootComponent->GetRelativeTransform().GetRotation());
	setUpLights();

	/*CollectibleComponent1->SetVisibility(false);
	CollectibleComponent2->SetVisibility(false);
	CollectibleComponent3->SetVisibility(false);*/

	ConnectToServer();

}
void ABallAndPlatePawn::printCoordinates() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Ball X Coordinate : %d "), ballXCoordinate));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Ball y Coordinate : %d "), ballYCoordinate));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("motot x Coordinate : %d "), motorXAngle));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("motor y Coordinate : %d "), motorYAngle));

}
void ABallAndPlatePawn::printYem() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("yem1 X Coordinate : %d "), yem1[0]));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("yem1 y Coordinate : %d "), yem1[1]));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("yem2 X Coordinate : %d "), yem2[0]));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("yem2 y Coordinate : %d "), yem2[1]));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("yem3 x Coordinate : %d "), yem3[0]));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("yem y Coordinate : %d "), yem3[1]));

}
// Called every frame
void ABallAndPlatePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	readValueFromSocket();
	checkConnection();
	//printCoordinates();
	//printYem();
	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Ball X Coordinate : %d "), ballXCoordinate));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Ball y Coordinate : %d "), ballYCoordinate));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("motot x Coordinate : %d "), motorXAngle));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("motor y Coordinate : %d "), motorYAngle));*/

	//if (!CurrentVelocity.IsZero())
	{
		FVector refXLocation = kolXReference->GetActorLocation();;
		FVector armXLocation = kolX->GetActorLocation();
		armXLocation.Z = refXLocation.Z - differenceX_Z;
		kolX->SetActorLocation(armXLocation);

		FVector refYLocation = kolYReference->GetActorLocation();;
		FVector armYLocation = kolY->GetActorLocation();
		armYLocation.Z = refYLocation.Z - differenceY_Z;
		kolY->SetActorLocation(armYLocation);

		//RootComponent->SetRelativeRotation(rotation);

		FVector ballRelativeLocation = OurVisibleComponent1->GetRelativeTransform().GetLocation();
		ballRelativeLocation.X = ballXCoordinate;
		ballRelativeLocation.Y = ballYCoordinate;
		//OurVisibleComponent1->SetRelativeLocation(ballRelativeLocation + CurrentVelocity * DeltaTime * 4);
		OurVisibleComponent1->SetRelativeLocation(ballRelativeLocation	);
		UpdateLights();

		if (IsGame) {

			FVector CollectibleComponent1Loaction = CollectibleComponent1->GetRelativeTransform().GetLocation();
			FVector CollectibleComponent2Loaction = CollectibleComponent2->GetRelativeTransform().GetLocation();
			FVector CollectibleComponent3Loaction = CollectibleComponent3->GetRelativeTransform().GetLocation();

			if (abs(ballRelativeLocation.X - CollectibleComponent1Loaction.X) < 50 && abs(ballRelativeLocation.Y - CollectibleComponent1Loaction.Y) < 20) {
				CollectibleComponent1->SetVisibility(false);
			}

			if (abs(ballRelativeLocation.X - CollectibleComponent2Loaction.X) < 50 && abs(ballRelativeLocation.Y - CollectibleComponent2Loaction.Y) < 20) {
				CollectibleComponent2->SetVisibility(false);
			}

			if (abs(ballRelativeLocation.X - CollectibleComponent3Loaction.X) < 50 && abs(ballRelativeLocation.Y - CollectibleComponent3Loaction.Y) < 20) {
				CollectibleComponent3->SetVisibility(false);
			}

		}

	}

	/*{
		FVector ballRelativeLocation = OurVisibleComponent1->GetRelativeTransform().GetLocation();

		OurVisibleComponent1->SetRelativeLocation(ballRelativeLocation + FVector(2.0f, 0.0f, 0.0f) * DeltaTime*25.0f);

		UpdateLights();
	}*/

	

}


void ABallAndPlatePawn::GameMode(int CollectibleComponent1LoactionX, int CollectibleComponent1LoactionY,
	int CollectibleComponent2LoactionX, int CollectibleComponent2LoactionY,
	int CollectibleComponent3LoactionX, int CollectibleComponent3LoactionY)
{
	FVector CollectibleComponent1Loaction = CollectibleComponent1->GetRelativeTransform().GetLocation();
	FVector CollectibleComponent2Loaction = CollectibleComponent2->GetRelativeTransform().GetLocation();
	FVector CollectibleComponent3Loaction = CollectibleComponent3->GetRelativeTransform().GetLocation();

	CollectibleComponent1Loaction.X = CollectibleComponent1LoactionX;
	CollectibleComponent1Loaction.Y = CollectibleComponent1LoactionY;

	CollectibleComponent2Loaction.X = CollectibleComponent2LoactionX;
	CollectibleComponent2Loaction.Y = CollectibleComponent2LoactionY;

	CollectibleComponent3Loaction.X = CollectibleComponent3LoactionX;
	CollectibleComponent3Loaction.Y = CollectibleComponent3LoactionY;

	CollectibleComponent1->SetRelativeLocation(CollectibleComponent1Loaction);
	CollectibleComponent2->SetRelativeLocation(CollectibleComponent2Loaction);
	CollectibleComponent3->SetRelativeLocation(CollectibleComponent3Loaction);

	CollectibleComponent1->SetVisibility(true);
	CollectibleComponent2->SetVisibility(true);
	CollectibleComponent3->SetVisibility(true);



}

// Called to bind functionality to input
void ABallAndPlatePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{

	Super::SetupPlayerInputComponent(InputComponent);

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("MoveX", this, &ABallAndPlatePawn::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &ABallAndPlatePawn::Move_YAxis);


}

void ABallAndPlatePawn::Move_XAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 10.0f) * 100.0f;
	rot.X = FMath::Clamp(AxisValue, -1.0f, 10.0f) * 5.0f;
	rotation.Pitch += (a = AxisValue + 0.01f);

}

void ABallAndPlatePawn::Move_YAxis(float AxisValue)
{
	// Move at 100 units per second right or left
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
	rot.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f)* 5.0f;
	rotation.Roll += (a = AxisValue + 0.01f);

}

void ABallAndPlatePawn::setUpLights()
{
	int i = 0, j = 0;

	lights.Add(UprightLights1);
	lights.Add(UprightLights2);
	lights.Add(UprightLights3);
	lights.Add(UprightLights4);
	lights.Add(UprightLights5);
	lights.Add(UprightLights6);
	lights.Add(UprightLights7);
	lights.Add(UprightLights8);
	lights.Add(UprightLights9);
	lights.Add(UprightLights10);
	lights.Add(UprightLights11);
	lights.Add(UprightLights12);
	lights.Add(UprightLights13);
	lights.Add(UprightLights14);


	for (i = 0; i < lights.Num(); ++i) {
		for (j = 0; j < lights[i].Num(); ++j) {
			if (lights[i][j] != nullptr) {
				lights[i][j]->GetRootComponent()->SetVisibility(false);
			}

		}
	}
}

void ABallAndPlatePawn::UpdateLights() {

	FVector ballRelativeLocation = OurVisibleComponent1->GetRelativeTransform().GetLocation();

	if (ballRelativeLocation.X > 800 || ballRelativeLocation.X < -800 || ballRelativeLocation.Y >600 || ballRelativeLocation.Y < -600) {
		return;
	}

	int mapX = map(ballRelativeLocation.X, 800, -800, 0, 14);
	int mapY = map(ballRelativeLocation.Y, 600, -600, 0, 10);

	UE_LOG(LogTemp, Warning, TEXT("MAP X : %d"), mapX);
	UE_LOG(LogTemp, Warning, TEXT("MAP Y : %d"), mapY);

	int sDx = mapX + 1;
	int sAx = mapX - 1;
	int sWy = mapY + 1;
	int sXy = mapY - 1;

	int sQx = mapX - 1;
	int sQy = mapY + 1;

	int sEx = mapX + 1;
	int sEy = mapY + 1;

	int sZx = mapX - 1;
	int sZy = mapY - 1;

	int sCx = mapX + 1;
	int sCy = mapY - 1;

	if (sDx <= 13)
		lights[sDx][mapY]->GetRootComponent()->SetVisibility(true);
	if (sAx >= 0)
		lights[sAx][mapY]->GetRootComponent()->SetVisibility(true);
	if (sWy <= 9)
		lights[mapX][sWy]->GetRootComponent()->SetVisibility(true);
	if (sXy >= 0)
		lights[mapX][sXy]->GetRootComponent()->SetVisibility(true);
	if (sQx >= 0 && sQy <= 9)
		lights[sQx][sQy]->GetRootComponent()->SetVisibility(true);
	if (sEx <= 13 && sEy <= 9)
		lights[sEx][sEy]->GetRootComponent()->SetVisibility(true);
	if (sZx >= 0 && sZy >= 0)
		lights[sZx][sZy]->GetRootComponent()->SetVisibility(true);
	if (sCx <= 13 && sCy >= 0)
		lights[sCx][sCy]->GetRootComponent()->SetVisibility(true);

	lights[mapX][mapY]->GetRootComponent()->SetVisibility(true);

	for (int i = 0; i < lights.Num(); ++i) {
		for (int j = 0; j < lights[i].Num(); ++j) {
			if (lights[i][j] != nullptr && (i != mapX || j != mapY) && (i != sDx || j != mapY)
				&& (i != sAx || j != mapY) && (i != mapX || j != sWy)
				&& (i != mapX || j != sXy) && (i != sQx || j != sQy)
				&& (i != sEx || j != sEy) && (i != sZx || j != sZy)
				&& (i != sCx || j != sCy)) {
				lights[i][j]->GetRootComponent()->SetVisibility(false);
			}

		}
	}

}

void ABallAndPlatePawn::ConnectToServer()
{
	FString address = TEXT("127.0.0.1");
	int32 port = PORT;
	FIPv4Address ipAddress;
	FIPv4Address::Parse(address, ipAddress);
	Connect("SocketListener", ipAddress, port);
}

void ABallAndPlatePawn::Connect(
	const FString& YourChosenSocketName,
	const FIPv4Address& ip,
	int32 port
) {

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(port);

	ConnectionSocket = FTcpSocketBuilder(YourChosenSocketName).AsBlocking().AsReusable().Build();
	didConnect = ConnectionSocket->Connect(*addr);
	if (didConnect) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Connect Successful")));
		// close program
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connect Failed")));
		// close program
	}
}

void ABallAndPlatePawn::readValueFromSocket() {
	FString handShakeSendMessage = TEXT("G");
	TCHAR *serializedChar = handShakeSendMessage.GetCharArray().GetData();
	int32 size = FCString::Strlen(serializedChar);
	int32 sent = 0;
	FString stringReceiveData;        // To convert binary to string.

	bool succesfull = ConnectionSocket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent); // Send handshake message.
	if (succesfull)
	{

		TArray<uint8> binaryReceivedData; // Socket reads binary values.

		uint32 Size = 1;
		binaryReceivedData.Init(0, FMath::Min(Size, 65507u));
		int32 Read = 0;
		bool HasPendingConnection = false;

		// Wait for data.
		while (ConnectionSocket->HasPendingConnection(HasPendingConnection) && HasPendingConnection == false && !ConnectionSocket->HasPendingData(Size));

		//// Sample coordinate packet : "C{ 12 34 14 144 }"
		//// Sample yem koordinatlar� : "Y{ 123 14 151 15 15 51}
		int counter = 0;
		bool status = false;
		while (!status) {
			succesfull = ConnectionSocket->Recv(binaryReceivedData.GetData(), binaryReceivedData.Num(), Read);
			if (succesfull) { // Receive succesfull.
				stringReceiveData.Append(StringFromBinaryArray(binaryReceivedData));
				if (stringReceiveData[stringReceiveData.Len() - 1] == '}') { // Receive end.
					status = true;
					stringReceiveData[stringReceiveData.Len() - 1] = '\0';
				}
			}
			else { // Receive failed.Socket failed.
				didConnect = false;
				status = true;
			}
		}
	}
	else // Send failed
		didConnect = false;

	if (didConnect) {
		parseInput(stringReceiveData);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(" x Before map : %s "),*stringReceiveData));
	}
}
void ABallAndPlatePawn::parseInput(FString& input) {

	TArray<FString> Parsed;
	input.ParseIntoArray(Parsed, TEXT(" "));
	if (input[0] == 'C' || input[0] == 'c')
	{
		ballXCoordinate = map(FCString::Atoi(*Parsed[1]), 0, 400, 800, -800);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(" x Before map : %d "), FCString::Atoi(*Parsed[1])));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("x After map : %d "), ballXCoordinate));

		ballYCoordinate = map(FCString::Atoi(*Parsed[2]), 0, 300, -600, 600);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("y Before map : %d "), FCString::Atoi(*Parsed[2])));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(" y After map : %d "), ballYCoordinate));
		motorXAngle = FCString::Atoi(*Parsed[3]);
		motorYAngle = FCString::Atoi(*Parsed[4]);

	}
	else if (input[0] == 'Y' || input[0] == 'y')
	{
		yem1[0] = map(FCString::Atoi(*Parsed[1]) , 0 , 400 , 800 ,-800) ;
		yem1[1] = map(FCString::Atoi(*Parsed[1]), 0, 300, -600, 600);
		yem2[0] = map(FCString::Atoi(*Parsed[1]), 0, 400, 800, -800);
		yem2[1] = map(FCString::Atoi(*Parsed[1]), 0, 300, -600, 600);
		yem3[0] = map(FCString::Atoi(*Parsed[1]), 0, 400, 800, -800);
		yem3[1] = map(FCString::Atoi(*Parsed[1]), 0, 300, -600, 600);
	}
}

void ABallAndPlatePawn::checkConnection() {
	if (!didConnect) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("close programm")));
		// close program
	}
}
void ABallAndPlatePawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (ConnectionSocket)
	{
		ConnectionSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
	}
}


FString ABallAndPlatePawn::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	//Create a string from a byte array!
	const std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());

	//FString can take in the c_str() of a std::string
	return FString(cstr.c_str());
}
