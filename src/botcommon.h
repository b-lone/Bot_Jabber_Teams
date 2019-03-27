#ifndef BOTCOMMON_H
#define BOTCOMMON_H

#include <QDebug>

#define BOTLOG(arg) qDebug() << "[" << __FILE__ << "][" << __LINE__ << "][" << __func__ << "]:" << arg

#endif // BOTCOMMON_H
