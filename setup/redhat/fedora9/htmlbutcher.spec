Summary: HTMLButcher is an advanced HTML slicing application
Name: htmlbutcher
Version: 1.1.0.18
Release: fedora9
License: Commercial
Group: Applications/Internet
BuildRoot: /var/tmp/%{name}-buildroot

%description
HTMLButcher - HTML slicing application
HTMLButcher is an advanced HTML slicing tool. It is used to create 
an HTML page from an image file, slicing images and creating an HTML table, 
preserving  the layout but allowing data and images to be individually changed.

%prep
#%setup -q
#%patch -p1 -b .buildroot

#%build
#make RPM_OPT_FLAGS="$RPM_OPT_FLAGS"

#%install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
#%doc ../../../README ../../../COPYING ../../../ChangeLog

/usr/bin/htmlbutcher
/usr/share/pixmaps/htmlbutcher.xpm 
/usr/share/applications/htmlbutcher.desktop 
/usr/share/application-registry/htmlbutcher.applications 
/usr/share/mime-info/htmlbutcher.keys 
/usr/share/mime-info/htmlbutcher.mime 
/usr/share/mime/packages/htmlbutcher.xml
/usr/share/htmlbutcher/htmlbutcher.htb 

%post
# update mime database for HTMLButcher mime types
update-mime-database /usr/share/mime
update-desktop-database -q

%postun
# update mime database for HTMLButcher mime types
update-mime-database /usr/share/mime
update-desktop-database -q

#%changelog
