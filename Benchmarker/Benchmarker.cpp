// Benchmarker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Benchmarker.h"

namespace TX
{
	namespace Tests
	{
		double Test::Run(size_t iters){
			Setup();
			reset();
			for (uint i = 0; i < iters; i++)	Body();
			double measured = elapsed();
			Cleanup();
			return measured * 1000.f;
		}

		Benchmarker *Benchmarker::instance = nullptr;

		void Benchmarker::RunAll(){
			double total;
			std::printf("============ Benchmarking %d tests =============\n\n", Instance()->configs.size());
			for (auto config : Instance()->configs){
				Test *test = config->NewTest();
				total = 0;
#ifdef _DEBUG
				config->runs = 1;
				config->iters = Math::Max(1, config->iters / 40);
#endif
				for (uint i = 0; i < config->runs; i++)
					total += test->Run(config->iters);
				std::printf("%-20s\t\t%f ms\n", config->name, total / config->runs);
				delete test;
			}
			std::printf("\n============ All tests finished =============\n");
		}
	}
}