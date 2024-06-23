// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
 	
#include "TimerManager.h"
// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    

    // Find the distance to the Tank
    if(InFireRange())
    {
        RotateTurret(Tank->GetActorLocation());
    }

    
    //Check to see if the tank is in range

    //if in range, rotate turret toward Tank
} 


void ATower::HandleDestruction()
{   
    Super::HandleDestruction();
    Destroy();

}



void ATower::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this,0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition()
{   
    if(Tank == nullptr)
    {
        return;
    }
    if(InFireRange() && Tank->bAlive)
    {
        Fire();
    }
}

bool ATower::InFireRange()
{
    if(Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if(Distance<=FireRange)
        {
           return true;
        }
    }

    return false;
}