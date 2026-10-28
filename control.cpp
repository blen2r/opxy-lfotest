// A triangle LFO on one engine knob of another instrument track, around the
// value the track itself sets. Not unique: each copy runs with its own phase.
#include <opxy/q31.hpp>
#include <opxy/track_control.hpp>

namespace lfotest {
struct ControlState { opxy::u32 phase; };

const opxy::u32 kRates = 6;
// Phase steps per microsecond for 0.25, 0.5, 1, 2, 4 and 8 Hz (2^32 per cycle).
const opxy::u32 kSteps[kRates] = {1074, 2147, 4295, 8590, 17180, 34360};

inline opxy::u32 choice(opxy::i32 value, opxy::u32 count) {
    return value < 0 ? 0 : ((static_cast<opxy::u32>(value) >> 16) * count) >> 15;
}

inline void control_reset(ControlState& state) { state.phase = 0; }

inline void control(ControlState& state, const opxy::control::Input& input, opxy::control::Overlay& overlay) {
    const opxy::i32* own = input.values[input.track];
    const opxy::u32 track = choice(own[0], opxy::control::tracks), knob = choice(own[1], 8);
    const opxy::u32 rate = choice(own[2], kRates);
    const opxy::i32 depth = own[3] < 0 ? 0 : own[3];
    // Wrapping is the phase's own modulo 2^32.
    state.phase += kSteps[rate < kRates ? rate : kRates - 1] * input.elapsed_us;
    // Triangle -2^30..2^30, scaled by depth: up to half the knob's range either side.
    const opxy::u32 folded = (state.phase & 0x80000000u) ? ~state.phase : state.phase;
    const opxy::i32 wave = static_cast<opxy::i32>(folded) - 0x40000000;
    const opxy::i32 swing = opxy::q31::multiply(wave, depth) * 2;
    const opxy::i32 base = input.values[track][knob];
    opxy::i32 value = base + (swing < 0 && base < -swing ? -base : swing);
    if (swing > 0 && base > 0x7fffffff - swing) value = 0x7fffffff;
    if (depth > 0 && track != input.track) {
        opxy::control::add(overlay, track, knob, value);
        overlay.mark = 1;
    }
    overlay.display[0] = swing;
}
}
