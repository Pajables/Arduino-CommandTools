#include "CommandAccelStepper.h"

void* globalCommandLinearAccelStepperActuatorPt2Object;

CommandAccelStepper::CommandAccelStepper(AccelStepper &myStepper, int myEnablePin)
{
    LinearAccelStepperActuator my_linearactuator = LinearAccelStepperActuator(myStepper, myEnablePin);
    linearactuator = my_linearactuator;
}

/**
* Register to Command Manager
*/
void CommandAccelStepper::registerToCommandManager(CommandManager &cmdMgr, const char *command)
{
    cmdMgr.addDevice(command, this, wrapper_init, wrapper_handleCommand, wrapper_setHeader, wrapper_update);
}

/**
* Init function to be run once in setup.
* We initialise all the callback here
* Wrapper is used by CommandManager
*/
void CommandAccelStepper::wrapper_init(void* pt2Object)
{
    CommandAccelStepper* self = (CommandAccelStepper*) pt2Object;
    self->init();
}

void CommandAccelStepper::init()
{
    #ifndef CommandAccelStepper_DEBUG
        Serial.println("Init CommandAccelStepper");
    #endif

    //do device init first
    linearactuator.init();

    //Register all commands.
    //ALL OF THEM
    cmdHdl.addCommand(BONJOUR_CMD, wrapper_bonjour);

    cmdHdl.addCommand(COMMANDACCELSTEPPER_SET_POSITION, wrapper_setCurrentPosition);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_SET_SPEED, wrapper_setSpeed);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_SET_MAX_SPEED, wrapper_setMaxSpeed);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_SET_ACC, wrapper_setAcceleration);

    cmdHdl.addCommand(COMMANDACCELSTEPPER_ENABLE_ACC, wrapper_enableAcceleration);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_DISABLE_ACC, wrapper_disableAcceleration);

    cmdHdl.addCommand(COMMANDACCELSTEPPER_MOVE_TO, wrapper_moveTo);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_MOVE, wrapper_move);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_STOP, wrapper_stop);

    cmdHdl.addCommand(COMMANDACCELSTEPPER_MOVING, wrapper_isMoving);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_DIST, wrapper_distanceToGo);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_TARGET, wrapper_targetPosition);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_POSITION, wrapper_currentPosition);

    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_SPEED, wrapper_speed);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_MAXSPEED, wrapper_maxSpeed);
    cmdHdl.addCommand(COMMANDACCELSTEPPER_REQUEST_ACCELERATION, wrapper_acceleration);

    //Default one
    cmdHdl.setDefaultHandler(wrapper_unrecognized);
}

/**
* Handling messages
* wrapper is used by CommandManager
*/
void CommandAccelStepper::wrapper_handleCommand(const char *command, void* pt2Object)
{
    //Each time the handleCommand is called, it is given the command and the pointer to the instance that should handle it
    globalCommandLinearAccelStepperActuatorPt2Object = pt2Object;
    //Cast it to class name
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;

    self->handleCommand(command);
}

void CommandAccelStepper::handleCommand(const char *command)
{
    #ifndef CommandAccelStepper_DEBUG
        Serial.print("Device received: ");
        Serial.println(command);
    #endif

    cmdHdl.processString(command);
}

/**
* Set Header for the message that comes out of the Device
* The wrapper is used by the CommandManager
*/
void CommandAccelStepper::wrapper_setHeader(const char *cmdHeader, void* pt2Object)
{
    //Explicitly cast to a pointer to classname
    CommandAccelStepper* self = (CommandAccelStepper*) pt2Object;
    self->setHeader(cmdHeader);
}

void CommandAccelStepper::setHeader(const char *cmdHeader)
{
    #ifndef CommandAccelStepper_DEBUG
        Serial.print("Set Header CommandAccelStepper to ");
        Serial.println(cmdHeader);
    #endif

    cmdHdl.setCmdHeader(cmdHeader);
}

/**
* Update function to be run each loop
* The wrapper is used by the CommandManager
*/
void CommandAccelStepper::wrapper_update(void* pt2Object)
{
    //Cast to pointer of classname
    CommandAccelStepper* self = (CommandAccelStepper*) pt2Object;
    self->update();
}

void CommandAccelStepper::update()
{
    //Do what needs to be done. Non-blocking things
    //Should be fast!!
    linearactuator.update();
}

/**
* A Bonjour behaviour eenable to know who we are talking to
* change the CommandAccelStepper_BONJOUR_ID  and ensure it is unique to your new device
* keep CommandAccelStepper_BONJOUR_ID short
* DONT FORGET TO CHANGE THE NAME OF THIS VARIABLE APPROPRIATELY!!
*/
void CommandAccelStepper::wrapper_bonjour()
{
    //Cast once more...
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->bonjour();
}

void CommandAccelStepper::bonjour()
{
    #ifndef CommandAccelStepper_DEBUG
        Serial.println("Device received bonjour command");
    #endif

    cmdHdl.initCmd();
    cmdHdl.addCmdString(BONJOUR_CMD);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_BONJOUR_ID);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

