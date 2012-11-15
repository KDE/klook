#This is a spec file for Klook

Summary:	Quick preview feature
Name:		klook
Version:	2.0
Release:	1
License:	GPLv3
Group:		Graphical desktop/KDE
Source:		%{name}-%{version}.tar.gz
Requires: 	okular
Requires: 	okular-ooo
Requires: 	okular-chm
Requires: 	okular-pdf
Requires:	gstreamer0.10-decoders
Requires:	gstreamer0.10-vp8
BuildRequires:	qt4-devel	>= 4.7.0
BuildRequires:	kdelibs4-devel	>= 4.6.5

%description
Klook is a quick preview feature based on Qt and Qt Quick

%prep
%setup -q

%build
%cmake_kde4

%install
%makeinstall_std -C build

%files
%_kde_appsdir/klook/*
%_kde_iconsdir/hicolor/*
%_kde_bindir/klook
%_kde_datadir/locale