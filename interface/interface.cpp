#include "interface.h"
using namespace std;

const vector<string> Interface::m_menus{
    " -------------------------------------------------------------- \n"
    "|                       DOCS_DB                                |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                 Selecting the main container.                |\n"
    " -------------------------------------------------------------- \n"
    "| 1. HASH_TABLE                                                |\n"
    " -------------------------------------------------------------- \n",
    " > ",
    " > OK\n",
    " > true\n",
    " > false\n",
    " > (null)\n",
    " > Not esixt path: ",
    " -------------------------------------------------------------- \n"
    "|                The written command is invalid                |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                             INFO                             |\n"
    " -------------------------------------------------------------- \n"
    "| 1. SET <key> <Last Name> <First Name> <Year of Birth> <City> |\n"
    "| <Current Number of Coins> EX <time in seconds>               |\n"
    "| 2. GET <key>                                                 |\n"
    "| 3. EXISTS <key>                                              |\n"
    "| 4. DEL <key>                                                 |\n"
    "| 5. UPDATE <key> <Last Name> <First Name> <Year of Birth>     |\n"
    "| <City> <Current Number of Coins>                             |\n"
    "| 6. KEYS <null>                                               |\n"
    "| 7. RENAME <key> <key>                                        |\n"
    "| 8. TTL <key>                                                 |\n"
    "| 9. FIND <Last Name> <First Name> <Year of Birth> <City>      |\n"
    "| <Current Number of Coins>                                    |\n"
    "| 10. SHOWALL <null>                                           |\n"
    "| 11. UPLOAD <path>                                            |\n"
    "| 12. EXPORT <path>                                            |\n"
    "| 13. INFO <null>                                              |\n"
    "| 14. EXIT <null>                                              |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                          Statistics                          |\n"
    " -------------------------------------------------------------- \n"
    "|   Operation                  |       HashTable               |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|            Successful completion of the programme            |\n"
    " -------------------------------------------------------------- \n"};

Interface::Interface() {
    cout << m_menus[MenuItem::kIntroduction];
    m_funcs.emplace("SET",bind(&Interface::Set, this, placeholders::_1));
    m_funcs.emplace("GET",bind(&Interface::Get, this, placeholders::_1));
    m_funcs.emplace("EXISTS",bind(&Interface::Exists, this, placeholders::_1));
    m_funcs.emplace("DEL",bind(&Interface::Del, this, placeholders::_1));
    m_funcs.emplace("UPDATE",bind(&Interface::Update, this, placeholders::_1));
    m_funcs.emplace("KEYS",bind(&Interface::Keys, this, placeholders::_1));
    m_funcs.emplace("RENAME",bind(&Interface::Rename, this, placeholders::_1));
    m_funcs.emplace("TTL",bind(&Interface::TTL, this, placeholders::_1));
    m_funcs.emplace("FIND",bind(&Interface::Find, this, placeholders::_1));
    m_funcs.emplace("SHOWALL",bind(&Interface::Showall, this, placeholders::_1));
    m_funcs.emplace("UPLOAD",bind(&Interface::Upload, this, placeholders::_1));
    m_funcs.emplace("EXPORT",bind(&Interface::Export, this, placeholders::_1));
    m_funcs.emplace("INFO",bind(&Interface::Info, this, placeholders::_1));
    m_funcs.emplace("EXIT",[](istringstream &) -> const bool { return false; });
}

Interface::~Interface() {
    m_ttl.clear();
    for (auto &it : m_threads) {
        it.join();
    }
    cout << m_menus[MenuItem::kCompletion];
}

void Interface::Exec() {
    cout << m_menus[MenuItem::kSelectContainer];
    size_t select = CheckInput(static_cast<size_t>(4));
    if (select == 1U) {
        m_container = make_unique<FKG::HashTable<string, FKG::Storage>>();
    }else if (select == 4U) {
        Statistics();
        return;
    }
    
    for (istringstream stream; m_funcs[CheckInput(stream)](stream);stream.clear());
}

