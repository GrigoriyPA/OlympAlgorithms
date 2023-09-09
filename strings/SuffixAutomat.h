namespace alg::str {
    // -----------------------------------
    // vvv -------SuffixAutomat------- vvv


    struct SuffixAutomatNode {
    private:
        friend class SuffixAutomat;

        size_t id_ = 0;
        size_t len_ = 0;
        size_t ref_count_ = 0;
        SuffixAutomatNode* link_ = nullptr;

        std::unordered_map<char, SuffixAutomatNode*> next_;

        void delete_node(SuffixAutomatNode*& node) {
            if (node == nullptr) {
                return;
            }

            --node->ref_count_;
            if (node->ref_count_ == 0) {
                delete node;
            }
            node = nullptr;
        }

        SuffixAutomatNode(size_t node_id) {
            id_ = node_id;
        }

    public:
        using Iterator = typename std::unordered_map<char, SuffixAutomatNode*>::iterator;
        using ConstIterator = typename std::unordered_map<char, SuffixAutomatNode*>::const_iterator;

        size_t get_id() noexcept {
            return id_;
        }

        size_t get_len() const noexcept {
            return len_;
        }

        SuffixAutomatNode* get_link() const noexcept {
            return link_;
        }

        SuffixAutomatNode* get_next(char symbol) const noexcept {
            if (auto it = next_.find(symbol); it != next_.end()) {
                return it->second;
            }
            return nullptr;
        }

        Iterator begin() noexcept {
            return next_.begin();
        }

        ConstIterator begin() const noexcept {
            return next_.begin();
        }

        Iterator end() noexcept {
            return next_.end();
        }

        ConstIterator end() const noexcept {
            return next_.end();
        }

        void insert(char symbol, SuffixAutomatNode* node) {
            erase(symbol);
            
            if (node != nullptr) {
                ++node->ref_count_;
                next_[symbol] = node;
            }
        }

        void erase(char symbol) {
            delete_node(next_[symbol]);
            next_.erase(symbol);
        }

        void clear() {
            for (auto& [symbol, node] : next_) {
                delete_node(node);
            }
        }

        bool contains(char symbol) const noexcept {
            return next_.contains(symbol);
        }

        bool empty() const noexcept {
            return next_.empty();
        }

        ~SuffixAutomatNode() {
            for (auto& [symbol, node] : next_) {
                delete_node(node);
            }
        }
    };


    class SuffixAutomat {
        SuffixAutomatNode* start_ = new SuffixAutomatNode(0);
        SuffixAutomatNode* last_ = start_;
        size_t size_ = 1;

        std::vector<SuffixAutomatNode*> nodes_;
        std::unordered_map<char, size_t> char_map;

        void build(const std::string& str) {
            start_->clear();
            for (char symbol : str) {
                push_back(symbol);
            }
        }

    public:
        SuffixAutomat() noexcept {
            nodes_.push_back(start_);
        }

        SuffixAutomat(const std::string& str) noexcept {
            nodes_.push_back(start_);
            build(str);
        }

        SuffixAutomat& operator=(const std::string& other)& {
            build(other);
            return *this;
        }

        SuffixAutomatNode* get_start() noexcept {
            return start_;
        }

        void push_back(char symbol) {
            if (!char_map.contains(symbol)) {
                char_map[symbol] = char_map.size() - 1;
            }

            SuffixAutomatNode* cur = new SuffixAutomatNode(size_++);
            nodes_.push_back(cur);
            cur->len_ = last_->get_len() + 1;
            
            SuffixAutomatNode* p = last_;
            for (; p != nullptr && !p->contains(symbol); p = p->get_link()) {
                p->insert(symbol, cur);
            }

            if (p == nullptr) {
                cur->link_ = start_;
                last_ = cur;
                return;
            }

            SuffixAutomatNode* q = p->get_next(symbol);

            if (p->get_len() + 1 == q->get_len()) {
                cur->link_ = q;
                last_ = cur;
                return;
            }

            SuffixAutomatNode* clone = new SuffixAutomatNode(size_++);
            nodes_.push_back(clone);
            clone->len_ = p->get_len() + 1;
            clone->link_ = q->link_;
            for (auto [symbol, node] : *q) {
                clone->insert(symbol, node);
            }

            cur->link_ = clone;
            q->link_ = clone;

            for (; p != nullptr && p->get_next(symbol) == q; p = p->get_link()) {
                p->erase(symbol);
                p->insert(symbol, clone);
            }

            last_ = cur;
        }

        SuffixAutomatNode* apply_string(const std::string& str) {
            SuffixAutomatNode* result = start_;
            for (char symbol : str) {
                result = result->get_next(symbol);

                if (result == nullptr) {
                    return nullptr;
                }
            }

            return result;
        }

        void build_automat(int& start, std::vector<int>& terminals, std::vector<std::vector<int>>& transitions) {
            start = 0;

            terminals.clear();
            for (SuffixAutomatNode* vertex = last_; vertex != nullptr; vertex = vertex->get_link()) {
                terminals.push_back(vertex->get_id());
            }

            transitions.clear();
            transitions.resize(size_, std::vector<int>(char_map.size(), -1));
            for (size_t i = 0; i < size_; ++i) {
                for (auto [symbol, to] : *nodes_[i]) {
                    transitions[i][char_map[symbol]] = to->get_id();
                }
            }

            bool is_created = false;
            for (size_t i = 0; i < size_; ++i) {
                for (size_t j = 0; j < char_map.size(); ++j) {
                    if (transitions[i][j] != -1) {
                        continue;
                    }

                    if (!is_created) {
                        transitions.push_back(std::vector<int>(char_map.size(), transitions.size()));
                        is_created = true;
                    }

                    transitions[i][j] = transitions.size() - 1;
                }
            }
        }

        size_t size() const noexcept {
            return size_;
        }

        ~SuffixAutomat() {
            delete start_;
        }
    };


    // ^^^ -------SuffixAutomat------- ^^^
    // -----------------------------------
}   // Functions | Version: 0.0

using namespace alg::str;