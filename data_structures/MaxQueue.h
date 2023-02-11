namespace alg::data_struct {
    // -----------------------------------
    // vvv ----------MaxQueue--------- vvv


    // ValueType constructors required: ValueType(ValueType); operators required: =(ValueType, ValueType)
    // Predicate operators required: (ValueType, ValueType) -> bool
    template <typename ValueType = int64_t, typename Predicate = std::less<ValueType>>
    class MaxQueue {
        struct QueueElement {
            ValueType value;
            size_t id;
        };

        size_t left_id_ = 0;
        size_t right_id_ = 0;

        const Predicate& predicate_;
        std::deque<QueueElement> max_queue_;

        void build(const std::vector<ValueType>& value) {
            for (const ValueType& element : value) {
                push(element);
            }
        }

    public:
        MaxQueue() : predicate_(Predicate()) {
        }

        explicit MaxQueue(const Predicate& predicate) : predicate_(predicate) {
        }

        MaxQueue(size_t size, const ValueType& init, const Predicate& predicate = Predicate()) : predicate_(predicate) {
            build(std::vector<ValueType>(size, init));
        }

        MaxQueue(const std::initializer_list<ValueType>& init, const Predicate& predicate = Predicate()) : predicate_(predicate) {
            build(init);
        }

        explicit MaxQueue(const std::vector<ValueType>& init, const Predicate& predicate = Predicate()) : predicate_(predicate) {
            build(init);
        }

        MaxQueue<ValueType, Predicate>& operator=(const MaxQueue<ValueType, Predicate>& other)& {
            left_id_ = other.left_id_;
            right_id_ = other.right_id_;
            max_queue_ = other.max_queue_;
            return *this;
        }

        MaxQueue<ValueType, Predicate>& operator=(const std::vector<ValueType>& value)& {
            clear();
            build(value);
            return *this;
        }

        size_t get_left_id() const noexcept {
            return left_id_;
        }

        size_t get_right_id() const noexcept {
            return right_id_;
        }

        const ValueType& top_element() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "top_element, attempt to find top element in empty queue.\n\n");
            }

            return max_queue_.front().value;
        }

        size_t top_element_id() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "top_element_id, attempt to find top element in empty queue.\n\n");
            }

            return max_queue_.front().id;
        }

        size_t size() const noexcept {
            return right_id_ - left_id_;
        }

        bool empty() const noexcept {
            return max_queue_.empty();
        }

        const ValueType& back() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "back, called from empty queue.\n\n");
            }

            return max_queue_.back().value;
        }

        void swap(MaxQueue<ValueType, Predicate>& other) noexcept {
            std::swap(left_id_, other.left_id_);
            std::swap(right_id_, other.right_id_);
            max_queue_.swap(other.max_queue_);
        }

        size_t push(const ValueType& value) {
            while (!max_queue_.empty() && predicate_(max_queue_.back().value, value)) {
                max_queue_.pop_back();
            }

            max_queue_.push_back(QueueElement{ .value = value, .id = right_id_ });
            return right_id_++;
        }

        // Delete all elements before id
        void pop(size_t id) {
            if (id > right_id_) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "pop, invalid index.\n\n");
            }

            for (; !max_queue_.empty() && max_queue_.front().id < id; max_queue_.pop_front()) {}
            left_id_ = id;
        }

        void pop() {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "pop, attempt to pop from empty queue.\n\n");
            }

            if (max_queue_.front().id == left_id_) {
                max_queue_.pop_front();
            }
            ++left_id_;
        }

        void clear() noexcept {
            left_id_ = 0;
            right_id_ = 0;
            max_queue_.clear();
        }
    };


    // ^^^ ----------MaxQueue--------- ^^^
    // -----------------------------------
}   // MaxQueue | Version: 1.0

using namespace alg::data_struct;