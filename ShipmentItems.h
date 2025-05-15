// ShipmentItems.h

#ifndef SHIPMENTITEMS_H
#define SHIPMENTITEMS_H

#include <QMainWindow>
#include <QTextBrowser>
#include <UserOrders.h>
#include <unordered_map>

namespace Ui {
    class ShipmentItems;
}

class QWidget;

class ShipmentItems : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor: explicit keyword prevents unintended conversions
    explicit ShipmentItems(QWidget* parent = nullptr);

    // Destructor
    ~ShipmentItems();

    void setOutputFilePath(QString path);
    QStringList parseCsvLine(const QString& line);
    void outputToDatabase(std::vector<UserOrders> userOrders);

    std::vector<UserOrders> readXlsxOrders(const QString& filePath);

    void getReferenceTable();
    void loadAndProcessCsv();

    int findUserOrderIndex(const std::vector<UserOrders>& orders, const std::string& name);
    void sortUserOrdersByName(std::vector<UserOrders>& orders);

private slots:
    void on_browseReferenceClicked();
    void on_browseOrdersClicked();
    void on_StartClicked();

private:
    // Pointer to the UI object that holds all the widgets
    Ui::ShipmentItems *ui;
    QTextBrowser* logStatus;

    QString orderFilePath;
    QString referenceFilePath;
    QString outputFilePath;
    std::unordered_map < std::string, std::string> referenceTable;
};

#endif