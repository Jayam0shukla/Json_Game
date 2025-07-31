    // Fill out your copyright notice in the Description page of Project Settings.


    #include "BoxDestroyedWidget.h"
    #include "TimerManager.h"
    #include "Engine/World.h"

    void UBoxDestroyedWidget::ShowBoxDestructionMessage()
    {
        SetVisibility(ESlateVisibility::Visible);

        FTimerHandle HideHandle;
        if (GetWorld()) {
            GetWorld()->GetTimerManager().SetTimer(HideHandle, [this]() {
                SetVisibility(ESlateVisibility::Hidden);
            }, 2.0f, false);
        }
    }