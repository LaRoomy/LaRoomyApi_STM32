#include "BindingController.h"
#include "flashStorageManager.h"

#define DATA_KEY_BINDING_PASSKEY   "bndg_pkey"
#define DATA_KEY_BINDING_REQUIRED   "bndg_reqr"

BindingController::BindingController(){
    this->init();
}

BindingController::~BindingController(){
    bindingControllerInstanceCreated = false;
}

BindingController* BindingController::GetInstance(){
    if(bindingControllerInstanceCreated){
        return bcInstance;
    }
    else {
        bcInstance = new BindingController();
        if(bcInstance != nullptr){
            bindingControllerInstanceCreated = true;
        }
        return bcInstance;
    }
}

bool BindingController::bindingControllerInstanceCreated = false;
BindingController* BindingController::bcInstance = nullptr;

void BindingController::init(){

    this->bKey = "";

    auto strReq = FlashStorageManager.readString(DATA_KEY_BINDING_PASSKEY);
    if(strReq.length() > 0){
        this->bKey = strReq;
    }

    auto bndReq = FlashStorageManager.readUInt(DATA_KEY_BINDING_REQUIRED);
    this->bRequired = (bndReq != 0) ? true : false;
}

BindingResponseType BindingController::handleBindingTransmission(BindingTransmissionTypes bType, const String& key){

    switch (bType)
    {
        case BindingTransmissionTypes::B_AUTH_REQUEST:
            if(this->bKey == key){
                // the key is valid
                return BindingResponseType::BINDING_AUTHENTICATION_SUCCESS;               
            }
            else {
                // key is invalid, auth fail
                return BindingResponseType::BINDING_AUTHENTICATION_FAIL_WRONG_KEY;
            }
        case BindingTransmissionTypes::B_ENABLE:
            // save the key + set required to true
            if(key.length() > 0 && key.length() < 11){
                this->bKey = key;
                this->bRequired = true;

                // write data to flash                
                FlashStorageManager.writeUInt(DATA_KEY_BINDING_REQUIRED, 1);
                FlashStorageManager.writeString(DATA_KEY_BINDING_PASSKEY, key);

                // report success
                return BindingResponseType::BINDING_ENABLE_SUCCESS;
            }
            else {
                return BindingResponseType::BINDING_ERROR_UNKNOWN;
            }
        case BindingTransmissionTypes::B_RELEASE:
            {
                // delete key + set required to false
                this->bKey = "";
                this->bRequired = false;

                // write data                
                FlashStorageManager.writeUInt(DATA_KEY_BINDING_REQUIRED, 0);
                FlashStorageManager.eraseData(DATA_KEY_BINDING_PASSKEY);

                // report release success
            }
            // report release success
            return BindingResponseType::BINDING_RELEASE_SUCCESS;
        default:
            return BindingResponseType::BINDING_ERROR_UNKNOWN_REQUEST;
    }
}