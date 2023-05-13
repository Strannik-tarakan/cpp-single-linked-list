#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <iterator>
#include <algorithm>


template <typename Type>
class SingleLinkedList {

    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {

        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) {
            node_ = node;
        }

    public:

        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            this->node_ = other.node_;
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(this->node_ == rhs.node_);
        }
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(this->node_ == rhs.node_);
        }

        BasicIterator& operator++() noexcept {
            assert(node_);
            node_ = node_->next_node;
            return *this;
        }
        BasicIterator operator++(int) noexcept {

            auto old_node(*this);
            node_ = node_->next_node;
            return old_node;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_);
            return this->node_->value;
        }
        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_);
            return &(node_->value);
        }
    private:
        Node* node_ = nullptr;
    };
public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return BasicIterator<Type>(head_.next_node);
    }
    [[nodiscard]] Iterator end() noexcept {
        return {};
    }
    [[nodiscard]] ConstIterator begin() const noexcept {
        return BasicIterator<Type>(head_.next_node);
    }
    [[nodiscard]] ConstIterator end() const noexcept {
        return {};
    }
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return BasicIterator<Type>(head_.next_node);
    }
    [[nodiscard]] ConstIterator cend() const noexcept {
        return {};
    }
    [[nodiscard]] Iterator before_begin() noexcept {
        return BasicIterator<Type>(&head_);
    }
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<SingleLinkedList<Type>::Node*>(&head_));
    }
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return BasicIterator<Type>(&head_);
    }

    SingleLinkedList() {

    }
    SingleLinkedList(std::initializer_list<Type> values) {
        CopyObject(values.begin(), values.end());
    }
    SingleLinkedList(const SingleLinkedList& other) {
        CopyObject(other.begin(), other.end());
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this->head_.next_node == rhs.head_.next_node && this->size_ == rhs.size_) {
            return *this;
        }
        SingleLinkedList tmp(rhs);
        swap(tmp);
        return *this;
    }

    ~SingleLinkedList() {
        Clear();
    }

    [[nodiscard]] size_t GetSize() const noexcept {

        return size_;
    }
    [[nodiscard]] bool IsEmpty() const noexcept {

        bool x = size_ == 0 ? true : false;
        return x;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    void PopFront() noexcept {
        assert(head_.next_node);
        Node* old_node = head_.next_node;
        head_.next_node = old_node->next_node;
        delete old_node;
        --size_;
    }
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_);
        Node* new_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = new_node;
        ++size_;
        return BasicIterator<Type>(new_node);
    }
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ && pos.node_->next_node);
        Node* old_node = pos.node_->next_node;
        pos.node_->next_node = old_node->next_node;
        delete old_node;
        --size_;
        return BasicIterator<Type>(pos.node_->next_node);
    }

    void Clear() noexcept {
        if (size_ == 0) {
            return;
        }
        Node* next_node = head_.next_node;
        for (int i = 0; i < static_cast<int>(size_); ++i) {
            Node* node = next_node;
            next_node = node->next_node;
            delete node;
        }
        head_.next_node = nullptr;
        size_ = 0;
    }

    void swap(SingleLinkedList& other) noexcept {
        int size_this = size_;
        auto head_this = head_.next_node;
        size_ = other.size_;
        head_.next_node = other.head_.next_node;
        other.head_.next_node = head_this;
        other.size_ = size_this;
    }


private:
    Node head_;
    size_t size_ = 0;

    template<typename Iterator>
    void CopyObject(Iterator begin, Iterator end, bool second_call = false) {
        SingleLinkedList tmp;
        while (begin != end) {
            tmp.PushFront(*begin);
            ++begin;
        }
        if (second_call) {
            swap(tmp);
        }
        else {
            CopyObject(tmp.begin(), tmp.end(), true);
        }
    }
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}
template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}
template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}
template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}
