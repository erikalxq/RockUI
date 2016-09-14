/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkAtomics_win_DEFINED
#define SkAtomics_win_DEFINED

/** Windows Interlocked atomics. */

// #include <intrin.h>
// #pragma intrinsic(_ReadWriteBarrier)
#include "sk_stdint.h"
#include "WinBase.h"

//MSDN says in order to declare an interlocked function for use as an
//intrinsic, include intrin.h and put the function in a #pragma intrinsic
//directive.
//The pragma appears to be unnecessary, but doesn't hurt.
// #pragma intrinsic(_InterlockedIncrement, _InterlockedExchangeAdd, _InterlockedDecrement)
// #pragma intrinsic(_InterlockedCompareExchange)

static inline int32_t sk_atomic_inc(int32_t* addr) {
    // InterlockedIncrement returns the new value, we want to return the old.
    return InterlockedIncrement(reinterpret_cast<long*>(addr)) - 1;
}

// static inline int64_t sk_atomic_inc(int64_t* addr) {
//     // InterlockedIncrement returns the new value, we want to return the old.
//     return InterlockedIncrement64((ULONGLONG*)addr) - 1;
// }

static inline int32_t sk_atomic_add(int32_t* addr, int32_t inc) {
    return InterlockedExchangeAdd(reinterpret_cast<long*>(addr), static_cast<long>(inc));
}

static inline int32_t sk_atomic_dec(int32_t* addr) {
    // InterlockedDecrement returns the new value, we want to return the old.
    return InterlockedDecrement(reinterpret_cast<long*>(addr)) + 1;
}

static inline void sk_membar_acquire__after_atomic_dec() { }

static inline bool sk_atomic_cas(int32_t* addr, int32_t before, int32_t after) {
    return InterlockedCompareExchange(reinterpret_cast<long*>(addr), after, before) == before;
}

static inline void* sk_atomic_cas(void** addr, void* before, void* after) {
    return InterlockedCompareExchangePointer(addr, after, before);
}

static inline void sk_membar_acquire__after_atomic_conditional_inc() { }

#endif
