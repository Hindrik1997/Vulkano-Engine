//
// Created by hindrik on 13-4-17.
//

#ifndef VULKANOENGINE_FUNCTIONWRAPPER_H
#define VULKANOENGINE_FUNCTIONWRAPPER_H

#include <functional>

using std::function;

class RenderMode;

class FunctionWrapper
{
private:
    function<void(uint32_t,uint32_t)> m_Func;
    void* m_ComparePointer;

public:
    FunctionWrapper(function<void(uint32_t,uint32_t)> func, void* ptr) : m_Func(func), m_ComparePointer(ptr)
    {
    }

    friend bool operator ==(const FunctionWrapper& rhs, const FunctionWrapper& lhs);
    friend bool operator !=(const FunctionWrapper& rhs, const FunctionWrapper& lhs);

    friend bool operator ==(const FunctionWrapper& rhs, const void* lhs);
    friend bool operator !=(const FunctionWrapper& rhs, const void* lhs);

    void operator()(uint32_t w, uint32_t h)
    {
        return m_Func(w,h);
    }
};

inline bool operator==(const FunctionWrapper &rhs, const FunctionWrapper &lhs)
{
    return rhs.m_ComparePointer == lhs.m_ComparePointer;
}

inline bool operator!=(const FunctionWrapper &rhs, const FunctionWrapper &lhs)
{
    return !(rhs == lhs);
}

inline bool operator==(const FunctionWrapper &rhs, const void *lhs)
{
    return rhs.m_ComparePointer == lhs;
}

inline bool operator!=(const FunctionWrapper &rhs, const void *lhs)
{
    return !(rhs == lhs);
}

#endif //VULKANOENGINE_FUNCTIONWRAPPER_H
