// The LFO swings the chosen knob of the chosen track around its own value,
// within range, and leaves its own track alone.
#include "../control.cpp"
#include <cstdio>

static opxy::i32 word(unsigned index, unsigned count) {
    return static_cast<opxy::i32>(((index * 32767u + (count - 1) / 2) / (count - 1)) << 16);
}

int main() {
    using namespace lfotest;
    static ControlState state;
    static opxy::control::Input input;
    static opxy::control::Overlay overlay;
    control_reset(state);
    input.track = 2;
    input.values[2][0] = word(0, 8);         // track 1
    input.values[2][1] = word(1, 8);         // knob 2
    input.values[2][2] = word(5, kRates);    // 8 Hz
    input.values[2][3] = 0x7fffffff;         // full depth
    input.values[0][1] = 0x40000000;
    opxy::i32 low = 0x7fffffff, high = 0;
    for (int t = 0; t < 200; ++t) {
        overlay = opxy::control::Overlay{};
        input.elapsed_us = 1000;
        control(state, input, overlay);
        if (overlay.count != 1 || overlay.entries[0].track != 0 || overlay.entries[0].index != 1 || !overlay.mark) {
            std::puts("FAIL wrong target");
            return 1;
        }
        const opxy::i32 v = overlay.entries[0].value;
        if (v < low) low = v;
        if (v > high) high = v;
    }
    if (low > 0x08000000 || high < 0x78000000) { std::printf("FAIL swing %d..%d\n", low, high); return 1; }
    // Its own track is never a target; no depth, no overlay.
    input.values[2][0] = word(2, 8);
    overlay = opxy::control::Overlay{};
    control(state, input, overlay);
    if (overlay.count) { std::puts("FAIL own track"); return 1; }
    input.values[2][0] = word(0, 8);
    input.values[2][3] = 0;
    overlay = opxy::control::Overlay{};
    control(state, input, overlay);
    if (overlay.count) { std::puts("FAIL overlay without depth"); return 1; }
    return 0;
}
