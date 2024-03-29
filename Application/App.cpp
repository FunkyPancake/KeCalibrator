//
// Created by PC on 23.05.2021.
//

#include "App.h"

#include <LpSpi/LpSpi.h>
#include <LpUartDma/LpUartDma.h>
#include <Tle9461/Tle9461.h>
#include "peripherals.h"
#include <FlexCan/FlexCan.h>
#include "Calibrator/Calibrator.h"

using namespace Communication::Can;
using namespace Communication::Spi;
using namespace Communication::Uart;
using namespace DeviceDrivers::TleSbc;
using namespace std;

constexpr UBaseType_t app_task_PRIORITY = (configMAX_PRIORITIES - 1);
#define HZ2TICKS(x) (configTICK_RATE_HZ / x)


[[noreturn]] static void InitTask(void* pvParameters);

[[noreturn]] static void InitTask(void* pvParameters)
{
    const auto sbcSpi = new LpSpiRtos{&LPSPI0_handle, ISpi::CsPin::Cs0, false};
    auto spiMeter = static_cast<unique_ptr<ISpi>>(make_unique<LpSpiRtos>(&LPSPI1_handle, ISpi::CsPin::Cs0, false));
    auto meter = std::make_unique<CalApp::Brymen869s>(std::move(spiMeter));
    constexpr Tle9461::SbcConfig sbcConfig{
        .WakeupEnable = true,
        .WatchdogWindowMode = false,
        .WatchdogPeriod = Tle9461::WdTimerPeriod::WgTimer1000ms,
    };
    const auto sbc = new Tle9461(*sbcSpi, sbcConfig);

    const auto can = new FlexCan(CAN0, 5, 16);
    auto calibrator = CalApp::Calibrator(can, std::move(meter));

    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true)
    {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));
        can->RxTask();
        calibrator.MainFunction();
        can->TxTask();

        sbc->RefreshWatchdog();
    }
}

TaskHandle_t* taskHandle;

void App()
{
    if (xTaskCreate(InitTask, "InitTask", configMINIMAL_STACK_SIZE + 512, nullptr, configMAX_PRIORITIES - 1, taskHandle)
        != pdPASS)
    {
        while (true);
    }
}
