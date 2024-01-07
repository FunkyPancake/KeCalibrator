//
// Created by PC on 06.01.2024.
//

#include "Brymen869s.h"

#include <fsl_gpio.h>

#include "pin_mux.h"
#include <fsl_port.h>
#include <FreeRTOS.h>
#include <task.h>

namespace CalApp
{
    uint32_t Brymen869s::ParseData(const std::vector<uint8_t>& data)
    {
        return 0;
    }

    Brymen869s::Brymen869s(std::unique_ptr<Communication::Spi::ISpi> spi): spi_(std::move(spi))
    {
    }


    uint32_t Brymen869s::GetMeasurement()
    {
        TickType_t xLastWakeTime = xTaskGetTickCount();
        //generate sync pulse

        //set as gpio
        PORT_SetPinMux(BOARD_INITPINS_BRYMEN_TX_PORT,BOARD_INITPINS_BRYMEN_TX_PIN, kPORT_MuxAsGpio);
        GPIO_PinWrite(GPIOD,BOARD_INITPINS_BRYMEN_TX_PIN, true);
        vTaskDelayUntil(&xLastWakeTime,pdMS_TO_TICKS(10));
        GPIO_PinWrite(GPIOD,BOARD_INITPINS_BRYMEN_TX_PIN, false);
        //set again as spi clk
        PORT_SetPinMux(BOARD_INITPINS_BRYMEN_TX_PORT,BOARD_INITPINS_BRYMEN_TX_PIN, kPORT_MuxAlt3);

        //wait untill meter starts to transmit data
        while (GPIO_PinRead(GPIOD,BOARD_INITPINS_BRYMEN_RX_PIN) == true)
        {
        }
        std::vector<uint8_t> data(160);
        spi_->readBytes(160, data);
        return ParseData(data);
    }
} // App
