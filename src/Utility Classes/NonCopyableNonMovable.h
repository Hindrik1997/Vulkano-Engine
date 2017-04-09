//
// Created by hindrik on 28-3-17.
//

#ifndef VULKANOENGINE_NONCOPYABLENONMOVABLE_H
#define VULKANOENGINE_NONCOPYABLENONMOVABLE_H

class NonCopyableNonMovable {
public:
    NonCopyableNonMovable() = default;
    virtual ~NonCopyableNonMovable() = 0;

    NonCopyableNonMovable(const NonCopyableNonMovable&) = delete;
    NonCopyableNonMovable& operator=(const NonCopyableNonMovable&) = delete;

    NonCopyableNonMovable(NonCopyableNonMovable&&) = delete;
    NonCopyableNonMovable& operator=(NonCopyableNonMovable&&) = delete;

};

inline NonCopyableNonMovable::~NonCopyableNonMovable(){}

#endif //VULKANOENGINE_NONCOPYABLENONMOVABLE_H
