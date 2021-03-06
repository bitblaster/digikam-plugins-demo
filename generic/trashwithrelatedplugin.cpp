/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2019-07-30
 * Description : Hello World demo generic plugin.
 *
 * Copyright (C) 2019-2020 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "trashwithrelatedplugin.h"

// Qt includes

#include <QApplication>
#include <QMessageBox>
#include <QList>
#include <QUrl>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QThread>

#include <iostream>

// digiKam includes

#include <dinfointerface.h>
#include <iteminfo.h>
#include "dio.h"
#include "digikam_debug.h"
#include "itemviewutilities.h"

// Local includes

#include "i18nutils.h"

#define REJECTED_VAL 1

namespace DigikamGenericTrashWithRelatedPlugin
{

TrashWithRelatedPlugin::TrashWithRelatedPlugin(QObject* const parent)
    : DPluginGeneric(parent)
{
    s_initI18nResource();
    s_loadI18n(name());
}

TrashWithRelatedPlugin::~TrashWithRelatedPlugin()
{
    s_cleanupI18nResource();
}

QString TrashWithRelatedPlugin::name() const
{
    return tr("Move to trash with related");
}

QString TrashWithRelatedPlugin::iid() const
{
    return QLatin1String(DPLUGIN_IID);
}

QIcon TrashWithRelatedPlugin::icon() const
{
    //std::cout << "QIcon::themeSearchPaths: " << QIcon::themeSearchPaths().join(", ").toUtf8().constData()  << std::endl;
    return QIcon::fromTheme(QLatin1String("dk-trash-with-related"));

}

QString TrashWithRelatedPlugin::description() const
{
    return tr("A plugin to quickly delete all files related to the selected ones");
}

QString TrashWithRelatedPlugin::details() const
{
    return tr("<p>This plugin allows to delete all the filenames with the same basename of the selected files.</p>"
        "<p>For example deleting 'foo.jpg' will also be deleted all the 'foo.*' files (foo.xmp, foo.dng, etc.) whose extensions are managed by Digikam (foo.txt will NOT be deleted)></p>");
}

QList<DPluginAuthor> TrashWithRelatedPlugin::authors() const
{
    return QList<DPluginAuthor>()
            << DPluginAuthor(QString::fromUtf8("Roberto Mozzicato"),
                             QString::fromUtf8("bitblasters at gmail dot com"),
                             QString::fromUtf8("(C) 2020"))
    ;
}

void TrashWithRelatedPlugin::setup(QObject* const parent)
{
    DPluginAction* const ac = new DPluginAction(parent);
    ac->setIcon(icon());
    ac->setText(tr("Move to trash with related"));
    ac->setObjectName(QLatin1String("TrashWithRelated"));
    ac->setActionCategory(DPluginAction::GenericTool);

    connect(ac, SIGNAL(triggered(bool)),
            this, SLOT(slotTrashWithRelated()));

    addAction(ac);
}

// REMOVE!
// #define qCDebug qCInfo


void TrashWithRelatedPlugin::slotTrashWithRelated()
{
    DInfoInterface* iface = infoIface(sender());
    QList<QUrl> images          = iface->currentSelectedItems();

    if (images.isEmpty())
        images  = iface->currentAlbumItems();

    if (!images.isEmpty())
    {
        QStringList items;
        QWidget* const w      = qApp->activeWindow();

        // If we are in the Light Table, iface->currentSelectedItems() returns always ALL the items, not the selected one.
        // However in the main app window iface->currentSelectedItems() returns just the selected items.
        // So we have to use a trick: if we are in the Light Table first we dinamically invoke slotAssignPickLabel() to 
        // flag the selected item as rejected, then we cycle over the items and process only those flagged as rejected.
        if (w && !strcmp("Digikam::LightTableWindow", w->metaObject()->className()))
        {
            qCDebug(DIGIKAM_GENERAL_LOG) << "Trash with related inside Light Table";
            
            // Dynamically invoke the public method slotAssignPickLabel
            QMetaObject::invokeMethod( w, "slotDeleteItem", Qt::DirectConnection);
            QUrl deletedItem;
            int count = 0;
            while(deletedItem.isEmpty() && count < 20) 
            {
                QThread::msleep(100);
                foreach (const QUrl& url, images)
                {
                    QFileInfo tmpFile(url.toLocalFile());
                    if(!tmpFile.exists())
                    {
                        deletedItem = url;
                    }
                }
                count++;
            }
            
            if(deletedItem.isEmpty())
            {
                QMessageBox::critical(qApp->activeWindow(), tr("Error"),
                    tr("Problems detecting deleted item, aborting deletion of related items"));
                return;
            }
            qCDebug(DIGIKAM_GENERAL_LOG) << "Detected deleted item: " << deletedItem.toLocalFile();
            images.clear();
            images.push_back(deletedItem);
        }
        
        QSet<ItemInfo> infos;
        foreach (const QUrl& url, images)
        {
            QFileInfo tmpFile(url.toLocalFile());
            QDir tmpDir = tmpFile.dir();
            QString baseName = QFileInfo(tmpFile.fileName()).completeBaseName() + ".*";
            QFileInfoList fileList = tmpDir.entryInfoList(QStringList(baseName));
            foreach (const QFileInfo& fileToDelete, fileList)
            {   
                //std::cout << "Deleting related item: " << fileToDelete.canonicalFilePath().toUtf8().constData() << std::endl;
                if (fileToDelete.exists() && fileToDelete.isWritable()) {
                    const ItemInfo &info = ItemInfo::fromLocalFile(fileToDelete.canonicalFilePath());
                    if(!infos.contains(info)) {
                        qCDebug(DIGIKAM_GENERAL_LOG) << "Deleting related item: " << fileToDelete.canonicalFilePath();
                        infos << info; // Here we are forced to use ItemInfo
                    }
                }
            }
        }        

        if(!infos.empty())
        {
            qCDebug(DIGIKAM_GENERAL_LOG) << "Executing deletion";
            DIO::del(infos.toList(), true);
        }
    }
}

} // namespace DigikamGenericTrashWithRelatedPlugin
