Name: gnutls
Version: 0.4.4
Release: 1
Source: ftp://ftp.gnutls.org/pub/%{name}-%{version}.tar.gz
License: LGPL
Summary: A TLS implementation.
Group: System Environment/Libraries
BuildRoot: %{_tmppath}/%{name}-%{version}-root
BuildPrereq: libgcrypt-devel
URL: http://www.gnutls.org/

%package devel
Summary: Development files for the %{name} package.
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description
The GNU TLS library implements TLS.  Someone needs to fix this description.

%description devel
The GNU TLS library implements TLS.  This package contains files needed
for developing applications with the GNU TLS library.  Someone needs to fix
this description.

%prep
%setup -q

%build
%configure --disable-openpgp-authentication --disable-libopencdktest
make

%install
rm -fr $RPM_BUILD_ROOT
%makeinstall

# XXX until %%configure is figgered
for bin in $RPM_BUILD_ROOT/%{_bindir}/* ; do
	newname=`echo ${bin} | sed 's,%{_target_platform}-,,g'`
	if test "$bin" != "$newname" ; then
		mv "$bin" "$newname"
	fi
done

%clean
rm -fr $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root)
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root)
%{_bindir}/*
%{_includedir}/*
%{_libdir}/*.a
%{_libdir}/*.la
%{_libdir}/*.so
%{_datadir}/aclocal/*

%changelog
* Tue Jun 25 2002 Jeff Johnson <jbj@redhat.com> 0.4.4-1
- update to 0.4.4.

* Fri Jun 21 2002 Tim Powers <timp@redhat.com>
- automated rebuild

* Sat May 25 2002 Jeff Johnson <jbj@redhat.com> 0.4.3-1
- update to 0.4.3.

* Tue May 21 2002 Jeff Johnson <jbj@redhat.com> 0.4.2-1
- update to 0.4.2.
- change license to LGPL.
- include splint annotations patch.

* Tue Apr  2 2002 Nalin Dahyabhai <nalin@redhat.com> 0.4.0-1
- update to 0.4.0

* Thu Jan 17 2002 Nalin Dahyabhai <nalin@redhat.com> 0.3.2-1
- update to 0.3.2

* Wed Jan 10 2002 Nalin Dahyabhai <nalin@redhat.com> 0.3.0-1
- add a URL

* Wed Dec 20 2001 Nalin Dahyabhai <nalin@redhat.com>
- initial package
