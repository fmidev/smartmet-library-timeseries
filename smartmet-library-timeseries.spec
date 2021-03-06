%define DIRNAME timeseries
%define LIBNAME smartmet-timeseries
%define SPECNAME smartmet-library-%{DIRNAME}
Summary: SmartMet Timeseries classes
Name: %{SPECNAME}
Version: 22.6.16
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
BuildRequires: smartmet-utils-devel >= 22.2.8
BuildRequires: smartmet-library-macgyver-devel >= 22.6.16
BuildRequires: smartmet-library-newbase-devel >= 22.6.16
BuildRequires: smartmet-library-spine-devel >= 22.6.16
%if %{with tests}
BuildRequires: smartmet-library-regression
%endif
Requires: %{smartmet_boost}-date-time
Requires: %{smartmet_boost}-system
Requires: %{smartmet_boost}-thread
Requires: smartmet-library-spine >= 22.6.16
Requires: ctpp2

#TestRequires: %{smartmet_boost}-devel
#TestRequires: gcc-c++
#TestRequires: make
#TestRequires: smartmet-library-regression
#TestRequires: smartmet-utils-devel >= 22.2.8
#TestRequires: smartmet-library-spine-devel >= 22.6.16
#TestRequires: smartmet-timezones
Provides: %{SPECNAME}

%description
FMI BrainStorm Timeseries Library

%package -n %{SPECNAME}-devel
Summary: SmartMet Timeseries development files
Group: SmartMet/Development
Requires: %{smartmet_boost}-devel
Requires: smartmet-library-macgyver-devel >= 22.6.16
Requires: smartmet-library-newbase-devel >= 22.6.16
Requires: smartmet-library-spine-devel
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
* Thu Jun 16 2022 Andris Pav??nis <andris.pavenis@fmi.fi> 22.6.16-1.fmi
- Add support of HEL9, upgrade to libpqxx-7.7.0 (rhel8+) and fmt-8.1.1

* Tue May 24 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.5.24-1.fmi
- Repackaged due to NFmiArea ABI changes

* Fri May 20 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.5.20-1.fmi
- Repackaged due to newbase ABI changes to LatLon methods

* Fri Mar 18 2022 Andris Pav??nis <andris.pavenis@fmi.fi> 22.3.18-1.fmi
- Import more code from smartmet-library-spine and require spine to be installed

* Thu Mar 10 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.3.10-1.fmi
- Removed incorrect dependencies

* Tue Mar 8 2022 Anssi Reponen <anssi.reponen@fmi.fi> - 22.3.8-1.fmi
- Timeseries library created (BRAINSTORM-2259)
