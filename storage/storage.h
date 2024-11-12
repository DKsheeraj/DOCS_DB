#ifndef STORAGE_H
#define STORAGE_H

#include <chrono>
#include <iomanip>
#include <optional>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

using namespace std;

namespace FKG {
class Storage {
public:
    optional<string> surname;
    optional<string> name;
    optional<size_t> year_of_birth;
    optional<string> city;
    optional<size_t> coins;

    friend ostream &operator<<(ostream &os, const Storage &storage);
    friend istringstream &operator>>(istringstream &stream, Storage &storage);

    Storage() = default;
    Storage(const Storage &other);
    ~Storage() = default;

    void SetRemovalTime(const chrono::seconds seconds);
    [[nodiscard]] const optional<chrono::steady_clock::time_point> GetRemovalTime() const;

    Storage &operator=(const Storage &other);
    [[nodiscard]] bool operator==(const Storage &other) const;

    [[nodiscard]] bool operator!=(const Storage &other) const;
    [[nodiscard]] bool operator<(const Storage &other) const;
    [[nodiscard]] bool operator>(const Storage &other) const;
    [[nodiscard]] bool operator>=(const Storage &other) const;
    [[nodiscard]] bool operator<=(const Storage &other) const;

private:
    optional<chrono::steady_clock::time_point> m_removal_time;

    template <class T>
    [[nodiscard]] inline bool Equal(const T &lhs, const T &rhs) const;
    template <class T>
    [[nodiscard]] inline bool NotEqual(const T &lhs, const T &rhs) const;
    template <class T>
    [[nodiscard]] inline bool IsLess(const T &lhs, const T &rhs) const;
    template <class T>
    [[nodiscard]] inline bool IsGreater(const T &lhs, const T &rhs) const;
    template <class T>
    [[nodiscard]] inline bool IsLessOrEqual(const T &lhs, const T &rhs) const;
    template <class T>
    [[nodiscard]] inline bool IsGreaterOrEqual(const T &lhs, const T &rhs) const;

    template <class T>
    inline void WriteValue(ostream &os, const T &src, const size_t width) const;
};

    ostream &operator<<(ostream &os, const Storage &storage);
    istringstream &operator>>(istringstream &stream, Storage &storage);

} 

#endif  