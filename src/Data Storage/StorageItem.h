//
// Created by hindrik on 29-11-16.
//

#ifndef VULKANO_ENGINE_STORAGEITEM_H
#define VULKANO_ENGINE_STORAGEITEM_H

#include "../Utility Classes/NonCopyable.h"

template<typename T>
class StorageItem final
{
public:
    union Data {
        Data(){};
        ~Data(){};

        Data(const Data&)   = delete;
        Data(Data&&)        = delete;

        Data& operator=(const Data&)    = delete;
        Data& operator=(Data&&)         = delete;

        T m_Object;
    } m_Object;
public:
    StorageItem();
    ~StorageItem();

    StorageItem(const StorageItem&)             = delete;
    StorageItem& operator=(const StorageItem&)  = delete;

    StorageItem(StorageItem&&)                  = delete;
    StorageItem& operator=(StorageItem&&)       = delete;
    
    void cleanUp();

    template<typename... Args>
    void reset(Args... arguments);
};

template<typename T>
StorageItem<T>::StorageItem()
{
}

template<typename T>
inline StorageItem<T>::~StorageItem()
{
    cleanUp();
}

template<typename T>
inline void StorageItem<T>::cleanUp()
{
    m_Object.m_Object.~T();
}

template<typename T>
template<typename... Args>
void StorageItem<T>::reset(Args... arguments)
{
    void* tVoid = &m_Object.m_Object;
    new (tVoid) T(arguments...);
}

#endif //VULKANO_ENGINE_STORAGEITEM_H
