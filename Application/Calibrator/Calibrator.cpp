//
// Created by PC on 06.01.2024.
//

#include "Calibrator.h"

#include <fsl_gpio.h>
#include <pin_mux.h>
#include "peripherals.h"
#include "fsl_dac32.h"

namespace CalApp
{
    Calibrator::Calibrator(Communication::Can::ICan* can,
                           std::unique_ptr<Brymen869s> meter) : meter_(std::move(meter)), results_(CalSteps.size()),
                                                                can_(can)
    {
        dac32_config_t config;
        DAC32_GetDefaultConfig(&config);
        DAC32_Init(DAC0, &config);
        DAC32_Enable(DAC0, true);
        DAC32_EnableBuffer(DAC0, false);

        can->RegisterRxFrame(canRxId_, [this](const Communication::Can::CanFrame& canFrame)
        {
            this->OnNewCalMessage(canFrame);
        });
        SetMuxOutput(0);
        GPIO_PinWrite(BOARD_INITPINS_LED_IP_GPIO, BOARD_INITPINS_LED_IP_PIN, true);
        GPIO_PinWrite(BOARD_INITPINS_LED_DONE_GPIO, BOARD_INITPINS_LED_DONE_PIN, true);
    }

    void Calibrator::SetMuxOutput(const uint8_t sel)
    {
        const auto bitSel = 1 << sel;
        //SEL 0
        GPIO_PinWrite(BOARD_INITPINS_MUX_A_GPIO, BOARD_INITPINS_MUX_A_PIN, sel & 0x01);
        GPIO_PinWrite(BOARD_INITPINS_LED1_GPIO, BOARD_INITPINS_LED1_PIN, !(bitSel & 0x01));

        //SEL 1
        GPIO_PinWrite(BOARD_INITPINS_MUX_B_GPIO, BOARD_INITPINS_MUX_B_PIN, sel & 0x02);
        GPIO_PinWrite(BOARD_INITPINS_LED2_GPIO, BOARD_INITPINS_LED2_PIN, !(bitSel & 0x02));

        //SEL 2
        GPIO_PinWrite(BOARD_INITPINS_MUX_C_GPIO, BOARD_INITPINS_MUX_C_PIN, sel & 0x04);
        GPIO_PinWrite(BOARD_INITPINS_LED3_GPIO, BOARD_INITPINS_LED3_PIN, !(bitSel & 0x04));
    }

    void Calibrator::UpdatePcViaCanFrame()
    {
    }

    bool Calibrator::CheckBoardPresent()
    {
        const auto boardPresent = GPIO_PinRead(BOARD_INITPINS_BRD_DETEC_GPIO,BOARD_INITPINS_BRD_DETEC_PIN);
        GPIO_PinWrite(BOARD_INITPINS_LED4_GPIO, BOARD_INITPINS_LED4_PIN, boardPresent);
        return !boardPresent;
    }

    void Calibrator::OnNewCalMessage(const Communication::Can::CanFrame& canFrame)
    {
        pcReady_ = canFrame.payload.b[0];
    }

    void Calibrator::NotifyStateChange()
    {
        Communication::Can::Payload payload;
        payload.b[0] = static_cast<uint8_t>(state_);
        can_->Send(canTxStautsId_, payload, 1);
    }

    void Calibrator::MainFunction()
    {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        const auto boardPresent = Calibrator::CheckBoardPresent();
        if (pcReady_)
        {
            Communication::Can::Payload payload;
            switch (state_)
            {
            case CalState::Wait:
                GPIO_PinWrite(BOARD_INITPINS_LED_IP_GPIO, BOARD_INITPINS_LED_IP_PIN, true);
                GPIO_PinWrite(BOARD_INITPINS_LED_DONE_GPIO, BOARD_INITPINS_LED_DONE_PIN, true);
                if (boardPresent)
                {
                    state_ = CalState::InProgress;
                    NotifyStateChange();
                }
                break;
            case CalState::InProgress:

                GPIO_PinWrite(BOARD_INITPINS_LED_IP_GPIO, BOARD_INITPINS_LED_IP_PIN, false);
                GPIO_PinWrite(BOARD_INITPINS_LED_DONE_GPIO, BOARD_INITPINS_LED_DONE_PIN, true);

                xLastWakeTime = xTaskGetTickCount();
                vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(1000));
                SetMuxOutput(0); //measure 5V rail
                xLastWakeTime = xTaskGetTickCount();
                vTaskDelayUntil(&xLastWakeTime, samplingTime_);
                payload.s[0] = meter_->GetMeasurement();
                can_->Send(canTxVsupId, payload, 2);
                results_.clear();


                DAC32_EnableBufferOutput(DAC0, true);
                for (const auto dacStep : CalSteps)
                {
                    DAC32_SetBufferValue(DAC0, 0, dacStep);
                    SetMuxOutput(1); //measure DAC output
                    xLastWakeTime = xTaskGetTickCount();
                    vTaskDelayUntil(&xLastWakeTime, samplingTime_);
                    results_.back() = meter_->GetMeasurement();
                    SetMuxOutput(2); //measure adapter output
                    xLastWakeTime = xTaskGetTickCount();
                    vTaskDelayUntil(&xLastWakeTime, samplingTime_);
                    results_.back() = meter_->GetMeasurement();;
                }
                DAC32_EnableBufferOutput(DAC0, false);


                if (!boardPresent)
                {
                    state_ = CalState::Wait;
                    NotifyStateChange();
                    break;
                }
                memcpy(payload.b, results_.data(), sizeof(payload));
                can_->Send(canTxCalId, payload, 8);

                GPIO_PinWrite(BOARD_INITPINS_LED_IP_GPIO, BOARD_INITPINS_LED_IP_PIN, true);
                GPIO_PinWrite(BOARD_INITPINS_LED_DONE_GPIO, BOARD_INITPINS_LED_DONE_PIN, false);
                state_ = CalState::Done;
                break;
            case CalState::Done:
                if (!boardPresent)
                {
                    state_ = CalState::Wait;
                    NotifyStateChange();
                }
                break;
            }
        }
    }
} // App
