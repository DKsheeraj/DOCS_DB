#ifndef INTERFACE_H
#define INTERFACE_H

#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <mutex>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>

#include "hash_table.h"

using namespace std;

class Interface {
public:
    Interface();
    ~Interface();

    void Exec();

private:
    enum MenuItem {
        kIntroduction = 0,
        kSelectContainer,
        kInput,
        kSuccess,
        kTrue,
        kFalse,
        kNull,
        kNotExist,
        kInvalid,
        kInfo,
        kStatistics,
        kCompletion
    };

    using fp_1arg = function<const bool(istringstream &)>;

    static const vector<string> m_menus;
    unordered_map<string, fp_1arg> m_funcs;

    unique_ptr<FKG::AbstractContainer<string, FKG::Storage>>m_container;

    map<chrono::steady_clock::time_point, string> m_ttl;
    vector<thread> m_threads;
    mutex m_mutex;

    const size_t CheckInput(const size_t count) const;
    const string CheckInput(istringstream &stream);

    /*nodiscard used so that caller doesnt ignore the return value*/

    [[nodiscard]] const bool Set(istringstream &stream);
    [[nodiscard]] const bool Get(istringstream &stream);
    [[nodiscard]] const bool Exists(istringstream &stream);
    [[nodiscard]] const bool Del(istringstream &stream);
    [[nodiscard]] const bool Update(istringstream &stream);
    [[nodiscard]] const bool Keys(istringstream &stream);
    [[nodiscard]] const bool Rename(istringstream &stream);
    [[nodiscard]] const bool TTL(istringstream &stream);
    [[nodiscard]] const bool Find(istringstream &stream);
    [[nodiscard]] const bool Showall(istringstream &stream);
    [[nodiscard]] const bool Upload(istringstream &stream);
    [[nodiscard]] const bool Export(istringstream &stream);
    [[nodiscard]] const bool Info(istringstream &stream);

    void SetTTL(const FKG::Storage &storage, const string &key);

    using vector_container = vector<unique_ptr<FKG::AbstractContainer<string, FKG::Storage>>>;
    using ms = chrono::milliseconds;
    using time_sc = chrono::steady_clock;

    void Statistics();
    int RandomInRange(int, int);
    [[nodiscard]] istringstream CreateRecord(int key);
    void FillRandonly(vector_container &, size_t);
    [[nodiscard]] vector<double> GetResults(vector_container &,size_t countRepeat);
    void TestGet(vector_container &, size_t, vector<double> &);
    void TestSet(vector_container &, size_t, vector<double> &);
    void TestDel(vector_container &, size_t, vector<double> &);
    void TestKeys(vector_container &, size_t, vector<double> &);
    void TestFind(vector_container &, size_t, vector<double> &);
};

#endif