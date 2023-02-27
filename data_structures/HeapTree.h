namespace alg::data_struct {
    // vvv --------HeapTreeNode------- vvv
    // -----------------------------------


    // Other Node classes must derive from HeapTreeNode and override all functions, that they use
    struct HeapTreeNode {
        const static uint32_t NOT_EXIST = std::numeric_limits<uint32_t>::max();

        uint32_t left = NOT_EXIST;
        uint32_t right = NOT_EXIST;
        uint32_t subtree_size = 0;
        uint64_t priority = 0;

        HeapTreeNode(std::mt19937_64& generator) {
            priority = generator();
            subtree_size = 1;
        }

        template <typename T>  // First node initialization (called in build, insert)
        void initialize(const T& value) {
        }

        template <typename T>  // Update node accumulated value (called in update_segment)
        void update(const T& value) {
        }

        template <typename T>  // Returns true if a descent result is found in this vertex (called in descent)
        bool is_finish_descent_in_vertex(const T& descent_value) {
            return false;
        }

        template <typename T>  // Returns true if a descent result is found in this node subtree (called in descent)
        bool is_finish_descent_in_subtree(const T& descent_value) {
            return false;
        }

        template <typename T>  // Update descent value after exiting vertex (called in descent)
        void change_descent_value_by_vertex(T& descent_value) {
        }

        template <typename T>  // Update descent value after exiting node subtree (called in descent)
        void change_descent_value_by_subtree(T& descent_value) {
        }

        // Push accumulated value to current vertex (called in get_result, descent, update_segment, insert, erase)
        void push() {
        }

        // Push accumulated value to some chaild of vertex (called in get_result, descent, update_segment, insert, erase)
        void push(HeapTreeNode& node) {
        }

        // Push accumulated value to children (called in get_result, descent, update_segment, insert, erase)
        void push(HeapTreeNode& left_node, HeapTreeNode& right_node) {
        }

        // Calculate node value from current vertex value (called in all functions)
        void unite() {
        }

        // Calculate node value from some chaild of vertex (called in all functions)
        void unite(const HeapTreeNode& node, bool is_left) {
        }

        // Calculate node value from children values (called in all functions)
        void unite(const HeapTreeNode& left_node, const HeapTreeNode& right_node) {
        }
    };


    // ^^^ --------HeapTreeNode------- ^^^
    // -----------------------------------
    // vvv ----------HeapTree--------- vvv


    // Expected: class Node : public HeapTreeNode {}
    template <typename Node = HeapTreeNode>
    class HeapTree {
        const static uint32_t NOT_EXIST = std::numeric_limits<uint32_t>::max();

        uint32_t root_ = NOT_EXIST;
        size_t tree_size_ = 0;

        std::mt19937_64 generator_;
        std::vector<Node> tree_;

        void push_node(Node& node) {
            if (node.left == NOT_EXIST && node.right == NOT_EXIST) {
                node.push();
                return;
            }

            if (node.right == NOT_EXIST) {
                node.push(tree_[node.left]);
                return;
            }

            if (node.left == NOT_EXIST) {
                node.push(tree_[node.right]);
                return;
            }

            node.push(tree_[node.left], tree_[node.right]);
        }

        void unite_node(Node& node) {
            node.subtree_size = 1;
            if (node.left == NOT_EXIST && node.right == NOT_EXIST) {
                node.unite();
                return;
            }

            if (node.right == NOT_EXIST) {
                node.subtree_size += tree_[node.left].subtree_size;
                push_node(tree_[node.left]);
                node.unite(tree_[node.left], true);
                return;
            }

            if (node.left == NOT_EXIST) {
                node.subtree_size += tree_[node.right].subtree_size;
                push_node(tree_[node.right]);
                node.unite(tree_[node.right], false);
                return;
            }

            node.subtree_size += tree_[node.left].subtree_size + tree_[node.right].subtree_size;
            push_node(tree_[node.left]);
            push_node(tree_[node.right]);
            node.unite(tree_[node.left], tree_[node.right]);
        }

        void init_tree(uint32_t vertex) {
            if (vertex == NOT_EXIST) {
                return;
            }

            init_tree(tree_[vertex].left);
            init_tree(tree_[vertex].right);
            unite_node(tree_[vertex]);
        }

        template <typename T>
        void build(const std::vector<T>& init) {
            if (init.empty()) {
                return;
            }

            std::vector<uint32_t> stack;
            tree_.reserve(init.size());
            stack.reserve(init.size());
            for (const T& element : init) {
                tree_.emplace_back(generator_);
                uint32_t new_node = tree_.size() - 1;
                tree_[new_node].initialize(element);

                uint32_t root = std::numeric_limits<uint32_t>::max();
                if (!stack.empty()) {
                    root = stack.front();
                }

                for (; !stack.empty() && tree_[stack.back()].priority < tree_[new_node].priority; stack.pop_back()) {}

                if (!stack.empty()) {
                    tree_[new_node].left = tree_[stack.back()].right;
                    tree_[stack.back()].right = new_node;
                }
                else {
                    tree_[new_node].left = root;
                }

                stack.push_back(new_node);
            }

            root_ = stack.front();
            init_tree(root_);
        }

        std::pair<uint32_t, uint32_t> split(uint32_t vertex, uint32_t current_index, uint32_t index) {
            if (vertex == NOT_EXIST) {
                return { NOT_EXIST, NOT_EXIST };
            }
            push_node(tree_[vertex]);

            uint32_t left_size = tree_[vertex].left == NOT_EXIST ? 0 : tree_[tree_[vertex].left].subtree_size;
            if (current_index + left_size < index) {
                auto [left, right] = split(tree_[vertex].right, current_index + left_size + 1, index);

                tree_[vertex].right = left;
                unite_node(tree_[vertex]);
                return { vertex, right };
            }
            else {
                auto [left, right] = split(tree_[vertex].left, current_index, index);

                tree_[vertex].left = right;
                unite_node(tree_[vertex]);
                return { left, vertex };
            }
        }

        uint32_t merge(uint32_t vertex_left, uint32_t vertex_right) {
            if (vertex_left == NOT_EXIST) {
                return vertex_right;
            }
            if (vertex_right == NOT_EXIST) {
                return vertex_left;
            }

            if (tree_[vertex_left].priority < tree_[vertex_right].priority) {
                push_node(tree_[vertex_right]);
                tree_[vertex_right].left = merge(vertex_left, tree_[vertex_right].left);

                unite_node(tree_[vertex_right]);
                return vertex_right;
            }
            else {
                push_node(tree_[vertex_left]);
                tree_[vertex_left].right = merge(tree_[vertex_left].right, vertex_right);

                unite_node(tree_[vertex_left]);
                return vertex_left;
            }
        }

        template <typename T>
        uint32_t descent_from_vertex(uint32_t vertex, uint32_t current_index, T value) {
            if (vertex == NOT_EXIST) {
                return size();
            }
            push_node(tree_[vertex]);

            if (tree_[vertex].left != NOT_EXIST) {
                push_node(tree_[tree_[vertex].left]);
                if (tree_[tree_[vertex].left].is_finish_descent_in_subtree(value)) {
                    return descent_from_vertex(tree_[vertex].left, current_index, value);
                }
                else {
                    tree_[tree_[vertex].left].change_descent_value_by_subtree(value);
                }
            }

            uint32_t left_size = tree_[vertex].left == NOT_EXIST ? 0 : tree_[tree_[vertex].left].subtree_size;
            if (tree_[vertex].is_finish_descent_in_vertex(value)) {
                return current_index + left_size;
            }
            tree_[vertex].change_descent_value_by_vertex(value);

            return descent_from_vertex(tree_[vertex].right, current_index + left_size + 1, value);
        }

        uint32_t insert(uint32_t vertex, uint32_t current_index, uint32_t index, uint32_t new_vertex) {
            if (vertex == NOT_EXIST || tree_[vertex].priority < tree_[new_vertex].priority) {
                auto [left, right] = split(vertex, current_index, index);
                tree_[new_vertex].left = left;
                tree_[new_vertex].right = right;

                unite_node(tree_[new_vertex]);
                return new_vertex;
            }
            push_node(tree_[vertex]);

            uint32_t left_size = tree_[vertex].left == NOT_EXIST ? 0 : tree_[tree_[vertex].left].subtree_size;
            if (current_index + left_size < index) {
                tree_[vertex].right = insert(tree_[vertex].right, current_index + left_size + 1, index, new_vertex);

                unite_node(tree_[vertex]);
                return vertex;
            }
            else {
                tree_[vertex].left = insert(tree_[vertex].left, current_index, index, new_vertex);

                unite_node(tree_[vertex]);
                return vertex;
            }
        }

        uint32_t erase(uint32_t vertex, uint32_t current_index, uint32_t index) {
            if (vertex == NOT_EXIST) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "erase, index not found.\n\n");
            }
            push_node(tree_[vertex]);

            uint32_t left_size = tree_[vertex].left == NOT_EXIST ? 0 : tree_[tree_[vertex].left].subtree_size;
            if (current_index + left_size == index) {
                return merge(tree_[vertex].left, tree_[vertex].right);
            }
            else if (current_index + left_size < index) {
                tree_[vertex].right = erase(tree_[vertex].right, current_index + left_size + 1, index);

                unite_node(tree_[vertex]);
                return vertex;
            }
            else {
                tree_[vertex].left = erase(tree_[vertex].left, current_index, index);

                unite_node(tree_[vertex]);
                return vertex;
            }
        }

    public:
        HeapTree(uint64_t seed) {
            tree_size_ = 0;
            generator_.seed(seed);
        }

        template <typename T>
        HeapTree(size_t size, const T& value, uint64_t seed) {
            tree_size_ = size;
            generator_.seed(seed);
            build(std::vector<T>(size, value));
        }

        template <typename T>
        HeapTree(const std::initializer_list<T>& init, uint64_t seed) {
            tree_size_ = init.size();
            generator_.seed(seed);
            build(std::vector<T>(init));
        }

        template <typename T>
        HeapTree(const std::vector<T>& init, uint64_t seed) {
            tree_size_ = init.size();
            generator_.seed(seed);
            build(init);
        }

        template <typename T>
        HeapTree<Node>& operator=(const std::vector<T>& value)& {
            clear();
            tree_size_ = value.size();
            build(value);
            return *this;
        }

        Node get_result(size_t left, size_t right) {
            if (left > right) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "get_result, invalid range.\n\n");
            }

            if (right >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_result, invalid range size.\n\n");
            }

            auto [root_left, root_middle_left] = split(root_, 0, left);
            auto [root_middle_right, root_right] = split(root_middle_left, 0, right - left + 1);

            if (root_middle_right == NOT_EXIST) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "get_result, root of cut not found.\n\n");
            }

            push_node(tree_[root_middle_right]);
            Node result = tree_[root_middle_right];

            root_middle_left = merge(root_middle_right, root_right);
            root_ = merge(root_left, root_middle_left);

            return result;
        }

        template <typename T>
        size_t descent(const T& value) {
            return descent_from_vertex(root_, 0, value);
        }

        template <typename T>
        size_t descent(size_t left, size_t right, const T& value) {
            if (left > right) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "descent, invalid range.\n\n");
            }

            if (right >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "descent, invalid range size.\n\n");
            }

            auto [root_left, root_middle_left] = split(root_, 0, left);
            auto [root_middle_right, root_right] = split(root_middle_left, 0, right - left + 1);

            if (root_middle_right == NOT_EXIST) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "update_segment, root of cut not found.\n\n");
            }

            size_t result = descent_from_vertex(root_middle_right, left, value);

            root_middle_left = merge(root_middle_right, root_right);
            root_ = merge(root_left, root_middle_left);

            return result;
        }

        template <typename T>
        void update_segment(size_t left, size_t right, const T& value) {
            if (left > right) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "update_segment, invalid range.\n\n");
            }

            if (right >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "update_segment, invalid range size.\n\n");
            }

            auto [root_left, root_middle_left] = split(root_, 0, left);
            auto [root_middle_right, root_right] = split(root_middle_left, 0, right - left + 1);

            if (root_middle_right == NOT_EXIST) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "update_segment, root of cut not found.\n\n");
            }

            tree_[root_middle_right].update(value);

            root_middle_left = merge(root_middle_right, root_right);
            root_ = merge(root_left, root_middle_left);
        }

        template <typename T>
        void insert(size_t index, const T& value) {
            if (index > size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "insert, invalid index.\n\n");
            }

            ++tree_size_;
            tree_.emplace_back(generator_);
            tree_.back().initialize(value);
            root_ = insert(root_, 0, index, tree_.size() - 1);
        }

        void erase(size_t index) {
            if (index >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "erase, invalid index.\n\n");
            }

            --tree_size_;
            root_ = erase(root_, 0, index);
        }

        size_t size() const noexcept {
            return tree_size_;
        }

        bool empty() const noexcept {
            return tree_size_ == 0;
        }

        void swap(HeapTree& other) {
            std::swap(tree_size_, other.tree_size_);
            std::swap(root_, other.root_);
            std::swap(generator_, other.generator_);
            tree_.swap(other.tree_);
        }

        void reserve(size_t size) {
            tree_.reserve(size);
        }

        void clear() noexcept {
            tree_size_ = 0;
            root_ = NOT_EXIST;
            tree_.clear();
        }
    };


    // ^^^ ----------HeapTree--------- ^^^
    // -----------------------------------
}   // HeapTreeNode, HeapTree | Version: 1.0

using namespace alg::data_struct;