#include "MapReduceFramework.h"
#include <cstdio>
//#include <string>
#include <array>
#include <unistd.h>
#include <fstream>
#include <iostream>
//#include <fstream>

#define PATH_TO_RANDOMSTRING "/home/mattan/Desktop/os/ex3_new/randomstring.txt"
pthread_mutex_t k2ResourcesMutex = PTHREAD_MUTEX_INITIALIZER;

class VString : public V1 {
public:
    VString(std::string content) : content(content) {}

    std::string content;
};

class KChar : public K2, public K3 {
public:
    KChar(char c) : c(c) {}

    virtual bool operator<(const K2 &other) const {
        return c < static_cast<const KChar &>(other).c;
    }

    virtual bool operator<(const K3 &other) const {
        return c < static_cast<const KChar &>(other).c;
    }

    char c;
};

class VCount : public V2, public V3 {
public:
    VCount(unsigned int count) : count(count) {}

    unsigned int count;
};


class CounterClient : public MapReduceClient {
public:
    std::vector<KChar *> *resourcesK2;
    std::vector<VCount *> *resourcesV2;

    CounterClient() {
//        resourcesK2 = new std::vector<KChar *>;
//        resourcesV2 = new std::vector<VCount *>;
        resourcesK2 = new std::vector<KChar *>;
        resourcesV2 = new std::vector<VCount *>;

    }

    ~CounterClient() {
//        std::cout<<"freeing map reduce client"<<std::endl;
        while (!resourcesK2->empty()) {
            delete resourcesK2->at(0);
            resourcesK2->erase(resourcesK2->begin());
        }
        delete resourcesK2;

        while (!resourcesV2->empty()) {
//            std::cout<<"v size is "<<resourcesV2->size()<<std::endl;
            delete resourcesV2->at(0);
            resourcesV2->erase(resourcesV2->begin());
        }
        delete resourcesV2;
    }

    void map(const K1 *key, const V1 *value, void *context) const {
        std::array<unsigned int, 256> counts;
        counts.fill(0);
        for (const char &c : static_cast<const VString *>(value)->content) {
            counts[(unsigned char) c]++;
        }

        for (int i = 0; i < 256; ++i) {
            if (counts[i] == 0)
                continue;

            KChar *k2 = new KChar(i);
            VCount *v2 = new VCount(counts[i]);
            pthread_mutex_lock(&k2ResourcesMutex);
            resourcesK2->push_back(k2);
            resourcesV2->push_back(v2);
            pthread_mutex_unlock(&k2ResourcesMutex);
            emit2(k2, v2, context);
        }
    }

    void reduce(const K2 *key, const std::vector<V2 *> &values, void *context) const {
        const char c = static_cast<const KChar *>(key)->c;
        unsigned int count = 0;
        for (V2 *val : values) {
            count += static_cast<const VCount *>(val)->count;
        }
        KChar *k3 = new KChar(c);
        VCount *v3 = new VCount(count);
        emit3(k3, v3, context);
    }
};


