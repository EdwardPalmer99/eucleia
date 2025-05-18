/**
 * @file CountTo1MBenchmark.cpp
 * @author Edward Palmer
 * @date 2025-05-18
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "EucleiaInterpreter.hpp"
#include "FileParser.hpp"
#include "Scope.hpp"
#include "test/utility/Utility.hpp"
#include <benchmark/benchmark.h>

namespace Loops
{
static void ParseCountTo1M(benchmark::State &state)
{
    auto path = (getTestDirPath() + "benchmark/data/CountToOneMillion.ek");

    for (auto _ : state)
    {
        (void)FileParser::parseMainFile(path);
    }
}


static void EvaluateCountTo1M(benchmark::State &state)
{
    auto path = (getTestDirPath() + "benchmark/data/CountToOneMillion.ek");

    auto ast = FileParser::parseMainFile(path);

    for (auto _ : state)
    {
        Scope globalScope;
        ast->evaluate(globalScope);
    }
}

} // namespace Loops


namespace Functions
{

static void ParseAndEvaluateFibTo25(benchmark::State &state)
{
    auto path = (getTestDirPath() + "benchmark/data/EvaluateFibToTwentyFive.ek");

    for (auto _ : state)
    {
        Interpreter::evaluateFile(path);
    }
}

} // namespace Functions


BENCHMARK(Loops::ParseCountTo1M)->Unit(benchmark::kMillisecond);
BENCHMARK(Loops::EvaluateCountTo1M)->Unit(benchmark::kMillisecond);
BENCHMARK(Functions::ParseAndEvaluateFibTo25)->Unit(benchmark::kMillisecond);