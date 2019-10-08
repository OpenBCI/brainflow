#pragma once

#include <math.h>

#include "openbci_wifi_shield_board.h"

#define ADS1299_Vref 4.5
#define ADS1299_gain 24.0

class CytonWifi : public OpenBCIWifiShieldBoard
{
    float eeg_scale = ADS1299_Vref / float((pow (2, 23) - 1)) / ADS1299_gain * 1000000.;
    float accel_scale = 0.002 / (pow (2, 4));

protected:
    void read_thread ();

public:
    // package num, 8 eeg channels, 3 accel channels
    CytonWifi (char *ip_addr) : OpenBCIWifiShieldBoard (12, ip_addr, (int)CYTON_WIFI_BOARD)
    {
    }
};
