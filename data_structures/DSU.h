namespace alg::data_struct {
    // -----------------------------------
    // vvv ------------DSU------------ vvv


    class DSU {
        std::vector<size_t> parent;
        std::vector<size_t> rank;

    public:
        DSU(size_t number_of_vertexes) {
            rank.resize(number_of_vertexes, 1);
            parent.reserve(number_of_vertexes);
            for (size_t i = 0; i < number_of_vertexes; ++i) {
                parent.push_back(i);
            }
        }

        size_t get(size_t vertex) {
            if (parent[vertex] == vertex) {
                return vertex;
            }

            return parent[vertex] = get(parent[vertex]);
        }

        bool unite(size_t vertex1, size_t vertex2) {
            vertex1 = get(vertex1);
            vertex2 = get(vertex2);
            if (vertex1 == vertex2) {
                return false;
            }

            if (rank[vertex1] > rank[vertex2]) {
                std::swap(vertex1, vertex2);
            }
            parent[vertex1] = vertex2;
            rank[vertex2] += rank[vertex1] == rank[vertex2];

            return true;
        }

        size_t size() const noexcept {
            return parent.size();
        }
    };


    // ^^^ ------------DSU------------ ^^^
    // -----------------------------------
}   // DSU | Version: 0.0

using namespace alg::data_struct;