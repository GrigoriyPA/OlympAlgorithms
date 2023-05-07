namespace alg::graph {
    // -----------------------------------
    // vvv ------------Tree----------- vvv


    class Tree {
        size_t root = std::numeric_limits<size_t>::max();

        std::vector<std::vector<WeightedEdge>> adj;
        std::vector<std::vector<size_t>> binary_up;
        std::vector<int64_t> height;

        void interal_height_building_dfs(size_t vertex, size_t parent) {
            for (auto [to, weight, id] : adj[vertex]) {
                if (to == parent) {
                    continue;
                }

                height[to] = height[vertex] + weight;
                interal_height_building_dfs(to, vertex);
            }
        }

        void interal_binary_ups_building_dfs(size_t vertex, size_t parent) {
            binary_up[0][vertex] = parent;
            for (uint32_t degree = 1; degree < binary_up.size(); ++degree) {
                binary_up[degree][vertex] = binary_up[degree - 1][binary_up[degree - 1][vertex]];
            }

            for (auto [to, weight, id] : adj[vertex]) {
                if (to == parent) {
                    continue;
                }

                interal_binary_ups_building_dfs(to, vertex);
            }
        }

    public:
        template <typename T>
        Tree(const std::vector<std::vector<T>>& adjacency_list) {
            adj.resize(adjacency_list.size());
            for (size_t i = 0; i < adj.size(); ++i) {
                adj[i].reserve(adjacency_list[i].size());
                for (const T& element : adjacency_list[i]) {
                    adj[i].emplace_back(element);
                }
            }
        }

        Tree(size_t number_of_vertexes, const std::vector<WeightedEdgeInfo>& edges) {
            if (edges.size() + 1 != number_of_vertexes) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "Tree, invalid number of edges.\n\n");
            }

            adj.resize(number_of_vertexes);
            for (size_t i = 0; i < edges.size(); ++i) {
                adj[edges[i].from].emplace_back(edges[i].to, edges[i].weight, i);
                adj[edges[i].to].emplace_back(edges[i].from, edges[i].weight, i);
            }
        }

#ifdef Graph__INCLUDED
        Tree(const Graph& graph, const std::vector<size_t>& markup) {
            adj.resize(func::get_value(markup.begin(), markup.end(), std::max));

            size_t edge_id = 0;
            std::set<std::pair<size_t, size_t>> used_edges;
            for (size_t vertex = 0; vertex < graph.size(); ++vertex) {
                for (auto [to, id] : graph[vertex]) {
                    size_t left = std::min(markup[vertex], markup[to]) - 1;
                    size_t right = std::max(markup[vertex], markup[to]) - 1;
                    if (left == right || used_edges.contains({ left, right })) {
                        continue;
                    }

                    used_edges.insert({ left, right });
                    adj[left].emplace_back(right, 1, edge_id);
                    adj[right].emplace_back(left, 1, edge_id);
                    ++edge_id;
                }
            }

            if (edge_id + 1 != size()) {
                adj.clear();
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "Tree, invalid graph markup.\n\n");
            }
        }
#endif // Graph__INCLUDED

        const std::vector<WeightedEdge>& operator[](size_t index) const {
            if (index >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], vertex index out of range.\n\n");
            }

            return adj[index];
        }

        const std::vector<std::vector<WeightedEdge>>& get_adjacency_list() const noexcept {
            return adj;
        }

        size_t get_height(size_t vertex) const {
            if (vertex >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_height, vertex index out of range.\n\n");
            }

            return height[vertex];
        }

        void set_root(size_t new_root) {
            if (new_root >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "set_root, root index out of range.\n\n");
            }

            root = new_root;
        }

        void build_heights() {
            if (root >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "build_heights, root index out of range.\n\n");
            }

            height.assign(size(), 0);
            interal_height_building_dfs(root, root);
        }

        void build_binary_ups() {
            if (root >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "build_binary_ups, root index out of range.\n\n");
            }

            size_t log_size = 1;
            for (; (static_cast<size_t>(1) << log_size) <= size(); ++log_size) {}

            binary_up.assign(log_size, std::vector<size_t>(size(), root));
            interal_binary_ups_building_dfs(root, root);
        }

        void build_lca() {
            if (root >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "build_lca, root index out of range.\n\n");
            }

            build_heights();
            build_binary_ups();
        }

        size_t up_vertex(size_t vertex, int64_t distance) const {
            if (vertex >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "up_vertex, vertex index out of range.\n\n");
            }

            if (distance >= height[vertex]) {
                return root;
            }

            distance = height[vertex] - distance;
            for (int32_t degree = static_cast<int32_t>(binary_up.size()) - 1; degree >= 0; --degree) {
                if (height[binary_up[degree][vertex]] >= distance) {
                    vertex = binary_up[degree][vertex];
                }
            }

            return vertex;
        }

        size_t find_lca(size_t vertex1, size_t vertex2) const {
            if (vertex1 >= size() || vertex2 >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "find_lca, vertex index out of range.\n\n");
            }

            if (height[vertex1] < height[vertex2]) {
                std::swap(vertex1, vertex2);
            }
            vertex1 = up_vertex(vertex1, height[vertex1] - height[vertex2]);

            if (vertex1 == vertex2) {
                return vertex1;
            }

            for (int32_t degree = static_cast<int32_t>(binary_up.size()) - 1; degree >= 0; --degree) {
                if (binary_up[degree][vertex1] != binary_up[degree][vertex2]) {
                    vertex1 = binary_up[degree][vertex1];
                    vertex2 = binary_up[degree][vertex2];
                }
            }

            return binary_up[0][vertex1];
        }

        int64_t find_distance(size_t vertex1, size_t vertex2) const {
            if (vertex1 >= size() || vertex2 >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "find_distance, vertex index out of range.\n\n");
            }

            size_t lca = find_lca(vertex1, vertex2);
            return height[vertex1] + height[vertex2] - 2 * height[lca];
        }

        size_t size() const noexcept {
            return adj.size();
        }
    };


    // ^^^ ------------Tree----------- ^^^
    // -----------------------------------
}   // Tree | Version: 0.0

using namespace alg::graph;