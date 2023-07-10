%define DIRNAME timeseries
%define LIBNAME smartmet-timeseries
%define SPECNAME smartmet-library-%{DIRNAME}
Summary: SmartMet Timeseries classes
Name: %{SPECNAME}
Version: 23.6.15
Release: 1%{?dist}.fmi
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
BuildRequires: smartmet-utils-devel >= 23.4.28
BuildRequires: smartmet-library-macgyver-devel >= 23.6.6
BuildRequires: smartmet-library-newbase-devel >= 23.7.10
BuildRequires: smartmet-library-spine-devel >= 23.7.10
%if %{with tests}
BuildRequires: smartmet-library-regression
%endif
Requires: %{smartmet_boost}-date-time
Requires: %{smartmet_boost}-system
Requires: %{smartmet_boost}-thread
Requires: smartmet-library-spine >= 23.7.10
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
Requires: smartmet-library-macgyver-devel >= 23.6.6
Requires: smartmet-library-newbase-devel >= 23.7.10
Requires: smartmet-library-spine-devel >= 23.7.10
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