const size_t Interface::CheckInput(const size_t count) const {
    cout << m_menus[MenuItem::kInput];

    string line;
    getline(cin, line);

    size_t result;
    while (!sscanf(line.c_str(), "%zu", &result) || result > count) {
        cout << m_menus[MenuItem::kInvalid];
        cout << m_menus[MenuItem::kInput];
        getline(cin, line);
    }
    return result;
}

const string Interface::CheckInput(istringstream &stream) {
    cout << m_menus[MenuItem::kInput];

    static const std::vector<std::regex> regex{
        std::regex(R"(^(UPDATE)\s+(\w{1,15}|[-])\s+(\S+|[-])\s+(\S+|[-])\s+(\d{4}|[-])\s+(\S+|[-])\s+(\d+|[-])(\s+)?$)"),
        std::regex(R"(^FIND\s+(\S+|[\-])\s+(\S+|[\-])\s+(\d{4}|[\-])\s+(\S+|[\-])\s+(\d{4}|[\-])(\s+)?$)"),
        std::regex(R"(^SET\s+\w{1,15}\s+\S+\s+\S+\s+\d{4}\s+\S+\s+\d+(\s+EX\s+\d+)?(\s+)?$)"),
        std::regex(R"(^(UPLOAD|EXPORT)\s+\S+(\s+)?$)"),
        std::regex(R"(^(GET|EXISTS|DEL|TTL)\s+\S+$)"),
        std::regex(R"(^(RENAME)\s+\S+\s+\S+(\s+)?$)"),
        std::regex(R"(^(KEYS|SHOWALL|EXIT|INFO)$)")
    };

    std::string line;
    while (std::getline(std::cin, line) &&
            std::find_if(regex.begin(), regex.end(), [&](const std::regex &src) {
            return std::regex_match(line, src);
            }) == regex.end()) {
        std::cout << m_menus[MenuItem::kInvalid];
        std::cout << m_menus[MenuItem::kInput];
    }

    stream.str(line);
    stream >> line;
    return line;
}

[[nodiscard]] const bool Interface::Set(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    size_t seconds;
    string key, ex;
    FKG::Storage storage;

    stream >> key;
    stream >> storage;
    if (stream >> ex >> seconds) {
        storage.SetRemovalTime(chrono::seconds(seconds));
    }

    if (m_container->Set(key, storage)) {
        SetTTL(storage, key);
        cout << m_menus[MenuItem::kSuccess];
    } else {
        cout << m_menus[MenuItem::kFalse];
    }
    return true;
}

[[nodiscard]] const bool Interface::Get(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    string key;
    stream >> key;

    auto storage{m_container->Get(key)};
    if (storage.has_value()) {
        cout << storage.value() << "\n";
    } else {
        cout << m_menus[MenuItem::kNull];
    }
    return true;
}

[[nodiscard]] const bool Interface::Exists(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    string key;
    stream >> key;

    if (m_container->Exists(key)) {
        cout << m_menus[MenuItem::kTrue];
    } else {
        cout << m_menus[MenuItem::kFalse];
    }

    return true;
}

[[nodiscard]] const bool Interface::Del(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    string key;
    stream >> key;

    if (m_container->Del(key)) {
        cout << m_menus[MenuItem::kTrue];
    } else {
        cout << m_menus[MenuItem::kFalse];
    }

    return true;
}

[[nodiscard]] const bool Interface::Update(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    string key;
    FKG::Storage storage;

    stream >> key;
    stream >> storage;

    if (m_container->Update(key, storage)) {
        cout << m_menus[MenuItem::kSuccess];
    }

    return true;
}

[[nodiscard]] const bool Interface::Keys(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    size_t iteration{};
    for (auto &it : m_container->Keys()) {
        cout << ++iteration << ") " << it << "\n";
    }
    return true;
}

