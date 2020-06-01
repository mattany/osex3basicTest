#include "MapReduceFramework.h"
#include <cstdio>
#include <array>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <random>

#include <gtest/gtest.h>


//static const int REPEATS = 10000;
//static const int DEADLOCK_REPEATS = 1000000;
<<<<<<< HEAD
static const int RANDOM_REPEATS = 2000;
=======
static const int RANDOM_REPEATS = 1000;
>>>>>>> 25a5e2c5cf6fea0185963070881ab943688713a5
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
	mutable std::vector<std::unique_ptr<KChar>> resourcesK2;
	mutable std::vector<std::unique_ptr<VCount>> resourcesV2;

	InputVec inputVec;
	OutputVec outputVec;

	CounterClient() : resourcesK2(), resourcesV2(), inputVec(), outputVec()
	{}

	~CounterClient()
	{
		for (auto& kvp: inputVec)
		{
			delete kvp.first;
			delete kvp.second;
		}
		for (auto& kvp: outputVec)
		{
			delete kvp.first;
			delete kvp.second;
		}
	}

	void map(const K1 *key, const V1 *value, void *context) const {
		(void)key;
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
			resourcesK2.emplace_back(k2);
			resourcesV2.emplace_back(v2);
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


TEST(MattanTests, errorMessageTest) {
	CounterClient client;
	auto s1 = new VString("This string is full of characters");
	auto s2 = new VString("Multithreading is awesome");
	auto s3 = new VString("conditions are race bad");
	client.inputVec.push_back({nullptr, s1});
	client.inputVec.push_back({nullptr, s2});
	client.inputVec.push_back({nullptr, s3});
<<<<<<< HEAD
=======

>>>>>>> 25a5e2c5cf6fea0185963070881ab943688713a5
	ASSERT_EXIT(startMapReduceJob(client, client.inputVec, client.outputVec, 20000000),
	            ::testing::ExitedWithCode(1),
	            ::testing::MatchesRegex("system error: .*\n")
	) << "When starting too many threads, thread creation should fail, causing program to exit with code 1 and print an error";
}

TEST(MattanTests, outputTest) {
	CounterClient client;
	std::vector<std::string> a;
	std::string line;
	std::ifstream f(RANDOM_STRINGS_PATH);
	if (f.is_open()) {
		while (getline(f, line)) {
			a.push_back(line);
		}
		for (std::string &str : a) {
			auto v = new VString(str);
			client.inputVec.push_back({nullptr, v});
		}
	} else {
		FAIL() << "(Technical error) Coludn't find strings file at " << RANDOM_STRINGS_PATH << " - maybe you deleted it by mistake?";
	}

	std::cout << "Starting job" << std::endl;

	JobState state;
	JobState last_state = {UNDEFINED_STAGE, 0};
	JobHandle job = startMapReduceJob(client, client.inputVec, client.outputVec, 200);
	getJobState(job, &state);
	last_state = state;
	while (!(state.stage == REDUCE_STAGE && state.percentage == 100.0)) {

		if (last_state.stage != state.stage || last_state.percentage != state.percentage) {
			printf("stage %d, %f%% \n", state.stage, state.percentage);
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
	for (OutputPair &pair: client.outputVec) {
		char c = ((const KChar *) pair.first)->c;
		int count = ((const VCount *) pair.second)->count;
		printf("The character %c appeared %u time%s\n",
		       c, count, count > 1 ? "s" : "");
		auto iter = expectedOutput.find(c);
		if(iter != expectedOutput.end())
		{
			//element found;
			if (count!=iter->second) {
				FAIL() << "Your program reported the value "<< count << " for the key " << c << std::endl
				       << "the actual value is "<< iter->second << std::endl;
			} else {
				expectedOutput.erase(iter);
			}
		} else {
			FAIL() << "The key "<<c<<" with value "<<count<<"Does not exist!" << std::endl;
		}
	}
	if (expectedOutput.size() > 0) {
		auto iter = expectedOutput.begin();
		FAIL() << "Your program has missed " << expectedOutput.size() << " letters, the first letter you missed is: " << iter->first << " whose count should be " << iter->second;
	}
}


void randbody(int iterations) {


    std::default_random_engine generator(time(nullptr));
    std::uniform_int_distribution<int> bernouli(0, 1);
    std::uniform_int_distribution<int> trinary(0, 2);
    std::uniform_int_distribution<int> dist(1, 1000);
    std::uniform_int_distribution<int> concurrentJobAmount(1, 20);
    std::vector<std::uniform_int_distribution<int>> dists;

    std::uniform_int_distribution<int> large_amount(100, 500);
    std::uniform_int_distribution<int> small_amount(6, 100);
    std::uniform_int_distribution<int> tiny_amount(2, 5);
    dists.push_back(large_amount);
    dists.push_back(small_amount);
    dists.push_back(tiny_amount);

    for (int i = 0; i < iterations; ++i) {
        int activeJobs = concurrentJobAmount(generator);
//        std::cout << "Job amount: " << activeJobs << std::endl;

        std::vector<CounterClient> clients(activeJobs);
        std::vector<JobHandle> jobs(activeJobs, nullptr);
        std::vector<std::pair<JobState, JobState>> jobStates(activeJobs); // [0]=prevstate [1]=curstate
        std::vector<int> levels;
        std::cout << "repetition #" << i << std::endl;
        int lineAmount = dist(generator);
//        std::cout<<"line amount: "<<lineAmount<<std::endl;

        for (int j = 0; j < activeJobs; ++j) {
//            std::cout<<"Job number: "<<j<<std::endl;
            auto &client = clients.at(j);
            std::vector<std::string> a;
            std::string line;
            std::ifstream f(RANDOM_STRINGS_PATH);
            if (f.is_open()) {
                int k = 0;
                while (getline(f, line) && k < lineAmount) {
                    if (bernouli(generator)) {
                        a.push_back(line);
                        k++;
                    }
                }
                for (std::string &str : a) {
                    auto v = new VString(str);
                    client.inputVec.push_back({nullptr, v});
                }
            } else {
                FAIL() << "(Technical error) Coludn't find strings file at " << RANDOM_STRINGS_PATH
                       << " - maybe you deleted it by mistake?";
            }
            int level = dists[trinary(generator)](generator);
            levels.push_back(level);
//            std::cout<<"Thread Amount: "<<level<<std::endl;
        }

        jobs.reserve(activeJobs);
        for (int j = 0; j < activeJobs; ++j) {
//            std::cout<<"job "<<j<<std::endl;
            auto &client = clients[j];
            JobHandle handle = startMapReduceJob(client, client.inputVec, client.outputVec, levels[j]);
            jobs[j] = handle;
        }
        int totalJobs = activeJobs;
        for (int j = 0; activeJobs > 0; ++j) {
            if (jobs[j] != nullptr) {
                JobHandle job = jobs[j];
                JobState &state = jobStates[j].second;
                getJobState(job, &state);
                JobState &last_state = jobStates[j].first;
                if (!(state.stage == REDUCE_STAGE && last_state.percentage == 100.0)) {
                    if (last_state.stage != state.stage || last_state.percentage != state.percentage) {
                        if (state.percentage > 100 || state.percentage < 0) {
                            FAIL() << "Invalid percentage(not in 0-100): " <<
                                   "Current stage:" << state.stage << " " << state.percentage << "%" << std::endl <<
                                   "Previous stage: " << last_state.stage << " " << last_state.percentage << "%"
                                   << std::endl;
                        }
                        if (last_state.stage == state.stage && state.percentage < last_state.percentage) {
                            FAIL() << "Bad percentage(smaller than previous percentage at same stage): " <<
                                   "Current stage:" << state.stage << " " << state.percentage << "%" << std::endl <<
                                   "Previous stage:" << last_state.stage << " " << last_state.percentage << "%"
                                   << std::endl;
                        }
                        if (last_state.stage > state.stage) {
                            FAIL() << "Bad stage: " <<
                                   "Current stage:" << state.stage << " " << state.percentage << "%" << std::endl <<
                                   "Previous stage:" << last_state.stage << " " << last_state.percentage << "%"
                                   << std::endl;
                        }
                    }
                    last_state = state;
                    getJobState(job, &state);
                } else {
                    closeJobHandle(job);
                    jobs[j] = nullptr;
                    activeJobs--;
                }
            }
            if (j == totalJobs - 1) {
                j = -1;
            }
        }

    }
}


TEST(MattanTests, randomTest) {
	EXPECT_EXIT(randbody(RANDOM_REPEATS), ::testing::KilledBySignal(24), ::testing::MatchesRegex(""));
    //TODO  If you fail this test, comment the line above and uncomment the line below to see what exit code you failed with more easily.
    // The task should be killed by signal 24 (SIGXCPU) which means the cpu time limit was exceeded. That should be the only reason that
    // the task dies. This should happen only after more than 100 iterations. Running sanitizer will slow the program down
    // a lot and could cause a false positive.
//    randbody(RANDOM_REPEATS);

}


TEST(MattanTests, waitAndCloseTest) {

	CounterClient client;
	auto s1 = new VString("This string is full of characters");
	auto s2 = new VString("Multithreading is awesome");
	auto s3 = new VString("conditions are race bad");
	client.inputVec.push_back({nullptr, s1});
	client.inputVec.push_back({nullptr, s2});
	client.inputVec.push_back({nullptr, s3});
	JobState state;
	JobState last_state={UNDEFINED_STAGE,0};
	JobHandle job = startMapReduceJob(client, client.inputVec, client.outputVec, 3);
	getJobState(job, &state);
	waitForJob(job);

	// Should work without system error, since we are supposed to check if join has already been called.
	closeJobHandle(job);

}

//
//TEST(MattanTests, progressTest) {
//    for (int i = 0; i < REPEATS; ++i)
//    {
//        std::cout<<"repetition #"<<i<<std::endl;
//        CounterClient client;
//        auto s1 = new VString("This string is full of characters");
//        auto s2 = new VString("Multithreading is awesome");
//        auto s3 = new VString("conditions are race bad");
//        client.inputVec.push_back({nullptr, s1});
//        client.inputVec.push_back({nullptr, s2});
//        client.inputVec.push_back({nullptr, s3});
//        JobState state;
//        JobState last_state={UNDEFINED_STAGE,0};
//        JobHandle job = startMapReduceJob(client, client.inputVec, client.outputVec, 6);
//        getJobState(job, &state);
//
//        while (state.stage != REDUCE_STAGE || state.percentage != 100.0)
//        {
//            if (last_state.stage != state.stage || last_state.percentage != state.percentage) {
//                printf("stage %d, %f%% \n", state.stage, state.percentage);
//                if (state.percentage > 100 || state.percentage < 0) {
//                    FAIL() << "Invalid percentage(not in 0-100): " << state.percentage << ", encountered during stage " << state.stage << ")";
//                }
//                if (last_state.stage == state.stage && state.percentage < last_state.percentage) {
//                    FAIL() << "Bad percentage(smaller than previous percentage at same stage): " << state.percentage << ", encountered during stage " << state.stage << ")";
//                }
//                if (last_state.stage > state.stage) {
//                    FAIL() << "Bad stage " << state.stage << " - smaller than previous stage, encountered with percentage " << state.percentage;
//                }
//            }
//            last_state = state;
//            getJobState(job, &state);
//        }
//        printf("Done!\n");
//
//        closeJobHandle(job);
//
//    }
//}

//TEST(MattanTests, deadlockTest) {
//    for (int i = 0; i < DEADLOCK_REPEATS; ++i)
//    {
//        std::cout<<"repetition #"<<i<<std::endl;
//        CounterClient client;
//        auto s1 = new VString("This string is full of characters");
//        auto s2 = new VString("Multithreading is awesome");
//        auto s3 = new VString("conditions are race bad");
//        client.inputVec.push_back({nullptr, s1});
//        client.inputVec.push_back({nullptr, s2});
//        client.inputVec.push_back({nullptr, s3});
//        JobState state;
//        JobState last_state={UNDEFINED_STAGE,0};
//        JobHandle job = startMapReduceJob(client, client.inputVec, client.outputVec, 3);
//        getJobState(job, &state);
//        while (state.stage != REDUCE_STAGE || state.percentage != 100.0)
//        {
//            last_state = state;
//            getJobState(job, &state);
//        }
//        closeJobHandle(job);
//    }
//}





