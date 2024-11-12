#ifndef CONTAINER_H
#define CONTAINER_H

#include <fstream>
#include <optional>
#include <utility>
#include <vector>

using namespace std;

namespace FKG{
template <class Key, class Value>
class AbstractContainer {
public:
    virtual ~AbstractContainer() = 0;

    [[nodiscard]] virtual const size_t GetAmountElements() const final;

    virtual const bool Set(const Key &key, const Value &value) = 0;
    [[nodiscard]] virtual optional<Value> Get(const Key &key) = 0;
    [[nodiscard]] virtual const bool Exists(const Key &key) = 0;
    virtual const bool Del(const Key &key) = 0;
    virtual const bool Update(const Key &key, const Value &value) = 0;
    [[nodiscard]] virtual vector<Key> Keys() = 0;
    virtual const bool Rename(const Key &key, const Key &rename) = 0;
    [[nodiscard]] virtual const optional<size_t> TTL(const Key &key) = 0;
    [[nodiscard]] virtual vector<Key> Find(const Value &value) = 0;
    virtual void Showall() = 0;
    virtual const size_t Upload(ifstream &is) = 0;
    virtual const size_t Export(ofstream &os) = 0;

protected:
    size_t m_amount_elements{};
};

#include "container.inc"
}

#endif 