[[nodiscard]] const bool Interface::Rename(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    string key, rename;
    stream >> key >> rename;

    if (m_container->Rename(key, rename)) {
        cout << m_menus[MenuItem::kSuccess];
    } else {
        cout << m_menus[MenuItem::kNull];
    }

    return true;
}

[[nodiscard]] const bool Interface::TTL(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    string key;
    stream >> key;

    auto ttl{m_container->TTL(key)};
    if (ttl.has_value()) {
        cout << " > " << ttl.value() << "\n";
    } else {
        cout << m_menus[MenuItem::kNull];
    }

    return true;
}

[[nodiscard]] const bool Interface::Find(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    FKG::Storage storage;
    stream >> storage;

    size_t iteration{};
    for (auto &it : m_container->Find(storage)) {
        cout << ++iteration << ") " << it << "\n";
    }

    return true;
}

[[nodiscard]] const bool Interface::Showall(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    cout << left;
    cout << setw(14) << "Surname";
    cout << setw(14) << "Name";
    cout << setw(14) << "Year";
    cout << setw(16) << "City";
    cout << setw(5) << "Coins";
    cout << "\n";

    m_container->Showall();
    return true;
}

[[nodiscard]] const bool Interface::Upload(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    filesystem::path path;
    stream >> path;

    if (filesystem::exists(path)) {
        ifstream is(path.native());
        cout << m_menus[MenuItem::kSuccess] << m_container->Upload(is);
    } else {
        cout << m_menus[MenuItem::kNotExist] << path << "\n";
    }
    return true;
}

[[nodiscard]] const bool Interface::Export(istringstream &stream) {
    lock_guard<mutex> lock(m_mutex);
    filesystem::path path;
    stream >> path;

    if (filesystem::exists(path)) {
        ofstream os(path.native());
        cout << m_menus[MenuItem::kSuccess] << m_container->Export(os);
    } else {
        cout << m_menus[MenuItem::kNotExist] << path << "\n";
    }
    return true;
}

[[nodiscard]] const bool Interface::Info(istringstream &stream) {
    cout << m_menus[MenuItem::kInfo];
    return true;
}

void Interface::SetTTL(const FKG::Storage &storage, const string &key) {
    auto tp = storage.GetRemovalTime();
    if (tp.has_value()) {
        m_ttl.emplace(tp.value(), key);
        if (m_ttl.size() == 1) {
        m_threads.emplace_back(thread([this]() -> void {
            while (m_ttl.size()) {
            auto time = chrono::steady_clock::now();
            for (auto it = m_ttl.begin();
                it != m_ttl.end() && time > it->first;) {
                m_container->Del(it->second);
                m_ttl.erase(it++);
            }
            }
            this_thread::sleep_for(chrono::seconds(1));
        }));
        }
    }
}

void Interface::Statistics() {
    vector_container containers;
    containers.emplace_back(make_unique<FKG::HashTable<string, FKG::Storage>>());
    // containers.emplace_back(make_unique<FKG::SelfBalancingBinarySearchTree<string, FKG::Storage>>());
    // containers.emplace_back(make_unique<FKG::BPlusTree<string, FKG::Storage>>());

    cout << "Enter count elements";
    size_t countElements = CheckInput(static_cast<size_t>(100000));
    cout << "Enter count repeat";
    size_t countRepeat = CheckInput(static_cast<size_t>(countElements));

    FillRandonly(containers, countElements);
    auto result = GetResults(containers, countRepeat);

    vector<string> commands = {"      GET       ", "      SET       ","      DEL       ", "      KEYS      ","      FIND      "};
    vector<int> column_width{15, 16, 12};
    cout << m_menus[MenuItem::kStatistics];
    for (int i = 0; i < 5; ++i) {
        cout << "|" << commands[i] << "|";
        for (int j = 0; j < 3; ++j) {
        cout << setw(column_width[j]) << result[i * 3 + j] << "|";
        }
        cout << endl;
    }
}

