//
// Created by PC on 06.01.2024.
//

#ifndef CALIBRATOR_H
#define CALIBRATOR_H
#include <Brymen869s.h>
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
        constexpr static uint32_t samplingTime_ =  pdMS_TO_TICKS(2750);
        constexpr static uint32_t canTxId_ = 0x500;
        constexpr static uint32_t canRxId_ = 0x510;
        inline const static std::vector<uint16_t> CalSteps{(500*4096/5000), (4500*4096/5000)};
        CalState state_{CalState::Wait};
        std::unique_ptr<Brymen869s> meter_;
        std::vector<uint16_t> results_;
        Communication::Can::ICan* can_;
        bool pcReady_{false};
        static void SetMuxOutput(uint8_t sel);
        void UpdatePcViaCanFrame();
        static bool CheckBoardPresent();
        void OnNewCalMessage(const Communication::Can::CanFrame& canFrame);
        void NotifyStateChange();
    public:
        explicit Calibrator(Communication::Can::ICan* can,std::unique_ptr<Brymen869s> meter);
        void MainFunction();
        ~Calibrator() = default;
    };
} // App

#endif //CALIBRATOR_H
