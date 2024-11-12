#include "hash_table.h"
using namespace std;

namespace FKG {
template <>
const size_t HashTable<string, Storage>::Upload(ifstream &is) {
    size_t loaded{0};

    Storage storage;
    istringstream stream;
    static const regex regex(R"(^\w{1,15}\s+\S+\s+\S+\s+\d{4}\s+\S+\s+\d+(\s+)?$)");
    for (string line, key; getline(is, line); stream.clear()) {
        if (stream.str(line), regex_match(line, regex)) {
            stream >> key;
            stream >> storage;
            loaded += Set(key, storage);
        }
    }
    this->m_amount_elements += loaded;
    return loaded;
}

template <>
const size_t HashTable<string, Storage>::Export(ofstream &os) {
    size_t unloaded{0};
    for (auto &list : m_data) {
        for (auto &item : list) {
            os << item->first << " " << item->second;
            if (++unloaded != this->m_amount_elements) {
                os << '\n';
            }
        }
    }
    return unloaded;
}

} 