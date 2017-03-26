//
// Created by hindrik on 29-11-16.
//

#ifndef VULKANO_ENGINE_STORAGEITEM_H
#define VULKANO_ENGINE_STORAGEITEM_H


template<typename T>
class StorageItem {
public:
    union Data {
        Data(){};
        ~Data(){};
        T m_Object;
    } m_Object;
public:
    StorageItem();
    ~StorageItem();
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
