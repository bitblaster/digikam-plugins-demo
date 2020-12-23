# launch from INSIDE build

mkdir -p appimage-pack/usr/share/icons/hicolor/16x16/apps
mkdir -p appimage-pack/usr/share/icons/hicolor/22x22/apps
mkdir -p appimage-pack/usr/share/icons/hicolor/32x32/apps
mkdir -p appimage-pack/usr/share/icons/hicolor/48x48/apps
mkdir -p appimage-pack/usr/share/icons/hicolor/scalable/apps
mkdir -p appimage-pack/usr/plugins/digikam/generic

cp ../generic/icons/16-apps-dk-trash-with-related.png  appimage-pack/usr/share/icons/hicolor/16x16/apps/dk-trash-with-related.png
cp ../generic/icons/22-apps-dk-trash-with-related.png  appimage-pack/usr/share/icons/hicolor/22x22/apps/dk-trash-with-related.png
cp ../generic/icons/32-apps-dk-trash-with-related.png  appimage-pack/usr/share/icons/hicolor/32x32/apps/dk-trash-with-related.png
cp ../generic/icons/48-apps-dk-trash-with-related.png  appimage-pack/usr/share/icons/hicolor/48x48/apps/dk-trash-with-related.png
cp ../generic/icons/sc-apps-dk-trash-with-related.svgz appimage-pack/usr/share/icons/hicolor/scalable/apps/dk-trash-with-related.svgz
cp generic/Generic_TrashWithRelated_Plugin.so appimage-pack/usr/plugins/digikam/generic/Generic_TrashWithRelated_Plugin.so
