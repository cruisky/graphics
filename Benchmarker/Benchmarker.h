#pragma once

#include <vector>
#include <map>
#include <memory>
#include "System\Tools.h"

#define PRECISION_HIGH   50
#define PRECISION_MEDIUM 10
#define PRECISION_LOW	 3
#define PRECISION_LOWEST 1


namespace TX{
	namespace Tests {
		class Test : public Timer {
		public:
			Test(){}
			virtual ~Test(){}
			double Run(size_t iters);
		protected:
			virtual void Body() = 0;
			virtual void Setup(){}
			virtual void Cleanup(){}
		};

		template <typename T> Test* NewTest(){ return new T(); }

		class TestConfig {
		public:
			TestConfig(const char* name, size_t runs, size_t iters) : name(name), runs(runs), iters(iters){}
			virtual Test* NewTest() = 0;
		public:
			const char *name;
			size_t runs;
			size_t iters;
		};

		template <typename T>
		class TestConfigImpl : public TestConfig {
		public:
			TestConfigImpl(const char* name, size_t runs, size_t iters) : TestConfig(name, runs, iters){}
			virtual Test* NewTest(){ return new T; }
		private:
			static const int id;
		};

		class Benchmarker {
		public:
			static Benchmarker * Instance(){
				if (!instance){
					instance = new Benchmarker();
				}
				return instance;
			}
			int Benchmarker::Register(TestConfig *config){
				configs.push_back(config);
				return configs.size();
			}
			void RunAll();
		private:
			Benchmarker(){}
		private:
			static Benchmarker *instance;
			std::vector<TestConfig*> configs;
		};
	}
}

#define BM_CLASS(className)	class className : public ::TX::Tests::Test
#define BM_SETUP() protected: void Setup()
#define BM_BODY() protected: void Body()
#define BM_CLEANUP() protected: void Cleanup()

#define BENCHMARK(className, runs, iters) \
	const int ::TX::Tests::TestConfigImpl<className>::id = \
		::TX::Tests::Benchmarker::Instance()->Register( \
			new ::TX::Tests::TestConfigImpl<className>(#className, runs, iters))
