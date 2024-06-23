// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h" 
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
 {
        if(DeadActor == Tank)
        {   
            UE_LOG(LogTemp, Warning, TEXT("Tank Destruction"));
            Tank->HandleDestruction();
            if(Tank->GetTankPlayerController())
            {
                ToonTanksPlayerController->SetPlayerEnabledState(false);
            }
            GameOver(false);
            
        }
        else if(ATower* DestroyedTower = Cast<ATower>(DeadActor))
        {   
            UE_LOG(LogTemp, Warning, TEXT("Tower Destruction"));
            DestroyedTower->HandleDestruction();
            --TargetTowers;
            if(TargetTowers == 0)
            {
                GameOver(true);
            }
        }
 }
    void AToonTanksGameMode::BeginPlay()
    {
        Super::BeginPlay();

        HandleGameStart();

        Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
        ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this,0));
    }

    void AToonTanksGameMode::HandleGameStart()
    {   
        TargetTowers = GetTargetTowerCount();
        Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
        ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this,0));

        StartGame();

        if(ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);

            FTimerHandle PlayerEnableTimerHandle;

            FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
                ToonTanksPlayerController,
                &AToonTanksPlayerController::SetPlayerEnabledState,
                true
            );
            GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,
                PlayerEnableTimerDelegate,
                StartDelay,
                false)
                ;

        }
    }

int32 AToonTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();

}