/**
* Default unrecognized command and wrapper
* Respond a message with header "?" and the command received as the argument
*/
void CommandAccelStepper::wrapper_unrecognized(const char *command)
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->unrecognized(command);
}

void CommandAccelStepper::unrecognized(const char *command)
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(UNRECOGNIZED_CMD);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdString(command);
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Sets the current position
void CommandAccelStepper::wrapper_setCurrentPosition()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->setCurrentPosition();
}

void CommandAccelStepper::setCurrentPosition()
{
    long steps = cmdHdl.readLongArg();
    if(cmdHdl.argOk)
    {
        linearactuator.setCurrentPosition(steps);
    }
}

//Sets the speed of the stepper
void CommandAccelStepper::wrapper_setSpeed()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->setSpeed();
}

void CommandAccelStepper::setSpeed()
{
    float stepsPerSec = cmdHdl.readFloatArg();
    if(cmdHdl.argOk)
    {
        linearactuator.setSpeed(stepsPerSec);
    }
}

//Gets the speed
void CommandAccelStepper::wrapper_speed()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->speed();
}

float CommandAccelStepper::speed()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_SPEED);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdFloat(linearactuator.speed());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Sets the max speed of the stepper
void CommandAccelStepper::wrapper_setMaxSpeed()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->setMaxSpeed();
}

void CommandAccelStepper::setMaxSpeed()
{
    float stepsPerSec = cmdHdl.readFloatArg();
    if(cmdHdl.argOk)
    {
        linearactuator.setMaxSpeed(stepsPerSec);
    }
}

//Gets the max speed of the stepper
void CommandAccelStepper::wrapper_maxSpeed()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->maxSpeed();
}

float CommandAccelStepper::maxSpeed()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_MAXSPEED);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdFloat(linearactuator.maxSpeed());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Sets the acceleration of the stepper
void CommandAccelStepper::wrapper_setAcceleration()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->setAcceleration();
}

void CommandAccelStepper::setAcceleration()
{
    float stepsPerSecPerSec = cmdHdl.readFloatArg();
    if(cmdHdl.argOk)
    {
        linearactuator.setAcceleration(stepsPerSecPerSec);
    }
}

//Gets the acceleration
void CommandAccelStepper::wrapper_acceleration()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->acceleration();
}

float CommandAccelStepper::acceleration()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_ACCELERATION);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdFloat(linearactuator.acceleration());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Enables the stepper acceleration
void CommandAccelStepper::wrapper_enableAcceleration()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->enableAcceleration();
}

void CommandAccelStepper::enableAcceleration()
{
    linearactuator.enableAcceleration();
}

//Disables the stepper acceleration
void CommandAccelStepper::wrapper_disableAcceleration()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->disableAcceleration();
}

void CommandAccelStepper::disableAcceleration()
{
    linearactuator.disableAcceleration();
}

//Moves the stepper to a location.
void CommandAccelStepper::wrapper_moveTo()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->moveTo();
}

void CommandAccelStepper::moveTo()
{
    long steps = cmdHdl.readLongArg();
    if(cmdHdl.argOk)
    {
        linearactuator.moveTo(steps);
    }
}

//Moves the stepper
void CommandAccelStepper::wrapper_move()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->move();
}

void CommandAccelStepper::move()
{
    long steps = cmdHdl.readLongArg();
    if(cmdHdl.argOk)
    {
        linearactuator.move(steps);
    }
}

//Stops the movement of the stepper
void CommandAccelStepper::wrapper_stop()
{
    CommandAccelStepper* self = (CommandAccelStepper*)globalCommandLinearAccelStepperActuatorPt2Object;
    self->stop();
}

void CommandAccelStepper::stop()
{
    linearactuator.stop();
}

//Determines if the stepper is moving
void CommandAccelStepper::wrapper_isMoving()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->isMoving();
}

void CommandAccelStepper::isMoving()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_MOVING);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdBool(linearactuator.isMoving());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Determines the distance left to travel
void CommandAccelStepper::wrapper_distanceToGo()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->distanceToGo();
}

void CommandAccelStepper::distanceToGo()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_DIST);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdLong(linearactuator.distanceToGo());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Gets the target position
void CommandAccelStepper::wrapper_targetPosition()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->targetPosition();
}

void CommandAccelStepper::targetPosition()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_TARGET);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdLong(linearactuator.targetPosition());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}

//Gets the current position
void CommandAccelStepper::wrapper_currentPosition()
{
    CommandAccelStepper* self = (CommandAccelStepper*) globalCommandLinearAccelStepperActuatorPt2Object;
    self->currentPosition();
}

void CommandAccelStepper::currentPosition()
{
    cmdHdl.initCmd();
    cmdHdl.addCmdString(COMMANDACCELSTEPPER_POSITION);
    cmdHdl.addCmdDelim();
    cmdHdl.addCmdLong(linearactuator.currentPosition());
    cmdHdl.addCmdTerm();
    cmdHdl.sendCmdSerial();
}
