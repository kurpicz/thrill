/*******************************************************************************
 * benchmarks/data/disk.cpp
 *
 * Part of Project c7a.
 *
 * Copyright (C) 2015 Tobias Sturm <mail@tobiassturm.de>
 *
 * This file has no license. Only Chuck Norris can compile it.
 ******************************************************************************/

#include <c7a/api/read.hpp>
#include <c7a/api/write.hpp>
#include <c7a/common/cmdline_parser.hpp>

#include <iostream>
#include <string>

using namespace c7a; // NOLINT

int main(int argc, const char** argv) {
    core::JobManager jobMan;
    jobMan.Connect(0, net::Endpoint::ParseEndpointList("127.0.0.1:8000"), 1);
    api::Context ctx(jobMan, 0);
    common::GetThreadDirectory().NameThisThread("benchmark");

    common::CmdlineParser clp;
    clp.SetDescription("c7a::data benchmark for disk I/O");
    clp.SetAuthor("Tobias Sturm <mail@tobiassturm.de>");
    std::string input_file, output_file;
    int iterations;
    clp.AddParamString("i", input_file,  "Input file");
    clp.AddParamString("o", output_file, "Output file");
    clp.AddParamInt(   "n", iterations,  "Iterations");
    if (!clp.Process(argc, argv)) return -1;

    auto overall_timer = ctx.stats().CreateTimer("all runs", "", true);
    for (int i = 0; i < iterations; i++) {
        auto timer = ctx.stats().CreateTimer("single run", "", true);
        auto lines = ReadLines(ctx, input_file, [](const std::string& line) { return line; });
        lines.WriteToFileSystem(output_file);
        timer->Stop();
    }
    overall_timer->Stop();

}

/******************************************************************************/
