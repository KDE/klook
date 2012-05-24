#This is a spec file for Klook

Summary:	Klook is a quick preview feature
Name:		klook
Version:	0.1
Release:	95
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
%_kde_appsdir/klook/main.qml
%_kde_appsdir/klook/Button.qml
%_kde_appsdir/klook/PlayButton.qml
%_kde_appsdir/klook/Slider.qml
%_kde_appsdir/klook/AudioDelegate.qml
%_kde_appsdir/klook/FolderDelegate.qml
%_kde_appsdir/klook/ImageDelegate.qml
%_kde_appsdir/klook/MimeDelegate.qml
%_kde_appsdir/klook/OkularDelegate.qml
%_kde_appsdir/klook/TextDelegate.qml
%_kde_appsdir/klook/VideoDelegate.qml
%_kde_appsdir/klook/ControlPanel.qml
%_kde_appsdir/klook/ControlPanelButton.qml
%_kde_appsdir/klook/ScrollBar.qml
%_kde_appsdir/klook/Preview.qml
%_kde_appsdir/klook/SingleDelegate.qml
%_kde_appsdir/klook/Delegate.qml
%_kde_appsdir/klook/InfoItem.qml
%_kde_appsdir/klook/images/bg.png
%_kde_appsdir/klook/images/separator.png
%_kde_appsdir/klook/DefaultImage.qml
%_kde_appsdir/klook/images/go-next.png
%_kde_appsdir/klook/images/go-previous.png
%_kde_appsdir/klook/images/gallery.png
%_kde_appsdir/klook/images/play.png
%_kde_appsdir/klook/images/resume.png
%_kde_appsdir/klook/images/pause.png
%_kde_appsdir/klook/images/pla-empty-box.png
%_kde_appsdir/klook/images/play-empty.png
%_kde_appsdir/klook/images/close.png
%_kde_appsdir/klook/images/slider.png
%_kde_appsdir/klook/images/buttons/normal/prev_normal.png
%_kde_appsdir/klook/images/buttons/normal/next_normal.png
%_kde_appsdir/klook/images/buttons/normal/gallery_normal.png
%_kde_appsdir/klook/images/buttons/normal/open_in_normal.png
%_kde_appsdir/klook/images/buttons/normal/fullscreen_normal.png
%_kde_appsdir/klook/images/buttons/normal/close_normal.png
%_kde_appsdir/klook/images/buttons/hover/prev_hover.png
%_kde_appsdir/klook/images/buttons/hover/next_hover.png
%_kde_appsdir/klook/images/buttons/hover/gallery_hover.png
%_kde_appsdir/klook/images/buttons/hover/open_in_hover.png
%_kde_appsdir/klook/images/buttons/hover/fullscreen_hover.png
%_kde_appsdir/klook/images/buttons/hover/close_hover.png
%_kde_appsdir/klook/images/buttons/disable/prev_disable.png
%_kde_appsdir/klook/images/buttons/disable/next_disable.png
%_kde_appsdir/klook/images/buttons/disable/gallery_disable.png
%_kde_appsdir/klook/images/buttons/disable/open_in_disable.png
%_kde_appsdir/klook/images/buttons/disable/fullscreen_disable.png
%_kde_appsdir/klook/images/buttons/disable/close_disable.png
%_kde_appsdir/klook/images/buttons/press/prev_press.png
%_kde_appsdir/klook/images/buttons/press/next_press.png
%_kde_appsdir/klook/images/buttons/press/gallery_press.png
%_kde_appsdir/klook/images/buttons/press/open_in_press.png
%_kde_appsdir/klook/images/buttons/press/fullscreen_press.png
%_kde_appsdir/klook/images/buttons/press/close_press.png
%_kde_appsdir/klook/images/arrow/arrow-top.png
%_kde_appsdir/klook/images/arrow/arrow-bottom.png
%_kde_appsdir/klook/images/arrow/arrow-right.png
%_kde_appsdir/klook/images/arrow/arrow-left.png
%_kde_iconsdir/hicolor/128x128/apps/klook.png
%_kde_iconsdir/hicolor/16x16/apps/klook.png
%_kde_iconsdir/hicolor/22x22/apps/klook.png
%_kde_iconsdir/hicolor/24x24/apps/klook.png
%_kde_iconsdir/hicolor/32x32/apps/klook.png
%_kde_iconsdir/hicolor/48x48/apps/klook.png
%_kde_iconsdir/hicolor/64x64/apps/klook.png
%_kde_iconsdir/hicolor/72x72/apps/klook.png
%_kde_appsdir/klook/images/audio.png
%_kde_bindir/klook
%_kde_datadir/locale

%changelog
* Thu Mar 29 2012 Sergey Borovkov <sergey.borovkov@osinit.ru>
- Move qml files and icons to %_kde_appsdir

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
