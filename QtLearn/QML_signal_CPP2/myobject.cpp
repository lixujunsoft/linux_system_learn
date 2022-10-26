#include "myobject.h"
#include <QDebug>

MyObject::MyObject(QObject *parent) : QObject(parent)
{
}

MyObject *MyObject::getInstance()
{
    // 教程中的单例写的不对，此处仅用于简单使用
    static MyObject *obj = new MyObject;
    return obj;
}

int MyObject::iValue() const
{
    return m_iValue;
}

void MyObject::setIValue(int newIValue)
{
    if (m_iValue == newIValue) {
        return;
    }

    m_iValue = newIValue;
    emit iValueChanged();
}

QString MyObject::sString() const
{
    return m_sString;
}

void MyObject::setSString(const QString &newsString)
{
    if (m_sString == newsString) {
        return;
    }

    m_sString = newsString;
    emit sStringChanged();
}

void MyObject::func()
{
    emit cppSig(QVariant(101), "王五");
    qDebug() << __FUNCTION__;
}

void MyObject::cppSlot(int i, QString s)
{
    qDebug() << __FUNCTION__ << " " << i << " " << s;
}
