%define DIRNAME timeseries
%define LIBNAME smartmet-timeseries
%define SPECNAME smartmet-library-%{DIRNAME}
Summary: SmartMet Timeseries classes
Name: %{SPECNAME}
Version: 22.3.18
Release: 1%{?dist}.fmi
License: MIT
Group: BrainStorm/Development
URL: https://github.com/fmidev/smartmet-library-timeseries
Source0: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: boost169-devel
BuildRequires: gcc-c++
BuildRequires: imake
BuildRequires: make
BuildRequires: rpm-build
BuildRequires: smartmet-utils-devel >= 22.2.8
BuildRequires: smartmet-library-macgyver-devel >= 22.3.8
BuildRequires: smartmet-library-newbase-devel >= 22.1.21
BuildRequires: smartmet-library-spine-devel >= 22.3.18
%if %{with tests}
BuildRequires: smartmet-library-regression
%endif
Requires: boost169-date-time
Requires: boost169-system
Requires: boost169-thread
Requires: smartmet-library-spine >= 22.3.18
Requires: ctpp2

#TestRequires: boost169-devel
#TestRequires: gcc-c++
#TestRequires: make
#TestRequires: smartmet-library-regression
#TestRequires: smartmet-utils-devel >= 22.2.8
#TestRequires: smartmet-library-spine-devel >= 22.3.18
#TestRequires: smartmet-timezones
Provides: %{SPECNAME}

%description
FMI BrainStorm Timeseries Library

%package -n %{SPECNAME}-devel
Summary: SmartMet Timeseries development files
Group: SmartMet/Development
Requires: boost169-devel
Requires: smartmet-library-macgyver-devel >= 22.3.8
Requires: smartmet-library-newbase-devel >= 22.1.21
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
* Fri Mar 18 2022 Andris Pavēnis <andris.pavenis@fmi.fi> 22.3.18-1.fmi
- Import more code from smartmet-library-spine and require spine to be installed

* Thu Mar 10 2022 Mika Heiskanen <mika.heiskanen@fmi.fi> - 22.3.10-1.fmi
- Removed incorrect dependencies

* Tue Mar 8 2022 Anssi Reponen <anssi.reponen@fmi.fi> - 22.3.8-1.fmi
- Timeseries library created (BRAINSTORM-2259)
