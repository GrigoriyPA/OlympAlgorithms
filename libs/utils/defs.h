#pragma once
#ifndef __ALG_DISABLE_SYSTEM_INCLUDES
#include <iostream>
#endif
using namespace std;

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
