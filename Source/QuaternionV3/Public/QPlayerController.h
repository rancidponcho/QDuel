#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QPlayerController.generated.h"

struct FInputActionValue;

UCLASS()
class QUATERNIONV3_API AQPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AQPlayerController();

	// Input Action LOOK
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<class UInputAction> ActionLook = nullptr;

	// IA POINT AT
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<class UInputAction> ActionPointAt = nullptr;

	// Input Action MOVE
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<class UInputAction> ActionMove = nullptr;

	// Input Action ROLL
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<class UInputAction> ActionRoll = nullptr;

	// Enhanced Input Context
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input|Character Movement")
	TObjectPtr<class UInputMappingContext> InputMappingContext = nullptr;

protected:
	virtual void BeginPlay() override;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Player Class")
	//TSubclassOf<class AQControlledActor> PlayerClassBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ship Class")
	TSubclassOf<class AQControlledActor> ShipClassBP;

	//void SpawnPlayer(FVector Location, FRotator Rotation);
	void SpawnShip(FVector Location, FRotator Rotation);

	// input handlers
	void BindInputs();
	void HandleLook(const FInputActionValue& InputActionValue);
	void HandlePointAt(const FInputActionValue& InputActionValue);
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleRoll(const FInputActionValue& InputActionValue);

private:
	TObjectPtr<AQControlledActor> Ship = nullptr;
	TObjectPtr<AQControlledActor> Possessed = nullptr;

	// reference to EnhancedInputComponent
	UPROPERTY()
	TObjectPtr<class UEnhancedInputComponent> EnhancedInputComponent = nullptr;
};
