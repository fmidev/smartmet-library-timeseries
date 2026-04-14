# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

`smartmet-library-timeseries` — a C++17 shared library providing time series data structures and statistical operations for meteorological data. Part of FMI's SmartMet Server ecosystem. Consumed primarily by `smartmet-plugin-timeseries` and `smartmet-engine-observation`.

## Build commands

```bash
make                  # Build libsmartmet-timeseries.so
make test             # Build and run all tests
make format           # Apply clang-format
make clean            # Clean build artifacts
make rpm              # Build RPM package
make install          # Install library and headers (PREFIX=/usr)
```

Run a single test:

```bash
cd test && make StatTest && ./StatTest
```

Run tests with sanitizers:

```bash
cd test && TSAN=yes make test    # ThreadSanitizer
cd test && ASAN=yes make test    # AddressSanitizer
```

## Dependencies

Build requires `smartmet-library-macgyver`, `smartmet-library-spine`, and `smartmet-library-newbase` (for `NFmiEnumConverter` used by `ParameterFactory`). Also links against `boost_regex` and `pthread`.

The build system loads shared configuration from `smartbuildcfg`:
```makefile
include $(shell echo $${PREFIX-/usr})/share/smartmet/devel/makefile.inc
```

## Architecture

**Namespace:** `SmartMet::TimeSeries` (commonly aliased as `TS`). The umbrella header is `TimeSeriesInclude.h`.

### Core type hierarchy

`Value_` is a `std::variant<None, string, double, int, LonLat, LocalDateTime>`. `Value` wraps it and adds `as_double()` / `as_int()` conversion methods. From there:

- `TimedValue` — pairs `Fmi::LocalDateTime` with a `Value`
- `TimeSeries` — inherits `std::vector<TimedValue>`, the fundamental data container
- `LonLatTimeSeries` — a coordinate + `TimeSeries` pair (one location's data)
- `TimeSeriesGroup` — `std::vector<LonLatTimeSeries>` (area queries across multiple locations)
- `TimeSeriesVector` — `std::vector<TimeSeries>` (multiple parameters at one location)

### Data flow: generation -> aggregation -> output

1. **TimeSeriesGenerator** produces timestamp lists (`LocalTimeList`) from `TimeSeriesGeneratorOptions` (start/end times, step, mode: DataTimes/GraphTimes/FixedTimes/TimeSteps). Results are cached in `TimeSeriesGeneratorCache`.

2. **TimeSeriesAggregator** applies `DataFunctions` (inner + outer function pair) over time series data. Functions are defined by `FunctionId` (Mean, Max, Min, Median, Sum, Integ, StdDev, Percentage, Count, Change, Trend, Nearest, Interpolate, CircleMean). Functions have a type (`TimeFunction` or `AreaFunction`) and optional aggregation intervals (max 7 days).

3. **Stat** implements the actual statistical computations — supports weighted/unweighted stats and circular mean for wind directions.

4. **TimeSeriesOutput** (`OStreamVisitor`, `StringVisitor`) and **TableFeeder** format results for HTTP responses.

### Parameter system

- **ParameterFactory** (singleton) parses parameter name strings, extracts embedded aggregation functions, and returns `ParameterAndFunctions` structs. Uses `NFmiEnumConverter` from newbase for meteorological parameter name resolution.
- **ParameterTools** classifies parameters as special vs. regular and checks data independence.
- **TimeParameters**, **LocationParameters**, **StationParameters** provide computed values (sunrise/sunset, moon phases, elevation, station metadata, etc.) using `Fmi::FunctionMap` dispatch.
- **ParameterKeywords** defines ~85 known parameter name constants.

### Other components

- **DataFilter** — generates SQL WHERE clauses from filter expressions for observation queries.
- **OptionParsers** — extracts parameter lists from HTTP requests.
- **RequestLimits** — enforces bounds on query dimensions (locations, parameters, times, levels).

## Notes

- `TimeSeriesAggregator.cpp` is compiled with `-Wno-deprecated-declarations` (same for its test).
- Tests are individual executables (not Boost.Test), each `*Test.cpp` compiles to a separate binary.
- The `test-installed` make target tests against system-installed headers/libraries rather than the local build.
