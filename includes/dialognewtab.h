#ifndef DIALOGNEWTAB_H
#define DIALOGNEWTAB_H

#include <QDialog>

namespace Ui {
class DialogNewTab;
}

class DialogNewTab : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewTab(QWidget *parent = nullptr);
    ~DialogNewTab();
    QString getTabName();

private:
    Ui::DialogNewTab *ui;
};

#endif // DIALOGNEWTAB_H
