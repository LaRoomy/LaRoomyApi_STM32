#include "flashStorageManager.h"

FStorage::FStorage(){
    this->getFlashIAPLimits(&this->limits);

    this-> iapBlockDevice =
        new FlashIAPBlockDevice(this->limits.start_address, this->limits.available_size);

    this->tdbStore =
        new TDBStore(this->iapBlockDevice);

    if(tdbStore != nullptr){
        auto result =
            tdbStore->init();

        if(result == MBED_SUCCESS)
        {
            this->err = true;
        }
    }
}

FStorage::~FStorage()
{
    delete this->tdbStore;
    delete this->iapBlockDevice;
    flashStorageManagerInst = nullptr;
    flash_storage_manager_inst_exist = false;
}

FStorage* FStorage::flashStorageManagerInst = nullptr;
bool FStorage::flash_storage_manager_inst_exist = false;

bool FStorage::writeString(const String& key, const String& value)
{
    auto bufferSize = value.length() + 1;
    auto buffer = new uint8_t[bufferSize];
    
    int result =
        (buffer != nullptr) ? MBED_SUCCESS : MBED_ERROR_ALLOC_FAILED;

    if(result == MBED_SUCCESS)
    {
        for(unsigned int i = 0; i < value.length(); i++)
        {
            buffer[i] = static_cast<uint8_t>(value.c_str()[i]);
        }
        buffer[value.length()] = 0;
        
        result =
            this->tdbStore->set(
                key.c_str(),
                buffer,
                bufferSize,
                0
            );

        delete[] buffer;
    }
    return result == MBED_SUCCESS ? true : false;
}

bool FStorage::writeUInt(const String& key, unsigned int value)
{
    auto result =
        this->tdbStore->set(key.c_str(), &value, sizeof(unsigned int), 0);
    
    return (result == MBED_SUCCESS) ? true : false;
}

bool FStorage::writeInt(const String& key, int value)
{
    auto result =
        this->tdbStore->set(key.c_str(), &value, sizeof(int), 0);
    
    return (result == MBED_SUCCESS) ? true : false;
}

bool FStorage::writeData(const String& key, void* data, size_t dataSize)
{
    auto result =
        this->tdbStore->set(key.c_str(), data, dataSize, 0);
    
    return (result == MBED_SUCCESS) ? true : false;
}

String FStorage::readString(const String& key)
{
    String value;

    TDBStore::info_t info;
    auto result =
        this->tdbStore->get_info(key.c_str(), &info);

    if(result != MBED_ERROR_ITEM_NOT_FOUND)
    {
        auto buffer = new uint8_t[info.size];
        if(buffer != nullptr)
        {
            size_t actual_size;
            result = this->tdbStore->get(key.c_str(), buffer, info.size, &actual_size);

            if(result == MBED_SUCCESS)
            {
                auto target = new char[info.size + 1];
                if(target != nullptr)
                {
                    for(unsigned int i = 0; i < info.size; i++)
                    {
                        target[i] = static_cast<char>(buffer[i]);
                    }
                    target[info.size] = '\0';
                    value = target;
                    delete[] target;
                }
            }
            delete[] buffer;
        }
    }
    return value;
}

unsigned int FStorage::readUInt(const String& key)
{
    unsigned int value = 0;

    TDBStore::info_t info;

    auto result =
        this->tdbStore->get_info(key.c_str(), &info);

    if(result != MBED_ERROR_ITEM_NOT_FOUND)
    {
        size_t actual_size;
        result =
            this->tdbStore->get(key.c_str(), &value, sizeof(unsigned int), &actual_size);

        if(result != MBED_SUCCESS)
        {
            value = 0;
        }
    }
    return value;
}

int FStorage::readInt(const String& key)
{
    int value = 0;

    TDBStore::info_t info;

    auto result =
        this->tdbStore->get_info(key.c_str(), &info);

    if(result != MBED_ERROR_ITEM_NOT_FOUND)
    {
        size_t actual_size;
        result =
            this->tdbStore->get(key.c_str(), &value, sizeof(int), &actual_size);

        if(result != MBED_SUCCESS)
        {
            value = 0;
        }
    }
    return value;
}

bool FStorage::readData(const String& key, void* data_out)
{
    auto result =
        (data_out != nullptr)
        ? MBED_SUCCESS : MBED_ERROR_INVALID_ARGUMENT;

    if(result == MBED_SUCCESS)
    {
        TDBStore::info_t info;

        result =
            this->tdbStore->get_info(key.c_str(), &info);
        
        if(result == MBED_SUCCESS)
        {
            size_t actual_size;

            result =
                this->tdbStore->get(key.c_str(), data_out, info.size, &actual_size);
        }
    }
    return (result == MBED_SUCCESS) ? true : false;
}

bool FStorage::eraseData(const String& key)
{
    auto result =
        this->tdbStore->remove(key.c_str());

    return (result == MBED_SUCCESS) ? true : false;
}

bool FStorage::getFlashIAPLimits(PFlashIAPLimits limits_out)
{
    // Alignment lambdas
    auto align_down = [](uint64_t val, uint64_t size) {
        return (((val) / size)) * size;
    };
    auto align_up = [](uint32_t val, uint32_t size) {
        return (((val - 1) / size) + 1) * size;
    };

    size_t flash_size;
    uint32_t flash_start_address;
    uint32_t start_address;
    FlashIAP flash;

    auto result = flash.init();

    if (result != 0)
        return false;

    // Find the start of first sector after text area
    int sector_size = 
        flash.get_sector_size(FLASHIAP_APP_ROM_END_ADDR);

    start_address =
        align_up(FLASHIAP_APP_ROM_END_ADDR, sector_size);

    flash_start_address =
        flash.get_flash_start();

    flash_size = flash.get_flash_size();

    result = flash.deinit();

    int available_size = flash_start_address + flash_size - start_address;

    if (available_size % (sector_size * 2)) {
        available_size = align_down(available_size, sector_size * 2);
    }

    limits_out->flash_size = flash_size;
    limits_out->start_address = start_address;
    limits_out->available_size = available_size;

    return true;
}