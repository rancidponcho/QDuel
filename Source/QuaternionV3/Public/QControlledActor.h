// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QControlledActor.generated.h"

UCLASS()
class QUATERNIONV3_API AQControlledActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQControlledActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UCameraComponent> Camera = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class USpringArmComponent> SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> SkeletalMesh;
	
	UPROPERTY(EditAnywhere, Category = "PID")
	float Kp;

	UPROPERTY(EditAnywhere, Category = "PID")
	float Ki;

	UPROPERTY(EditAnywhere, Category = "PID")
	float Kd;

public:
	void Look(const FVector2D Direction);
	void PointAt(const bool isPointing);
	void MoveXYZ(const FVector Direction);
	void Roll(const float Direction);

private:
	float PrevAngle = 0.0f; // Previous error for the derivative term
	float IntegralError = 0.0f; // Accumulated error for the integral term
};
