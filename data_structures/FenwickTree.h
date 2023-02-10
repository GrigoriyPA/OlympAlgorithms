namespace alg::data_struct {
    // -----------------------------------
    // vvv -------FenwickTree1D------- vvv


    template <typename VT = int64_t>  // Constructors required: VT(0), VT(VT); Operators required: +(VT, VT), -(VT, VT)
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

            ProxyObject& operator=(const VT& other) {
                tree_->update(id_, other);
            }

            operator VT() const noexcept {
                return tree_->at(id_);
            }

            ProxyObject& operator+=(const VT& other) {
                tree_->change(id_, other);
                return *this;
            }

            ProxyObject& operator-=(const VT& other) {
                tree_->change(id_, tree_->zero_ - other);
                return *this;
            }
        };

        friend class ProxyObject;

        VT zero_;
        std::vector<VT> tree_;
        std::vector<VT> value_;

        template <typename T>  // Casts required: VT(T)
        void build_tree(const std::vector<T>& value) {
            value_.resize(value.size(), zero_);
            tree_.resize(value.size() + 1, zero_);
            for (size_t i = 0; i < value.size(); ++i) {
                change(i, value[i]);
            }
        }

    public:
        class Iterator {
            friend class FenwickTree<VT>;

            int64_t id_;
            FenwickTree* tree_;

            Iterator(int64_t id, FenwickTree* tree) noexcept {
                id_ = id;
                tree_ = tree;
            }

        public:
            bool operator==(const Iterator& other) const {
                return id_ == other.id_ && tree_ == other.tree_;
            }

            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }

            std::strong_ordering operator<=>(const Iterator& other) const {
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

        using ConstIterator = std::vector<VT>::const_iterator;

        FenwickTree(size_t size, const VT& init) noexcept : zero_(0) {
            build_tree(std::vector<VT>(size, init));
        }

        FenwickTree(size_t size, const VT& init, const VT& zero) noexcept : zero_(zero) {
            build_tree(std::vector<VT>(size, init));
        }

        template <typename T>  // Casts required: VT(T)
        FenwickTree(const std::vector<T>& init) noexcept : zero_(0) {
            build_tree(init);
        }

        template <typename T>  // Casts required: VT(T)
        FenwickTree(const std::vector<T>& init, const VT& zero) noexcept : zero_(zero) {
            build_tree(init);
        }

        template <typename T>  // Casts required: VT(T)
        FenwickTree(const std::initializer_list<T>& init) noexcept : zero_(0) {
            build_tree(std::vector<T>(init));
        }

        template <typename T>  // Casts required: VT(T)
        FenwickTree(const std::initializer_list<T>& init, const VT& zero) noexcept : zero_(zero) {
            build_tree(std::vector<T>(init));
        }

        ProxyObject operator[](size_t id) {
            if (id >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return ProxyObject(id, this);
        }

        const VT& operator[](size_t id) const {
            if (id >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return value_[id];
        }

        bool operator==(const FenwickTree& other) const noexcept {
            return value_ == other.value_ && zero_ == other.zero_;
        }

        bool operator!=(const FenwickTree& other) const noexcept {
            return !(*this == other);
        }

        VT at(size_t id) const {
            if (id >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get, invalid index.\n\n");
            }

            return value_[id];
        }

        size_t size() const noexcept {
            return value_.size();
        }

        bool empty() const noexcept {
            return value_.empty();
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

        VT prefix_sum(size_t prfix_size) const {
            if (prfix_size > value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "prefix_sum, invalid prfix size.\n\n");
            }

            VT result = zero_;
            for (int64_t i = prfix_size; i > 0; i -= i & -i) {
                result = result + tree_[i];
            }
            return result;
        }

        VT sum(size_t left, size_t right) const {
            if (left > right || right >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "sum, invalid range.\n\n");
            }

            return prefix_sum(right + 1) - prefix_sum(left);
        }

        void swap(FenwickTree& other) noexcept {
            std::swap(zero_, other.zero_);
            std::swap(tree_, other.tree_);
            std::swap(value_, other.value_);
        }

        void reserve(size_t size) noexcept {
            value_.reserve(size);
            tree_.reserve(size + 1);
        }

        void push_back(const VT& value) noexcept {
            value_.push_back(zero_);

            int64_t left = static_cast<int64_t>(value_.size()) - (static_cast<int64_t>(value_.size()) & -static_cast<int64_t>(value_.size()));
            int64_t right = static_cast<int64_t>(value_.size()) - 2;
            if (left <= right) {
                tree_.push_back(sum(left, right));
            } else {
                tree_.push_back(zero_);
            }

            update(value_.size() - 1, value);
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

            tree_.resize(1, zero_);
        }

        void update(size_t id, const VT& value) {
            if (id >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "update, invalid index.\n\n");
            }

            change(id, value - value_[id]);
        }

        void change(size_t id, const VT& delta) {
            if (id >= value_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "change, invalid index.\n\n");
            }

            value_[id] = value_[id] + delta;
            for (int64_t i = id + 1; i < static_cast<int64_t>(tree_.size()); i += i & -i) {
                tree_[i] = tree_[i] + delta;
            }
        }

        template <typename T>
        friend std::ostream& operator<<(std::ostream& fout, const FenwickTree<T>& vector) noexcept;
    };

    template <typename T>
    std::istream& operator>>(std::istream& fin, FenwickTree<T>& tree) noexcept {
        for (size_t i = 0; i < tree.size(); ++i) {
            T value;
            fin >> value;

            tree.update(i, value);
        }
        return fin;
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& fout, const FenwickTree<T>& tree) noexcept {
        std::vector<std::vector<std::string>> description(tree.size() + 2);
        description[0] = { "index", "value", "tree state" };
        for (size_t i = 1; i < tree.size() + 2; ++i) {
            description[i] = { func::to_string(i - 1), ".", func::to_string(tree.tree_[i - 1]) };
            if (i > 1) {
                description[i][1] = func::to_string(tree.value_[i - 2]);
            }
        }

        fout << func::make_table_decorated(description);
        return fout;
    }


    // ^^^ -------FenwickTree1D------- ^^^
    // -----------------------------------
}   // FenwickTree1D | Version: 0.0

using namespace alg::data_struct;