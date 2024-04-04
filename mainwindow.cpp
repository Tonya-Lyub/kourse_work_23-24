#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTableWidget>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDate>
#include <QRegularExpression>
#include <QColor>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent){
    tableWidget = new QTableWidget(this);
    setCentralWidget(tableWidget);
    
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    QMenu* fileMenu = menuBar->addMenu(tr("File"));
    
    QAction* createAction = fileMenu->addAction(tr("Create"));
    connect(createAction, &QAction::triggered, this, &MainWindow::createRecord);
    
    QAction* saveAction = fileMenu->addAction(tr("Save"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveData);
    
    QAction* deleteAllAction = fileMenu->addAction(tr("Delete all"));
    connect(deleteAllAction, &QAction::triggered, this, &MainWindow::deleteAll);
    
    QAction* sortAction = fileMenu->addAction(tr("Sort"));
    connect(sortAction, &QAction::triggered, this, &MainWindow::sortData);
    
    QAction* searchAction = fileMenu->addAction(tr("Search"));
    connect(searchAction, &QAction::triggered, this, &MainWindow::search);

    QAction* openAction = fileMenu->addAction(tr("Open"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    tableWidget->setColumnCount(7);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Name" << "Last Name" << "Second Name" << "Adress" << "Birth date" << "Email" << "Phone");
    connect(tableWidget, &QTableWidget::cellClicked, this, &MainWindow::cellClicked);
}
bool MainWindow::validateName(QString& name) {
    QRegularExpression nameRegExp("^[a-zA-Z][a-zA-Z0-9\\s-]*[a-zA-Z0-9]$");
    bool firstAttempt = true;  // Добавим флаг для определения первой попытки ввода
    while (!nameRegExp.match(name).hasMatch()) {
        if (!firstAttempt) {
            qDebug() << "Invalid name entered. Prompting user again.";
            name = QInputDialog::getText(this, tr("Enter Name"), tr("Name: Example: Ivan")).trimmed();
            if (name.isEmpty()) {
                qDebug() << "User cancelled input. Exiting validation.";
                return false;
            }
        }
        firstAttempt = false;  // Устанавливаем флаг в false после первой попытки

        // Display an error message only for invalid input
        if (!nameRegExp.match(name).hasMatch()) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid name format. Please follow the specified pattern."));
        }
    }
    qDebug() << "Name validation successful.";
    return true;
}
bool MainWindow::validateLastName(QString& lastName) {
    QRegularExpression lastNameRegExp("^[a-zA-Z][a-zA-Z0-9\\s-]*[a-zA-Z0-9]$");
    bool firstAttempt = true;
    while (!lastNameRegExp.match(lastName).hasMatch()) {
        if (!firstAttempt) {
            qDebug() << "Invalid last name entered. Prompting user again.";
            lastName = QInputDialog::getText(this, tr("Enter Last Name"), tr("Last Name: Example: Ivanov")).trimmed();
            if (lastName.isEmpty()) {
                qDebug() << "User cancelled input. Exiting validation.";
                QMessageBox::warning(this, tr("Error"), tr("Invalid last name format. Please follow the specified pattern."));
                return false;
            }
        }
        firstAttempt = false;

        // Display an error message only for invalid input
        if (!lastNameRegExp.match(lastName).hasMatch()) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid last name format. Please follow the specified pattern."));
        }
    }
    qDebug() << "Last name validation successful.";
    return true;
}

bool MainWindow::validateSecondName(QString& secondName) {
    QRegularExpression secondNameRegExp("^[a-zA-Z0-9\\s-]*$");
    bool firstAttempt = true;
    while (!secondNameRegExp.match(secondName).hasMatch()) {
        if (!firstAttempt) {
            qDebug() << "Invalid second name entered. Prompting user again.";
            secondName = QInputDialog::getText(this, tr("Enter Second Name"), tr("Second Name: Example: Ivanovich")).trimmed();
            if (secondName.isEmpty()) {
                qDebug() << "User cancelled input. Exiting validation.";
                return false;
            }
        }
        firstAttempt = false;

        // Display an error message only for invalid input
        if (!secondNameRegExp.match(secondName).hasMatch()) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid second name format. Please follow the specified pattern."));
        }
    }
    qDebug() << "Second name validation successful.";
    return true;
}

bool MainWindow::validatePhone(QString& phone) {
    QRegularExpression validationRegExp("^[0-9+\\s]+$");

    while (true) {
        bool ok;
        QString newPhone = QInputDialog::getText(this, tr("Enter Phone"), tr("Phone: Example: +7 999 1234567 or 89991234567"), QLineEdit::Normal, phone, &ok);

        if (!ok) {
            qDebug() << "User cancelled input. Exiting validation.";
            return false;
        }

        newPhone = newPhone.replace(QRegularExpression("[^0-9+\\s]"), "");

        if (!validationRegExp.match(newPhone).hasMatch()) {
            // Display an error message for invalid input without closing it immediately
            QMessageBox::warning(this, tr("Error"), tr("Invalid phone number format. Please enter digits, plus sign, and spaces only."));
        }
        else {
            qDebug() << "Phone validation successful.";
            phone = newPhone; // Обновляем phone только если введенный номер соответствует формату
            break;  // Выход из цикла при успешной валидации
        }
    }

    return true;
}

