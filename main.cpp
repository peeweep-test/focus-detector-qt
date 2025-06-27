#include <QApplication>
#include <QDebug>
#include "focusmanager.h"
#include "focuswindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    qDebug() << "焦点检测器 (Qt Widgets版本) 启动中...";
    
    // 创建全局焦点管理器实例
    FocusManager globalFocusManager;
    
    // 创建第一个窗口（主窗口）
    FocusWindow *mainWindow = new FocusWindow(nullptr, "窗口-1", true, &globalFocusManager);
    mainWindow->show();
    
    qDebug() << "主窗口创建成功: 窗口-1";
    
    return app.exec();
}
