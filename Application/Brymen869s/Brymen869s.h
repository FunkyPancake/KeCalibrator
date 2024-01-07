//
// Created by PC on 06.01.2024.
//

#ifndef BRYMEN869S_H
#define BRYMEN869S_H
#include <cstdint>
#include <ISpi.h>
#include <memory>

namespace CalApp
{
    /**
 * \brief
 */
    class Brymen869s
    {
        std::unique_ptr<Communication::Spi::ISpi> spi_;
        uint32_t ParseData(const std::vector<uint8_t>& data);

    public:
        explicit Brymen869s(std::unique_ptr<Communication::Spi::ISpi> spi);
        ~Brymen869s() = default;
        uint32_t GetMeasurement();
    };
} // App

#endif //BRYMEN869S_H
