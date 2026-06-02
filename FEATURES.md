# smartmet-library-timeseries — Feature List

A structured inventory of capabilities provided by the timeseries
library. Use as a checklist when drafting release notes. When new
functionality is added, append the new entry under the matching
section (and bump the *Last updated* line at the bottom).

`smartmet-library-timeseries` is a C++17 shared library that defines
the time-series data model and algorithms used across SmartMet
Server. It is consumed primarily by `smartmet-plugin-timeseries`,
`smartmet-plugin-edr`, and `smartmet-engine-observation`. All code
lives under the `SmartMet::TimeSeries` namespace (commonly aliased as
`TS`); the umbrella header is `TimeSeriesInclude.h`.

---

## 1. Core data types

A small hierarchy built on `std::variant`:

- **`TS::Value_`** — `std::variant<None, string, double, int, LonLat,
  LocalDateTime>`. Carries any single time-series cell.
- **`TS::Value`** — wraps `Value_` and adds `as_double()` /
  `as_int()` conversion methods.
- **`TS::None`** — explicit missing-value marker.
- **`TS::LonLat`** — coordinate value type.
- **`TS::TimedValue`** — pair of `Fmi::LocalDateTime` + `TS::Value`.
- **`TS::TimeSeries`** — `std::vector<TimedValue>`, the fundamental
  container.
- **`TS::LonLatTimeSeries`** — `LonLat + TimeSeries`, one location's
  data.
- **`TS::TimeSeriesGroup`** — `std::vector<LonLatTimeSeries>`, an
  area query result.
- **`TS::TimeSeriesVector`** — `std::vector<TimeSeries>`, multiple
  parameters at one location.
- **`TS::TimeSeriesData`** — `std::variant` over the above three
  collection types.

## 2. Time series generation

- **`TimeSeriesGenerator`** — produces `LocalTimeList`s from a
  `TimeSeriesGeneratorOptions`.
- **`TimeSeriesGeneratorOptions`** — start / end time, step, and
  generation **mode**:
  - **`DataTimes`** — match the source data's native timesteps.
  - **`GraphTimes`** — graph-aligned ticks (hourly / daily / etc.).
  - **`FixedTimes`** — exact named instants.
  - **`TimeSteps`** — fixed step from start.
- **`TimeSeriesGeneratorCache`** — caches recently generated time
  lists for repeated requests.

## 3. Aggregation & statistics

- **`TimeSeriesAggregator`** — applies a `DataFunction` (inner +
  outer pair) over a series.
- **`DataFunction`** — function definition (id, type, arguments).
- **`FunctionId`** enum:
  - **`Mean`**, **`Amean`** (arithmetic mean).
  - **`CircleMean`** — for directional data (e.g. wind direction).
  - **`Maximum`**, **`Minimum`**, **`Median`**.
  - **`Sum`**, **`Integ`** (integral).
  - **`StandardDeviation`**.
  - **`Percentage`**, **`Count`**.
  - **`Change`**, **`Trend`** — derivatives.
  - **`Nearest`**, **`Interpolate`** — time-direction interpolators.
- **`FunctionType`**:
  - **`TimeFunction`** — aggregate along the time axis.
  - **`AreaFunction`** — aggregate across multiple locations.
- **Aggregation intervals** — configurable, up to **7 days**.
- **`Stat`** — low-level statistical engine. Supports:
  - **Weighted** and **unweighted** stats.
  - **Circular mean** for wind directions.
  - **Missing-value handling**.

## 4. Parameter system

- **`ParameterFactory`** (singleton) — parses parameter name strings
  into `ParameterAndFunctions` structs:
  - Extracts embedded aggregation functions (e.g. `t2m:mean:1h`).
  - Resolves meteorological parameter names via
    `NFmiEnumConverter` from newbase.
- **`ParameterAndFunctions`** — parameter + inner/outer function
  pair.
- **`ParameterTools`** — classify parameters as special vs regular,
  check data independence.
