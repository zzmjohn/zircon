// Copyright 2016 The Fuchsia Authors
// Copyright (c) 2014 Travis Geiselbrecht
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#pragma once

#include <fbl/algorithm.h>
#include <ktl/limits.h>

// Is the range [offset, offset + len] fully inside the range [0, max] ?
template <typename O, typename L>
static inline bool InRange(O offset, L len, O max) {
    static_assert(ktl::numeric_limits<O>::is_signed == false, "InRange requires unsigned type O");
    static_assert(ktl::numeric_limits<L>::is_signed == false, "InRange requires unsigned type L");

    // trim offset/len to the range
    if (offset + len < offset) {
        return false; // offset + len wrapped
    }

    // we started off the end of the range
    if (offset > max) {
        return false;
    }

    // does the end exceed the range?
    if (offset + len > max) {
        return false;
    }

    return true;
}

// utility function to trim offset + len to trim_to_len
// returns new length in *len_out
// returns false if out of range
// may return length 0 if it precisely trims
// NOTE: only use unsigned lengths
template <typename O, typename L>
static inline bool TrimRange(O offset, L len, O trim_to_len, L* len_out) {
    static_assert(ktl::numeric_limits<O>::is_signed == false, "TrimRange requires unsigned type O");
    static_assert(ktl::numeric_limits<L>::is_signed == false, "TrimRange requires unsigned type L");

    // start off returning the initial value
    *len_out = len;

    // trim offset/len to the range
    if (offset + len < offset) {
        return false; // offset + len wrapped
    }

    // we started off the end of the range
    if (offset > trim_to_len) {
        return false;
    }

    // trim to the range
    if (offset + len > trim_to_len) {
        *len_out = static_cast<L>(trim_to_len - offset);
    }

    return true;
}

// given two offset/length pairs, determine if they overlap at all
template <typename O, typename L>
static inline bool Intersects(O offset1, L len1, O offset2, L len2) {
    static_assert(ktl::numeric_limits<O>::is_signed == false, "Intersects requires unsigned type O");
    static_assert(ktl::numeric_limits<L>::is_signed == false, "Intersects requires unsigned type L");

    // Can't overlap a zero-length region.
    if (len1 == 0 || len2 == 0) {
        return false;
    }

    if (offset1 <= offset2) {
        // doesn't intersect, 1 is completely below 2
        if (offset1 + len1 <= offset2) {
            return false;
        }
    } else if (offset1 >= offset2 + len2) {
        // 1 is completely above 2
        return false;
    }

    return true;
}

// given two offset/length pairs, determine if they overlap and compute the intersection
// returns results in *offset_out and *len_out
template <typename O, typename L>
static inline bool GetIntersect(O offset1, L len1, O offset2, L len2, O* offset_out, L* len_out) {
    static_assert(ktl::numeric_limits<O>::is_signed == false, "GetIntersect requires unsigned type O");
    static_assert(ktl::numeric_limits<L>::is_signed == false, "GetIntersect requires unsigned type L");

    // see if they intersect at all
    if (!Intersects(offset1, len1, offset2, len2)) {
        return false;
    }

    // they intersect in some way, 2 cases
    if (offset1 < offset2) {
        // range 1 starts lower then range 2, but must extend into it or across it
        *offset_out = offset2;
        *len_out = fbl::min((offset1 + len1) - offset2, len2);
    } else { // (offset2 <= offset1)
        // range 2 starts lower then range 1, but must extend into it or across it
        // also range 1 and two may start at the same address
        *offset_out = offset1;
        *len_out = fbl::min((offset2 + len2) - offset1, len1);
    }

    return true;
}
