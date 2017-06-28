//
// Created by hindrik on 25-3-17.
//

#ifndef VULKANOENGINE_NONMOVABLE_H
#define VULKANOENGINE_NONMOVABLE_H

class NonMovable
{
public:
    NonMovable() = default;
    virtual ~NonMovable() = 0;

    NonMovable(NonMovable&&) = delete;
    NonMovable& operator=(NonMovable&&) = delete;

    NonMovable(const NonMovable&) = default;
    NonMovable& operator=(const NonMovable&) = default;

};

inline NonMovable::~NonMovable(){}

#endif //VULKANOENGINE_NONMOVABLE_H
