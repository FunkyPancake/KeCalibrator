//
// Created by PC on 06.01.2024.
//

#include "Calibrator.h"

#include <fsl_gpio.h>
#include <pin_mux.h>
#include "peripherals.h"

namespace CalApp
{
    void Calibrator::SetMuxOutput(const uint8_t sel)
    {
        //SEL 0
        GPIO_PinWrite(BOARD_INITPINS_MUX_A_GPIO, BOARD_INITPINS_MUX_A_PIN, sel & 0x01);
        GPIO_PinWrite(BOARD_INITPINS_LED1_GPIO, BOARD_INITPINS_LED1_PIN, sel & 0x01);

        //SEL 1
        GPIO_PinWrite(BOARD_INITPINS_MUX_B_GPIO, BOARD_INITPINS_MUX_B_PIN, sel & 0x02);
        GPIO_PinWrite(BOARD_INITPINS_LED2_GPIO, BOARD_INITPINS_LED2_PIN, sel & 0x01);

        //SEL 2
        GPIO_PinWrite(BOARD_INITPINS_MUX_C_GPIO, BOARD_INITPINS_MUX_C_PIN, sel & 0x04);
        GPIO_PinWrite(BOARD_INITPINS_LED3_GPIO, BOARD_INITPINS_LED3_PIN, sel & 0x01);
    }

    void Calibrator::UpdatePcViaCanFrame()
    {
    }

    void Calibrator::OnNewCalMessage(const Communication::Can::CanFrame& canFrame)
    {
    }

    void Calibrator::MainFunction()
    {
        if(pcReady_ == true)
        {
            // if()
        }
    }

    Calibrator::Calibrator(Communication::Can::FlexCan* can) : can_(can)
    {
        can->RegisterRxFrame(canRxId_, [this](const Communication::Can::CanFrame& canFrame)
        {
            this->OnNewCalMessage(canFrame);
        });
    }
} // App