int Interface::RandomInRange(int start, int end) {
    random_device rd;
    default_random_engine engine(rd());
    uniform_int_distribution<int> dist(start, end);
    return dist(engine);
}

istringstream Interface::CreateRecord(int key){
    int min = numeric_limits<int>::min();
    int max = numeric_limits<int>::max();

    stringstream stream;
    stream << key << " " << RandomInRange(min, max) << " ";
    stream << RandomInRange(min, max) << " " << RandomInRange(1970, 2004) << " ";
    stream << RandomInRange(0, 10) << " " << RandomInRange(0, 10000);
    istringstream istream(stream.str());

    return istream;
}

void Interface::FillRandonly(vector_container &containers, size_t count) {
    for(int i = 0; i < count; ++i) {
        auto stream = CreateRecord(i);
        string key;
        FKG::Storage storage;
        stream >> key;
        stream >> storage;
        containers[0]->Set(key, storage);
        containers[1]->Set(key, storage);
        containers[2]->Set(key, storage);
    }
}

vector<double> Interface::GetResults(vector_container &containers,size_t countRepeat){
    vector<double> result(15);
    TestGet(containers, countRepeat, result);
    TestSet(containers, countRepeat, result);
    TestDel(containers, countRepeat, result);
    TestKeys(containers, countRepeat, result);
    TestFind(containers, countRepeat, result);

    return result;
}

void Interface::TestGet(vector_container &containers, size_t countRepeat,vector<double> &result){
    time_sc::time_point start, end;
    for (int i = 0; i < countRepeat; ++i) {
        string key =
            to_string(RandomInRange(1, containers[0]->GetAmountElements()));
        for (int j = 0; j < containers.size(); ++j) {
        start = time_sc::now();
        auto temp = containers[j]->Get(key);
        end = time_sc::now();
        result[j] += chrono::duration<double>(end - start).count();
        }
    }
}

void Interface::TestSet(vector_container &containers, size_t countRepeat,vector<double> &result){
    time_sc::time_point start, end;
    for (int i = 0; i < countRepeat; ++i) {
        auto stream = CreateRecord(containers[0]->GetAmountElements());
        string key;
        FKG::Storage storage;
        stream >> key;
        stream >> storage;
        for (int j = 0; j < containers.size(); ++j) {
            start = time_sc::now();
            containers[j]->Set(key, storage);
            end = time_sc::now();
            result[3 + j] += chrono::duration<double>(end - start).count();
        }
    }
}

void Interface::TestDel(vector_container &containers, size_t countRepeat,vector<double> &result){
    time_sc::time_point start, end;
    for (int i = 0; i < countRepeat; ++i) {
        string key = to_string(containers[0]->GetAmountElements() - 1);
        for (int j = 0; j < containers.size(); ++j){
            start = time_sc::now();
            containers[j]->Del(key);
            end = time_sc::now();
            result[6 + j] += chrono::duration<double>(end - start).count();
        }
    }
}

void Interface::TestKeys(vector_container &containers, size_t countRepeat,vector<double> &result) {
    time_sc::time_point start, end;
    for (int i = 0; i < countRepeat; ++i) {
        for (int j = 0; j < containers.size(); ++j) {
            start = time_sc::now();
            auto trash = containers[j]->Keys();
            end = time_sc::now();
            result[9 + j] += chrono::duration<double>(end - start).count();
        }
    }
}

void Interface::TestFind(vector_container &containers, size_t countRepeat,vector<double> &result) {
    time_sc::time_point start, end;
    for (int i = 0; i < countRepeat; ++i) {
        string key = to_string(RandomInRange(1, containers[0]->GetAmountElements()));
        optional<FKG::Storage> temp = containers[0]->Get(key);
        for (int j = 0; j < containers.size(); ++j){
            start = time_sc::now();
            auto _ = containers[j]->Find(temp.value());
            end = time_sc::now();
            result[12 + j] += chrono::duration<double>(end - start).count();
        }
    }
}
