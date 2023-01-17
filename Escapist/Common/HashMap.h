//
// Created by Escap on 1/11/2023.
//

#ifndef ESCAPIST_HASHMAP_H
#define ESCAPIST_HASHMAP_H

#include "../General.h"

template<typename Key, typename Value>
class HashMap {
private:
    struct Node {
        Node *next_;
        Key key_;
        Value value_;
    };
    using HashCodeFunc = SizeType(*)(const Key &);

    SizeType width_;
    SizeType count_;
    Node **head_;
    HashCodeFunc hashCode_;

public:
    HashMap(SizeType width, HashCodeFunc hashCode) : width_(width), count_(0), hashCode_(hashCode) {
        assert(width_);
        assert(hashCode_);
        head_ = (Node **) ::malloc(width_ * sizeof(Node *));
        ::memset(head_, 0, width_ * sizeof(Node *));
        assert(head_);
    }

    ~HashMap() {
        for (SizeType index = 0; index < width_; ++index) {
            Node *target = head_[index];
            Node *old;
            while (target != nullptr) {
                old = target;
                target = target->next_;
                delete old;
            }
        }
        ::free((void *) head_);
    }

    SizeType GetCount() const noexcept {
        return count_;
    }

    HashMap<Key, Value> &Put(const Key &key, const Value &value) noexcept {
        assert(head_);
        SizeType index = hashCode_(key) & (width_ - 1);
        Node *target = head_[index];
        Node *prev = nullptr;
        while (target && target->key_ == key) {
            prev = target;
            target = target->next_;
        }
        if (target) {
            target->value_ = value;
        } else {
            if (prev) {
                prev->next_ = new Node;
                prev->next_->next_ = nullptr;
                prev->next_->key_ = key;
                prev->next_->value_ = value;
            } else {
                head_[index] = new Node;
                head_[index]->next_ = nullptr;
                head_[index]->key_ = key;
                head_[index]->value_ = value;
            }
            ++count_;
        }
        return *this;
    }

    Value &GetValue(const Key &key) {
        assert(head_);
        SizeType index = hashCode_(key) & (width_ - 1);
        Node *target = head_[index];
        assert(target);
        while (target) {
            if (target->key_ == key) {
                return target->value_;
            }
            target = target->next_;
        }
        assert(false);
    }

    const Value &GetConstValue(const Key &key) const {
        assert(head_);
        SizeType index = hashCode_(key) & (width_ - 1);
        Node *target = head_[index];
        assert(target);
        while (target) {
            if (target->key_ == key) {
                return target->value_;
            }
            target = target->next_;
        }
        assert(false);
    }

    HashMap<Key, Value> &SetValue(const Key &key, const Value &value) {
        assert(head_);
        SizeType index = hashCode_(key) & (width_ - 1);
        Node *target = head_[index];
        assert(target);
        while (target) {
            if (target->key_ == key) {
                target->value_ = value;
                return *this;
            }
            target = target->next_;
        }
        assert(false);
    }

    HashMap<Key, Value> &RemoveValue(const Key &key) {
        assert(head_);
        SizeType index = hashCode_(key) & (width_ - 1);
        Node *target = head_[index];
        Node *prev = nullptr;
        assert(target);
        while (target) {
            if (target->key_ == key) {
                Node *next = target->next_;
                delete target;
                if (prev) {
                    prev->next_ = next;
                } else {
                    head_[index] = next;
                }
                --count_;
                return *this;
            }
            prev = target;
            target = target->next_;
        }
        return *this;
    }
};

#endif //ESCAPIST_HASHMAP_H
