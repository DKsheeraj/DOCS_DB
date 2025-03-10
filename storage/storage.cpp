#include "storage.h"
using namespace std;
namespace FKG {

Storage::Storage(const Storage &other) { *this = other; }

Storage &Storage::operator=(const Storage &other) {
    if (this != &other) {
        if (other.surname.has_value()) {
            this->surname = other.surname;
        }
        if (other.name.has_value()) {
            this->name = other.name;
        }
        if (other.year_of_birth.has_value()) {
            this->year_of_birth = other.year_of_birth;
        }
        if (other.city.has_value()) {
            this->city = other.city;
        }
        if (other.coins.has_value()) {
            this->coins = other.coins;
        }
        if (other.m_removal_time.has_value()) {
            this->m_removal_time = other.m_removal_time;
        }
    }
    return *this;
}

[[nodiscard]] bool Storage::operator==(const Storage &other) const {
    if (!this->Equal(this->surname, other.surname) ||
        !this->Equal(this->name, other.name) ||
        !this->Equal(this->year_of_birth, other.year_of_birth) ||
        !this->Equal(this->city, other.city) ||
        !this->Equal(this->coins, other.coins)) {
        return false;
    }
    return true;
}

template <class T>
[[nodiscard]] inline bool Storage::Equal(const T &lhs, const T &rhs) const {
    return !rhs || (lhs == rhs);
}

[[nodiscard]] bool Storage::operator!=(const Storage &other) const {
    if (!this->NotEqual(this->surname, other.surname) ||
        !this->NotEqual(this->name, other.name) ||
        !this->NotEqual(this->year_of_birth, other.year_of_birth) ||
        !this->NotEqual(this->city, other.city) ||
        !this->NotEqual(this->coins, other.coins)) {
        return false;
    }
    return true;
}

template <class T>
[[nodiscard]] inline bool Storage::NotEqual(const T &lhs, const T &rhs) const {
    return !rhs || (lhs != rhs);
}

[[nodiscard]] bool Storage::operator<(const Storage &other) const {
    if (!this->IsLess(this->surname, other.surname) ||
        !this->IsLess(this->name, other.name) ||
        !this->IsLess(this->year_of_birth, other.year_of_birth) ||
        !this->IsLess(this->city, other.city) ||
        !this->IsLess(this->coins, other.coins)) {
        return false;
    }
    return true;
}

template <class T>
[[nodiscard]] inline bool Storage::IsLess(const T &lhs, const T &rhs) const {
    return !rhs || (lhs < rhs);
}

[[nodiscard]] bool Storage::operator>(const Storage &other) const {
    if (!this->IsGreater(this->surname, other.surname) ||
        !this->IsGreater(this->name, other.name) ||
        !this->IsGreater(this->year_of_birth, other.year_of_birth) ||
        !this->IsGreater(this->city, other.city) ||
        !this->IsGreater(this->coins, other.coins)) {
        return false;
    }
    return true;
}

template <class T>
[[nodiscard]] inline bool Storage::IsGreater(const T &lhs, const T &rhs) const {
    return !rhs || (lhs > rhs);
}

[[nodiscard]] bool Storage::operator<=(const Storage &other) const {
    if (!this->IsLessOrEqual(this->surname, other.surname) ||
        !this->IsLessOrEqual(this->name, other.name) ||
        !this->IsLessOrEqual(this->year_of_birth, other.year_of_birth) ||
        !this->IsLessOrEqual(this->city, other.city) ||
        !this->IsLessOrEqual(this->coins, other.coins)) {
        return false;
    }
    return true;
}

template <class T>
[[nodiscard]] inline bool Storage::IsLessOrEqual(const T &lhs, const T &rhs) const {
    return !rhs || (lhs <= rhs);
}

[[nodiscard]] bool Storage::operator>=(const Storage &other) const {
    if (!this->IsGreaterOrEqual(this->surname, other.surname) ||
        !this->IsGreaterOrEqual(this->name, other.name) ||
        !this->IsGreaterOrEqual(this->year_of_birth, other.year_of_birth) ||
        !this->IsGreaterOrEqual(this->city, other.city) ||
        !this->IsGreaterOrEqual(this->coins, other.coins)) {
        return false;
    }
    return true;
}

template <class T>
[[nodiscard]] inline bool Storage::IsGreaterOrEqual(const T &lhs, const T &rhs) const {
    return !rhs || (lhs >= rhs);
}

void Storage::SetRemovalTime(const chrono::seconds seconds) {
    m_removal_time = chrono::steady_clock::now() + seconds;
}

[[nodiscard]] const optional<chrono::steady_clock::time_point>
Storage::GetRemovalTime() const {
    return m_removal_time;
}

template <class T>
inline void Storage::WriteValue(ostream &os, const T &src, const size_t width) const {
    os << setw(width);
    if (src.has_value()) {
        os << src.value();
    } else {
        os << "-";
    }
}

ostream &operator<<(ostream &os, const Storage &storage) {
    os << left;
    storage.WriteValue(os, storage.surname, static_cast<size_t>(20));
    storage.WriteValue(os, storage.name, static_cast<size_t>(20));
    storage.WriteValue(os, storage.year_of_birth, static_cast<size_t>(12));
    storage.WriteValue(os, storage.city, static_cast<size_t>(30));
    storage.WriteValue(os, storage.coins, static_cast<size_t>(10));
    return os;
}

istringstream &operator>>(istringstream &stream, Storage &storage) {
    static string surname, name, city, year_of_birth, coins;

    stream >> surname;
    stream >> name;
    stream >> year_of_birth;
    stream >> city;
    stream >> coins;

    if (year_of_birth != "-") storage.year_of_birth = stoull(year_of_birth);
    if (coins != "-") storage.coins = stoull(coins);
    if (surname != "-") storage.surname = surname;
    if (name != "-") storage.name = name;
    if (city != "-") storage.city = city;

    return stream;
}

}