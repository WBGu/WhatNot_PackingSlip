// ShipmentItems.cpp

#include "ShipmentItems.h"
#include "ui_ShipmentItems.h"
#include <QFileDialog>
#include <QLineEdit>
#include <QFile>          // For file handling
#include <QTextStream>    // For reading text file easily
#include <QMessageBox>    // For showing error messages
#include <QStringList>    // To hold split fields
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QString>
#include <string>
#include <vector>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <sstream>
#include <xlsxio_read.h>
#include <xlsxwriter.h>
#include "UserOrders.h"
#include <unordered_map>


ShipmentItems::ShipmentItems(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ShipmentItems)
    , orderFilePath("")
    , referenceFilePath("")
    , outputFilePath("")
{
    ui->setupUi(this);

    ui->logStatus->append("HI");

    // Connect the button's clicked signal to the slot
    connect(ui->browseReference, &QPushButton::clicked, this, &ShipmentItems::on_browseReferenceClicked);
    connect(ui->browseOrders, &QPushButton::clicked, this, &ShipmentItems::on_browseOrdersClicked);

    // Connect Start button signal
    connect(ui->start, &QPushButton::clicked, this, &ShipmentItems::on_StartClicked);
}

ShipmentItems::~ShipmentItems()
{
    delete ui;
}


void ShipmentItems::on_browseReferenceClicked() {
    QString startDir = "";
    QString filter = "XLSX Files (*.xlsx);;All Files (*.*)";
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Select Reference Table"),
        startDir,
        filter
    );

    if (!filePath.isEmpty()) {
        referenceFilePath = filePath;
        qDebug() << "Selected Referece file:" << referenceFilePath;
        ui->logStatus->append("Selected Reference file: " + referenceFilePath);

        if (ui->referencePathLineEdit) {
            ui->referencePathLineEdit->setText(referenceFilePath);
        }
    }
    else {
        qDebug() << "No file selected.";
        ui->logStatus->append("No file selected.");
    }
}

void ShipmentItems::on_browseOrdersClicked() {
    QString startDir = "";
    QString filter = "CSV Files (*.csv);;All Files (*.*)";
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Select Orders File"),
        startDir,
        filter
    );

    if (!filePath.isEmpty()) {
        orderFilePath = filePath;
        qDebug() << "Selected Order file:" << orderFilePath;
        ui->logStatus->append("Selected Order file: " + orderFilePath);

        if (ui->orderPathLineEdit) {
            ui->orderPathLineEdit->setText(orderFilePath);
        }
    }
    else {
        orderFilePath.clear();
        qDebug() << "No file selected.";
        ui->logStatus->append("No file selected.");
    }
    
}

void ShipmentItems::on_StartClicked() {
    // Process reference table into a hash map
    getReferenceTable();
    loadAndProcessCsv();

    std::vector<UserOrders> buyerProductData = ShipmentItems::readXlsxOrders(outputFilePath);
    outputToDatabase(buyerProductData);

    qDebug() << "Finished processing. The Database can be found in the source folder named 'printOrders.xlsx'";
    ui->logStatus->append("Finished processing " + orderFilePath +".\nThe Database can be found in the source folder named 'printOrders.xlsx'");
}

void ShipmentItems::setOutputFilePath(QString path) {
    outputFilePath = path;
}

QStringList ShipmentItems::parseCsvLine(const QString& line) {
    /*
    Given that some fields have multiple lines in between the commas,
    and that those fields start and end with quotation marks, ",
    Use the quotations to recognize when the start and the end of the cell in avoidance of "\n" problems
    */ 
    
    // All cells
    QStringList fields;
    
    // One cell
    QString field;
    bool inQuotes = false;

    for (int i = 0; i < line.length(); ++i) {
        // go through character by character to recognize if it is a quotation
        QChar current = line[i];

        if (current == '"') {
            // if the it is already in quotes, close the quote
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                field += '"';
                ++i;
            }
            else {
                // if the quotation is at the first char of the field, indicate that it is now in quotes.
                inQuotes = !inQuotes;
            }
        }
        else if (current == ',' && !inQuotes) {
            // if the char is , and it is not in a quote, and append it to the fields list
            fields << field;

            // clear the current field and get ready for the next cell
            field.clear();
        }
        else {
            // if it is not " or ,, then just add to the field String
            field += current;
        }
    }
    fields << field;
    return fields;
}

