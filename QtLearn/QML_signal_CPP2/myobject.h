#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>
#include <QVariant>

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

    // 在函数前面加了 Q_INVOKABLE 宏后，当前函数就可被QML端访问了
    Q_INVOKABLE void func();

signals:
    void iValueChanged();
    void sStringChanged();

    void cppSig(QVariant i, QVariant s);

public slots:
    void cppSlot(int i, QString s);

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
