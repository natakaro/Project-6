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



// Sets default values
ANetworkPlayerController::ANetworkPlayerController(const FObjectInitializer& ObjectInitializer)
	: APlayerController(ObjectInitializer)
	, Socket(nullptr)

{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/**How fast we move*/
}



ANetworkPlayerController::~ANetworkPlayerController()
{
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
}

// Called when the game starts or when spawned
void ANetworkPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FString address = TEXT("0.0.0.0");
	int32 port = 65000;

	bool valid = true;

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	//ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr;
	addr->SetIp(*address, valid);
	addr->SetPort(port);
	//addr->GetIp(addresssss);

	FIPv4Endpoint Endpoint(addr);

	Socket = FTcpSocketBuilder(TEXT("default")).AsReusable().BoundToEndpoint(Endpoint).Listening(16);
	//Socket->GetAddress(*adddre);
	int32 new_size;
	Socket->SetReceiveBufferSize(2 << 20, new_size);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection ~> %d"), Socket != nullptr));
	FTimerHandle czaso;
	GetWorldTimerManager().ClearTimer(czaso);
	GetWorldTimerManager().SetTimer(czaso, this, &ANetworkPlayerController::TCPConnectionListener, 0.01f, true);//*/
	
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
}

void ANetworkPlayerController::TCPConnectionListener()
{
	if (!Socket) return;

	//Remote address
	//RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, RemoteAddress->ToString(true));

	// handle incoming connections
	if (Socket->HasPendingConnection(Pending) && Pending)
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//Already have a Connection? destroy previous
		if (Connection)
		{
			Connection->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Connection);
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		//New Connection receive!
		Connection = Socket->Accept(*RemoteAddress, TEXT("connection"));
		

		if (Connection != NULL)
		{
			//can thread this too
			FTimerHandle czaso;
			GetWorldTimerManager().ClearTimer(czaso);
			GetWorldTimerManager().SetTimer(czaso, this, &ANetworkPlayerController::TCPSocketListener, 0.01, true);
		}
	}
}

FString ANetworkPlayerController::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	//Create a string from a byte array!
	const std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());

	//FString can take in the c_str() of a std::string
	return FString(cstr.c_str());
}

void ANetworkPlayerController::TCPSocketListener()
{
	if (!Connection) return;

	TArray<uint8> ReceivedData;
	uint32 Size;

	while (Connection->HasPendingData(Size))
	{
		ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
		//ReceivedData.Init(FMath::Min(Size, 65507u));
		int32 Read = 0;
		Connection->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
		//FInternetAddr OutAddr = new FInternetAddr();
		//->GetPeerAddress(OutAddr);
		//Connection->GetAddress(FInternetAddr::FInternetAddr);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "as");
		
	}

	if (ReceivedData.Num() <= 0)
	{
		//No Data Received
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, StringFromBinaryArray(ReceivedData));
	
	
	int length = ReceivedData.Num() / sizeof(float);

	const float* angles = reinterpret_cast<const float*>(ReceivedData.GetData());

	pitch = -FMath::RadiansToDegrees(angles[length - 3]);
	yaw = FMath::RadiansToDegrees(angles[length - 4]);
	roll = -FMath::RadiansToDegrees(angles[length - 2]);
	shoot = angles[length - 1];

	//FInternetAddr::GetIp;
	//FString a = FInternetAddr::ToString(true);
	/*pitch = -FMath::RadiansToDegrees(angles[length - 2]);
	yaw = FMath::RadiansToDegrees(angles[length - 3]);*/
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f %f %f"), pitch, yaw, shoot));
	//float roll = -FMath::RadiansToDegrees(angles[length - 1]);
	
	//FQuat Pitch = FQuat(FRotator(pitch, 0, 0));
	//FQuat Yaw = FQuat(FRotator(0, yaw, 0));
	//FQuat Roll = FQuat(FRotator(0, 0, roll));
	//FQuat Roll = FQuat(FRotator(0, 0, 0));
	//FQuat Rotation = Yaw * Pitch * Roll;
	//FString a = FString::FromHexBlob(ReceivedData.GetData(), ReceivedData.Num());
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, a);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f %f %f"), pitch, yaw, roll));
	//SetControlRotation(FRotator(Rotation));
	//SetControlRotation(FRotator(pitch,yaw,0));
	//rotations = FRotator(Rotation);
}

