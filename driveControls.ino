#include <Servo.h>
#include "Gizmo.h"

Servo DriveL, DriveR;
Gizmo gizmo;

void setup() { // initialization (runs once before loop)
    gizmo.begin();

    pinMode(GIZMO_MOTOR_1, OUTPUT);
    pinMode(GIZMO_MOTOR_2, OUTPUT);

    Ldrive.attach(GIZMO_MOTOR_1);
    Rdrive.attach(GIZMO_MOTOR_2);

    pinMode(LED_BUILTIN, OUTPUT);

    static double driveMult = 1.0;
    static double turnMult = 0.7;
}

void loop() { // runs continuously
    // flash LED to indicate program running
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    
    // get new data from gizmo
    gizmo.refresh();

    // get controller (joystick) inputs for movement
    int rawDrive = gizmo.getAxis(GIZMO_AXIS_LY);
    int rawTurn = gizmo.getAxis(GIZMO_AXIS_RX);
    // set drive/turn variables according to joystick inputs and drive/turn constants
    int drive = map(rawDrive, 0, 255, 0, 180)*driveSpeed;
    int turn = map(rawTurn, 0, 255, 0, 180)*turnSpeed*(1 + 0.6*sqrt(std::abs(drive)/127)); // adjust turning based on linear speed (faster movement -> faster turning)

    // turn motors at calculated speed
    Ldrive.write(drive + turn);
    Rdrive.write(drive - turn);

    delay(20);
}