int ShipmentItems::findUserOrderIndex(const std::vector<UserOrders>& orders, const std::string& name) {
    // Finds the index of the order in the list of userOrders base on the attribute name
    for (size_t i = 0; i < orders.size(); i++) {
        if (orders[i].getBuyerName() == name) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void ShipmentItems::sortUserOrdersByName(std::vector<UserOrders>& orders) {
    std::sort(orders.begin(), orders.end(), [](const UserOrders& a, const UserOrders& b) {
        return a.getBuyerName() < b.getBuyerName();
        }
    );
}

std::vector<UserOrders> ShipmentItems::readXlsxOrders(const QString& filePath) {
    std::vector<UserOrders> userOrdersList;
    xlsxioreader reader = xlsxioread_open(filePath.toUtf8().constData());
    if (!reader) {
        qWarning() << "Unable to open XLSX file.";
        ui->logStatus->append("Unable to open XLSX file.");
        return userOrdersList;
    }

    xlsxioreadersheet sheet = xlsxioread_sheet_open(reader, NULL, XLSXIOREAD_SKIP_EMPTY_ROWS);
    if (!sheet) {
        qWarning() << "Unable to read sheet.";
        ui->logStatus->append("Unable to read sheet.");
        xlsxioread_close(reader);
        return userOrdersList;
    }

    std::vector<std::string> buyerUserNames;

    char* value;
    while (xlsxioread_sheet_next_row(sheet)) {
        QStringList row;
        while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
            row << QString::fromUtf8(value);
            free(value);
        }

        // row[2] is buyer username
        std::string userName = row[2].toStdString();

        //row[3] is product name
        std::string item = row[3].toStdString();

        //row[5] is product quantity
        std::string quantity = row[5].toStdString();
        
        std::string sku;

        if (21 < row.size()) {
            sku = row[21].toStdString();
        }
        else {
            sku = "";
        }
        
        char delimiter = ',';
        if (userName == "buyer" || row[7].toStdString() == "failed" || row[7].toStdString() == "cancelled") {
        }
        else if (std::find(buyerUserNames.begin(), buyerUserNames.end(), userName) == buyerUserNames.end()) {
            // if the name is not in the names vector, add it to the names vector
            buyerUserNames.push_back(userName);

            // Create an object for this name user
            UserOrders newOrder(userName);
            int commaPos = row[13].toStdString().find(delimiter);
            newOrder.setBuyerRealName(row[13].toStdString().substr(0, commaPos));

            // Append the item to corresponding NumberItems or TextItems
            
            std::string groupName;
            std::string slice;
            int groupNameItemNum;
            int poundPos;
            int quantity_num = std::stoi(quantity);
            for (int i = 0; i < quantity_num; i++) {
                if (item.find("#") != std::string::npos) {
                    poundPos = item.find("#");
                    groupName = item.substr(0, poundPos -1);
                    slice = item.substr(poundPos + 1);
                    groupNameItemNum = std::stoi(slice);
                    try {
                        groupName = referenceTable.at(groupName);
                    }
                    catch (const std::out_of_range& e) {
                    }
                    newOrder.addAuctionGroup(groupName);
                    newOrder.addNumToAuctionGroup(groupName, groupNameItemNum);
                }
                else if (sku != "") {
                    int num;
                    try {
                        num = std::stoi(sku);
                        //std::cout << "The integer value is: " << num << std::endl;
                        newOrder.addToAucNumberItems(num);
                    }
                    catch (const std::invalid_argument& e) {
                        std::cerr << "Invalid argument: " << e.what() << std::endl;
                        qDebug() << sku;
                        try {
                            std::string referenceItem = referenceTable.at(item);
                            newOrder.addToAucTextItems(referenceItem);
                        }
                        catch (const std::out_of_range& e) {
                            newOrder.addToAucTextItems(item);
                        }
                    }
                    catch (const std::out_of_range& e) {
                        std::cerr << "Out of range: " << e.what() << std::endl;
                    }
                }
                else {
                    try {
                        std::string referenceItem = referenceTable.at(item);
                        newOrder.addToAucTextItems(referenceItem);
                    }
                    catch (const std::out_of_range& e) {
                        newOrder.addToAucTextItems(item);
                    }
                }
            }

            // Append this object to the Users Vector
            userOrdersList.push_back(newOrder);
        }
        else {
            // user already in the vector
            // Find the object in the Users vector
            int index = findUserOrderIndex(userOrdersList, userName);


            // Append the item to NumberItems or TextItems
            std::string groupName;
            std::string slice;
            int groupNameItemNum;
            int quantity_num = std::stoi(quantity);
            for (int i = 0; i < quantity_num; i++) {
                if (item.find("#") != std::string::npos){
                    int poundPos = item.find("#");
                    groupName = item.substr(0, poundPos - 1);
                    slice = item.substr(poundPos + 1);
                    groupNameItemNum = std::stoi(slice);
                    try {
                        groupName = referenceTable.at(groupName);
                    }
                    catch (const std::out_of_range& e) {
                    }
                    userOrdersList[index].addAuctionGroup(groupName);
                    userOrdersList[index].addNumToAuctionGroup(groupName, groupNameItemNum);
                } 
                else if (sku != "") {
                    int num;
                    try {
                        num = std::stoi(sku);
                        //std::cout << "The integer value is: " << num << std::endl;
                        userOrdersList[index].addToAucNumberItems(num);
                    }
                    catch (const std::invalid_argument& e) {
                        std::cerr << "Invalid argument: " << e.what() << std::endl;
                        qDebug() << sku;
                        try {
                            std::string referenceItem = referenceTable.at(item);
                            userOrdersList[index].addToAucTextItems(referenceItem);
                        }
                        catch (const std::out_of_range& e) {
                            userOrdersList[index].addToAucTextItems(item);
                        }
                    }
                    catch (const std::out_of_range& e) {
                        std::cerr << "Out of range: " << e.what() << std::endl;
                    }
                }
                else {
                    try {
                        std::string referenceItem = referenceTable.at(item);
                        userOrdersList[index].addToAucTextItems(referenceItem);
                    }
                    catch (const std::out_of_range& e) {
                        userOrdersList[index].addToAucTextItems(item);
                    }
                }
            }
        }
    }
    xlsxioread_sheet_close(sheet);
    xlsxioread_close(reader);

    // Return a vector of the Objects containing username, number items, and text items
    return userOrdersList;
}

void ShipmentItems::getReferenceTable()
{   
    // Check if the field is empty
    if (referenceFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No Reference file selected to process. Order file is processed without reference table."));
        return;
    }
    xlsxioreader reader = xlsxioread_open(referenceFilePath.toUtf8().constData());
    if (!reader) {
        qWarning() << "Unable to open XLSX file.";
        ui->logStatus->append("Unable to open XLSX file.");
        return;
    }

    xlsxioreadersheet sheet = xlsxioread_sheet_open(reader, NULL, XLSXIOREAD_SKIP_EMPTY_ROWS);
    if (!sheet) {
        qWarning() << "Unable to read sheet.";
        ui->logStatus->append("Unable to read sheet.");
        xlsxioread_close(reader);
        return;
    }

    char* fieldText;
    while (xlsxioread_sheet_next_row(sheet)) {
        QStringList row;
        while ((fieldText = xlsxioread_sheet_next_cell(sheet)) != NULL) {
            row << QString::fromUtf8(fieldText);
            free(fieldText);
        }

        // Name of the long name items
        std::string key = row[0].toStdString();

        // Name of the shorter name items
        std::string value = row[1].toStdString();

        qDebug() << key;
        qDebug() << value;

        referenceTable[key] = value;
    }

    xlsxioread_sheet_close(sheet);
    xlsxioread_close(reader);
}

void ShipmentItems::loadAndProcessCsv() {
    /*
    * To load the CSV into readable xlsx file
    * Some fields in the CSV have extra \n in them. Hard to process it
    */

    // Check if the field is empty
    if (orderFilePath.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No Order file selected to process."));
        return;
    }

    QFile file(orderFilePath);
    QFileInfo fileInfo(orderFilePath);
    setOutputFilePath(fileInfo.absolutePath() + "/output.xlsx");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"),
            tr("Could not open file: %1").arg(file.errorString()));
        qDebug() << "Error opening file:" << file.errorString();
        // Stop processing if file cannot be opened
        return;
    }

    qDebug() << "Save .csv order file as .xlsx: " << outputFilePath;
    ui->logStatus->append("Saved .csv order file as .xlsx: " + outputFilePath);

    lxw_workbook* workbook = workbook_new(outputFilePath.toStdString().c_str());
    lxw_worksheet* worksheet = workbook_add_worksheet(workbook, NULL);
    QTextStream in(&file);
    QString buffer;
    int row = 0;

    while (!in.atEnd()) {
        QString line = in.readLine();

        buffer += line;
        // Count quotes to determine if the line continues
        int quoteCount = buffer.count('"');
        if (quoteCount % 2 == 1) {
            buffer += "\n";  // multi-line field continues
            continue;
        }

        QStringList cells = parseCsvLine(buffer);
        for (int col = 0; col < cells.size(); ++col) {
            worksheet_write_string(worksheet, row, col, cells[col].toUtf8().constData(), NULL);
        }
        buffer.clear();
        row++;
    }
    workbook_close(workbook);
    file.close();
}


