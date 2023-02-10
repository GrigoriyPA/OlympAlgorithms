#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <list>
#include <stdexcept>
#include <vector>


template <typename KeyType, typename ValueType, typename Hash = std::hash<KeyType>>
class HashMap {
    using Accordance = std::pair<const KeyType, ValueType>;

    struct TableElement {
        size_t probe_sequence_length = 0;
        bool exists = false;

        typename std::list<Accordance>::iterator accordance_it;

        TableElement() noexcept {
        }

        TableElement(typename std::list<Accordance>::iterator accordance_it) noexcept {
            this->accordance_it = accordance_it;
            exists = true;
        }

        void swap(TableElement& other) noexcept {
            std::swap(accordance_it, other.accordance_it);
            std::swap(probe_sequence_length, other.probe_sequence_length);
            std::swap(exists, other.exists);
        }
    };

    inline static const double LOAD_FACTOR = 0.75;

    std::list<Accordance> accordances_;
    std::vector<TableElement> table_;
    Hash hasher_;

    size_t hash_key(const KeyType& key) const {
        return hasher_(key) % table_.size();
    }

    size_t find_key(const KeyType& key) const {
        if (empty()) {
            return table_.size();
        }

        for (size_t id = hash_key(key); table_[id].exists; id = id + 1 < table_.size() ? id + 1 : 0) {
            if (table_[id].accordance_it->first == key) {
                return id;
            }
        }
        return table_.size();
    }

    size_t insert_accordance(typename std::list<Accordance>::iterator accordance_it) {
        size_t result_id = table_.size();
        const KeyType& key = accordance_it->first;
        TableElement new_element(accordance_it);
        for (size_t id = hash_key(key); true; id = id + 1 < table_.size() ? id + 1 : 0) {
            TableElement& current_element = table_[id];
            if (!current_element.exists) {
                current_element.swap(new_element);
                return result_id == table_.size() ? id : result_id;
            }

            if (current_element.accordance_it->first == key) {
                accordances_.erase(accordance_it);
                return id;
            }

            if (new_element.probe_sequence_length > current_element.probe_sequence_length) {
                current_element.swap(new_element);
                if (result_id == table_.size()) {
                    result_id = id;
                }
            }

            ++new_element.probe_sequence_length;
        }
        return table_.size();
    }

    size_t insert_accordance(const KeyType& key, const ValueType& value) {
        reserve(accordances_.size() + 1);
        accordances_.push_front({ key, value });
        return insert_accordance(accordances_.begin());
    }

    void erase_key(const KeyType& key) {
        size_t key_id = find_key(key);
        if (key_id == table_.size()) {
            return;
        }

        accordances_.erase(table_[key_id].accordance_it);
        table_[key_id].exists = false;
        for (size_t id = key_id + 1 < table_.size() ? key_id + 1 : 0; table_[id].exists && table_[id].probe_sequence_length > 0; key_id = id, id = id + 1 < table_.size() ? id + 1 : 0) {
            table_[id].swap(table_[key_id]);
            --table_[key_id].probe_sequence_length;
        }
    }

    void resize(size_t new_size) {
        for (TableElement& element : table_) {
            element.exists = false;
        }

        table_.resize(new_size);
        for (auto it = accordances_.begin(); it != accordances_.end(); ++it) {
            insert_accordance(it);
        }
    }

public:
    using iterator = typename std::list<Accordance>::iterator;
    using const_iterator = typename std::list<Accordance>::const_iterator;

    HashMap() {
        hasher_ = std::hash<KeyType>();
    }

    explicit HashMap(const Hash& hasher) : hasher_(hasher) {
    }

    template <typename It>
    HashMap(It begin, It end, const Hash& hasher = Hash()) : hasher_(hasher) {
        for (; begin != end; ++begin) {
            insert(*begin);
        }
    }

    HashMap(const std::initializer_list<std::pair<KeyType, ValueType>>& init, const Hash& hasher = Hash()) : hasher_(hasher) {
        reserve(init.size());
        for (const auto& [key, value] : init) {
            insert_accordance(key, value);
        }
    }

    HashMap& operator=(const HashMap& other)& {
        std::list<Accordance> accordances_copy(other.accordances_);
        accordances_.swap(accordances_copy);

        table_ = other.table_;
        return *this;
    }

    ValueType& operator[](const KeyType& key) {
        size_t key_id = insert_accordance(key, ValueType());
        return table_[key_id].accordance_it->second;
    }

    const ValueType& at(const KeyType& key) const {
        size_t key_id = find_key(key);
        if (key_id == table_.size()) {
            throw std::out_of_range("Invalid key in function at.");
        }

        return table_[key_id].accordance_it->second;
    }

    iterator find(const KeyType& key) {
        size_t key_id = find_key(key);
        if (key_id == table_.size()) {
            return end();
        }

        return table_[key_id].accordance_it;
    }

    const_iterator find(const KeyType& key) const {
        size_t key_id = find_key(key);
        if (key_id == table_.size()) {
            return end();
        }

        return table_[key_id].accordance_it;
    }

    iterator begin() noexcept {
        return accordances_.begin();
    }

    const_iterator begin() const noexcept {
        return accordances_.begin();
    }

    iterator end() noexcept {
        return accordances_.end();
    }

    const_iterator end() const noexcept {
        return accordances_.end();
    }

    Hash hash_function() const noexcept {
        return hasher_;
    }

    size_t size() const noexcept {
        return accordances_.size();
    }

    bool empty() const noexcept {
        return accordances_.empty();
    }

    void insert(const std::pair<KeyType, ValueType>& accordance) {
        insert_accordance(accordance.first, accordance.second);
    }

    void erase(const KeyType& key) {
        erase_key(key);
    }

    void reserve(size_t new_number_elements) {
        size_t new_size = static_cast<size_t>(std::ceil(static_cast<double>(new_number_elements) / LOAD_FACTOR));
        if (new_size <= table_.size()) {
            return;
        }

        resize(std::max(2 * table_.size(), new_size));
    }

    void clear() {
        for (; !empty();) {
            erase_key(accordances_.begin()->first);
        }
    }
};

// | Version: ?
