/**
 * @file SingletonT.hpp
 * @author Edward Palmer
 * @date 2025-04-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

/*
 * Generic Singleton-class wrapper
 * https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 */
template <class Parent>
class SingletonT
{
public:
    /* Singleton */
    static Parent &instance()
    {
        static Parent instance;
        return instance;
    }

protected:
    /* Prevent direct initialization */
    SingletonT() = default;

    /* Delete implicit copy constructors */
    SingletonT &operator=(const SingletonT &) = delete;
    SingletonT(const SingletonT &) = delete;
};
