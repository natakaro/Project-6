// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject6.h"
#include "MyGameInstance.h"
#include <string>
#include "SocketSubsystem.h"
#include "IPv4SubnetMask.h"
#include "IPv4Address.h"
#include "IPv4Endpoint.h"
#include "TcpSocketBuilder.h"
#include "UnrealMathUtility.h"
#include "TimerManager.h"



void UMyGameInstance::run()
{
	//Socket = SGI->Socket;
	//Connection = SGI->Connection;


	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "URUCHOMIONO");
	if (!dupy)
	{
		dupy = 1;
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



		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connection ~> %d"), Socket != nullptr));
		FTimerHandle czaso;
		GetWorld()->GetTimerManager().ClearTimer(czaso);
		GetWorld()->GetTimerManager().SetTimer(czaso, this, &UMyGameInstance::TCPConnectionListener, 0.01f, true);//*/

	}
}

UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	

}





UMyGameInstance::~UMyGameInstance()
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

void UMyGameInstance::dd()
{
	x = x + "a";
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, x);
}



void UMyGameInstance::TCPConnectionListener()
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
			//Connection->Close();
			//ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Connection);

		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		//New Connection receive!


			Connection = Socket->Accept(*RemoteAddress, TEXT("connection"));




		if (Connection != NULL)
		{
			//can thread this too
			FTimerHandle czaso;
			GetWorld()->GetTimerManager().ClearTimer(czaso);
			GetWorld()->GetTimerManager().SetTimer(czaso, this, &UMyGameInstance::TCPSocketListener, 0.01, true);
		}
	}
}


void UMyGameInstance::TCPSocketListener()
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

}