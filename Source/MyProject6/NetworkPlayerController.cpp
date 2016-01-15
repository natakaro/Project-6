// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject6.h"
#include "NetworkPlayerController.h"
#include <string>
#include "SocketSubsystem.h"
#include "IPv4SubnetMask.h"
#include "IPv4Address.h"
#include "IPv4Endpoint.h"
#include "TcpSocketBuilder.h"
#include "UnrealMathUtility.h"
#include "TimerManager.h"
#include "MyGameInstance.h"



// Sets default values
ANetworkPlayerController::ANetworkPlayerController(const FObjectInitializer& ObjectInitializer)
	: APlayerController(ObjectInitializer)
	//, Socket(nullptr)

{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/**How fast we move*/
}



ANetworkPlayerController::~ANetworkPlayerController()
{
	/*
	if (Connection != nullptr)
	{
		Connection->Close();

		delete Connection;
	}

	if (Socket != nullptr)
	{
		Socket->Close();

		delete Socket;
	}
	*/
}

// Called when the game starts or when spawned
void ANetworkPlayerController::BeginPlay()
{

	Super::BeginPlay();
	SGI = Cast<UMyGameInstance>(GetGameInstance());
	SGI->dd();
	SGI->run();
	
	bool canBind = false;
	TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);

	if (localIp->IsValid())
	{
		GLog->Log(localIp->ToString(false)); // if you want to append the port (true) or not (false).
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, localIp->ToString(false));
		IP = localIp->ToString(false);
	}



	
}

// Called every frame
void ANetworkPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	connect();
}

void ANetworkPlayerController::connect()
{
	pitch = SGI->pitch;
	roll = SGI->roll;
	yaw = SGI->yaw;
	shoot = SGI->shoot;
}


FString ANetworkPlayerController::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	//Create a string from a byte array!
	const std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());

	//FString can take in the c_str() of a std::string
	return FString(cstr.c_str());
}
