#This is a spec file for Klook

Summary:	Klook is a quick preview feature
Name:		klook
Version:	0.1
Release:	65
License:	GPL v.3
Group:		Graphical desktop
Source:		%{name}-%{version}.tar.gz
BuildRequires:	qt4-devel	>= 4.7.0
BuildRequires:	kdelibs4-devel	>= 4.6.5
BuildRoot:	/tmp/klook-build

%description
Klook is a quick preview feature based on Qt and Qt Quick, allows users to look at the contents of a file in the Dolphin

%prep
%setup -c

%build
%cmake_kde4

%install
%makeinstall_std -C build

%files
%{_prefix}/share/klook/main.qml
%{_prefix}/share/klook/Button.qml
%{_prefix}/share/klook/PlayButton.qml
%{_prefix}/share/klook/Slider.qml
%{_prefix}/share/klook/Delegate.qml
%{_prefix}/share/klook/ControlPanel.qml
%{_prefix}/share/klook/ControlPanelButton.qml
%{_prefix}/share/klook/ScrollBar.qml
%{_prefix}/share/klook/Preview.qml
%{_prefix}/share/klook/SingleDelegate.qml
%{_prefix}/share/klook/InfoItem.qml
%{_prefix}/share/klook/images/bg.png
%{_prefix}/share/klook/images/separator.png
%{_prefix}/share/klook/DefaultImage.qml
%{_prefix}/share/klook/images/go-next.png
%{_prefix}/share/klook/images/go-previous.png
%{_prefix}/share/klook/images/gallery.png
%{_prefix}/share/klook/images/play.png
%{_prefix}/share/klook/images/resume.png
%{_prefix}/share/klook/images/pause.png
%{_prefix}/share/klook/images/pla-empty-box.png
%{_prefix}/share/klook/images/play-empty.png
%{_prefix}/share/klook/images/close.png
%{_prefix}/share/klook/images/slider.png
%{_prefix}/share/klook/images/buttons/normal/prev_normal.png
%{_prefix}/share/klook/images/buttons/normal/next_normal.png
%{_prefix}/share/klook/images/buttons/normal/gallery_normal.png
%{_prefix}/share/klook/images/buttons/normal/open_in_normal.png
%{_prefix}/share/klook/images/buttons/normal/fullscreen_normal.png
%{_prefix}/share/klook/images/buttons/normal/close_normal.png
%{_prefix}/share/klook/images/buttons/hover/prev_hover.png
%{_prefix}/share/klook/images/buttons/hover/next_hover.png
%{_prefix}/share/klook/images/buttons/hover/gallery_hover.png
%{_prefix}/share/klook/images/buttons/hover/open_in_hover.png
%{_prefix}/share/klook/images/buttons/hover/fullscreen_hover.png
%{_prefix}/share/klook/images/buttons/hover/close_hover.png
%{_prefix}/share/klook/images/buttons/disable/prev_disable.png
%{_prefix}/share/klook/images/buttons/disable/next_disable.png
%{_prefix}/share/klook/images/buttons/disable/gallery_disable.png
%{_prefix}/share/klook/images/buttons/disable/open_in_disable.png
%{_prefix}/share/klook/images/buttons/disable/fullscreen_disable.png
%{_prefix}/share/klook/images/buttons/disable/close_disable.png
%{_prefix}/share/klook/images/buttons/press/prev_press.png
%{_prefix}/share/klook/images/buttons/press/next_press.png
%{_prefix}/share/klook/images/buttons/press/gallery_press.png
%{_prefix}/share/klook/images/buttons/press/open_in_press.png
%{_prefix}/share/klook/images/buttons/press/fullscreen_press.png
%{_prefix}/share/klook/images/buttons/press/close_press.png
%{_prefix}/share/klook/images/arrow/arrow-top.png
%{_prefix}/share/klook/images/arrow/arrow-bottom.png
%{_prefix}/share/klook/images/arrow/arrow-right.png
%{_prefix}/share/klook/images/arrow/arrow-left.png
/usr/bin/klook
/usr/share/locale

%changelog
* Thu Mar 29 2012 Sergey Borovkov <sergey.borovkov@osinit.ru>
- Move qml files and icons to /usr/share

* Tue Feb 21 2012 abf
- The release updated by ABF

* Tue Feb 21 2012 abf
- The release updated by ABF

* Mon Feb 20 2012 abf
- The release updated by ABF

* Mon Feb 20 2012 abf
- The release updated by ABF

* Mon Feb 20 2012 abf
- The release updated by ABF

* Mon Feb 20 2012 abf
- The release updated by ABF

* Mon Feb 20 2012 abf
- The release updated by ABF

* Mon Feb 20 2012 abf
- The release updated by ABF
