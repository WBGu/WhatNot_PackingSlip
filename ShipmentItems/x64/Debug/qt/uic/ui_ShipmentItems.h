/********************************************************************************
** Form generated from reading UI file 'ShipmentItems.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHIPMENTITEMS_H
#define UI_SHIPMENTITEMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShipmentItems
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QGroupBox *Process;
    QVBoxLayout *verticalLayout_2;
    QPushButton *start;
    QGroupBox *ReferenceTable;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *referencePathLineEdit;
    QPushButton *browseReference;
    QGroupBox *Orders;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *orderPathLineEdit;
    QPushButton *browseOrders;
    QGroupBox *Status;
    QVBoxLayout *verticalLayout;
    QTextBrowser *logStatus;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ShipmentItems)
    {
        if (ShipmentItems->objectName().isEmpty())
            ShipmentItems->setObjectName("ShipmentItems");
        ShipmentItems->resize(469, 298);
        ShipmentItems->setMaximumSize(QSize(998, 661));
        centralWidget = new QWidget(ShipmentItems);
        centralWidget->setObjectName("centralWidget");
        centralWidget->setMinimumSize(QSize(469, 150));
        centralWidget->setMaximumSize(QSize(993, 596));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName("gridLayout");
        Process = new QGroupBox(centralWidget);
        Process->setObjectName("Process");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Process->sizePolicy().hasHeightForWidth());
        Process->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(Process);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        start = new QPushButton(Process);
        start->setObjectName("start");
        sizePolicy.setHeightForWidth(start->sizePolicy().hasHeightForWidth());
        start->setSizePolicy(sizePolicy);
        start->setMinimumSize(QSize(141, 31));
        start->setMaximumSize(QSize(971, 271));

        verticalLayout_2->addWidget(start);


        gridLayout->addWidget(Process, 1, 0, 1, 2);

        ReferenceTable = new QGroupBox(centralWidget);
        ReferenceTable->setObjectName("ReferenceTable");
        sizePolicy.setHeightForWidth(ReferenceTable->sizePolicy().hasHeightForWidth());
        ReferenceTable->setSizePolicy(sizePolicy);
        horizontalLayout_2 = new QHBoxLayout(ReferenceTable);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        referencePathLineEdit = new QLineEdit(ReferenceTable);
        referencePathLineEdit->setObjectName("referencePathLineEdit");
        sizePolicy.setHeightForWidth(referencePathLineEdit->sizePolicy().hasHeightForWidth());
        referencePathLineEdit->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(referencePathLineEdit);

        browseReference = new QPushButton(ReferenceTable);
        browseReference->setObjectName("browseReference");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(browseReference->sizePolicy().hasHeightForWidth());
        browseReference->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(browseReference);


        gridLayout->addWidget(ReferenceTable, 0, 0, 1, 1);

        Orders = new QGroupBox(centralWidget);
        Orders->setObjectName("Orders");
        sizePolicy.setHeightForWidth(Orders->sizePolicy().hasHeightForWidth());
        Orders->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(Orders);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        orderPathLineEdit = new QLineEdit(Orders);
        orderPathLineEdit->setObjectName("orderPathLineEdit");
        sizePolicy.setHeightForWidth(orderPathLineEdit->sizePolicy().hasHeightForWidth());
        orderPathLineEdit->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(orderPathLineEdit);

        browseOrders = new QPushButton(Orders);
        browseOrders->setObjectName("browseOrders");
        sizePolicy1.setHeightForWidth(browseOrders->sizePolicy().hasHeightForWidth());
        browseOrders->setSizePolicy(sizePolicy1);

        horizontalLayout_3->addWidget(browseOrders);


        gridLayout->addWidget(Orders, 0, 1, 1, 1);

        Status = new QGroupBox(centralWidget);
        Status->setObjectName("Status");
        sizePolicy.setHeightForWidth(Status->sizePolicy().hasHeightForWidth());
        Status->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(Status);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        logStatus = new QTextBrowser(Status);
        logStatus->setObjectName("logStatus");

        verticalLayout->addWidget(logStatus);


        gridLayout->addWidget(Status, 2, 0, 1, 2);

        ShipmentItems->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ShipmentItems);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 469, 33));
        ShipmentItems->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ShipmentItems);
        mainToolBar->setObjectName("mainToolBar");
        ShipmentItems->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ShipmentItems);
        statusBar->setObjectName("statusBar");
        ShipmentItems->setStatusBar(statusBar);

        retranslateUi(ShipmentItems);

        QMetaObject::connectSlotsByName(ShipmentItems);
    } // setupUi

    void retranslateUi(QMainWindow *ShipmentItems)
    {
        ShipmentItems->setWindowTitle(QCoreApplication::translate("ShipmentItems", "ShipmentItems", nullptr));
        Process->setTitle(QCoreApplication::translate("ShipmentItems", "Process", nullptr));
        start->setText(QCoreApplication::translate("ShipmentItems", "Start", nullptr));
        ReferenceTable->setTitle(QCoreApplication::translate("ShipmentItems", "Reference Table", nullptr));
        browseReference->setText(QCoreApplication::translate("ShipmentItems", "Browse", nullptr));
        Orders->setTitle(QCoreApplication::translate("ShipmentItems", "Orders", nullptr));
        browseOrders->setText(QCoreApplication::translate("ShipmentItems", "Browse", nullptr));
        Status->setTitle(QCoreApplication::translate("ShipmentItems", "Status", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShipmentItems: public Ui_ShipmentItems {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHIPMENTITEMS_H