void ShipmentItems::outputToDatabase(std::vector<UserOrders> userOrders) {
    QFile file(orderFilePath);
    QFileInfo fileInfo(orderFilePath);
    QString printOrdersFilePath = fileInfo.absolutePath() + "/printOrders.xlsx";

    lxw_workbook* printOrders = workbook_new(printOrdersFilePath.toStdString().c_str());
    lxw_worksheet* printOrdersSheet = workbook_add_worksheet(printOrders, NULL);
    lxw_format* wrap_format = workbook_add_format(printOrders);
    format_set_text_wrap(wrap_format);

    sortUserOrdersByName(userOrders);

    for (int row = 0; row < userOrders.size(); row++) {
        std::string buyerName = userOrders[row].getBuyerName();
        std::string buyerRealName = userOrders[row].getBuyerRealName();

        userOrders[row].sortAll();
        std::vector<int> numberItems = userOrders[row].getAucNumberItems();
        std::vector<UserOrders::Items> textItems = userOrders[row].getAucTextItems();

        std::string combineText = "Buyer Username: " + buyerName + "\n";
        combineText += "Buyer real name: " + buyerRealName + "\n";

        for (const auto& pair : userOrders[row].getAllAuctionGroups()) {
            const std::string& groupName = pair.first;
            const std::vector<int>& items = pair.second;
            combineText += groupName + ": ";
            for (int i = 0; i < items.size(); i++) {
                combineText += std::to_string(items[i]);
                if (i != items.size()-1) combineText += ", ";
            }
            combineText += "\n";
        }

        // Combine numbers cleanly
        if (numberItems.size() > 0) {
            combineText += "SKU Numbers: ";
            for (size_t i = 0; i < numberItems.size(); ++i) {
                combineText += std::to_string(numberItems[i]);
                if (i != numberItems.size() - 1) combineText += ", ";
            }
            combineText += "\n";
        }

        if (textItems.size() > 0) {
            // Combine text items cleanly
            combineText += "Other Items:\n";
            for (size_t i = 0; i < textItems.size(); ++i) {
                combineText += "- ";
                combineText += std::to_string(textItems[i].count);
                combineText += "x ";
                combineText += textItems[i].itemName;
                if (i != textItems.size() - 1) combineText += "\n";
            }
        }

        worksheet_write_string(printOrdersSheet, row, 0, combineText.c_str(), wrap_format);
    }
    worksheet_set_column(printOrdersSheet, 0, 0, 35, NULL);
    workbook_close(printOrders);
}