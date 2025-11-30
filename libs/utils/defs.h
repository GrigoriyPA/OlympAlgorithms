#pragma once
#ifndef __ALG_DISABLE_SYSTEM_INCLUDES
#include <algorithm>
#include <bitset>
#include <iostream>
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

} using namespace NAlg::NUtil;
