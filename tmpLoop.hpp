#ifndef TEMPLATE_META_PROGRAMMING_LOOP_HPP
#define TEMPLATE_META_PROGRAMMING_LOOP_HPP

#include <stdint.h>

namespace Helpers
{

namespace TMP
{

/*
 * Generic loop functionality for template-meta-programming.
 *
 * The Wrapper-class will have to implement an "impl()" method and have an uint8_t template argument.
 *
 * Please note that this code will be expanded to separate instructions by the compiler and not remain
 * and actual loop during runtime.
 *
 * The Loop-Index-template-parameter shall specify the number of loops N, where the Wrapper-impl() will
 * be called for N-1, N-2, ..., till 0. It shall never be specified as 0.
 *
 * Example usage:
 *
 * template <uint8_t index>
 * class InstanceClasses;
 *
 * template <> class InstanceClasses<0> : public Base<X0, Y0> {};
 * template <> class InstanceClasses<1> : public Base<X1, Y1> {};
 * template <> class InstanceClasses<2> : public Base<X2, Y2> {};
 *
 * template<uint8_t Index>
 * struct WrapperForFunctionality
 * {
 *     static void impl(float const parameter)
 *     {
 *         InstanceClasses<Index>::method(parameter);
 *     }
 * };
 *
 * Loop<3, WrapperForFunctionality, float>::impl(parameter);
 *
 **/


// Template-Meta-Programming loop
template<uint8_t Index, template<uint8_t I> class Wrapper, typename... Args>
struct Loop
{
    static void impl(Args... args)
    {
        //Execute some code
        Wrapper<Index - 1>::impl(args...);

        //Recurse
        Loop<Index - 1, Wrapper, Args...>::impl(args...);
    }
};

// Loop end specialization.
template<template<uint8_t I> class Wrapper, typename... Args>
struct Loop<1, Wrapper, Args...>
{
    static void impl(Args... args)
    {
        //Execute some code
        Wrapper<0>::impl(args...);
    }
};

// Loop past-end safety specialization.
template<template<uint8_t I> class Wrapper, typename... Args>
struct Loop<0, Wrapper, Args...>
{
    static void impl(Args... args) = delete;
};

} // namespace TMP

} // namespace Helpers

#endif // TEMPLATE_META_PROGRAMMING_LOOP_HPP
