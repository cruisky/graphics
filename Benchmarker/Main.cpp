#include "stdafx.h"
#include "Benchmarker.h"

using namespace TX;
using namespace TX::Tests;

int main(){
	Benchmarker::Instance()->RunAll();
	getchar();
}