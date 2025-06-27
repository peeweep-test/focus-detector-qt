#ifndef FOCUSMANAGER_H
#define FOCUSMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QDebug>

class FocusManager : public QObject
{
    Q_OBJECT

public:
    explicit FocusManager(QObject *parent = nullptr);

public slots:
    // 记录窗口获得焦点
    void recordFocusGained(const QString &windowId);
    
    // 记录窗口失去焦点  
    void recordFocusLost(const QString &windowId);
    
    // 获取当前精确时间字符串
    QString getCurrentTimeString();
    
    // 获取全局最后失去焦点的时间
    QDateTime getLastFocusLostTime() const { return m_lastFocusLostTime; }
    
    // 获取当前有焦点的窗口ID
    QString getCurrentFocusedWindow() const { return m_currentFocusedWindow; }
    
    // 检查获得焦点的时间差是否应该显示（如果获得焦点和失去焦点时间相近，说明是应用内切换）
    bool shouldShowTimeDiff(const QDateTime &gainTime) const;

signals:
    // 焦点状态变化信号
    void focusChanged(const QString &windowId, bool hasFocus, const QString &timestamp);

private:
    // 输出日志到控制台
    void logToConsole(const QString &windowId, const QString &action, const QString &timestamp);
    
    // 记录状态
    QString m_currentFocusedWindow;     // 当前有焦点的窗口ID（空字符串表示没有窗口有焦点）
    QDateTime m_lastFocusLostTime;      // 全局最后失去焦点的时间
    QDateTime m_lastFocusGainTime;      // 记录最后一次获得焦点的时间，用于判断是否是应用内切换
    int m_focusedWindowCount;           // 当前有焦点的窗口数量（用于调试）
};

#endif // FOCUSMANAGER_H 