#include <debug.hpp>

#include <newDishWindow.hpp>
#include "ui_newDishWindow.h"

NewDishWindow::NewDishWindow(QWidget *parent) :
    QDialog {parent},
    ui {new Ui::NewDishWindow},
    foodStringList {new QStringList},
    itemListModel {new QStringListModel {*foodStringList, NULL}} {

    ui->setupUi(this);

    ui->listView_1->setModel(itemListModel);

    ui->tableWidget_1->setColumnCount(2);

    PRINT_OBJ("NewDishWindow created");
}

NewDishWindow::~NewDishWindow() {
    delete ui;

    PRINT_OBJ("NewDishWindow destroyed");
}

void NewDishWindow::on_buttonBox_1_accepted() {
    if ( dishItemsList.size() ) {
        emit itemObjectReady(createNewDish());
    }
    this->hide();
}

void NewDishWindow::on_buttonBox_1_rejected() {
    this->hide();
}

void NewDishWindow::fillItemList(std::list<Item *> *itemsList) {
    avaliableItemsList = itemsList;

    for ( auto& entry: *itemsList ) {
        foodStringList->append(QString::fromStdString(entry->getName()));
    }

    itemListModel->setStringList(*foodStringList);
}

Dish* NewDishWindow::createNewDish(void) {
    std::list<Item *>::iterator item = dishItemsList.begin();
    int pos_y = 0;

    newDish = new Dish(ui->lineEdit_1->text().toStdString(),
                       *dynamic_cast<Food *>(*item),
                       static_cast<uint64_t>(ui->tableWidget_1->item(pos_y, 1)->text().toLong()),
                       static_cast<uint64_t>(ui->lineEdit_2->text().toLong()));
    pos_y += 1;
    std::advance(item, 1);

    for ( ; item != dishItemsList.end(); item++ ) {
        newDish->addFood(*dynamic_cast<Food *>(*item),
                         static_cast<uint64_t>(ui->tableWidget_1->item(pos_y, 1)->text().toLong()));
        pos_y += 1;
    }

    return newDish;
}

void NewDishWindow::on_pushButton_1_clicked() {
    QModelIndexList selected = ui->listView_1->selectionModel()->selectedIndexes();

    if ( !selected.isEmpty() ) {
        std::list<Item *>::iterator item = avaliableItemsList->begin();

        std::advance(item, selected.first().row());

        if ( std::find(dishItemsList.begin(), dishItemsList.end(), *item) == dishItemsList.end() ) {
            QTableWidgetItem *newItem;
            int rowCount = ui->tableWidget_1->rowCount();

            dishItemsList.push_back(*item);

            newItem = new QTableWidgetItem(tr("%1").arg(QString::fromStdString((*item)->getName())));
            ui->tableWidget_1->setRowCount(rowCount+1);
            ui->tableWidget_1->setItem(rowCount, 0, newItem);
        }
   }
}

void NewDishWindow::on_pushButton_2_clicked() {
    if ( ui->tableWidget_1->currentRow() != -1 ) {
        std::list<Item *>::iterator item = dishItemsList.begin();
        int currentRow = ui->tableWidget_1->currentRow();

        std::advance(item, currentRow);
        dishItemsList.remove(*item);

        ui->tableWidget_1->removeRow(currentRow);
    }
}
