#include "myobject.h"

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
