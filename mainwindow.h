#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void openFile();
    void saveData();
    void deleteAll();
    void sortData();
    void search();
    void cellClicked(int row, int column);
    void loadFromFile(const QString& fileName);

private:
    QString currentFileName;
    QTableWidget* tableWidget;

    bool validateName(QString& name);
    bool validateLastName(QString& lastName);
    bool validateSecondName(QString& secondName);
    bool validatePhone(QString& phone);
    bool validateBirthDate(QString& birthDate);
    bool validateEmail(QString& email);
    bool isValidDate(QString& birthDate);

    void createRecord();
};

#endif // MAINWINDOW_H