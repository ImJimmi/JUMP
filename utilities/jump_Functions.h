#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    namespace Math
    {
        //==============================================================================================================
        /** Returns a value that, on a logarithmic scale, is [proportion * 100] percent of the way between start and
            end.

            For example, logspace(20.f, 20000.f, 1.f / 3.f) == 200.f.
        */
        template <typename T>
        inline T logspace(T start, T end, float proportion)
        {
            jassert(start > 0);
            jassert(end >= start);
            jassert(proportion >= 0 && proportion <= 1);

            return start * std::pow(end / start, static_cast<T>(proportion));
        }
    }   // namespace Math
}   // namespace jump
