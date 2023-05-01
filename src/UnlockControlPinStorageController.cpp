#include "UnlockControlPinStorageController.h"
#include "flashStorageManager.h"

#define DATA_KEY_UNLOCK_CONTROL_PIN "uc_pin_dkey"
#define DATA_KEY_UNLOCK_PIN_VALID   "vldd_uc_pin_r"

bool UnlockControlPinStorageController::unlockControlPinStorageControllerInstanceCreated = false;
UnlockControlPinStorageController* UnlockControlPinStorageController::ucpInstance = nullptr;

UnlockControlPinStorageController::UnlockControlPinStorageController()
{}

UnlockControlPinStorageController::~UnlockControlPinStorageController()
{
    unlockControlPinStorageControllerInstanceCreated = false;
}

UnlockControlPinStorageController* UnlockControlPinStorageController::GetInstance()
{
    if(unlockControlPinStorageControllerInstanceCreated)
    {
        return ucpInstance;
    }
    else
    {
        ucpInstance = new UnlockControlPinStorageController();
        if(ucpInstance != nullptr)
        {
            unlockControlPinStorageControllerInstanceCreated = true;
        }
        return ucpInstance;
    }
}

String UnlockControlPinStorageController::loadPin(){
    // default value
    String pin("12345");

    // load data
    auto temp_pin = FlashStorageManager.readString(DATA_KEY_UNLOCK_CONTROL_PIN);
    if(temp_pin.length() > 0)
    {
        pin = temp_pin;
    }
    return pin;
}

bool UnlockControlPinStorageController::savePin(const String& pin){

    // validate pin length
    if(pin.length() > 0 && pin.length() < 11){

        // save
        return FlashStorageManager.writeString(DATA_KEY_UNLOCK_CONTROL_PIN, pin);
    }
    return false;
}