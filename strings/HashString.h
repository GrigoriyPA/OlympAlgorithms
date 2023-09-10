namespace alg::str {
    // -----------------------------------
    // vvv ---------SubString--------- vvv


    template <uint64_t BASE, uint64_t MODUL>
    class HashString;

    template <uint64_t BASE, uint64_t MODUL>
    class SubString {
        friend class HashString<BASE, MODUL>;

        size_t left_id_;
        size_t right_id_;
        const HashString<BASE, MODUL>* str_;

        SubString(size_t left_id, size_t right_id, const HashString<BASE, MODUL>* str) {
            if (left_id > right_id) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "SubString, invalid range.\n\n");
            }

            if (right_id >= str->size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "SubString, invalid range size.\n\n");
            }

            left_id_ = left_id;
            right_id_ = right_id;
            str_ = str;
        }

    public:
        char operator[](size_t index) const {
            if (index >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            return (*str_)[left_id_ + index];
        }

        bool operator==(const SubString<BASE, MODUL>& other) const noexcept {
            return get_hash() == other.get_hash();
        }

        bool operator!=(const SubString<BASE, MODUL>& other) const noexcept {
            return get_hash() != other.get_hash();
        }

        std::strong_ordering operator<=>(const SubString<BASE, MODUL>& other) const noexcept {
            if (*this == other) {
                return std::strong_ordering::equal;
            }

            size_t equal_prefix_size = *this & other;
            if (equal_prefix_size == std::min(size(), other.size())) {
                return size() <=> other.size();
            }
            return (*str_)[left_id_ + equal_prefix_size] <=> (*other.str_)[other.left_id_ + equal_prefix_size];
        }

        size_t operator&(const SubString<BASE, MODUL>& other) const noexcept {
            if ((*str_)[left_id_] != (*other.str_)[other.left_id_]) {
                return 0;
            }

            size_t left = 0;
            size_t right = std::min(size(), other.size());
            while (right - left > 1) {
                size_t middle = (left + right) / 2;

                if (str_->get_hash(left_id_, left_id_ + middle) == other.str_->get_hash(other.left_id_, other.left_id_ + middle)) {
                    left = middle;
                }
                else {
                    right = middle;
                }
            }
            return right;
        }

        uint64_t get_hash(size_t left_id, size_t right_id) const {
            if (left_id > right_id) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "get_hash, invalid range.\n\n");
            }

            if (right_id >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_hash, invalid range size.\n\n");
            }

            return str_->get_hash(left_id_ + left_id, left_id_ + right_id);
        }

        uint64_t get_hash() const noexcept {
            return str_->get_hash(left_id_, right_id_);
        }

        std::string get_string() const {
            std::string str;
            str.reserve(size());
            for (size_t i = 0; i < size(); ++i) {
                str.push_back((*str_)[left_id_ + i]);
            }
            return str;
        }

        char front() const noexcept {
            return (*str_)[left_id_];
        }

        char back() const noexcept {
            return (*str_)[right_id_];
        }

        size_t size() const noexcept {
            return right_id_ - left_id_ + 1;
        }
    };

    template <uint64_t BASE, uint64_t MODUL>
    std::ostream& operator<<(std::ostream& fout, const SubString<BASE, MODUL>& sub_str) {
        fout << sub_str.get_string();
        return fout;
    }


    // ^^^ ---------SubString--------- ^^^
    // -----------------------------------
    // vvv ---------HashString-------- vvv


    // Polynomial hashing by base BASE with module MODUL
    // Expected BASE and MODUL:
    // -- BASE, MODUL - prime values
    // -- 256 < BASE < MODUL < 2^32
    template <uint64_t BASE = 263, uint64_t MODUL = 998244353>
    class HashString {
        inline static std::vector<uint64_t> power_ = std::vector<uint64_t>(1, 1);

        std::vector<uint64_t> hash_;

        void update_hash(const std::string& str) {
            hash_.clear();
            hash_.resize(str.size() + 1, 0);
            for (size_t i = 1; i <= str.size(); ++i) {
                hash_[i] = (hash_[i - 1] * BASE + static_cast<uint64_t>(str[i - 1]) + 1) % MODUL;
            }

            update_power(str.size() + 1);
        }

        static void update_power(size_t new_size) {
            power_.reserve(new_size);
            while (power_.size() < new_size) {
                power_.push_back((power_.back() * BASE) % MODUL);
            }
        }

    public:
        class ConstIterator {
            friend class HashString<BASE, MODUL>;

            int64_t id_;
            const HashString<BASE, MODUL>* str_;

            ConstIterator(int64_t id, const HashString<BASE, MODUL>* str) noexcept {
                id_ = id;
                str_ = str;
            }

        public:
            bool operator==(const ConstIterator& other) const noexcept {
                return id_ == other.id_ && str_ == other.str_;
            }

            bool operator!=(const ConstIterator& other) const noexcept {
                return id_ != other.id_ || str_ != other.str_;
            }

            std::strong_ordering operator<=>(const ConstIterator& other) const noexcept {
                return id_ <=> other.id_;
            }

            ConstIterator& operator+=(int64_t shift) noexcept {
                id_ += shift;
                return *this;
            }

            ConstIterator& operator-=(int64_t shift) noexcept {
                id_ -= shift;
                return *this;
            }

            const char& operator*() const {
                return (*str_)[id_];
            }

            ConstIterator& operator++() noexcept {
                id_++;
                return *this;
            }

            ConstIterator operator++(int) noexcept {
                ConstIterator result = *this;
                id_++;
                return result;
            }

            ConstIterator& operator--() noexcept {
                id_--;
                return *this;
            }

            ConstIterator operator--(int) noexcept {
                ConstIterator result = *this;
                id_--;
                return result;
            }

            ConstIterator operator+(int64_t shift) const noexcept {
                return ConstIterator(id_ + shift, str_);
            }

            ConstIterator operator-(int64_t shift) const noexcept {
                return ConstIterator(id_ - shift, str_);
            }

            int64_t operator-(ConstIterator other) const noexcept {
                return id_ - other.id_;
            }
        };

        HashString() {
            hash_.push_back(0);
        }

        explicit HashString(size_t size, char symbol = '\0') {
            update_hash(std::string(size, symbol));
        }

        explicit HashString(const std::string& str) {
            update_hash(str);
        }

        HashString<BASE, MODUL>& operator=(const std::string& other)& {
            update_hash(other);
            return *this;
        }

        char operator[](size_t index) const {
            if (index >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator[], invalid index.\n\n");
            }

            uint64_t hash = hash_[index + 1] + (MODUL - (hash_[index] * BASE) % MODUL) - 1;
            return static_cast<char>(hash < MODUL ? hash : hash - MODUL);
        }

        SubString<BASE, MODUL> operator()(size_t left_id, size_t right_id) const {
            if (left_id > right_id) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "operator(), invalid range.\n\n");
            }

            if (right_id >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "operator(), invalid range size.\n\n");
            }

            return SubString<BASE, MODUL>(left_id, right_id, this);
        }

        bool operator==(const HashString<BASE, MODUL>& other) const noexcept {
            return get_hash() == other.get_hash();
        }

        bool operator!=(const HashString<BASE, MODUL>& other) const noexcept {
            return get_hash() != other.get_hash();
        }

        std::strong_ordering operator<=>(const HashString<BASE, MODUL>& other) const noexcept {
            if (empty() && other.empty()) {
                return std::strong_ordering::equal;
            }
            if (empty()) {
                return std::strong_ordering::less;
            }
            if (other.empty()) {
                return std::strong_ordering::greater;
            }
            return SubString<BASE, MODUL>(0, size() - 1, this) <=> SubString<BASE, MODUL>(0, other.size() - 1, &other);
        }

        size_t operator&(const HashString<BASE, MODUL>& other) const noexcept {
            if (empty() || other.empty()) {
                return 0;
            }
            return SubString<BASE, MODUL>(0, size() - 1, this)& SubString<BASE, MODUL>(0, other.size() - 1, &other);
        }

        uint64_t get_hash(size_t left_id, size_t right_id) const {
            if (left_id > right_id) {
                throw func::AlgInvalidArgument(__FILE__, __LINE__, "get_hash, invalid range.\n\n");
            }

            if (right_id >= size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_hash, invalid range size.\n\n");
            }

            uint64_t hash = hash_[right_id + 1] + MODUL - (hash_[left_id] * power_[right_id - left_id + 1]) % MODUL;
            return hash < MODUL ? hash : hash - MODUL;
        }

        uint64_t get_hash() const noexcept {
            return hash_.back() % MODUL;
        }

        std::string get_string() const {
            std::string str;
            str.reserve(size());
            for (size_t i = 0; i < size(); ++i) {
                str.push_back((*this)[i]);
            }
            return str;
        }

        uint64_t get_power(size_t degree) const {
            if (degree >= power_.size()) {
                throw func::AlgOutOfRange(__FILE__, __LINE__, "get_power, invalid degree.");
            }

            return power_[degree];
        }

        size_t size() const noexcept {
            return hash_.size() - 1;
        }

        bool empty() const noexcept {
            return hash_.size() == 1;
        }

        char front() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "front, called from empty string.\n\n");
            }

            return (*this)[0];
        }

        char back() const {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "back, called from empty string.\n\n");
            }

            return (*this)[size() - 1];
        }

        ConstIterator begin() const noexcept {
            return ConstIterator(0, this);
        }

        ConstIterator end() const noexcept {
            return ConstIterator(size(), this);
        }

        void swap(HashString<BASE, MODUL>& other) noexcept {
            hash_.swap(other.hash_);
        }

        void reserve(size_t size) {
            hash_.reserve(size + 1);
            update_power(size + 1);
        }

        void push_back(char symbol) {
            hash_.push_back((hash_.back() * BASE + static_cast<uint64_t>(symbol) + 1) % MODUL);
            update_power(hash_.size());
        }

        void pop_back() {
            if (empty()) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "pop_back, attempt to pop from empty string.\n\n");
            }

            hash_.pop_back();
        }

        void clear() {
            hash_.clear();
            hash_.push_back(0);
        }
    };

    template <uint64_t BASE, uint64_t MODUL>
    std::istream& operator>>(std::istream& fin, HashString<BASE, MODUL>& hash_str) {
        std::string str;
        fin >> str;
        hash_str = str;
        return fin;
    }

    template <uint64_t BASE, uint64_t MODUL>
    std::ostream& operator<<(std::ostream& fout, const HashString<BASE, MODUL>& hash_str) {
        fout << hash_str.get_string();
        return fout;
    }


    // ^^^ ---------HashString-------- ^^^
    // -----------------------------------
}   // HashString | Version: 1.0