// TODO:
// open storage box motor setup and button interaction
// reset claw placement? for when the claw is moved manually and is thrown out of whack. Maybe the center mode button could trigger it
// tune Arm movement
// tune claw movement
// tune drive and turn speed


#include <Servo.h>
#include "Gizmo.h"

Servo DriveL, DriveR, Arm, Claw; // initialize motor objects
Gizmo gizmo; // initalize gizmo circut board object

void setup() { // initialization (runs once before loop)
    
    gizmo.begin(); // connect gizmo board to initalized gizmo object

    // set mode of motor pins
    pinMode(GIZMO_MOTOR_1, OUTPUT);
    pinMode(GIZMO_MOTOR_2, OUTPUT);
    pinMode(GIZMO_MOTOR_3, OUTPUT);
    pinMode(GIZMO_MOTOR_4, OUTPUT);

    // attach physical motors to initalized motor objects
    Ldrive.attach(GIZMO_MOTOR_1);
    Rdrive.attach(GIZMO_MOTOR_2);
    Arm.attach(GIZMO_MOTOR_3);
    Claw.attach(GIZMO_MOTOR_4);

    // set I/O mode of LED light
    pinMode(LED_BUILTIN, OUTPUT);

    // base variables
    static double driveMult = 1.0;
    static double turnMult = 0.7;
    static double clawTurn = 100; // not tuned
    static double armSpeed = 5; // not tuned
}

void loop() { // runs continuously

    // flash LED to indicate program running
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    
    // get new data from gizmo
    gizmo.refresh();


    // DRIVE
    // get controller (joystick) inputs for movement    Left stick is Drive and Right stick is Turn
    int rawDrive = gizmo.getAxis(GIZMO_AXIS_LY);
    int rawTurn = gizmo.getAxis(GIZMO_AXIS_RX);

    // set drive/turn variables according to joystick inputs and drive/turn constants
    int drive = map(rawDrive, 0, 255, 0, 180)*driveSpeed;
    int turn = map(rawTurn, 0, 255, 0, 180)*turnSpeed*(1 + 0.6*sqrt(std::abs(drive)/127)); // adjust turning based on linear speed (faster movement -> faster turning)

    // turn motors at calculated speed
    Ldrive.write(drive + turn);
    Rdrive.write(drive - turn);


    // ARM
    // get button inputs    RT for arm down - RB for arm up
    int rawArm; // positive is up and negitive is down - when both arm buttons are pressed, each check cancels the other out and removes any potential jittering
    if (gizmo.getButton(GIZMO_BUTTON_RSHOULDER)){
        rawArm+=armSpeed; // since getButton outputs a true/false, this code checks if the button is pressed then adds the speed to rawArm
    }
    if (gizmo.getButton(GIZMO_BUTTON_RT)){
        rawArm-=armSpeed; // same as above except it subtracts rather than adds
    }
    // move arm
    Arm.write(rawArm);


    // CLAW
    // LB is claw
    int clawDir=1;
    if (gizmo.getButton(GIZMO_BUTTON_LSHOULDER)){ // when the button is pressed the claws moving direction is flipped and multiplied by clawTurn to move the claw
        clawDir*=-1; // flip direction
        Claw.write(clawDir * clawTurn);
    }

    //[FROM EXAMPLE CODE]
    // Sleep for 20ms, which means this loop will run at ~50Hz.  This is
    // because in this sample code all we do is get control inputs and
    // map them to motors to drive around, so we don't need to go any
    // faster.  You should probably remove this in your code so that
    // your main loop runs faster when polling sensors or animating
    // lights.
    delay(20);
}