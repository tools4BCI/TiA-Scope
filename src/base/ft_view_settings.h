#ifndef FT_VIEW_SETTINGS_H
#define FT_VIEW_SETTINGS_H

#include <QObject>

namespace tobiss { namespace scope {

class FTViewSettings : public QObject
{
    Q_OBJECT
public:
    explicit FTViewSettings (QObject *parent = 0);

Q_SIGNALS:

public Q_SLOTS:

};

} } // namespace tobiss scope

#endif // FT_VIEW_SETTINGS_H
