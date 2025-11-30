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

    void Dfs(int v, const std::function<bool(int v, int n)>& onNext, const std::function<void(int v)>& onIn = nullptr, const std::function<void(int v)>& onOut = nullptr) const {
        if (onIn) {
            onIn(v);
        }

        for (int next : Adjacent[v]) {
            if (onNext(v, next)) {
                Dfs(next, onNext, onIn, onOut);
            }
        }

        if (onOut) {
            onOut(v);
        }
    }

    bool HasCycles() const {
        enum class EState {
            Undefined,
            InProgress,
            Processed,
        };

        bool hasCycles = false;
        vector<EState> states(Adjacent.size());

        const auto onIn = [&](int v) {
            states[v] = EState::InProgress;
        };
        const auto onNext = [&](int, int n) {
            if (states[n] == EState::InProgress) {
                hasCycles = true;
                return false;
            }
            return !hasCycles && states[n] == EState::Undefined;
        };
        const auto onOut = [&](int v) {
            states[v] = EState::Processed;
        };

        for (int v : Range(Adjacent)) {
            if (states[v] == EState::Undefined) {
                Dfs(v, onNext, onIn, onOut);
            }
            if (hasCycles) {
                return true;
            }
        }

        return false;
    }

protected:
    vector<vector<int>> Adjacent;
};

} using namespace NAlg::NGraphs;
