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

#ifndef DIGIKAM_TRASH_WITH_RELATED_GENERIC_PLUGIN_H
#define DIGIKAM_TRASH_WITH_RELATED_GENERIC_PLUGIN_H

// Local includes

#include "dplugingeneric.h"

#define DPLUGIN_IID "it.bitblaster.digikam.plugin.generic.TrashWithRelated"

using namespace Digikam;

namespace DigikamGenericTrashWithRelatedPlugin
{

class TrashWithRelatedPlugin : public DPluginGeneric
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DPLUGIN_IID)
    Q_INTERFACES(Digikam::DPluginGeneric)

public:

    explicit TrashWithRelatedPlugin(QObject* const parent = nullptr);
    ~TrashWithRelatedPlugin();

    QString name()                 const override;
    QString iid()                  const override;
    QIcon   icon()                 const override;
    QString details()              const override;
    QString description()          const override;
    QList<DPluginAuthor> authors() const override;

    void setup(QObject* const)           override;

private Q_SLOTS:

    void slotTrashWithRelated();
};

} // namespace DigikamGenericTrashWithRelatedPlugin

#endif // DIGIKAM_TRASH_WITH_RELATED_GENERIC_PLUGIN_H
