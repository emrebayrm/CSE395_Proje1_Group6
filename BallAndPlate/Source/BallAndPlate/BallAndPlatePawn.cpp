// Fill out your copyright notice in the Description page of Project Settings.

#include "BallAndPlate.h"
#include "BallAndPlatePawn.h"

float a=0.0f;

// Sets default values
ABallAndPlatePawn::ABallAndPlatePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create a dummy root component we can attach things to.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));

//	OurCamera->SetupAttachment(RootComponent);
	//OurCamera->SetRelativeLocation(FVector(0.0f, -40.0f, 1200.0f));
	//OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

	OurVisibleComponent1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent1"));
	OurVisibleComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent2"));

	OurVisibleComponent1->AttachTo(RootComponent);
	OurVisibleComponent2->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ABallAndPlatePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABallAndPlatePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


	// Handle growing and shrinking based on our "Grow" action
	{
		float CurrentScale = OurVisibleComponent1->GetComponentScale().X;
		if (bGrowing)
		{
			// Grow to double size over the course of one second
			CurrentScale += DeltaTime;
		}
		else
		{
			// Shrink half as fast as we grow
			CurrentScale -= (DeltaTime * 0.5f);
		}
		// Make sure we never drop below our starting size, or increase past double size.
		CurrentScale = FMath::Clamp(CurrentScale, 1.0f, 2.0f);
		OurVisibleComponent1->SetWorldScale3D(FVector(CurrentScale));
		//OurVisibleComponent->
	}

	// Handle movement based on our "MoveX" and "MoveY" axes
	{
		if (!CurrentVelocity.IsZero())
		{
			rot.X = OurVisibleComponent1->GetRelativeTransform().GetLocation().X;
			rot.X += 10.0f;

			rot.Y = OurVisibleComponent1->GetRelativeTransform().GetLocation().Y;
			rot.Z = OurVisibleComponent1->GetRelativeTransform().GetLocation().Z;

			FVector NewLocation = rot;
			OurVisibleComponent1->SetRelativeLocation(NewLocation);
			FRotator rotation = FRotator(OurVisibleComponent1->GetRelativeTransform().GetRotation());
			//FRotator b = FRotator(CurrentVelocity.Rotation);
			//rotation += b;
			rotation.Pitch += (a = a + 1.0f);
			rotation.Roll += (a = a + 1.0f);
			//RootComponent->SetRelativeRotation(rotation);

			
		}
	}

}

// Called to bind functionality to input
void ABallAndPlatePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{

	Super::SetupPlayerInputComponent(InputComponent);

	// Respond when our "Grow" key is pressed or released.
	InputComponent->BindAction("Grow", IE_Pressed, this, &ABallAndPlatePawn::StartGrowing);
	InputComponent->BindAction("Grow", IE_Released, this, &ABallAndPlatePawn::StopGrowing);

	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("MoveX", this, &ABallAndPlatePawn::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &ABallAndPlatePawn::Move_YAxis);

}

void ABallAndPlatePawn::Move_XAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 10.0f) * 100.0f;
	rot.X = FMath::Clamp(AxisValue, -1.0f, 10.0f) * 5.0f;
}

void ABallAndPlatePawn::Move_YAxis(float AxisValue)
{
	// Move at 100 units per second right or left
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
	rot.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f)* 5.0f;
	UE_LOG(LogTemp, Warning, TEXT("Your message : %f"), AxisValue);
}

void ABallAndPlatePawn::StartGrowing()
{
	bGrowing = true;
}

void ABallAndPlatePawn::StopGrowing()
{
	bGrowing = false;
}

