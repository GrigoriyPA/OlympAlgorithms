namespace alg::graph {
    // -----------------------------------
    // vvv -----------Graph----------- vvv


    class Graph {
        size_t number_of_edges = 0;

        std::vector<std::vector<Edge>> adj;

        void calculation_tin_up_dfs(size_t vertex, size_t parent, size_t& timer, std::vector<size_t>& tin, std::vector<size_t>& up) const {
            tin[vertex] = ++timer;
            up[vertex] = tin[vertex];
            for (auto [to, id] : adj[vertex]) {
                if (tin[to] != 0) {
                    if (to != parent) {
                        up[vertex] = std::min(up[vertex], tin[to]);
                    }
                    continue;
                }

                calculation_tin_up_dfs(to, vertex, timer, tin, up);

                up[vertex] = std::min(up[vertex], up[to]);
            }
        }

        void interal_bridge_finding_dfs(size_t vertex, size_t& current_color, std::vector<EdgeInfo>& bridges, std::vector<size_t>& markup, const std::vector<size_t>& tin, const std::vector<size_t>& up) const {
            for (auto [to, id] : adj[vertex]) {
                if (markup[to] != std::numeric_limits<size_t>::max()) {
                    continue;
                }

                if (up[to] == tin[to]) {
                    bridges.emplace_back(vertex, to, id);
                    markup[to] = ++current_color;
                    interal_bridge_finding_dfs(to, current_color, bridges, markup, tin, up);
                }
                else {
                    markup[to] = markup[vertex];
                    interal_bridge_finding_dfs(to, markup[vertex], bridges, markup, tin, up);
                }
            }
        }

        void interal_points_of_articulation_finding_dfs(size_t vertex, size_t parent_id, size_t parent_color, size_t& current_color, std::vector<bool>& used, std::vector<size_t>& points, std::vector<EdgeInfo>& markup, const std::vector<size_t>& tin, const std::vector<size_t>& up) const {
            used[vertex] = true;
            bool is_articulation = parent_id == std::numeric_limits<size_t>::max() && adj[vertex].size() > 1;
            for (auto [to, id] : adj[vertex]) {
                if (used[to]) {
                    if (id != parent_id && tin[to] < tin[vertex]) {
                        markup.emplace_back(vertex, to, id, parent_color);
                    }
                    continue;
                }

                if (up[to] >= tin[vertex]) {
                    is_articulation = is_articulation || parent_id != std::numeric_limits<size_t>::max();
                    ++current_color;
                    markup.emplace_back(vertex, to, id, current_color);
                    interal_points_of_articulation_finding_dfs(to, id, current_color, current_color, used, points, markup, tin, up);
                }
                else {
                    markup.emplace_back(vertex, to, id, parent_color);
                    interal_points_of_articulation_finding_dfs(to, id, parent_color, current_color, used, points, markup, tin, up);
                }
            }
            if (is_articulation) {
                points.push_back(vertex);
            }
        }

    public:
        template <typename T>
        Graph(const std::vector<std::vector<T>>& adjacency_list) {
            adj.resize(adjacency_list.size());
            for (size_t i = 0; i < adj.size(); ++i) {
                adj[i].reserve(adjacency_list[i].size());
                for (const T& element : adjacency_list[i]) {
                    adj[i].emplace_back(element);
                    number_of_edges = std::max(number_of_edges, adj[i].back().id + 1);
                }
            }
        }

        Graph(size_t number_of_vertexes) {
            adj.resize(number_of_vertexes);
        }

        const std::vector<std::vector<Edge>>& get_adjacency_list() const noexcept {
            return adj;
        }

        size_t push_edge(size_t from, size_t to) {
            if (from >= size() || to >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "push_edge, vertex index out of range.\n\n");
            }

            adj[from].emplace_back(to, number_of_edges);
            return number_of_edges++;
        }

        size_t push_undirected_edge(size_t from, size_t to) {
            if (from >= size() || to >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "push_undirected_edge, vertex index out of range.\n\n");
            }

            adj[from].emplace_back(to, number_of_edges);
            adj[to].emplace_back(from, number_of_edges);
            return number_of_edges++;
        }

        std::pair<std::vector<EdgeInfo>, std::vector<size_t>> find_bridges() const {
            size_t timer = 0;
            std::vector<size_t> tin(size(), 0);
            std::vector<size_t> up(size(), 0);
            for (size_t vertex = 0; vertex < size(); ++vertex) {
                if (tin[vertex] > 0) {
                    continue;
                }

                calculation_tin_up_dfs(vertex, vertex, timer, tin, up);
            }

            size_t current_color = 1;
            std::vector<size_t> markup(size(), std::numeric_limits<size_t>::max());
            std::vector<EdgeInfo> bridges;
            for (size_t vertex = 0; vertex < size(); ++vertex) {
                if (markup[vertex] != std::numeric_limits<size_t>::max()) {
                    continue;
                }

                markup[vertex] = current_color;
                interal_bridge_finding_dfs(vertex, current_color, bridges, markup, tin, up);
                ++current_color;
            }

            return { bridges, markup };
        }

        std::pair<std::vector<size_t>, std::vector<EdgeInfo>> find_points_of_articulation() const {
            size_t timer = 0;
            std::vector<size_t> tin(size(), 0);
            std::vector<size_t> up(size(), 0);
            for (size_t vertex = 0; vertex < size(); ++vertex) {
                if (tin[vertex] > 0) {
                    continue;
                }

                calculation_tin_up_dfs(vertex, vertex, timer, tin, up);
            }

            size_t current_color = 0;
            std::vector<bool> used(size(), false);
            std::vector<EdgeInfo> markup;
            std::vector<size_t> points;
            for (size_t vertex = 0; vertex < size(); ++vertex) {
                if (used[vertex]) {
                    continue;
                }

                interal_points_of_articulation_finding_dfs(vertex, std::numeric_limits<size_t>::max(), current_color, current_color, used, points, markup, tin, up);
            }

            return { points, markup };
        }

        size_t size() const noexcept {
            return adj.size();
        }

        size_t count_edges() const noexcept {
            return number_of_edges;
        }
    };


    // ^^^ -----------Graph----------- ^^^
    // -----------------------------------
    // vvv ------BipartiteGraph------- vvv


    class BipartiteGraph {
        std::vector<std::vector<size_t>> adj_left;
        std::vector<std::vector<size_t>> adj_right;
        std::vector<size_t> matching;

        bool interal_kuhn_dfs(size_t vertex, std::vector<bool>& used, std::vector<size_t>& matching) const {
            used[vertex] = true;
            for (size_t to : adj_left[vertex]) {
                if (matching[to] == std::numeric_limits<size_t>::max() || !used[matching[to]] && interal_kuhn_dfs(matching[to], used, matching)) {
                    matching[to] = vertex;
                    return true;
                }
            }
            return false;
        }

    public:
        BipartiteGraph(size_t left_size, size_t right_size) {
            adj_left.resize(left_size);
            adj_right.resize(right_size);
        }

        void push_edge(size_t left, size_t right) {
            if (left >= size_left() || right >= size_right()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "push_edge, vertex index out of range.\n\n");
            }

            adj_left[left].push_back(right);
            adj_right[right].push_back(left);
        }

        void build_matching() {
            matching.clear();
            matching.resize(size_right(), std::numeric_limits<size_t>::max());
            std::vector<bool> pair(size_left(), false);
            std::vector<bool> used(size_left(), false);
            for (bool run = true; run;) {
                run = false;
                for (size_t vertex = 0; vertex < size_left(); ++vertex) {
                    if (!pair[vertex]) {
                        pair[vertex] = interal_kuhn_dfs(vertex, used, matching);
                        run = run || pair[vertex];
                    }
                }
                used.assign(size_left(), false);
            }
        }

        std::vector<std::pair<size_t, size_t>> get_matching() const {
            std::vector<std::pair<size_t, size_t>> result;
            result.reserve(std::min(size_left(), size_right()));
            for (size_t i = 0; i < size_right(); ++i) {
                if (matching[i] != std::numeric_limits<size_t>::max()) {
                    result.emplace_back(matching[i], i);
                }
            }
            return result;
        }

        size_t size_left() const noexcept {
            return adj_left.size();
        }

        size_t size_right() const noexcept {
            return adj_right.size();
        }
    };


    // ^^^ ------BipartiteGraph------- ^^^
    // -----------------------------------
}   // Graph, BipartiteGraph | Version: 0.0

using namespace alg::graph;