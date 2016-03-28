#include <debug.hpp>
#include <item.hpp>
#include <dish.hpp>
#include <food.hpp>

#include <QString>

#include <mainWindow.hpp>
#include "ui_mainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow {parent},
    ui {new Ui::MainWindow},
    itemStringList {new QStringList},
    dishStringList {new QStringList},
    itemListModel {new QStringListModel {*itemStringList, NULL}},
    dishListModel {new QStringListModel {*dishStringList, NULL}},
    itemForm {new ItemForm} {

    ui->setupUi(this);

    ui->listView_1->setModel(itemListModel);
    ui->listView_2->setModel(dishListModel);

    redrawItemList();
    redrawDishList();

    PRINT_OBJ("MainWindow created");
}

MainWindow::~MainWindow() {
    delete ui;
    delete itemStringList;
    delete dishStringList;
    delete itemListModel;
    delete dishListModel;
    delete itemForm;

    PRINT_OBJ("MainWindow destroyed");
}

void MainWindow::on_pushButton_1_clicked() {
    NewIngridientWindow newIngridientWindow;

    QObject::connect(&newIngridientWindow, SIGNAL(itemObjectReady(Item *)), this, SLOT(addNewItemObject(Item *)));
    QObject::connect(&newIngridientWindow, SIGNAL(foodObjectReady(Food *)), this, SLOT(addNewFoodObject(Food *)));

    newIngridientWindow.exec();
}

void MainWindow::on_pushButton_2_clicked() {
    QModelIndexList selected = ui->listView_1->selectionModel()->selectedIndexes();

    if ( !selected.isEmpty() ) {
        int row = selected.first().row();

        const QModelIndex index = itemListModel->index(row?row-1:0);
        std::list<Item *>::iterator item = itemForm->avaliableItems.begin();

        std::advance(item, row);
        itemForm->avaliableItems.erase(item);

        delete *item;

        itemStringList->removeAt(row);
        itemListModel->setStringList(*itemStringList);

        if ( itemStringList->size() ) {
            ui->listView_1->setCurrentIndex(index);
            displayListViewInfoItem(index);
        } else {
            ui->textEdit_1->clear();
        }
   }
}

void MainWindow::on_pushButton_3_clicked() {
    NewDishWindow newDishWindow;

    QObject::connect(&newDishWindow, SIGNAL(itemObjectReady(Dish *)), this, SLOT(addNewDishObject(Dish *)));
    QObject::connect(this, SIGNAL(newDishRequest(std::list<Item *> *)), &newDishWindow, SLOT(fillItemList(std::list<Item *> *)));

    emit newDishRequest(&itemForm->avaliableItems);

    newDishWindow.exec();
}

void MainWindow::on_pushButton_4_clicked() {
    QModelIndexList selected = ui->listView_2->selectionModel()->selectedIndexes();

    if ( !selected.isEmpty() ) {
        int row = selected.first().row();

        const QModelIndex index = dishListModel->index(row?row-1:0);
        std::list<Dish *>::iterator dish = itemForm->avaliableDish.begin();

        std::advance(dish, row);
        itemForm->avaliableDish.erase(dish);

        delete *dish;

        dishStringList->removeAt(row);
        dishListModel->setStringList(*dishStringList);

        if ( dishStringList->size() ) {
            ui->listView_2->setCurrentIndex(index);
            displayListViewInfoDish(index);
        } else {
            ui->textEdit_1->clear();
        }
   }
}

void MainWindow::on_pushButton_5_clicked() {
    if ( infoWindowType == ITEM || infoWindowType == FOOD ) {
        QModelIndexList selected = ui->listView_1->selectionModel()->selectedIndexes();

        if ( !selected.isEmpty() ) {
            editItem(selected);
        }
    } else if ( infoWindowType == DISH ) {
        QModelIndexList selected = ui->listView_2->selectionModel()->selectedIndexes();

        if ( !selected.isEmpty() ) {
            editDish(selected);
        }
    }
}

void MainWindow::on_listView_1_clicked(const QModelIndex &index) {
    displayListViewInfoItem(index);

    PRINT_DEBUG("Clicked item/food: " << index.data().toString().toStdString());
}

void MainWindow::on_listView_2_clicked(const QModelIndex &index) {
    displayListViewInfoDish(index);

    PRINT_DEBUG("Clicked dish: " << index.data().toString().toStdString());
}

void MainWindow::on_listView_1_entered(const QModelIndex &index) {
    displayListViewInfoItem(index);

    PRINT_DEBUG("Entered item/food: " << index.data().toString().toStdString());
}

void MainWindow::on_listView_2_entered(const QModelIndex &index) {
    displayListViewInfoDish(index);

    PRINT_DEBUG("Entered dish: " << index.data().toString().toStdString());
}

void MainWindow::addNewItemObject(Item *item) {
    QModelIndex index;

    itemStringList->append(QString::fromStdString(item->getName()));
    itemListModel->setStringList(*itemStringList);

    itemForm->avaliableItems.push_back(item);

    index = itemListModel->index(itemForm->avaliableItems.size()-1);
    ui->listView_1->setCurrentIndex(index);
    displayListViewInfoItem(index);

    PRINT_DEBUG("New item added");
}

