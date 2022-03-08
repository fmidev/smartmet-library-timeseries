%define DIRNAME timeseries
%define LIBNAME smartmet-timeseries
%define SPECNAME smartmet-library-%{DIRNAME}
Summary: SmartMet Timeseries classes
Name: %{SPECNAME}
Version: 22.3.8
Release: 1%{?dist}.fmi
License: MIT
Group: BrainStorm/Development
URL: https://github.com/fmidev/smartmet-library-timeseries
Source0: %{name}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: boost169-devel
BuildRequires: ctpp2-devel
BuildRequires: fmt-devel >= 7.1.3
BuildRequires: gcc-c++
BuildRequires: imake
BuildRequires: libicu-devel
BuildRequires: make
BuildRequires: rpm-build
BuildRequires: smartmet-timezones >= 21.2.2
BuildRequires: smartmet-utils-devel >= 22.1.20
%if %{with tests}
BuildRequires: smartmet-library-regression
%endif
Requires: boost169-chrono
Requires: boost169-date-time
Requires: boost169-filesystem
Requires: boost169-regex
Requires: boost169-system
Requires: boost169-thread
Requires: ctpp2
Requires: fmt >= 7.1.3
Requires: libicu >= 50.2

#TestRequires: boost169-devel
#TestRequires: fmt-devel
#TestRequires: gcc-c++
#TestRequires: make
#TestRequires: postgresql13-libs
#TestRequires: smartmet-library-regression
#TestRequires: smartmet-timezones >= 21.2.2
#TestRequires: smartmet-utils-devel >= 22.1.20
Provides: %{SPECNAME}

%description
FMI BrainStorm Timeseries Library

%package -n %{SPECNAME}-devel
Summary: SmartMet Timeseries development files
Group: SmartMet/Development
Requires: boost169-devel
Requires: smartmet-library-macgyver-devel >= 22.3.8
Requires: smartmet-library-newbase-devel >= 21.1.21
Requires: libconfig17-devel
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

* Tue Mar 8 2022 Anssi Reponen <anssi.reponen@fmi.fi> - 22.3.8-1.fmi
- Timeseries library created (BRAINSTORM-2259)
