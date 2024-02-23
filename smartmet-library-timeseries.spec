%define DIRNAME timeseries
%define LIBNAME smartmet-timeseries
%define SPECNAME smartmet-library-%{DIRNAME}
Summary: SmartMet Timeseries classes
Name: %{SPECNAME}
Version: 24.2.23
Release: 2%{?dist}.fmi
License: MIT
Group: BrainStorm/Development
URL: https://github.com/fmidev/smartmet-library-timeseries
Source0: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%if 0%{?rhel} && 0%{rhel} < 9
%define smartmet_boost boost169
%else
%define smartmet_boost boost
%endif

BuildRequires: %{smartmet_boost}-devel
BuildRequires: gcc-c++
BuildRequires: imake
BuildRequires: make
BuildRequires: rpm-build
BuildRequires: smartmet-utils-devel >= 23.9.6
BuildRequires: smartmet-library-macgyver-devel >= 24.1.17
BuildRequires: smartmet-library-newbase-devel >= 24.1.30
BuildRequires: smartmet-library-spine-devel >= 24.2.8
%if %{with tests}
BuildRequires: smartmet-library-regression
%endif
Requires: %{smartmet_boost}-date-time
Requires: %{smartmet_boost}-system
Requires: %{smartmet_boost}-thread
Requires: smartmet-library-spine >= 24.2.8
Requires: ctpp2

#TestRequires: %{smartmet_boost}-devel
#TestRequires: gcc-c++
#TestRequires: make
#TestRequires: smartmet-library-regression
#TestRequires: smartmet-utils-devel
#TestRequires: smartmet-library-spine-devel
#TestRequires: smartmet-timezones
Provides: %{SPECNAME}

%description
FMI BrainStorm Timeseries Library

%package -n %{SPECNAME}-devel
Summary: SmartMet Timeseries development files
Group: SmartMet/Development
Requires: %{smartmet_boost}-devel
Requires: smartmet-library-macgyver-devel >= 24.1.17
Requires: smartmet-library-newbase-devel >= 24.1.30
Requires: smartmet-library-spine-devel >= 24.2.8
Requires: %{SPECNAME} = %{version}-%{release}
%description -n %{SPECNAME}-devel
SmartMet Timeseries development files

%prep
rm -rf $RPM_BUILD_ROOT

%setup -q -n %{SPECNAME}

%build
make %{_smp_mflags}

%install
%makeinstall

%clean
# rm -rf $RPM_BUILD_ROOT

%files
%defattr(0755,root,root,0755)
%{_libdir}/lib%{LIBNAME}.so

%files -n %{SPECNAME}-devel
%defattr(0644,root,root,0755)
%{_includedir}/smartmet/%{DIRNAME}

%changelog
* Fri Feb 23 2024 Mika Heiskanen <mheiskan@rhel8.dev.fmi.fi> - 24.2.23-2.fmi
- Fixed push_back not to use emplace_back

* Fri Feb 23 2024 Mika Heiskanen <mheiskan@rhel8.dev.fmi.fi> - 24.2.23-1.fmi
- Full repackaging

* Tue Feb 20 2024 Mika Heiskanen <mheiskan@rhel8.dev.fmi.fi> - 24.2.20-1.fmi
- Fixed misleadingly named emplace_back to push_back (BRAINSTORM-2861)

* Tue Jan 30 2024 Mika Heiskanen <mika.heiskanen@fmi.fi> 24.1.30-1.fmi
- Repackaged due to newbase ABI changes

* Fri Jan 26 2024 Mika Heiskanen <mika.heiskanen@fmi.fi> - 24.1.26-1.fmi
- Added station metadata related parameters to list of location independent parameters

* Thu Jan  4 2024 Mika Heiskanen <mika.heiskanen@fmi.fi> - 24.1.4-1.fmi
- Added option 'day' for timeseries generation (BRAINSTORM-2826)

* Tue Nov 21 2023 Mika Heiskanen <mika.heiskanen@fmi.fi> - 23.11.21-1.fmi
- Fixed the "sdev" to return sample standard deviation instead of population standard deviation

* Thu Nov 16 2023 Mika Heiskanen <mika.heiskanen@fmi.fi> - 23.11.16-1.fmi
- Added maxradius request limit

* Mon Oct 30 2023 Anssi Reponen <anssi.reponen@fmi.fi> - 23.10.30-1.fmi
- Added stationtype in list of special parameters (BRAINSTORM-2756)

* Fri Oct 20 2023 Anssi Reponen <anssi.reponen@fmi.fi> - 23.10.20-1.fmi
- Added amean_t function (BRAINSTORM-2575)

