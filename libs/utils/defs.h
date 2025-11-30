#pragma once
#ifndef __ALG_DISABLE_SYSTEM_INCLUDES
#include <algorithm>
#include <bitset>
#include <iostream>
#include <ranges>
#include <utility>
#include <vector>
#endif
using namespace std;

#define ALL(c) c.begin(), c.end()

namespace NAlg::NUtil {

namespace NDetail {

class TInitializer {
public:
    TInitializer() {
        SetupStdIO();
    }

private:
    static void SetupStdIO() {
        ios_base::sync_with_stdio(false);
        cin.tie(nullptr);
        cout.tie(nullptr);
        cout.precision(32);
    }
};

static const TInitializer init;

} // namespace NDetail

template <typename TLeft, typename TRight>
auto Range(TLeft&& l, TRight&& r) {
    return views::iota(l, max(l, r));
}

template <std::ranges::sized_range TContainer>
auto Range(const TContainer& container) {
    return views::iota(0zu, std::ranges::size(container));
}

template <typename T>
inline void MinEq(T& l, const T& r) {
    l = min(l, r);
}

template <typename T>
inline void MaxEq(T& l, const T& r) {
    l = max(l, r);
}

} using namespace NAlg::NUtil;
