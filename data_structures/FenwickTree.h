namespace alg::data_struct {
    // -----------------------------------
    // vvv -------FenwickTree1D------- vvv


    // ValueType constructors required: ValueType(ValueType); operators required: =(ValueType, ValueType)
    // ForwardOperation operators required: (ValueType, ValueType) -> ValueType
    // ReverseOperation operators required: (ValueType, ValueType) -> ValueType
    // Expected: ForwardOperation, ReverseOperation -- associativity, commutativity operations; ReverseOperation(ForwardOperation(x, y), y) = x
    template <typename ValueType = int64_t, typename ForwardOperation = std::plus<ValueType>, typename ReverseOperation = std::minus<ValueType>>
    class FenwickTree {
        class ProxyObject {
            size_t id_;
            FenwickTree* tree_;

        public:
            ProxyObject(size_t id, FenwickTree* tree) {
                if (id >= tree->size()) {
                    throw func::AlgOutOfRange(__FILE__, __LINE__, "ProxyObject, invalid index.\n\n");
                }

                id_ = id;
                tree_ = tree;
            }

            ProxyObject& operator=(const ValueType& other) {
                tree_->update(id_, other);
            }

            operator ValueType() const noexcept {
                return tree_->at(id_);
            }

            ProxyObject& operator+=(const ValueType& other) {
                tree_->change(id_, other);
                return *this;
            }
        };

        friend class ProxyObject;

        const ForwardOperation& forward_operation_;
        const ReverseOperation& reverse_operation_;
        std::vector<ValueType> tree_;
        std::vector<ValueType> value_;

        void build_tree(const std::vector<ValueType>& value) {
            value_.reserve(value.size());
            tree_.reserve(value.size());
            for (const ValueType& element : value) {
                push_back(element);
            }
        }

    public:
        class Iterator {
            friend class FenwickTree<ValueType>;

            int64_t id_;
            FenwickTree* tree_;

            Iterator(int64_t id, FenwickTree* tree) noexcept {
                id_ = id;
                tree_ = tree;
            }

        public:
            bool operator==(const Iterator& other) const noexcept {
                return id_ == other.id_ && tree_ == other.tree_;
            }

            bool operator!=(const Iterator& other) const noexcept {
                return !(*this == other);
            }

            std::strong_ordering operator<=>(const Iterator& other) const noexcept {
                return id_ <=> other.id_;
            }

            Iterator& operator+=(int64_t shift) noexcept {
                id_ += shift;
                return *this;
            }

            Iterator& operator-=(int64_t shift) noexcept {
                id_ -= shift;
                return *this;
            }

            ProxyObject operator*() const {
                return ProxyObject(static_cast<size_t>(id_), tree_);
            }

            Iterator& operator++() noexcept {
                id_++;
                return *this;
            }

            Iterator operator++(int) noexcept {
                Iterator result = *this;
                id_++;
                return result;
            }

            Iterator& operator--() noexcept {
                id_--;
                return *this;
            }

            Iterator operator--(int) noexcept {
                Iterator result = *this;
                id_--;
                return result;
            }

            Iterator operator+(int64_t shift) const noexcept {
                return Iterator(id_ + shift, tree_);
            }

            Iterator operator-(int64_t shift) const noexcept {
                return Iterator(id_ - shift, tree_);
            }

            int64_t operator-(Iterator other) const noexcept {
                return id_ - other.id_;
            }
        };

        using ConstIterator = std::vector<ValueType>::const_iterator;

        FenwickTree() : forward_operation_(ForwardOperation()), reverse_operation_(ReverseOperation()) {
        }

        explicit FenwickTree(const ForwardOperation& forward_operation, const ReverseOperation& reverse_operation = ReverseOperation()) :
            forward_operation_(forward_operation), reverse_operation_(reverse_operation) {
        }

        FenwickTree(size_t size, const ValueType& init, const ForwardOperation& forward_operation = ForwardOperation(), const ReverseOperation& reverse_operation = ReverseOperation()) :
            forward_operation_(forward_operation), reverse_operation_(reverse_operation) {
            build_tree(std::vector<ValueType>(size, init));
        }

        FenwickTree(const std::initializer_list<ValueType>& init, const ForwardOperation& forward_operation = ForwardOperation(), const ReverseOperation& reverse_operation = ReverseOperation()) :
            forward_operation_(forward_operation), reverse_operation_(reverse_operation) {
            build_tree(init);
        }

        explicit FenwickTree(const std::vector<ValueType>& init, const ForwardOperation& forward_operation = ForwardOperation(), const ReverseOperation& reverse_operation = ReverseOperation()) :
            forward_operation_(forward_operation), reverse_operation_(reverse_operation) {
            build_tree(init);
        }

        FenwickTree<ValueType, ForwardOperation, ReverseOperation>& operator=(const FenwickTree<ValueType, ForwardOperation, ReverseOperation>& other)& {
            tree_ = other.tree_;
            value_ = other.value_;
            return *this;
        }

        FenwickTree<ValueType, ForwardOperation, ReverseOperation>& operator=(const std::vector<ValueType>& other)& {
            clear();
            build_tree(other);
            return *this;
        }

        bool operator==(const FenwickTree<ValueType, ForwardOperation, ReverseOperation>& other) const {
            return value_ == other.value_;
        }

        bool operator!=(const FenwickTree<ValueType, ForwardOperation, ReverseOperation>& other) const {
            return value_ != other.value_;
        }

        ProxyObject operator[](size_t id) {
            if (id >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return ProxyObject(id, this);
        }

        const ValueType& operator[](size_t id) const {
            if (id >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return value_[id];
        }

        ValueType at(size_t id) const {
            if (id >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "at, invalid index.\n\n");
            }

            return value_[id];
        }

        size_t size() const noexcept {
            return value_.size();
        }

        bool empty() const noexcept {
            return value_.empty();
        }

        ProxyObject front() {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "front, called from empty tree.\n\n");
            }

            return ProxyObject(0, this);
        }

        const ValueType& front() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "front, called from empty tree.\n\n");
            }

            return value_.front();
        }

        ProxyObject back() {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "back, called from empty tree.\n\n");
            }

            return ProxyObject(size() - 1, this);
        }

        const ValueType& back() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "back, called from empty tree.\n\n");
            }

            return value_.back();
        }

        Iterator begin() noexcept {
            return Iterator(0, this);
        }

        Iterator end() noexcept {
            return Iterator(value_.size(), this);
        }

        ConstIterator begin() const noexcept {
            return value_.begin();
        }

        ConstIterator end() const noexcept {
            return value_.end();
        }

        ValueType get_prefix_result(size_t prfix_size) const {
            if (prfix_size == 0 || prfix_size > value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_prefix_result, invalid prfix size.\n\n");
            }

            ValueType result(tree_[prfix_size - 1]);
            int64_t start = static_cast<int64_t>(prfix_size);
            for (int64_t i = start - (start & -start); i > 0; i -= i & -i) {
                result = forward_operation_(result, tree_[i - 1]);
            }

            return result;
        }

        ValueType get_result(size_t left, size_t right) const {
            if (left > right) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "get_result, invalid range.\n\n");
            }

            if (right >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_result, invalid range size.\n\n");
            }

            if (left == 0) {
                return get_prefix_result(right + 1);
            }
            return reverse_operation_(get_prefix_result(right + 1), get_prefix_result(left));
        }

        void swap(FenwickTree<ValueType, ForwardOperation, ReverseOperation>& other) noexcept {
            tree_.swap(other.tree_);
            value_.swap(other.value_);
        }

        void reserve(size_t size) {
            value_.reserve(size);
            tree_.reserve(size);
        }

        void push_back(const ValueType& value) noexcept {
            value_.push_back(value);

            int64_t left = static_cast<int64_t>(value_.size()) - (static_cast<int64_t>(value_.size()) & -static_cast<int64_t>(value_.size()));
            int64_t right = static_cast<int64_t>(value_.size()) - 2;
            tree_.push_back(left <= right ? forward_operation_(get_result(left, right), value) : value);
        }

        void pop_back() {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "pop_back, attempt to pop from empty tree.\n\n");
            }

            value_.pop_back();
            tree_.pop_back();
        }

        void clear() noexcept {
            tree_.clear();
            value_.clear();
        }

        void update(size_t id, const ValueType& value) {
            if (id >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "update, invalid index.\n\n");
            }

            change(id, reverse_operation_(value, value_[id]));
        }

        void change(size_t id, const ValueType& delta) {
            if (id >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "change, invalid index.\n\n");
            }

            value_[id] = forward_operation_(value_[id], delta);
            for (int64_t i = id + 1; i < static_cast<int64_t>(tree_.size() + 1); i += i & -i) {
                tree_[i - 1] = forward_operation_(tree_[i - 1], delta);
            }
        }
    };


    // ^^^ -------FenwickTree1D------- ^^^
    // -----------------------------------
}   // FenwickTree1D | Version: 1.0

using namespace alg::data_struct;