//
// Created by PC on 06.01.2024.
//

#ifndef CALIBRATOR_H
#define CALIBRATOR_H
#include <FlexCan/FlexCan.h>

namespace CalApp
{
    class Calibrator
    {

    private:
        enum class CalState
        {
            Wait,
            InProgress,
            Done
        };
        constexpr static uint32_t samplingTime_ =  pdMS_TO_TICKS(3000);
        constexpr static uint32_t canTxId_ = 0x500;
        constexpr static uint32_t canRxId_ = 0x510;
        inline const static std::vector<uint16_t> CalSteps{(500*4096/5000), (4500*4096/5000)};
        CalState state_{CalState::Wait};
        std::vector<uint16_t> results_{};
        Communication::Can::FlexCan* can_;
        bool pcReady_{false};
        static void SetMuxOutput(uint8_t sel);
        void UpdatePcViaCanFrame();
        static bool CheckBoardPresent();
        void OnNewCalMessage(const Communication::Can::CanFrame& canFrame);

    public:
        void MainFunction();
        explicit Calibrator(Communication::Can::FlexCan* can);
    };
} // App

#endif //CALIBRATOR_H
