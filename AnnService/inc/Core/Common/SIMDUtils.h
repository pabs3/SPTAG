// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef _SPTAG_COMMON_SIMDUTILS_H_
#define _SPTAG_COMMON_SIMDUTILS_H_

#include <functional>
#include <iostream>
#include <cstdint>

#include "CommonUtils.h"
#include "InstructionUtils.h"

namespace SPTAG
{
    namespace COMMON
    {
#ifdef _MSC_VER
        template <typename T>
        using SumCalcReturn = void(*)(T*, const T*, DimensionType);
        template<typename T>
        inline SumCalcReturn<T> SumCalcSelector();
#endif // _MSC_VER

        class SIMDUtils
        {
        public:
            SIMDUtils();

            template <typename T>
            static void ComputeSum_Naive(T* pX, const T* pY, DimensionType length)
            {
                const T* pEnd1 = pX + length;
                while (pX < pEnd1) {
                    *pX++ += *pY++;
                }
            }

#ifndef _MSC_VER
            /*
               GCC cannot yet do target-specific template specialisation.
               As a workaround add target-specific non-template functions
               that just call the template functions and hope for inlining.
               https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81276
            */
            f_Naive(static void ComputeSum)(std::int8_t* pX, const std::int8_t* pY, DimensionType length) { ComputeSum_Naive<std::int8_t>(pX, pY, length); }
            f_Naive(static void ComputeSum)(std::uint8_t* pX, const std::uint8_t* pY, DimensionType length) { ComputeSum_Naive<std::uint8_t>(pX, pY, length); }
            f_Naive(static void ComputeSum)(std::int16_t* pX, const std::int16_t* pY, DimensionType length) { ComputeSum_Naive<std::int16_t>(pX, pY, length); }
            f_Naive(static void ComputeSum)(float* pX, const float* pY, DimensionType length) { ComputeSum_Naive<float>(pX, pY, length); }
#endif // !_MSC_VER

            f_SSE(static void ComputeSum)(std::int8_t* pX, const std::int8_t* pY, DimensionType length);
            f_AVX(static void ComputeSum)(std::int8_t* pX, const std::int8_t* pY, DimensionType length);
            f_AVX512(static void ComputeSum)(std::int8_t* pX, const std::int8_t* pY, DimensionType length);

            f_SSE(static void ComputeSum)(std::uint8_t* pX, const std::uint8_t* pY, DimensionType length);
            f_AVX(static void ComputeSum)(std::uint8_t* pX, const std::uint8_t* pY, DimensionType length);
            f_AVX512(static void ComputeSum)(std::uint8_t* pX, const std::uint8_t* pY, DimensionType length);

            f_SSE(static void ComputeSum)(std::int16_t* pX, const std::int16_t* pY, DimensionType length);
            f_AVX(static void ComputeSum)(std::int16_t* pX, const std::int16_t* pY, DimensionType length);
            f_AVX512(static void ComputeSum)(std::int16_t* pX, const std::int16_t* pY, DimensionType length);

            f_SSE(static void ComputeSum)(float* pX, const float* pY, DimensionType length);
            f_AVX(static void ComputeSum)(float* pX, const float* pY, DimensionType length);
            f_AVX512(static void ComputeSum)(float* pX, const float* pY, DimensionType length);

#ifdef _MSC_VER
             template<typename T>
            static inline void ComputeSum(T* p1, const T* p2, DimensionType length)
            {
                auto func = SumCalcSelector<T>();
                return func(p1, p2, length);
            }
#endif // _MSC_VER
        };

#ifdef _MSC_VER
        template<typename T>
        inline SumCalcReturn<T> SumCalcSelector()
        {
            if (InstructionSet::AVX512())
            {
                return &(SIMDUtils::ComputeSum_AVX512);
            }
            bool isSize4 = (sizeof(T) == 4);
            if (InstructionSet::AVX2() || (isSize4 && InstructionSet::AVX()))
            {
                return &(SIMDUtils::ComputeSum_AVX);
            }
            if (InstructionSet::SSE2() || (isSize4 && InstructionSet::SSE()))
            {
                return &(SIMDUtils::ComputeSum_SSE);
            }
            return &(SIMDUtils::ComputeSum_Naive);
        }
#endif // _MSC_VER
    }
}

#endif // _SPTAG_COMMON_SIMDUTILS_H_
