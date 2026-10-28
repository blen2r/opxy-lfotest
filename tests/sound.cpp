// The engine is silent; the LFO itself is tested in tests/control.cpp.
#include "../engine.cpp"
#include <cstdio>

int main() {
    static lfotest::Voice voice;
    static lfotest::Shared shared;
    const opxy::i32 parameters[8] = {};
    opxy::u32 pitch[opxy::block_frames] = {};
    opxy::i32 output[opxy::block_frames];
    lfotest::initialize(voice, shared, 1);
    lfotest::start(voice, shared, 1);
    lfotest::render(voice, shared, 1, output, parameters, pitch);
    for (unsigned i = 0; i < opxy::block_frames; ++i)
        if (output[i]) { std::puts("FAIL not silent"); return 1; }
    return 0;
}
