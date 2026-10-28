# LFO Test

**Temporary test machine.** It exists to show and test the Open-XY SDK's
[track control](https://github.com/etcetc000/open-xy/blob/staging/docs/api.md#track-control)
(SDK 1.5, proposed upstream) with more than one machine at once, beside
[Morphose](https://github.com/blen2r/opxy-morpher). This repository will be deleted once that
work is merged.

A silent synth (ID `lfotest`): a triangle LFO on one engine knob of another instrument track,
around the value that track sets itself.

| Knob | Values |
| --- | --- |
| Track | t1 to t8: the track to modulate |
| Knob | k1 to k8: which of its engine knobs |
| Rate | 0.25, 0.5, 1, 2, 4 or 8 Hz |
| Depth | 0 (off) to 100: up to half the knob's range either side |

It is not unique: several copies run side by side, each with its own phase. When two machines
steer the same knob, the one on the later track wins. It draws its own marks (a thin blue line at
the top of steered tracks' pages) through `marks.svg.fragment`. It can steer another track control
machine too, such as Morphose's morph knob. Tested on an OP-XY with OS 1.2.1.

## Develop

Keep the [SDK](https://github.com/etcetc000/open-xy) beside this repository as
`../open-xy`, on `staging`; adjust the commands if yours is elsewhere. The
[first-machine guide](https://github.com/etcetc000/open-xy/blob/staging/docs/first-machine.md)
covers prerequisites. Read the
[core machines](https://github.com/etcetc000/open-xy-core-machines) as worked
examples and the [machine API](https://github.com/etcetc000/open-xy/blob/staging/docs/api.md)
for the supported interfaces. Follow the
[UX conventions](https://github.com/etcetc000/open-xy/blob/staging/docs/ux.md);
Macro Osc is the two-page reference. Synth starters with five to eight controls
use the [shared header](https://github.com/etcetc000/open-xy/blob/staging/docs/display.md#two-page-header):
keep its SVG block and update the labels/value captions with your controls.
Columns without alternates repeat the same saved parameter on both rows.
Agent instructions are in [AGENTS.md](AGENTS.md).

Build and audition the synth in the browser:

```sh
python ../open-xy/sdk.py preview --machine . --out ../previews/lfotest-001
python ../open-xy/tools/serve_harness.py --preview ../previews/lfotest-001
```

```sh
python ../open-xy/sdk.py test --machine . --cxx clang++ --host-cxx g++
```

Extend `tests/` as the machine's behavior grows. The starter is a minimal
implementation, not a finished instrument. Keep generated output outside this
repository. The generated workflow checks source, sound tests and synth previews
on Linux, and compiles the native module on Windows; it never uses a device.
Its `OPEN_XY_REF` starts at `staging`; pin a compatible SDK commit for a release.

## Hardware and publication

Follow the SDK's [firmware build guide](https://github.com/etcetc000/open-xy/blob/staging/docs/build.md)
and [hardware profiling workflow](https://github.com/etcetc000/open-xy/blob/staging/docs/profiling.md).
Keep firmware and stock content outside every checkout. Commit the machine,
build with profiling enabled, install when authorized and collect three runs.
The host estimate printed by `test` cannot substitute for those measurements.

For the builder, follow [catalog submission](https://github.com/etcetc000/open-xy/blob/staging/docs/catalog.md#submit-a-machine).
Every new or changed draft/published entry needs a hardware CPU record tied to
its pinned commit. Hardware validation may be performed by a maintainer if you
do not own an XY. Publication remains pending until that evidence is available.

The generated starter is covered by [LICENSE](LICENSE). Preserve notices for
code you reuse and include the licences of any dependencies or ports.