bool MainWindow::validateBirthDate(QString& birthDate) {
    QDate currentDate = QDate::currentDate();
    bool firstAttempt = true;

    QDate enteredDate = QDate::fromString(birthDate, "dd.MM.yyyy");

    while (!enteredDate.isValid() || enteredDate >= currentDate) {
        if (!firstAttempt) {
            qDebug() << "Invalid birth date entered. Prompting user again.";
            birthDate = QInputDialog::getText(this, tr("Enter Birth Date"), tr("Birth Date: Example: 01.01.1990")).trimmed();

            if (birthDate.isEmpty()) {
                qDebug() << "User cancelled input. Exiting validation.";
                return false;
            }

            enteredDate = QDate::fromString(birthDate, "dd.MM.yyyy");
        }

        firstAttempt = false;

        // Display an error message only for invalid input
        if (!enteredDate.isValid() || enteredDate >= currentDate) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid birth date. Please enter a valid date in the format DD.MM.YYYY and ensure it is not in the future."));
        }
    }

    qDebug() << "Birth date validation successful.";
    return true;
}

bool MainWindow::validateEmail(QString& email) {
    QRegularExpression emailRegExp("^[a-zA-Z0-9]+@[a-zA-Z0-9]+\\.[a-zA-Z0-9]+$");
    bool firstAttempt = true;
    while (!emailRegExp.match(email).hasMatch()) {
        if (!firstAttempt) {
            qDebug() << "Invalid email format entered. Prompting user again.";
            email = QInputDialog::getText(this, tr("Enter Email"), tr("Email: Example: ivanov@example.com")).trimmed();
            if (email.isEmpty()) {
                qDebug() << "User cancelled input. Exiting validation.";
                return false;
            }
        }
        firstAttempt = false;

        // Display an error message only for invalid input
        if (!emailRegExp.match(email).hasMatch()) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid email format. Please follow the specified pattern."));
        }
    }
    qDebug() << "Email validation successful.";
    return true;
}

void MainWindow::createRecord() {
    QString name, lastName, secondName, address, birthDate, email, phone;

    // Input and validate name
    do {
        name = QInputDialog::getText(this, tr("Enter Name"), tr("Name: Example: Ivan")).trimmed();
        if (name.isEmpty()) {
            qDebug() << "User cancelled input. Exiting validation.";
            return;
        }
    } while (!validateName(name));

    // Input and validate last name
    do {
        lastName = QInputDialog::getText(this, tr("Enter Last Name"), tr("Last Name: Example: Ivanov")).trimmed();
        if (lastName.isEmpty()) {
            qDebug() << "User cancelled input. Exiting validation.";
            return;
        }
    } while (!validateLastName(lastName));

    // Input and validate second name
    do {
        secondName = QInputDialog::getText(this, tr("Enter Second Name"), tr("Second Name: Example: Ivanovich")).trimmed();
        if (secondName.isEmpty()) {
            qDebug() << "User cancelled input. Exiting validation.";
            return;
        }
    } while (!validateSecondName(secondName));

    address = QInputDialog::getText(this, tr("Enter Address"), tr("Address: Example: st. Pushkin, b. 1"));

    // Input and validate birth date
    do {
        birthDate = QInputDialog::getText(this, tr("Enter Birth Date"), tr("Birth Date: Example: 01.01.1990")).trimmed();
        if (birthDate.isEmpty()) {
            qDebug() << "User cancelled input. Exiting validation.";
            return;
        }
    } while (!validateBirthDate(birthDate));

    // Input and validate email
    do {
        email = QInputDialog::getText(this, tr("Enter Email"), tr("Email: Example: ivanov@example.com")).trimmed();
        if (email.isEmpty()) {
            qDebug() << "User cancelled input. Exiting validation.";
            return;
        }
    } while (!validateEmail(email));

    // Input and validate phone number
    if (!validatePhone(phone)) {
        // Error message already displayed inside the function
        return;
    }
    // After successful validation, add a new row to the table
    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    tableWidget->setItem(row, 0, new QTableWidgetItem(name));
    tableWidget->setItem(row, 1, new QTableWidgetItem(lastName));
    tableWidget->setItem(row, 2, new QTableWidgetItem(secondName));
    tableWidget->setItem(row, 3, new QTableWidgetItem(address));
    tableWidget->setItem(row, 4, new QTableWidgetItem(birthDate));
    tableWidget->setItem(row, 5, new QTableWidgetItem(email));
    tableWidget->setItem(row, 6, new QTableWidgetItem(phone));
}