- **`ParameterKeywords`** — ~85 known parameter constants (e.g.
  `LATLON_PARAM`, `SUNRISE_PARAM`, `LOCALTIME_PARAM`,
  `MOONPHASE_PARAM`).

## 5. Computed special parameters

Three dispatcher modules backed by `Fmi::FunctionMap` provide values
that aren't fetched from data but computed on the fly:

- **`TimeParameters`** — time-related values:
  - `time`, `isotime`, `xmltime`, `localtime`, `origintime`, `utc`,
    `utctime`, `epochtime`, weekday, month, etc.
  - Sun position: `sunrise`, `sunset`, `sunelevation`,
    `sundeclination`, `sunazimuth`, `dark`.
  - Moon: `moonphase`, `moonrise`, `moonset`, `moonrisetoday`,
    `moonsettoday`, etc.
- **`LocationParameters`** — `lon`, `lat`, `latlon`, `nearlatlon`,
  `longitude`, `latitude`, `name`, `iso2`, `region`, `country`,
  `elevation`.
- **`StationParameters`** — station-metadata values for observation
  queries (FMISID, station name, distance, etc.).

## 6. Output formatting

- **`TimeSeriesOutput`**:
  - **`OStreamVisitor`** — write any `TS::Value` to a stream.
  - **`StringVisitor`** — convert to a string.
- **`TableFeeder`** — fills a `Spine::Table` with time-series rows
  for plugin response generation.
- **`TableVisitor`** — visitor pattern over feed operations.
- **`TimeSeriesUtility`** — helpers for combining / splitting
  series, alignment, etc.

## 7. Request validation & filtering

- **`OptionParsers`** — extract parameter lists from HTTP requests
  (used by the timeseries / EDR plugins).
- **`RequestLimits`** — bounds on query dimensions (max locations,
  max parameters, max times, max levels).
- **`DataFilter`** — generate SQL `WHERE` clauses from filter
  expressions (used by observation queries).

## 8. Concurrency / thread safety

- **`TimeSeriesGeneratorCache`** is thread-safe.
- **`ParameterFactory`** is a thread-safe singleton.
- The data types themselves are plain-old-data and follow the usual
  "shared read-only access is fine, concurrent writes need
  synchronisation" rule.

## 9. Testing

- **~8 standalone test executables** under `test/`:
  - `DataFilterTest.cpp`
  - `LocationParametersTest.cpp`
  - `ParameterToolsTest.cpp`
  - `StatTest.cpp`
  - `TimeParametersTest.cpp`
  - `TimeSeriesAggregatorTest.cpp`
  - `TimeSeriesGeneratorTest.cpp`
  - `ValueTest.cpp`
- **Per-test build**: `cd test && make StatTest && ./StatTest`.
- **Framework**: standalone executables (not Boost.Test).
- **Sanitiser builds**:
  - `cd test && TSAN=yes make test` — thread sanitiser.
  - `cd test && ASAN=yes make test` — address sanitiser.
- **`test-installed`** target — runs the suite against the
  system-installed headers rather than the local build.

## 10. Build & integration

- **Output**: `libsmartmet-timeseries.so`.
- **Build**: `make` (consumes `smartbuildcfg`'s shared
  `makefile.inc`).
- **Format**: `make format` runs clang-format.
- **Install**: `make install` (default `PREFIX=/usr`).
- **RPM**: `make rpm`.
- **SmartMet dependencies**: `smartmet-library-macgyver`,
  `smartmet-library-spine`, `smartmet-library-newbase`.
- **External libraries**: Boost (regex), pthread.
- **Compile note**: `TimeSeriesAggregator.cpp` is built with
  `-Wno-deprecated-declarations` (and so is its test).
- **CI**: CircleCI on RHEL 8 / RHEL 10 with the
  `fmidev/smartmet-cibase-{8,10}` Docker images.
- **Public headers** installed under
  `/usr/include/smartmet/timeseries/`.

---

*Last updated: 2026-06-01.*
