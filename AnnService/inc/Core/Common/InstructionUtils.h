#ifndef _SPTAG_COMMON_INSTRUCTIONUTILS_H_
#define _SPTAG_COMMON_INSTRUCTIONUTILS_H_

#include <string>
#include <vector>
#include <bitset>
#include <array>

#ifndef _MSC_VER
#include <cpuid.h>
#include <xmmintrin.h>
#include <immintrin.h>

void cpuid(int info[4], int InfoType);

#else
#include <intrin.h>
#define cpuid(info, x)    __cpuidex(info, x, 0)
#endif

// MSVC has no attributes, target attribute, ifunc and function multi-versions
#ifdef _MSC_VER
// MSVC SIMD functions just have different names
#define f_Naive(func) func_Naive
#define f_SSE(func) func_SSE
#define f_SSE2(func) func_SSE2
#define f_AVX(func) func_AVX
#define f_AVX2(func) func_AVX2
#define f_AVX512(func) func_AVX512
// Inline functions
#define if_SSE(func) func
#define if_SSE2(func) func
#define if_AVX(func) func
#define if_AVX2(func) func
#define if_AVX512(func) func
#else
// GCC SIMD functions use function multi-versioning
#define f_Naive(func) __attribute__ ((target ("default"))) func
#define f_SSE(func) __attribute__ ((target ("sse"))) func
#define f_SSE2(func) __attribute__ ((target ("sse2"))) func
#define f_AVX(func) __attribute__ ((target ("avx"))) func
#define f_AVX2(func) __attribute__ ((target ("avx2"))) func
#define f_AVX512(func) __attribute__ ((target ("avx512f,avx512bw,avx512dq"))) func
// Inline functions
#define if_SSE(func) f_SSE(func)
#define if_SSE2(func) f_SSE2(func)
#define if_AVX(func) f_AVX(func)
#define if_AVX2(func) f_AVX2(func)
#define if_AVX512(func) f_AVX512(func)
#endif

namespace SPTAG {
    namespace COMMON {

        class InstructionSet
        {
            // forward declarations
            class InstructionSet_Internal;

        public:
            // getters
            static bool AVX(void);
            static bool SSE(void);
            static bool SSE2(void);
            static bool AVX2(void);
            static bool AVX512(void);
            static void PrintInstructionSet(void);

        private:
            static const InstructionSet_Internal CPU_Rep;

            class InstructionSet_Internal
            {
            public:
                InstructionSet_Internal();
                bool HW_SSE;
                bool HW_SSE2;
                bool HW_AVX;
                bool HW_AVX2;
                bool HW_AVX512;
#ifndef _MSC_VER
            private:
                f_Naive(void Initialise)(void);
                f_SSE(void Initialise)(void);
                f_SSE2(void Initialise)(void);
                f_AVX(void Initialise)(void);
                f_AVX2(void Initialise)(void);
                f_AVX512(void Initialise)(void);
#endif // !_MSC_VER
            };
        };
    }
}

#endif
