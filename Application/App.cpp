//
// Created by PC on 23.05.2021.
//

#include "App.h"

#include <CanTp.h>
#include <Uds.h>
#include <LpSpi/LpSpi.h>
#include <LpUartDma/LpUartDma.h>
#include <Tle9461/Tle9461.h>
#include "peripherals.h"
#include <FlexCan/FlexCan.h>


using namespace Communication::Can;
using namespace Communication::Spi;
using namespace Communication::Uart;
using namespace DeviceDrivers::TleSbc;

constexpr UBaseType_t app_task_PRIORITY = (configMAX_PRIORITIES - 1);
#define HZ2TICKS(x) (configTICK_RATE_HZ / x)

typedef struct
{
    Tle9461* sbc;
    SyvecsCan* canStream;
    ICan* can;
} TaskParams;

[[noreturn]] static void CommTask(void* pvParameters);
[[noreturn]] static void GpsTask(void* pvParameters);
static void InitTask(void* pvParameters);

static void InitTask(void* pvParameters)
{
    const auto sbcSpi = new LpSpiRtos{&LPSPI0_handle, ISpi::CsPin::Cs0, false};
    constexpr Tle9461::SbcConfig sbcConfig{
        .WakeupEnable = true,
        .WatchdogWindowMode = false,
        .WatchdogPeriod = Tle9461::WdTimerPeriod::WgTimer1000ms,
    };
    const auto sbc = new Tle9461(*sbcSpi, sbcConfig);

    const auto can = new FlexCan{CAN0, 5, 16};

    TaskParams params{
        .sbc = sbc,
        .canStream = canStream,
        .can = can
    };
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (int i = 0; i < 13; i++)
    {
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
        sbc->RefreshWatchdog();
    }
    gps->Config();
    if (xTaskCreate(CommTask, "CommTask", configMINIMAL_STACK_SIZE + 256, &params,
                    (configMAX_PRIORITIES - 1), nullptr)
        != pdPASS)
    {
        while (true);
    }
    if (xTaskCreate(GpsTask, "GpsTask", configMINIMAL_STACK_SIZE+256, &params,
                    (configMAX_PRIORITIES - 3), nullptr)
        != pdPASS)
    {
        while (true);
    }
    vTaskDelete(nullptr);
}

[[noreturn]] static void GpsTask(void* pvParameters)
{
    const auto canStream = static_cast<TaskParams*>(pvParameters)->canStream;
    for (;;)
    {
        canStream->mainFunction();
    }
}

[[noreturn]] static void CommTask(void* pvParameters)
{
    constexpr TickType_t xFrequency = pdMS_TO_TICKS(5);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    const auto sbc = static_cast<TaskParams*>(pvParameters)->sbc;
    const auto can = static_cast<TaskParams*>(pvParameters)->can;
    const auto canTp = new Diag::CanTp(*can);
    const auto Uds = new Diag::Uds(canTp);

    for (;;)
    {
        can->RxTask();
        sbc->RefreshWatchdog();
        Uds->MainFunction();
        canTp->TxMainFunction();
        can->TxTask();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void App()
{
    if (xTaskCreate(InitTask, "InitTask", configMINIMAL_STACK_SIZE + 384, nullptr, configMAX_PRIORITIES - 1, nullptr)
        != pdPASS)
    {
        while (true);
    }
}
