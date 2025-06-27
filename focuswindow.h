#ifndef FOCUSWINDOW_H
#define FOCUSWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QFrame>
#include <QTimer>
#include "focusmanager.h"

class FocusWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FocusWindow(QWidget *parent = nullptr, 
                        const QString &windowId = "Main",
                        bool isMainWindow = true,
                        FocusManager *globalManager = nullptr);
    ~FocusWindow();

    QString getWindowId() const { return m_windowId; }
    bool isMainWindow() const { return m_isMainWindow; }

protected:
    // 重写事件处理函数来检测焦点变化
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void changeEvent(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onCreateNewWindow();
    void onCloseWindow();
    void onFocusChanged(const QString &winId, bool hasFocus, const QString &timestamp);
    void onFocusSwitchTimeDiff(const QString &windowId, qint64 timeDiffMs, const QString &timeDiffText);

private:
    void setupUI();
    void updateBackgroundColor();
    void updateFocusStatus();
    
    // 成员变量
    QString m_windowId;
    bool m_isMainWindow;
    bool m_hasFocus;
    
    FocusManager *m_globalFocusManager;
    FocusManager *m_localFocusManager;
    
    // UI组件
    QVBoxLayout *m_mainLayout;
    QLabel *m_titleLabel;
    QFrame *m_windowIdFrame;
    QLabel *m_windowIdLabel;
    QFrame *m_statusFrame;
    QLabel *m_statusLabel;
    QGroupBox *m_timeGroupBox;
    QLineEdit *m_focusGainedEdit;
    QLineEdit *m_focusLostEdit;
    QLineEdit *m_timeDiffEdit;
    QPushButton *m_newWindowBtn;
    QPushButton *m_closeBtn;
    
    QString m_focusGainedTime;
    QString m_focusLostTime;
};

#endif // FOCUSWINDOW_H 