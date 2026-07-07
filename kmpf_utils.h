
#ifndef KAMPF_UTILS_
#define KAMPF_UTILS_

#include <stdint.h>

namespace kmpf {

    enum class JoystickDirection : uint8_t {
        Centered = 0,
        Forward = 1,
        Backward = 2,
        Left = 3,
        Right = 4
    };

    // Instead of the custom packet, use atmt::JoystickState

    // // Role of default values?
    // struct ESPNowPacket {
    //     uint8_t sender_mac_address[6]; // macHandshake
    //     JoystickDirection joystick_direction[2]; // Direction
    //     uint8_t motor_pwm_max[2]; // DcMax
    //     uint8_t motor_pwm_min[2]; // DcMin
    //     uint8_t unused[2]; // Originally named dcMagnitude
    //     bool arm_weapon_motor; // This and next originally named objectArm[6]
    //     bool unused[5];
    //     bool run_weapon_motor; // This and next originally named objectRun[6]
    //     bool unused[5];
    //     float weapon_ramp_speed; // escSpeed
    //     bool unused; // Originally named connectCheck
    // } __attribute__((packed));

    struct Packet_PairingPacket { // 16 bytes
        uint8_t sender_mac_address[6];
        char sender_name[10];
    } __attribute__((packed));

    enum class PacketPrefixes : uint8_t {
        Receiver_InitiatePair = 0,
        Receiver_SendHeartbeat = 1,
        Controller_AcknowledgePair = 2,
        Controller_SendJoystickState = 3
    };

};

#endif