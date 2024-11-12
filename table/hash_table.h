#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <regex>
#include <tuple>

#include "container.h"
#include "storage.h"

using namespace std;

namespace FKG {
template <class Key, class Value>
class HashTable : virtual public AbstractContainer<Key, Value> {
public:
    HashTable();
    HashTable(HashTable &other);
    HashTable(HashTable &&other);
    ~HashTable();

    virtual const bool Set(const Key &key, const Value &value) override final;
    [[nodiscard]] virtual optional<Value> Get(const Key &key) override final;
    [[nodiscard]] virtual const bool Exists(const Key &key) override final;
    virtual const bool Del(const Key &key) override final;
    virtual const bool Update(const Key &key, const Value &value) override final;
    [[nodiscard]] virtual vector<Key> Keys() override final;
    virtual const bool Rename(const Key &key, const Key &rename) override final;
    [[nodiscard]] virtual const optional<size_t> TTL(const Key &key) override final;
    [[nodiscard]] virtual vector<Key> Find(const Value &value) override final;
    virtual void Showall() override final;
    virtual const size_t Upload(ifstream &is) override final;
    virtual const size_t Export(ofstream &os) override final;

    HashTable &operator=(HashTable &other);
    HashTable &operator=(HashTable &&other);

private:
    size_t m_mpn_cur_idx;
    static const vector<size_t> m_mpn;

    vector<list<shared_ptr<pair<Key, Value> > > > m_data;

    void ResizeHashTable();
    [[nodiscard]] tuple<typename list<shared_ptr<pair<Key, Value> > >::iterator,size_t> GetListIteratorByKey(const Key &key);
};

    #include "hash_table.inc"
}

#endif