void MainWindow::saveData() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), "", tr("Text files (*.txt);;All files (*)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to open the file for writing"));
        return;
    }
    QTextStream out(&file);
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            if (col > 0) {
                out << "\t";  // Разделитель между значениями
            }

            QTableWidgetItem* item = tableWidget->item(row, col);
            if (item) {
                out << item->text();
            }
        }
        out << "\n";  // Переход на новую строку после каждой строки данных
    }

    file.close();
}

void MainWindow::deleteAll() {
    int rowCount = tableWidget->rowCount();

    if (rowCount > 0) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete all", "Are you sure you want to delete all entries?",
            QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            tableWidget->setRowCount(0); // Устанавливаем количество строк в 0
        }
    }
    else {
        QMessageBox::information(this, "No such entries", "No entries for delete");
    }
}


void MainWindow::sortData() {
    QStringList headers;
    for (int i = 0; i < tableWidget->columnCount(); ++i) {
        headers.append(tableWidget->horizontalHeaderItem(i)->text());
    }

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Choose item for sort"),
        tr("Item:"), headers, 0, false, &ok);

    if (ok && !item.isEmpty()) {
        int columnIndex = headers.indexOf(item);

        if (columnIndex >= 0) {
            tableWidget->sortItems(columnIndex, Qt::AscendingOrder);
        }
    }
}


void MainWindow::search() {
    QStringList headers;
    for (int i = 0; i < tableWidget->columnCount(); ++i) {
        headers.append(tableWidget->horizontalHeaderItem(i)->text());
    }

    bool ok;
    QString item = QInputDialog::getItem(this, tr("Choose item for search"),
        tr("Item:"), headers, 0, false, &ok);

    if (ok && !item.isEmpty()) {
        QString searchText = QInputDialog::getText(this, tr("Enter a text for searching"), tr("Text:"));

        if (!searchText.isEmpty()) {
            // Снимаем выделение в ячейках перед новым поиском
            for (int row = 0; row < tableWidget->rowCount(); ++row) {
                for (int col = 0; col < tableWidget->columnCount(); ++col) {
                    QTableWidgetItem* currentItem = tableWidget->item(row, col);
                    if (currentItem) {
                        currentItem->setData(Qt::BackgroundRole, QVariant()); // Очищаем цвет фона
                    }
                }
            }

            int columnIndex = headers.indexOf(item);

            for (int row = 0; row < tableWidget->rowCount(); ++row) {
                QTableWidgetItem* currentItem = tableWidget->item(row, columnIndex);
                if (currentItem && currentItem->text().contains(searchText, Qt::CaseInsensitive)) {
                    currentItem->setData(Qt::BackgroundRole, QColor(Qt::red));
                }
            }
        }
    }
}

void MainWindow::cellClicked(int row, int column) {
    QTableWidgetItem* currentItem = tableWidget->item(row, column);
    QString currentValue = currentItem->text();

    bool ok;
    QString newValue = QInputDialog::getText(this, tr("Edit Cell"), tr("Enter new value:"), QLineEdit::Normal, currentValue, &ok);

    if (!ok) {
        return; // Если пользователь отменил ввод, выходим из функции
    }

    // Проверим, какую ячейку редактируем, и применим соответствующую валидацию
    switch (column) {
    case 0: // Имя
        if (!validateName(newValue)) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid name format. Please follow the specified pattern."));
            return;
        }
        break;
    case 1: // Фамилия
        if (!validateLastName(newValue)) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid last name format. Please follow the specified pattern."));
            return;
        }
        break;
    case 2: // Отчество
        if (!validateSecondName(newValue)) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid second name format. Please follow the specified pattern."));
            return;
        }
        break;
    case 6: // Телефон
        if (!validatePhone(newValue)) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid phone format. Please follow the specified pattern."));
            return;
        }
        break;
    case 4: // Дата рождения
        if (!validateBirthDate(newValue)) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid birth date format. Please follow the specified pattern."));
            return;
        }
        break;
    case 5: // Email
        if (!validateEmail(newValue)) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid email format. Please follow the specified pattern."));
            return;
        }
        break;
    }

    // Установим новое значение в ячейку
    currentItem->setText(newValue);
}
void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open file"), "", tr("Text files (*.txt);;All files (*)"));

    if (!fileName.isEmpty()) {
        tableWidget->clearContents();
        tableWidget->setRowCount(0);
        currentFileName = fileName;  // Сохраняем текущий открытый файл
        loadFromFile(fileName);
    }
}
void MainWindow::loadFromFile(const QString& fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("ERROR"), tr("File couldn`t be opened"));
        return;
    }

    QTextStream in(&file);

    int row = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList items = line.split('\t');

        // Проверяем количество колонок
        if (items.size() != 7) {
            qDebug() << "Invalid data format in the file.";
            QMessageBox::critical(this, tr("ERROR"), tr("Invalid data format in the file."));
            return;
        }

        tableWidget->insertRow(row);

        for (int col = 0; col < 7; ++col) {
            tableWidget->setItem(row, col, new QTableWidgetItem(items.at(col)));
        }

        ++row;
    }

    file.close();
}