#ifndef QTMATERIALCHECKBOX_P_H
#define QTMATERIALCHECKBOX_P_H

#include "components/lib/qtmaterialcheckable_p.h"

class QtMaterialCheckBox;

class QtMaterialCheckBoxPrivate : public QtMaterialCheckablePrivate
{
    Q_DISABLE_COPY(QtMaterialCheckBoxPrivate)
    Q_DECLARE_PUBLIC(QtMaterialCheckBox)

public:
    QtMaterialCheckBoxPrivate(QtMaterialCheckBox *q);
    ~QtMaterialCheckBoxPrivate();

    void init();
};

#endif // QTMATERIALCHECKBOX_P_H