void MainWindow::addNewFoodObject(Food *food) {
    QModelIndex index;

    itemStringList->append(QString::fromStdString(food->getName()));
    itemListModel->setStringList(*itemStringList);

    itemForm->avaliableItems.push_back(food);

    index = itemListModel->index(itemForm->avaliableItems.size()-1);
    ui->listView_1->setCurrentIndex(index);
    displayListViewInfoItem(index);

    PRINT_DEBUG("New food added");
}

void MainWindow::addNewDishObject(Dish *dish) {
    dishStringList->append(QString::fromStdString(dish->getName()));
    dishListModel->setStringList(*dishStringList);

    itemForm->avaliableDish.push_back(dish);

    PRINT_DEBUG("New dish added");
}

void MainWindow::editItem(QModelIndexList &selected) {
    NewIngridientWindow *ingridientWindow;
    int row = selected.first().row();
    const QModelIndex index = itemListModel->index(row);

    std::list<Item *>::iterator item = itemForm->avaliableItems.begin();
    std::advance(item, row);

    ingridientWindow = (infoWindowType == ITEM) ? new NewIngridientWindow(*item) : new NewIngridientWindow(dynamic_cast<Food *>(*item));

    ingridientWindow->exec();

    delete ingridientWindow;

    displayListViewInfoItem(index);
}

void MainWindow::editDish(QModelIndexList &selected) {
    NewDishWindow *dishWindow;
    int row = selected.first().row();
    const QModelIndex index = dishListModel->index(row);

    std::list<Dish *>::iterator dish = itemForm->avaliableDish.begin();
    std::advance(dish, row);

    dishWindow = new NewDishWindow(*dish);

    QObject::connect(this, SIGNAL(newDishRequest(std::list<Item *> *)), dishWindow, SLOT(fillItemList(std::list<Item *> *)));

    emit newDishRequest(&itemForm->avaliableItems);
    dishWindow->exec();

    delete dishWindow;

    displayListViewInfoDish(index);
}

void MainWindow::displayListViewInfoItem(const QModelIndex &index) {
    std::list<Item *>::iterator item = itemForm->avaliableItems.begin();
    QString itemInfo;

    std::advance(item, index.row());

    if ( dynamic_cast<Food *>(*item) ) {
        infoWindowType = FOOD;
    } else {
        infoWindowType = ITEM;
    }

    itemInfo += "Name: " + QString::fromStdString((*item)->getName()) + "\n";
    itemInfo += "Price: " + QString::number((*item)->getPrice()) + "\n";
    itemInfo += "Mass: " + QString::number((*item)->getMass()) + "\n";
    itemInfo += "Unit: " + QString::fromStdString((*item)->getUnitTypeName()) + "\n";

    if ( infoWindowType == FOOD ) {
       itemInfo += "Fats: " + QString::number((*item)->getFats()) + " (in 100 g)\n";
       itemInfo += "Proteins: "+ QString::number((*item)->getProteins()) + " (in 100 g)\n";
       itemInfo += "Carbohydrates: " + QString::number((*item)->getCarbohydrates()) + " (in 100 g)\n";
       itemInfo += "Calories: " + QString::number((*item)->getCalories()) + " (in 100 g)\n";
    }

    ui->textEdit_1->setText(itemInfo);
}

void MainWindow::displayListViewInfoDish(const QModelIndex &index) {
    std::list<Dish *>::iterator item = itemForm->avaliableDish.begin();
    QString itemInfo;

    std::advance(item, index.row());

    infoWindowType = DISH;

    itemInfo += "Name: " + QString::fromStdString((*item)->getName()) + "\n";
    itemInfo += "Price: " + QString::number((*item)->getPrice()) + "\n";
    itemInfo += "Mass: " + QString::number((*item)->getMass()) + "\n";
    itemInfo += "Fats: " + QString::number((*item)->getFats()) + "\n";
    itemInfo += "Proteins: " + QString::number((*item)->getProteins()) + "\n";
    itemInfo += "Carbohydrates: " + QString::number((*item)->getCarbohydrates()) + "\n";
    itemInfo += "Calories: " + QString::number((*item)->getCalories()) + "\n\n";
    itemInfo += "Amount of people: " + QString::number((*item)->getAmountOfPeople()) + "\n\n";
    itemInfo += "Ingridients:\n";

    for ( auto& entry: (*item)->getIngridientMap() ) {
        itemInfo += QString::fromStdString(entry.first->getName()) + " (";
        itemInfo += QString::number(entry.second) + " g)\n";
    }

    ui->textEdit_1->setText(itemInfo);
}

void MainWindow::redrawItemList(void) {
    for ( auto& entry: itemForm->avaliableItems ) {
        itemStringList->append(QString::fromStdString(entry->getName()));
    }
    itemListModel->setStringList(*itemStringList);
}

void MainWindow::redrawDishList(void) {
    for ( auto& entry: itemForm->avaliableDish ) {
        dishStringList->append(QString::fromStdString(entry->getName()));
    }
    dishListModel->setStringList(*dishStringList);
}

void MainWindow::on_actionAbout_triggered() {
    AboutWindow aboutWindow;

    aboutWindow.exec();
}
