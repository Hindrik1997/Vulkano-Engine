//
// Created by hindrik on 17-6-17.
//

#ifndef VULKANOENGINE_NULLABLE_H
#define VULKANOENGINE_NULLABLE_H


#include <algorithm>

template<typename T>
class Nullable final
{
private:
    union Data
    {
        Data(){};
        ~Data(){};

        Data(const Data&)   = delete;
        Data(Data&&)        = delete;

        Data& operator=(const Data&) = delete;
        Data& operator=(Data&&)      = delete;

        T m_Data;
    } m_Data;

    bool m_IsUsed = false;
public:
    Nullable() = default;
    ~Nullable();

    Nullable(T& object);
    Nullable(T&& object);

    Nullable(const Nullable& object);
    Nullable(Nullable&& object);

    Nullable& operator=(const Nullable& object);
    Nullable& operator=(Nullable&& object);

    Nullable& operator=(const T& object);
    Nullable& operator=(T&& object);

    operator bool() const;

    template<typename ...Args>
    void set(Args&&... args);

    T& get();
    const T& get() const;

    bool isNull() const;
    bool isSet() const;

    void set(T &&object);

    void set(const T &object);

    void clear();

    void reset(const T& object);

    void reset(T&& object);

};

template<typename T>
void Nullable<T>::set(T &&object)
{
    m_IsUsed = true;
    m_Data.m_Data = std::move(object);
}

template<typename T>
void Nullable<T>::set(const T &object)
{
    m_IsUsed = true;
    m_Data.m_Data = object;
}

template<typename T>
Nullable<T>::~Nullable()
{
    if(m_IsUsed)
        m_Data.m_Data.~T();
}

template<typename T>
Nullable<T>& Nullable<T>::operator=(const Nullable<T>& rhs)
{
    if(&rhs == this)
        return *this;

    if(isNull())
    {
        m_Data.m_Data.~T();
    }

    m_Data.m_Data = rhs.m_Data.m_Data;
    m_IsUsed = true;
    return *this;
}

template<typename T>
Nullable<T>& Nullable<T>::operator=(Nullable<T> && rhs)
{
    if(&rhs == this)
        return *this;

    if(isNull())
    {
        m_Data.m_Data.~T();
    }

    m_Data.m_Data = std::move(rhs.m_Data.m_Data);
    rhs.m_IsUsed = false;
    m_IsUsed = true;
    return *this;
}

template<typename T>
Nullable<T>::Nullable(const Nullable<T> & rhs)
{
    if(isNull())
    {
        m_Data.m_Data.~T();
    }
    m_Data.m_Data = rhs.m_Data.m_Data;
    m_IsUsed = true;
}

template<typename T>
Nullable<T>::Nullable(Nullable<T> && rhs)
{
    if(isNull())
    {
        m_Data.m_Data.~T();
    }
    m_Data.m_Data = std::move(rhs.m_Data.m_Data);
    rhs.m_IsUsed = false;
    m_IsUsed = true;
}

template<typename T>
bool Nullable<T>::isNull() const
{
    return !m_IsUsed;
}

template<typename T>
void Nullable<T>::clear()
{
    m_Data.m_Data.~T();
}

template<typename T>
void Nullable<T>::reset(const T& object)
{
    if(&object == this)
        return;

    if(isNull())
    {
        m_Data.m_Data.~T();
    }
    m_Data.m_Data = object;
    m_IsUsed = true;
}

template<typename T>
void Nullable<T>::reset(T&& object)
{
    if(&object == this)
        return;

    if(isNull())
    {
        m_Data.m_Data.~T();
    }
    m_Data.m_Data = std::move(object);
    m_IsUsed = true;
}

template<typename T>
Nullable<T>& Nullable<T>::operator=(const T& object)
{
    if(&object == &this->m_Data.m_Data)
        return *this;

    if(isNull())
    {
        m_Data.m_Data.~T();
    }

    m_Data.m_Data = object;
    m_IsUsed = true;
    return *this;
}

template<typename T>
Nullable<T>& Nullable<T>::operator=(T&& object)
{
    if(&object == &this->m_Data.m_Data)
        return *this;

    if(isNull())
    {
        m_Data.m_Data.~T();
    }

    m_Data.m_Data = std::move(object);
    m_IsUsed = true;
    return *this;
}

template<typename T>
Nullable<T>::Nullable(T& object)
{
    m_Data.m_Data = object;
    m_IsUsed = true;
}

template<typename T>
Nullable<T>::Nullable(T&& object)
{
    m_Data.m_Data = std::move(object);
    m_IsUsed = true;
}

template<typename T>
T& Nullable<T>::get()
{
    return m_Data.m_Data;
}

template<typename T>
const T& Nullable<T>::get() const
{
    return m_Data.m_Data;
}

template<typename T>
Nullable<T>::operator bool() const
{
    return m_IsUsed;
}

template<typename T>
bool Nullable<T>::isSet() const
{
    return !isNull();
}

template<typename T>
template<typename... Args>
void Nullable<T>::set(Args&& ...args)
{
    if(m_IsUsed)
        m_Data.m_Data.~T();
    void* tVoid = &m_Data.m_Data;
    new (tVoid) T(std::forward<Args>(args)...);
    m_IsUsed = true;
}


#endif //VULKANOENGINE_NULLABLE_H
