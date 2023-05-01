#ifndef FS_MANAGER_H
#define FS_MANAGER_H

#include <Arduino.h>
#include <FlashIAP.h>
#include <FlashIAPBlockDevice.h>
#include <TDBStore.h>

using namespace mbed;

typedef struct _FlashIAPLimits {
  size_t flash_size;
  uint32_t start_address;
  uint32_t available_size;
} FlashIAPLimits, *PFlashIAPLimits;


class FStorage
{
public:
    ~FStorage();

    static FStorage* GetInstance(){
        if(flash_storage_manager_inst_exist){
            return flashStorageManagerInst;
        }
        else {
            flashStorageManagerInst = new FStorage();
            if(flashStorageManagerInst != nullptr){
                flash_storage_manager_inst_exist = true;
            }
            return flashStorageManagerInst;
        }
    }

    /**
     * @brief Write a string value to storage
     * 
     * @param key The key that identifies the value. Must not include '*' '/' '?' ':' ';'
     * @param value The value to store
     * 
     * @return bool - true if successful or false otherwise
     */
    bool writeString(const String& key, const String& value);

    /**
     * @brief Write an unsigned int value to storage
     * 
     * @param key The key that identifies the value. Must not include '*' '/' '?' ':' ';'
     * @param value The value to store
     * 
     * @return bool - true if successful or false otherwise
     */
    bool writeUInt(const String& key, unsigned int value);

    /**
     * @brief Write an integer value to storage
     * 
     * @param key The key that identifies the value. Must not include '*' '/' '?' ':' ';'
     * 
     * @param value The value to store
     * 
     * @return bool - true if successful or false otherwise 
     */
    bool writeInt(const String& key, int value);

    /**
     * @brief Write a variable data set to storage
     * 
     * @param key The key that identifies the value. Must not include '*' '/' '?' ':' ';'
     * @param data Pointer to the data to store
     * @param dataSize Size of the data to store
     * 
     * @return bool - true if successful or false otherwise 
     */
    bool writeData(const String& key, void* data, size_t dataSize);

    /**
     * @brief Read a string value from storage
     * 
     * @param key The key that identifies the value. Must not include '*' '/' '?' ':' ';'
     * @return String - the value. If the key is not found the value is empty.
     */
    String readString(const String& key);

    /**
     * @brief Read an unsigned int value from storage
     * 
     * @param key The key that identifies the value. Must not include '*' '/' '?' ':' ';'
     * @return unsigned int - The stored value or zero if the value does not exist.
     */
    unsigned int readUInt(const String& key);

    /**
     * @brief Read an integer value from storage
     * 
     * @param key The key that identifies the value. Must not include '*' '/' '?' ':' ';'
     * @return int - The stored value or zero if the value does not exist.
     */
    int readInt(const String& key);

    /**
     * @brief Read a variable data set from storage
     * 
     * @param key The key that identifies the value. Must not include '*' '/' '?' ':' ';'
     * @param data_out Pointer to an object to receive the data
     *  
     * @return bool - true if successful or false otherwise 
     */
    bool readData(const String& key, void* data_out);

    /**
     * @brief Erase the stored value with the given key
     * 
     * @param key The key that identifies the value. Must not include '*' '/' '?' ':' ';'
     * 
     * @return bool - true if successful or false otherwise
     */
    bool eraseData(const String& key);

    // check if there was an initialization error
    bool initSuccess(){
        return this->err;
    }

private:
    FlashIAPLimits limits;

    bool err = false;

    static bool flash_storage_manager_inst_exist;
    static FStorage* flashStorageManagerInst;

    TDBStore* tdbStore = nullptr;
    FlashIAPBlockDevice* iapBlockDevice = nullptr;

    FStorage();

    bool getFlashIAPLimits(PFlashIAPLimits limits_out);
};

/**
 * @brief Access the FlashStorageManager object
 * 
 */
#define     FlashStorageManager     (*FStorage::GetInstance())


#endif // FS_MANAGER_H