* Wed Oct 11 2023 Anssi Reponen <anssi.reponen@fmi.fi> - 23.10.11-2.fmi
- Moved aggregation related functions here from timeseries/edr plugin (BRAINSTORM-2457)

* Wed Oct 11 2023 Mika Heiskanen <mika.heiskanen@fmi.fi> - 23.10.11-1.fmi
- Loosen the required syntax for defining a parameter alias by allowing ")as name"

* Thu Oct 5 2023 Anssi Reponen <anssi.reponen@fmi.fi> - 23.10.5-1.fmi
- Fixed parameter name parsing bug (BRAINSTORM-2743)

* Mon Sep 18 2023 Mika Heiskanen <mika.heiskanen@fmi.fi> - 23.9.18-1.fmi
- Refactored code for simplicity

* Tue Sep 12 2023 Anssi Reponen <anssi.reponen@fmi.fi> - 23.9.12-1.fmi
- Refactored the code (BRAINSTORM-2694)

* Fri Jul 28 2023 Andris Pavēnis <andris.pavenis@fmi.fi> 23.7.28-1.fmi
- Repackage due to bulk ABI changes in macgyver/newbase/spine

* Mon Jul 10 2023 Andris Pavēnis <andris.pavenis@fmi.fi> 23.7.10-1.fmi
- Use postgresql 15, gdal 3.5, geos 3.11 and proj-9.0

* Thu Jun 15 2023 Mika Heiskanen <mika.heiskanen@fmi.fi> - 23.6.15-1.fmi
- Changed status of station_elevation so that is read from observation metadata instead of the geonames database

* Wed Mar 15 2023 Mika Heiskanen <mika.heiskanen@fmi.fi> - 23.3.15-1.fmi
- Added interpolatedir_t, meandir_t and stddevdir_t

* Tue Mar 14 2023 Anssi Reponen <mika.heiskanen@fmi.fi> - 23.3.14-1.fmi
- Added new metaparameters: CloudCeiling,CloudCeilingFT,CloudCeilingHFT (BRAINSTORM-2556)

* Thu Feb 16 2023 Anssi Reponen <anssi.reponen@fmi.fi> - 23.2.16-1.fmi
- Added utility function to print out TimeSeries::OutputData data structure

* Tue Jan 31 2023 Anssi Reponen <anssi.reponen@fmi.fi> - 23.1.31-1.fmi
- Fixed aggregation function parsing bug (BRAINSTORM-2529)

* Thu Jan 26 2023 Anssi Reponen <anssi.reponen@fmi.fi> - 23.1.26-1.fmi
- Added support for request size limits (BRAINSTORM-2443)

* Tue Oct 25 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.10.25-1.fmi
- Fixed parameter name comparisons to use lower case

* Tue Oct 18 2022 Anssi Reponen <anssi.reponen@fmi.fi> - 22.10.18-1.fmi
- Fixed parameter name mapping bug (BRAINSTORM-2433)

* Wed Oct 12 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.10.12-1.fmi
- Return the original timezone name for 'tz' instead of the Boost 'standard name' which is +0545 for Asia/Kathmandu

* Mon Oct 10 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.10.10-1.fmi
- Improved keyword listing

* Wed Oct 5 2022 Anssi Reponen <anssi.reponen@fmi.fi> - 22.10.5-1.fmi
- Fixed tz-parameter return value (BRAINSTORM-2420)

* Tue Oct  4 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.10.4-1.fmi
- Use TimeFormatter for sunrise etc parameters

* Fri Sep  9 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.9.9-1.fmi
- Silenced several compiler warnings, some on unnecessary copying

* Wed Jul 27 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.7.27-1.fmi
- Repackaged since macgyver CacheStats ABI changed

* Thu Jun 16 2022 Andris Pavēnis <andris.pavenis@fmi.fi> 22.6.16-1.fmi
- Add support of HEL9, upgrade to libpqxx-7.7.0 (rhel8+) and fmt-8.1.1

* Tue May 24 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.5.24-1.fmi
- Repackaged due to NFmiArea ABI changes

* Fri May 20 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.5.20-1.fmi
- Repackaged due to newbase ABI changes to LatLon methods

* Fri Mar 18 2022 Andris Pavēnis <andris.pavenis@fmi.fi> 22.3.18-1.fmi
- Import more code from smartmet-library-spine and require spine to be installed

* Thu Mar 10 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.3.10-1.fmi
- Removed incorrect dependencies

* Tue Mar 8 2022 Anssi Reponen <anssi.reponen@fmi.fi> - 22.3.8-1.fmi
- Timeseries library created (BRAINSTORM-2259)
