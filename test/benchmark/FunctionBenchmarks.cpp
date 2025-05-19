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

static void ParseAndEvaluateSumOfMultiplesOf3Or5To1000Naive(benchmark::State &state)
{
    auto path = (getTestDirPath() + "benchmark/data/SumMultiplesThreeFiveNaive.ek");

    for (auto _ : state)
    {
        Interpreter::evaluateFile(path);
    }
}

static void ParseAndEvaluateSumOfMultiplesOf3Or5To1000Opt(benchmark::State &state)
{
    auto path = (getTestDirPath() + "benchmark/data/SumMultiplesThreeFiveOpt.ek");

    for (auto _ : state)
    {
        Interpreter::evaluateFile(path);
    }
}

static void ParseAndEvaluateDifferenceSumOfSquaresAndSquareOfSum(benchmark::State &state)
{
    auto path = (getTestDirPath() + "benchmark/data/DifferenceSumOfSquaresAndSquareOfSum.ek");

    for (auto _ : state)
    {
        Interpreter::evaluateFile(path);
    }
}

} // namespace Functions


BENCHMARK(Functions::ParseAndEvaluateFibTo25)->Unit(benchmark::kMillisecond);
BENCHMARK(Functions::ParseAndEvaluateSumOfMultiplesOf3Or5To1000Naive)->Unit(benchmark::kMillisecond);
BENCHMARK(Functions::ParseAndEvaluateSumOfMultiplesOf3Or5To1000Opt)->Unit(benchmark::kMillisecond);
BENCHMARK(Functions::ParseAndEvaluateDifferenceSumOfSquaresAndSquareOfSum)->Unit(benchmark::kMillisecond);