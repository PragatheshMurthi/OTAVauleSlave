/*******************************************************************************
 * @file        lorahal.cpp
 * @brief       LoRa HAL implementation using the Sandeep Mistry LoRa library.
 ******************************************************************************/

#include "lorahal.h"
#include "comm.h"
#include <string.h>
#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>


namespace {
constexpr UINT16 LORA_BUFFER_SIZE = 256U;
UINT8 au8LoRaReceiveBuffer[LORA_BUFFER_SIZE];
}

ERROR_CODE initialize_LoRa(void)
{
    SPI.begin(LORA_SCK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_SS_PIN);
    LoRa.setPins(LORA_SS_PIN, LORA_RESET_PIN, LORA_DIO0_PIN);
    LoRa.setSPIFrequency(LORA_SPI_FREQUENCY_HZ);

    if (!LoRa.begin(LORA_FREQUENCY_HZ)) {
        print_err("initialize_LoRa: LoRa.begin failed");
        return ERR_NET_IF_FAIL;
    }

    LoRa.setTxPower(LORA_TX_POWER_DBM);
    LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
    LoRa.setSignalBandwidth(LORA_SIGNAL_BANDWIDTH_HZ);
    LoRa.setCodingRate4(LORA_CODING_RATE);
    LoRa.setPreambleLength(LORA_PREAMBLE_LENGTH);
    LoRa.setSyncWord(LORA_SYNC_WORD);
    print_info("initialize_LoRa: ready");
    return ERR_OK;
}

ERROR_CODE send_LoRa(PVOID pvData, UINT16 u16Len)
{
    if ((nullptr == pvData) || (0U == u16Len)) {
        print_err("send_LoRa: invalid data or length");
        return ERR_INVALID_PARAM;
    }

    if (u16Len > LORA_BUFFER_SIZE) {
        print_err("send_LoRa: packet too large");
        return ERR_INVALID_PARAM;
    }

    if (!LoRa.beginPacket()) {
        return ERR_NET_IF_FAIL;
    }

    LoRa.write(static_cast<const UINT8 *>(pvData), u16Len);
    if (LoRa.endPacket() != 0) {
        return ERR_OK;
    }
    return ERR_NET_IF_FAIL;

}

ERROR_CODE receive_LoRa(PVOID *ppvData)
{
    if (nullptr == ppvData) {
        print_err("receive_LoRa: invalid output pointer");
        return ERR_INVALID_PARAM;
    }

    *ppvData = nullptr;

    const unsigned long startTime = millis();
    int packetLength = LoRa.parsePacket();
    while ((0 == packetLength) && ((millis() - startTime) < LORA_RX_TIMEOUT_MS)) {
        delay(1U);
        packetLength = LoRa.parsePacket();
    }

    if (packetLength <= 0) {
        return ERR_NET_IF_FAIL;
    }
    if (packetLength > LORA_BUFFER_SIZE) {
        while (LoRa.available()) {
            (void)LoRa.read();
        }
        return ERR_INVALID_PARAM;
    }

    const int bytesRead = LoRa.read(au8LoRaReceiveBuffer, packetLength);
    if (bytesRead != packetLength) {
        return ERR_NET_IF_FAIL;
    }

    *ppvData = au8LoRaReceiveBuffer;
    return ERR_OK;
}