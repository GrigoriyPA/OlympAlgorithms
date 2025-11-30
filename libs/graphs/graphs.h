#pragma once
#include "libs/utils/defs.h"

namespace NAlg::NGraphs {

class TGraph {
public:
    explicit TGraph(int size)
        : Adjacent(size)
    {}

    TGraph& PushOneSide(int u, int v) {
        Adjacent[u].push_back(v);
        return *this;
    }

protected:
    vector<vector<int>> Adjacent;
};

} using namespace NAlg::NGraphs;
