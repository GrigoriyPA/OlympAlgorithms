namespace alg::graph {
    // -----------------------------------
    // vvv -----------Edges----------- vvv


    struct Edge {
        size_t to = 0;
        size_t id = 0;

        Edge(size_t to, size_t id) noexcept {
            this->to = to;
            this->id = id;
        }
    };


    struct EdgeInfo {
        size_t from = 0;
        size_t to = 0;
        size_t id = 0;
        size_t color = 0;

        EdgeInfo(size_t from, size_t to, size_t id, size_t color = 0) noexcept {
            this->from = from;
            this->to = to;
            this->id = id;
            this->color = color;
        }
    };


    // ^^^ -----------Edges----------- ^^^
    // -----------------------------------
    // vvv -------WeightedEdges------- vvv


    struct WeightedEdge {
        size_t to = 0;
        int64_t weight = 0;
        size_t id = 0;

        WeightedEdge(size_t to, int64_t weight, size_t id) noexcept {
            this->to = to;
            this->weight = weight;
            this->id = id;
        }

        explicit operator Edge() const noexcept {
            return Edge(to, id);
        }

        bool operator <(const WeightedEdge& other) const noexcept {
            return weight < other.weight;
        }
    };


    struct WeightedEdgeInfo {
        size_t from = 0;
        size_t to = 0;
        int64_t weight = 0;
        size_t id = 0;

        WeightedEdgeInfo(size_t from, size_t to, int64_t weight, size_t id = 0) noexcept {
            this->from = from;
            this->to = to;
            this->weight = weight;
            this->id = id;
        }

        explicit operator EdgeInfo() const noexcept {
            return EdgeInfo(from, to, id);
        }

        bool operator <(const WeightedEdgeInfo& other) const noexcept {
            return weight < other.weight;
        }
    };


    struct CapacityEdge {
        size_t to = 0;
        int64_t capacity = 0;
        int64_t cost = 0;
        size_t id = 0;

        CapacityEdge(size_t to, int64_t capacity, int64_t cost, size_t id) noexcept {
            this->to = to;
            this->capacity = capacity;
            this->cost = cost;
            this->id = id;
        }

        explicit operator WeightedEdge() const noexcept {
            return WeightedEdge(to, cost, id);
        }

        explicit operator Edge() const noexcept {
            return Edge(to, id);
        }

        bool operator <(const CapacityEdge& other) const noexcept {
            return cost < other.cost;
        }
    };


    // ^^^ -------WeightedEdges------- ^^^
    // -----------------------------------
}   // Edges, WeightedEdges | Version: 0.0

using namespace alg::graph;