namespace alg::data_struct {
    // vvv ------SegmentTreeNode------ vvv
    // -----------------------------------


    struct SegmentTreeNode {
        SegmentTreeNode() {
        }

        template <typename T>
        SegmentTreeNode(const T& x) {
        }

        void push(SegmentTreeNode& left, SegmentTreeNode& right) {
        }

        template <typename T>
        void update(const T& value) {
        }

        template <typename T>
        bool check_descent_value(const T& descent_value) {
            return false;
        }

        template <typename T>
        void change_descent_value(T& descent_value) {
        }

        static SegmentTreeNode unite(const SegmentTreeNode& left, const SegmentTreeNode& right) {
            return left;
        }
    };


    // ^^^ ------SegmentTreeNode------ ^^^
    // -----------------------------------
    // vvv --------SegmentTree-------- vvv


    template <typename Node = SegmentTreeNode>
    class SegmentTree {
        size_t tree_size_;
        std::vector<Node> tree_;

        template <typename T>
        void build(size_t vertex, size_t left, size_t right, const std::vector<T>& init) {
            if (left == right) {
                tree_[vertex] = Node(init[left]);
                return;
            }

            size_t middle = (left + right) / 2;
            build(2 * vertex, left, middle, init);
            build(2 * vertex + 1, middle + 1, right, init);
            tree_[vertex] = Node::unite(tree_[2 * vertex], tree_[2 * vertex + 1]);
        }

        Node get_result(size_t vertex, size_t left, size_t right, size_t request_left, size_t request_right) {
            tree_[vertex].push(tree_[std::min(2 * vertex, 4 * tree_size_)], tree_[std::min(2 * vertex + 1, 4 * tree_size_)]);

            if (left == request_left && right == request_right) {
                return tree_[vertex];
            }

            size_t middle = (left + right) / 2;
            if (request_right <= middle) {
                return get_result(2 * vertex, left, middle, request_left, request_right);
            }
            if (request_left > middle) {
                return get_result(2 * vertex + 1, middle + 1, right, request_left, request_right);
            }
            return Node::unite(get_result(2 * vertex, left, middle, request_left, middle), get_result(2 * vertex + 1, middle + 1, right, middle + 1, request_right));
        }

        template <typename T>
        void update_segment(size_t vertex, size_t left, size_t right, size_t request_left, size_t request_right, const T& value) {
            tree_[vertex].push(tree_[std::min(2 * vertex, 4 * tree_size_)], tree_[std::min(2 * vertex + 1, 4 * tree_size_)]);

            if (left == request_left && right == request_right) {
                tree_[vertex].update(value);
                tree_[vertex].push(tree_[std::min(2 * vertex, 4 * tree_size_)], tree_[std::min(2 * vertex + 1, 4 * tree_size_)]);
                return;
            }

            size_t middle = (left + right) / 2;
            if (request_right <= middle) {
                update_segment(2 * vertex, left, middle, request_left, request_right, value);
                tree_[2 * vertex + 1].push(tree_[std::min(4 * vertex + 2, 4 * tree_size_)], tree_[std::min(4 * vertex + 3, 4 * tree_size_)]);
            }
            else if (request_left > middle) {
                update_segment(2 * vertex + 1, middle + 1, right, request_left, request_right, value);
                tree_[2 * vertex].push(tree_[std::min(4 * vertex, 4 * tree_size_)], tree_[std::min(4 * vertex + 1, 4 * tree_size_)]);
            }
            else {
                update_segment(2 * vertex, left, middle, request_left, middle, value);
                update_segment(2 * vertex + 1, middle + 1, right, middle + 1, request_right, value);
            }
            tree_[vertex] = Node::unite(tree_[2 * vertex], tree_[2 * vertex + 1]);
        }

