//
// Created by PC on 06.01.2024.
//

#ifndef CALIBRATOR_H
#define CALIBRATOR_H
#include <FlexCan/FlexCan.h>

namespace CalApp {

class Calibrator {
private:
    constexpr static uint32_t canTxId_= 0x501;
    constexpr static uint32_t canRxId_= 0x501;

    uint8_t mux{};
    Communication::Can::FlexCan* can_;
    bool pcReady_{false};
    static void SetMuxOutput(uint8_t sel);
    void UpdatePcViaCanFrame();
    void OnNewCalMessage(const Communication::Can::CanFrame& canFrame);

public:
    void MainFunction();
    explicit Calibrator(Communication::Can::FlexCan* can);
};

} // App

#endif //CALIBRATOR_H