int main() {
//    for (int i = 0; i < 10000; ++i) {
//        std::cout<<"new run"<<i<<"\n";
    CounterClient client;
    InputVec inputVec;
    OutputVec outputVec;
    std::vector<std::string> a;
    std::string line;
    std::ifstream f(PATH_TO_RANDOMSTRING);
    if (f.is_open()) {
        while (getline(f, line)) {
            a.push_back(line);
        }
        for (std::string &str : a) {
            auto v = new VString(str);
            inputVec.push_back({nullptr, v});
        }
    } else {
        std::cerr << "Bad path, please insert the correct path to the file randomstring.txt in row 10. \nWindows slashes should be escaped. \nExample: C:\\\\KimJongUn\\\\Personal\\\\nukecodes\\\\randomstring.txt" <<std::endl;
        exit(1);
    }

    JobState state;
    JobState last_state = {UNDEFINED_STAGE, 0};
    JobHandle job = startMapReduceJob(client, inputVec, outputVec, 200);
    getJobState(job, &state);
//        auto jobContext = static_cast<JobContext*>(job);
    while (!(state.stage == REDUCE_STAGE && state.percentage == 100.0)) {

        if (last_state.stage != state.stage || last_state.percentage != state.percentage) {
            printf("stage %d, %f%% \n", state.stage, state.percentage);
            if (state.percentage > 100 || state.percentage < 0) {
                std::cerr << "FAIL! Bad Percentage!" << std::endl;
                exit(1);
            }
            if (last_state.stage == state.stage && state.percentage < last_state.percentage) {
                std::cerr << "FAIL Bad Percentage! Make sure that you are updating the atomic variable correctly."
                          << std::endl;
                exit(1);
            }
        }
        last_state = state;
        getJobState(job, &state);
    }

    closeJobHandle(job);
    std::map<char, int> expectedOutput = {{'0',  13258},
                                                        {'1',  13014},
                                                        {'2',  13241},
                                                        {'3',  13246},
                                                        {'4',  13042},
                                                        {'5',  13479},
                                                        {'6',  13085},
                                                        {'7',  12949},
                                                        {'8',  13168},
                                                        {'9',  13033},
                                                        {'a',  13119},
                                                        {'b',  13078},
                                                        {'c',  13360},
                                                        {'d',  13126},
                                                        {'e',  13055},
                                                        {'f',  13460},
                                                        {'g',  12940},
                                                        {'h',  13231},
                                                        {'i',  13112},
                                                        {'j',  13063},
                                                        {'k',  13232},
                                                        {'l',  13016},
                                                        {'m',  13172},
                                                        {'n',  13161},
                                                        {'o',  12989},
                                                        {'p',  13148},
                                                        {'q',  13148},
                                                        {'r',  13200},
                                                        {'s',  13238},
                                                        {'t',  12964},
                                                        {'u',  13106},
                                                        {'v',  13334},
                                                        {'w',  13039},
                                                        {'x',  13202},
                                                        {'y',  13199},
                                                        {'z',  13010},
                                                        {'A',  13149},
                                                        {'B',  13103},
                                                        {'C',  13305},
                                                        {'D',  13132},
                                                        {'E',  13009},
                                                        {'F',  13217},
                                                        {'G',  13107},
                                                        {'H',  13050},
                                                        {'I',  13394},
                                                        {'J',  13248},
                                                        {'K',  13283},
                                                        {'L',  13129},
                                                        {'M',  13095},
                                                        {'N',  13190},
                                                        {'O',  13426},
                                                        {'P',  13187},
                                                        {'Q',  13223},
                                                        {'R',  13360},
                                                        {'S',  13056},
                                                        {'T',  13249},
                                                        {'U',  13175},
                                                        {'V',  13117},
                                                        {'W',  13244},
                                                        {'X',  13228},
                                                        {'Y',  13023},
                                                        {'Z',  13032},
                                                        {'!',  13352},
                                                        {'"',  13271},
                                                        {'#',  13242},
                                                        {'$',  13176},
                                                        {'%',  13172},
                                                        {'&',  13121},
                                                        {'\'', 13274},
                                                        {'(',  13250},
                                                        {')',  13107},
                                                        {'*',  13157},
                                                        {'+',  13199},
                                                        {',',  13143},
                                                        {'-',  13206},
                                                        {'.',  13191},
                                                        {'/',  13266},
                                                        {':',  13308},
                                                        {';',  13309},
                                                        {'<',  13158},
                                                        {'=',  13345},
                                                        {'>',  13061},
                                                        {'?',  13238},
                                                        {'@',  13130},
                                                        {'[',  13050},
                                                        {'\\', 13152},
                                                        {']',  13123},
                                                        {'^',  13143},
                                                        {'_',  13010},
                                                        {'`',  13226},
                                                        {'{',  13054},
                                                        {'|',  13043},
                                                        {'}',  13164},
                                                        {'~',  13234},
                                                        {' ',  13096}};
    for (OutputPair &pair: outputVec) {
        char c = ((const KChar *) pair.first)->c;
        int count = ((const VCount *) pair.second)->count;
        printf("The character %c appeared %u time%s\n",
               c, count, count > 1 ? "s" : "");
        auto iter = expectedOutput.find(c);
        if(iter != expectedOutput.end())
        {
            //element found;
            if (count!=iter->second) {
                std::cout<<"FAIL: Your program reported the value "<< count << " for the key " << c << std::endl;
                std::cout<<"the actual value is "<< iter->second << std::endl;
                exit(1);
            } else {
                expectedOutput.erase(iter);
            }
        } else {
            std::cout<<"FAIL: The key "<<c<<" with value "<<count<<"Does not exist!" << std::endl;
            exit(1);
        }
        delete pair.first;
        delete pair.second;
    }
    for (auto pair : inputVec) {
        delete pair.second;
    }
    if (expectedOutput.size() > 0) {
        auto iter = expectedOutput.begin();
        std::cout<<"(highly unlikely) you missed the letter: " << iter->first<<std::endl;
    }
    std::cout << "PASS!" <<std::endl <<  std::endl;
    exit(0);
}

