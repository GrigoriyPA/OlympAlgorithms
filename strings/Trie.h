namespace alg::str {
    // -----------------------------------
    // vvv -----------Trie------------ vvv


    struct TrieNode {
    private:
        friend class Trie;

        size_t terminal_count_ = 0;

        std::unordered_map<char, TrieNode*> next_;

    public:
        using Iterator = typename std::unordered_map<char, TrieNode*>::iterator;
        using ConstIterator = typename std::unordered_map<char, TrieNode*>::const_iterator;

        size_t get_terminal_count() const noexcept {
            return terminal_count_;
        }

        TrieNode* get_next(char symbol) const noexcept {
            if (auto it = next_.find(symbol); it != next_.end()) {
                return it->second;
            }
            return nullptr;
        }

        TrieNode* add_next(char symbol) {
            if (auto it = next_.find(symbol); it != next_.end()) {
                return it->second;
            }

            next_[symbol] = new TrieNode();
            return next_[symbol];
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

        void erase(char symbol) {
            delete next_[symbol];
            next_.erase(symbol);
        }

        void clear() {
            for (auto& [symbol, node] : next_) {
                delete node;
                node = nullptr;
            }
        }

        bool contains(char symbol) const noexcept {
            return next_.contains(symbol);
        }

        bool empty() const noexcept {
            return next_.empty();
        }

        ~TrieNode() {
            for (auto& [symbol, node] : next_) {
                delete node;
                node = nullptr;
            }
        }
    };


    class Trie {
        TrieNode root_ = TrieNode();

        void interal_pushing_string_dfs(TrieNode* node, size_t index, const std::string& str) {
            if (index == str.size()) {
                ++node->terminal_count_;
                return;
            }

            interal_pushing_string_dfs(node->add_next(str[index]), index + 1, str);
        }

        bool interal_erasing_string_dfs(TrieNode* node, size_t index, const std::string& str) {
            if (index == str.size()) {
                if (node->terminal_count_ == 0) {
                    throw func::AlgRuntimeError(__FILE__, __LINE__, "erase_string, the string does not exist.\n\n");
                }

                --node->terminal_count_;
                return node->terminal_count_ == 0 && node->empty();
            }

            TrieNode* next = node->get_next(str[index]);
            if (next == nullptr) {
                throw func::AlgRuntimeError(__FILE__, __LINE__, "erase_string, the string does not exist.\n\n");
            }

            bool del = interal_erasing_string_dfs(next, index + 1, str);
            if (del) {
                node->erase(str[index]);
            }

            return del && node->terminal_count_ == 0 && node->empty();
        }

    public:
        Trie() noexcept {
        }

        void insert_string(const std::string& str) {
            interal_pushing_string_dfs(&root_, 0, str);
        }

        void erase_string(const std::string& str) {
            interal_erasing_string_dfs(&root_, 0, str);
        }

        TrieNode* get_root() {
            return &root_;
        }
    };
    

    // ^^^ -----------Trie------------ ^^^
    // -----------------------------------
}   // Functions | Version: 0.0

using namespace alg::str;