        template <typename T>
        std::pair<size_t, Node> descent(size_t vertex, size_t left, size_t right, size_t request_left, size_t request_right, T value) {
            tree_[vertex].push(tree_[std::min(2 * vertex, 4 * tree_size_)], tree_[std::min(2 * vertex + 1, 4 * tree_size_)]);

            if (left == request_left && request_right == right) {
                if (tree_[vertex].check_descent_value(value)) {
                    if (left == right) {
                        return { left, tree_[vertex] };
                    }

                    size_t middle = (left + right) / 2;
                    if (tree_[2 * vertex].check_descent_value(value)) {
                        return descent(2 * vertex, left, middle, left, middle, value);
                    }

                    tree_[2 * vertex].change_descent_value(value);
                    return descent(2 * vertex + 1, middle + 1, right, middle + 1, right, value);
                }
                return { tree_size_, tree_[vertex] };
            }

            size_t middle = (left + right) / 2;
            if (request_right <= middle) {
                return descent(2 * vertex, left, middle, request_left, request_right, value);
            }
            if (request_left > middle) {
                return descent(2 * vertex + 1, middle + 1, right, request_left, request_right, value);
            }

            std::pair<size_t, Node> result_left = descent(2 * vertex, left, middle, request_left, middle, value);
            if (result_left.first < tree_size_) {
                return result_left;
            }

            result_left.second.change_descent_value(value);
            std::pair<size_t, Node> result_right = descent(2 * vertex + 1, middle + 1, right, middle + 1, request_right, value);
            if (result_right.first < tree_size_) {
                return result_right;
            }

            return { tree_size_, Node::unite(result_left.second, result_right.second) };
        }

    public:
        SegmentTree() noexcept {
            tree_size_ = 0;
        }

        explicit SegmentTree(size_t size) {
            tree_size_ = size;
            if (size == 0) {
                return;
            }

            tree_.resize(4 * size + 1);
        }

        template <typename T>
        SegmentTree(size_t size, const T& value) {
            tree_size_ = size;
            if (size == 0) {
                return;
            }

            tree_.resize(4 * size + 1);
            build(1, 0, tree_size_ - 1, std::vector<T>(size, value));
        }

        template <typename T>
        SegmentTree(const std::initializer_list<T>& init) {
            tree_size_ = init.size();
            if (tree_size_ == 0) {
                return;
            }

            tree_.resize(4 * init.size() + 1);
            build(1, 0, tree_size_ - 1, std::vector<T>(init));
        }

        template <typename T>
        explicit SegmentTree(const std::vector<T>& init) {
            tree_size_ = init.size();
            if (init.empty()) {
                return;
            }

            tree_.resize(4 * init.size() + 1);
            build(1, 0, tree_size_ - 1, init);
        }

        template <typename T>
        SegmentTree<Node>& operator=(const std::vector<T>& value)& {
            clear();
            tree_size_ = value.size();
            if (value.empty()) {
                return;
            }

            tree_.resize(4 * value.size() + 1);
            build(1, 0, tree_size_ - 1, value);
            return *this;
        }

        Node get_result(size_t left, size_t right) {
            if (left > right) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "get_result, invalid range.\n\n");
            }

            if (right >= tree_size_) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_result, invalid range size.\n\n");
            }

            return get_result(1, 0, tree_size_ - 1, left, right);
        }

        template <typename T>
        size_t descent(size_t left, size_t right, const T& value) {
            return descent(1, 0, tree_size_ - 1, left, right, value).first;
        }

        template <typename T>
        void update_segment(size_t left, size_t right, const T& value) {
            update_segment(1, 0, tree_size_ - 1, left, right, value);
        }

        size_t size() const noexcept {
            return tree_size_;
        }

        bool empty() const noexcept {
            return tree_size_ == 0;
        }

        void swap(SegmentTree<Node>& other) noexcept {
            std::swap(tree_size_, other.tree_size_);
            tree_.swap(other.tree_);
        }

        void clear() noexcept {
            tree_size_ = 0;
            tree_.clear();
        }
    };


    // ^^^ --------SegmentTree-------- ^^^
    // -----------------------------------
}   // SegmentTreeNod, SegmentTree | Version: 1.0

using namespace alg::data_struct;