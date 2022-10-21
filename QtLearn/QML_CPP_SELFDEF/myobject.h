#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>

class MyObject : public QObject
{
    Q_OBJECT
public:
    explicit MyObject(QObject *parent = nullptr);
    static MyObject *getInstance();

    int iValue() const;
    void setIValue(int iValue);

    QString sString() const;
    void setSString(const QString &sString);

signals:
    void iValueChanged();
    void sStringChanged();

private:
    int m_iValue;
    QString m_sString;

    // Q_PROPERTY(int iValue READ iValue WRITE setIValue NOTIFY iValueChanged)
    // Q_PROPERTY(QString sString READ sString WRITE setSString NOTIFY sStringChanged)

    // 如果读写的变量是类内部的成员，可以采用如下写法
    Q_PROPERTY(int iValue MEMBER m_iValue NOTIFY iValueChanged)
    Q_PROPERTY(QString sString MEMBER m_sString NOTIFY sStringChanged)
};

#endif // MYOBJECT_H
