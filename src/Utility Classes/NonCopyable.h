//
// Created by hindrik on 25-3-17.
//

#ifndef VULKANOENGINE_NONCOPYABLE_H
#define VULKANOENGINE_NONCOPYABLE_H

/*If you want your class to be movable, specify so by implementing those constructors in your derived class! */
class NonCopyable {
public:
    NonCopyable() = default;
    virtual ~NonCopyable() = 0;

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;

    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;

};

inline NonCopyable::~NonCopyable(){}


#endif //VULKANOENGINE_NONCOPYABLE_H
