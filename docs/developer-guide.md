# timeseries library developer guide

This guide is for developers who change `smartmet-library-timeseries`, or use it in a
plugin or engine. The library defines the time series data types, the syntax of
parameters with aggregation functions, the generation of output times, the aggregation
itself, and output helpers, all shared by the timeseries, edr, wfs and wms plugins and
by the querydata and observation engines.

[CLAUDE.md](../CLAUDE.md) has an architecture summary and [FEATURES.md](../FEATURES.md)
the feature inventory. The user-level request syntax is documented in the timeseries
plugin's [Using-the-Timeseries-API.md](https://github.com/fmidev/smartmet-plugin-timeseries/blob/master/docs/Using-the-Timeseries-API.md).

## Contents

1. [Building and testing](#1-building-and-testing)
2. [Source files](#2-source-files)
3. [Data types](#3-data-types)
4. [Parameters and aggregation functions](#4-parameters-and-aggregation-functions)
5. [Generating the output times](#5-generating-the-output-times)
6. [Aggregation](#6-aggregation)
7. [Computed parameters](#7-computed-parameters)
8. [Data filters](#8-data-filters)
9. [Request limits](#9-request-limits)
10. [Output](#10-output)
11. [Compatibility](#11-compatibility)
12. [Known pitfalls](#12-known-pitfalls)

---

## 1. Building and testing

```bash
make                       # libsmartmet-timeseries.so
make test                  # the Boost.Test programs in test/
cd test && make StatTest && ./StatTest
cd test && TSAN=yes make test
cd test && ASAN=yes make test
make -C test test-installed   # against the installed headers and library
```

The library depends on macgyver, spine and newbase (for `NFmiEnumConverter`).

## 2. Source files

| File | Contents |
|------|----------|
| `TimeSeries.{h,cpp}`, `TimeSeriesTypes.h`, `TimeSeriesInclude.h` | `Value`, `TimedValue`, `TimeSeries`, the group and vector types. |
| `ParameterFactory.{h,cpp}`, `ParameterKeywords.h`, `ParameterTools.{h,cpp}` | Parsing parameter strings with functions; the known special parameter names. |
| `DataFunction.{h,cpp}` | One aggregation function with its type, limits and intervals. |
| `TimeSeriesGenerator*.{h,cpp}` | Output times from the time options, with a cache. |
| `OptionParsers.{h,cpp}` | Reading parameters and time options from an HTTP request. |
| `TimeSeriesAggregator.{h,cpp}`, `Stat.{h,cpp}` | Time and area aggregation; the statistics. |
| `TimeParameters`, `LocationParameters`, `StationParameters` | Values computed from the time, the location or the station (sunrise, elevation, station metadata, …). |
| `DataFilter.{h,cpp}` | Value filters turned into SQL clauses (observations). |
| `RequestLimits.{h,cpp}` | Limits on locations, parameters, times, levels and elements. |
| `TimeSeriesOutput`, `TableFeeder`, `TableVisitor`, `TimeSeriesUtility` | Writing results into `Spine::Table`s and streams. |

## 3. Data types

Namespace `SmartMet::TimeSeries`, usually aliased as `TS`.

| Type | Meaning |
|------|---------|
| `Value` | A `std::variant` of `None`, `std::string`, `double`, `int`, `LonLat` and `Fmi::LocalDateTime`, with `as_double()` / `as_int()`. `None` is a missing value. |
| `TimedValue` | A `Fmi::LocalDateTime` and a `Value`. |
| `TimeSeries` | `std::vector<TimedValue>`: one parameter at one place. |
| `TimeSeriesVector` | One `TimeSeries` per parameter. |
| `LonLatTimeSeries`, `TimeSeriesGroup` | A coordinate with its series; the series of all points of an area. |
| `TimeSeriesVectorPtr`, `TimeSeriesGroupPtr`, `TimeSeriesPtr` | The shared pointers the engines return. |

The times are **local times** in the requested time zone. Engines therefore generate
their series with the time zone of the request, and aggregation works on those local
times.

## 4. Parameters and aggregation functions

`ParameterFactory::instance().parseNameAndFunctions(name)` turns one entry of `param=`
into a `Spine::Parameter` and a pair of `DataFunctions`:

```
Temperature                          plain parameter
Temperature as T                     with an output alias (" as ")
mean_t(Temperature/3h/0h)            time aggregation: 3 h behind, 0 h ahead
max_a(Temperature)                   area aggregation (over the points of an area)
max_a(mean_t(Temperature/24h))       inner function over time, outer function over the area
percentage_t[0:5](Temperature/24h)   with value limits [lower:upper]
nanmean_t(Temperature/6h)            "nan" prefix: missing values are ignored, not propagated
date(%Y-%m-%d)                       formatted time
```

* A function name ends in **`_t`** (over time) or **`_a`** (over an area).
* Functions: `mean`, `amean`, `max`, `min`, `median`, `sum`, `integ`, `sdev`, `percentage`,
  `count`, `change`, `trend`, `circlemean` (all with a `nan` variant), and the time-only
  `nearest`, `interpolate`, `interpolatedir`, `meandir`, `sdevdir`.
* The **intervals** follow the parameter name, separated by `/`, `;` or `:`: the first is
  the interval behind the time step, the second (default 0) the interval ahead. They are
  durations (`3h`, `90m`, `1d`). Without intervals in the name, the interval is left
  **unset**, marked by `std::numeric_limits<unsigned int>::max()`, and the caller must
  fill it in: the timeseries plugin uses the request's `interval=behind[:ahead]` option,
  default 0. (A `DataFunction` constructed directly defaults to 7 days,
  `MAX_AGGREGATION_INTERVAL`.)
* Two functions of the same kind (both `_t` or both `_a`) collapse to one: the outer one is
  dropped.
* Sensor information may be embedded in the innermost parentheses, and a `qc_` prefix
  selects the quality-control parameter.

Parameter names are lower-cased for matching, but the original spelling is kept as the
output name. Newbase parameter names are resolved with `NFmiEnumConverter`; the factory
holds one converter for the whole process, because constructing it builds the entire
name table.

## 5. Generating the output times

`parseTimes(request)` (`TimeSeriesGeneratorOptions.h`) fills `TimeSeriesGeneratorOptions` from `starttime`,
`endtime`, `startstep`, `timestep`, `timesteps`, `hour`, `day` and `time`, and
`TimeSeriesGenerator::generate(options, timezone)` turns them into a list of local times:

| Mode | Set by | Times |
|------|--------|-------|
| `TimeSteps` (default) | `timestep=60` (or `1h`) | Every `timestep` minutes from `starttime`, `timesteps` of them or up to `endtime`. |
| `DataTimes` | `timestep=data` or `all` | The times present in the data. |
| `GraphTimes` | `timestep=graph` | The data times plus "now". |
| `FixedTimes` | `hour=`, `day=`, `time=` | The listed times of day, days and times in the range. |

`starttime=data` / `endtime=data` take the start or end from the data. Mixing `timestep`
with an option that implies another mode is an error. The results are cached
(`TimeSeriesGeneratorCache`), since the same options are generated for every location.

## 6. Aggregation

`TimeSeriesAggregator::aggregate(series, functions)` applies the `DataFunctions`:

* **time functions** use, for each output time `t`, the values in
  `[t − behind, t + ahead]`. The caller must fetch data for the correspondingly extended
  period (the timeseries plugin extends its query by the largest interval), or the first
  and last output times have partial intervals;
* **area functions** combine the values of all points of a `TimeSeriesGroup` at each
  time, and produce one series;
* with both, the inner (time) function runs first for every point, then the outer (area)
  function over the points.

`Stat` does the arithmetic. When the values come with timestamps it computes
**time-weighted** statistics (unless `useWeights(false)` is called), with circular statistics for directions (`circlemean`, `meandir`,
`sdevdir`). Without the `nan` prefix, one missing value makes the result missing; with it,
missing values are skipped.

## 7. Computed parameters

Some parameters are not read from data but computed:

* **`TimeParameters`**: time strings, epoch, sun and moon (sunrise, sunset, day length,
  moon phase), and the like, from the time and the location;
* **`LocationParameters`**: name, coordinates, elevation, region, country, time zone, and
  so on, from the `Spine::Location`;
* **`StationParameters`**: station identifiers, names, distance and direction to the
  station, from the observation station.

`ParameterTools` tells which parameter names are special in this sense
(`ParameterKeywords.h` lists them) and whether a parameter depends on the data at all.

## 8. Data filters

`DataFilter` turns filter expressions into SQL `WHERE` clauses for the observation
engine's queries:

```
"123"                   ->  (x = 123)
"lt 5", "ge 10"         ->  (x < 5), (x >= 10)
"ge 1 AND lt 9"         ->  (x < 9 AND x >= 1)
"lt 5 OR ge 10"         ->  (x < 5 OR x >= 10)
```

Operators are `lt`, `le`, `eq`, `ge`, `gt`, combined with `AND` / `OR`. Filters are
stored per name (`data_quality`, `station_id`, …) and applied to a database field with
`getSqlClause(name, dbfield)`.

## 9. Request limits

`RequestLimits` holds `maxlocations`, `maxparameters`, `maxtimes`, `maxlevels`,
`maxelements` and `maxradius`. `check_request_limit(limits, amount, member)` throws when an
amount exceeds its limit. **0 means unlimited.** The plugins read the limits from their
configuration and check them before querying.

## 10. Output

`TableFeeder` writes values into a `Spine::Table` column by column, applying the
`Fmi::ValueFormatter` (precision, missing text) and the time formatter. The
`TimeSeriesOutput` visitors print series for debugging and tests.

## 11. Compatibility

The types here appear in the public interfaces of the querydata and observation engines
and in the plugins. A change to `Value`'s variant, `TimeSeries`, `DataFunction`,
`TimeSeriesGeneratorOptions` or `Spine::Parameter` changes the ABI of all of them. Bump
the version, and rebuild and release the engines and plugins together.

## 12. Known pitfalls

* **Unset intervals are a sentinel.** A time function parsed without intervals carries
  `UINT_MAX` as its interval. Code that uses the library outside the timeseries plugin must
  replace it (as `Query::parse_aggregation_intervals()` there does) before aggregating or
  extending the data period.
* **Missing values propagate** unless the `nan` variant of a function is used.
* **Same-kind nesting is silently simplified.** `max_t(mean_t(T/3h))` keeps only one of
  the two time functions.
* **Local times.** Series are in the request's time zone; daylight-saving changes give
  23- and 25-hour days, which affects hourly aggregation around the change.
* **`NFmiEnumConverter` is expensive to construct.** Never create one per call; use
  `ParameterFactory` or a shared instance.
