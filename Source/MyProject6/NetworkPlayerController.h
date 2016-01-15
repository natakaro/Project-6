// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Networking/Public/Networking.h"
#include "NetworkPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class MYPROJECT6_API ANetworkPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PitchYawRoll)
		FRotator rotations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PitchYawRoll)
		float shoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PitchYawRoll)
		float pitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PitchYawRoll)
		float yaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PitchYawRoll)
		float roll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Connetttt)
		FString IP;


	
	// Sets default values for this actor's properties
	ANetworkPlayerController(const FObjectInitializer& ObjectInitializer);

	virtual ~ANetworkPlayerController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	void connect();
	FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);

	//FSocket* Socket;
	//FSocket* Connection;
	UMyGameInstance *SGI;
	
	
};
