// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "Runtime/Networking/Public/Networking.h"
#include "MyGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class MYPROJECT6_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()



	public:
			float shoot;
			float pitch;
			float yaw;
			float roll;

		void TCPConnectionListener();
		void TCPSocketListener();
		UMyGameInstance(const FObjectInitializer& ObjectInitializer);
		~UMyGameInstance();
		void dd();
		void run();
	FSocket* Socket;
	FSocket* Connection;
	TSharedPtr<FInternetAddr> RemoteAddress;
	FString x;
	bool dupy=0;
	
	
	
};
