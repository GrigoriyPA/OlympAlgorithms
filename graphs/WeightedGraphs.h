namespace alg::graph {
    // -----------------------------------
    // vvv -------WeightedGraph------- vvv


    class WeightedGraph {
        size_t number_of_edges = 0;

        std::vector<std::vector<WeightedEdge>> adj;

    public:
        template <typename T>
        WeightedGraph(const std::vector<std::vector<T>>& adjacency_list) {
            adj.resize(adjacency_list.size());
            for (size_t i = 0; i < adj.size(); ++i) {
                adj[i].reserve(adjacency_list[i].size());
                for (const T& element : adjacency_list[i]) {
                    adj[i].emplace_back(element);
                    number_of_edges = std::max(number_of_edges, adj[i].back().id + 1);
                }
            }
        }

        WeightedGraph(size_t number_of_vertexes) {
            adj.resize(number_of_vertexes);
        }

        const std::vector<std::vector<WeightedEdge>>& get_adjacency_list() const noexcept {
            return adj;
        }

        size_t push_edge(size_t from, size_t to, int64_t weight) {
            if (from >= size() || to >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "push_edge, vertex index out of range.\n\n");
            }

            adj[from].emplace_back(to, weight, number_of_edges);
            return number_of_edges++;
        }

        size_t push_undirected_edge(size_t from, size_t to, int64_t weight) {
            if (from >= size() || to >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "push_undirected_edge, vertex index out of range.\n\n");
            }

            adj[from].emplace_back(to, weight, number_of_edges);
            adj[to].emplace_back(from, weight, number_of_edges);
            return number_of_edges++;
        }

        std::vector<WeightedEdgeInfo> find_mst() const {
            std::vector<int64_t> distance(size(), std::numeric_limits<int64_t>::max());
            distance[0] = 0;

            std::priority_queue<std::pair<int64_t, size_t>, std::vector<std::pair<int64_t, size_t>>, std::greater<std::pair<int64_t, size_t>>> queue;
            queue.push({ 0, 0 });

            std::vector<bool> used(size(), false);
            std::vector<WeightedEdgeInfo> parent(size(), WeightedEdgeInfo(0, 0, 0, 0));
            for (; !queue.empty();) {
                auto [min_distance, vertex] = queue.top();
                queue.pop();
                used[vertex] = true;

                for (auto [to, weight, id] : adj[vertex]) {
                    if (!used[to] && distance[to] > weight) {
                        distance[to] = weight;
                        parent[to] = WeightedEdgeInfo(vertex, to, weight, id);
                        queue.push({ distance[to], to });
                    }
                }
            }

            std::vector<WeightedEdgeInfo> mst;
            mst.reserve(size());
            for (size_t i = 1; i < size(); ++i) {
                if (!used[i]) {
                    throw func::AlgRuntimeError(__FILE__, __LINE__, "find_mst, the graph is not connected.\n\n");
                }

                mst.push_back(parent[i]);
            }

            return mst;
        }

        size_t size() const noexcept {
            return adj.size();
        }

        size_t count_edges() const noexcept {
            return number_of_edges;
        }
    };


    // ^^^ -------WeightedGraph------- ^^^
    // -----------------------------------
    // vvv -------CapacityGraph------- vvv


    class CapacityGraph {
        size_t number_of_edges = 0;
        size_t source = 0;
        size_t terminal = 0;

        std::vector<std::vector<CapacityEdge>> adj;
        std::vector<int64_t> flow;

        bool interal_blocking_flow_finding_dfs(size_t vertex, int64_t minimal_capacity, std::vector<size_t>& current_edge, const std::vector<size_t>& distance) {
            if (vertex == terminal) {
                return true;
            }

            for (; current_edge[vertex] < adj[vertex].size(); ++current_edge[vertex]) {
                auto [to, capacity, cost, id] = adj[vertex][current_edge[vertex]];

                if (distance[to] != distance[vertex] + 1 || capacity - flow[id] < minimal_capacity) {
                    continue;
                }

                if (interal_blocking_flow_finding_dfs(to, minimal_capacity, current_edge, distance)) {
                    flow[id] += minimal_capacity;
                    flow[id ^ 1] -= minimal_capacity;
                    return true;
                }
            }
            return false;
        }

    public:
        template <typename T>
        CapacityGraph(const std::vector<std::vector<T>>& adjacency_list) {
            adj.resize(adjacency_list.size());
            for (size_t i = 0; i < adj.size(); ++i) {
                adj[i].reserve(adjacency_list[i].size());
                for (const T& element : adjacency_list[i]) {
                    adj[i].emplace_back(element);
                    number_of_edges = std::max(number_of_edges, adj[i].back().id + 1);
                }
            }
        }

        CapacityGraph(size_t number_of_vertexes) {
            adj.resize(number_of_vertexes);
        }

        const std::vector<std::vector<CapacityEdge>>& get_adjacency_list() const noexcept {
            return adj;
        }

        int64_t get_flow_on_edge(size_t edge_id) const {
            if (edge_id >= number_of_edges) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_flow_on_edge, edge index out of range.\n\n");
            }

            return flow[2 * edge_id];
        }

        size_t get_source() const noexcept {
            return source;
        }

        size_t get_terminal() const noexcept {
            return terminal;
        }

        int64_t get_flow_size() const noexcept {
            int64_t result = 0;
            for (auto [to, capacity, cost, id] : adj[source]) {
                result += flow[id];
            }
            return result;
        }

        size_t push_edge(size_t from, size_t to, int64_t capacity, int64_t cost = 0) {
            if (from >= size() || to >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "push_edge, vertex index out of range.\n\n");
            }

            adj[from].emplace_back(to, capacity, cost, 2 * number_of_edges);
            adj[to].emplace_back(from, 0, cost, 2 * number_of_edges + 1);
            return number_of_edges++;
        }

        void build_max_flow(size_t source, size_t terminal) {
            if (source >= size() || terminal >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "build_max_flow, vertex index out of range.\n\n");
            }

            this->source = source;
            this->terminal = terminal;
            flow.assign(2 * number_of_edges, 0);
            for (int64_t minimal_capacity = std::numeric_limits<int64_t>::max(); minimal_capacity > 0;) {
                std::vector<size_t> distance(size(), std::numeric_limits<size_t>::max());
                std::queue<size_t> q;

                distance[source] = 0;
                q.push(source);
                for (; !q.empty(); q.pop()) {
                    size_t vertex = q.front();
                    for (auto [to, capacity, cost, id] : adj[vertex]) {
                        if (distance[to] != std::numeric_limits<size_t>::max() || capacity - flow[id] < minimal_capacity) {
                            continue;
                        }

                        distance[to] = distance[vertex] + 1;
                        q.push(to);
                    }
                }

                if (distance[terminal] == std::numeric_limits<size_t>::max()) {
                    minimal_capacity >>= 1;
                    continue;
                }

                std::vector<size_t> current_edge(size(), 0);
                for (; interal_blocking_flow_finding_dfs(source, minimal_capacity, current_edge, distance);) {}
            }
        }

        size_t size() const noexcept {
            return adj.size();
        }

        size_t count_edges() const noexcept {
            return number_of_edges;
        }
    };


    // ^^^ -------CapacityGraph------- ^^^
    // -----------------------------------
}   // WeightedGraph, CapacityGraph | Version: 0.0

using namespace alg::graph;