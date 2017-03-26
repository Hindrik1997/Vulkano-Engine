//
// Created by hindrik on 25-3-17.
//

#ifndef VULKANOENGINE_NONCOPYABLE_H
#define VULKANOENGINE_NONCOPYABLE_H


class NonCopyable {
public:
    NonCopyable() = default;
    virtual ~NonCopyable() = 0;

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

inline NonCopyable::~NonCopyable(){}


#endif //VULKANOENGINE_NONCOPYABLE_H
