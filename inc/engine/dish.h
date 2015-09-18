#ifndef DISH_H
#define DISH_H

#include <iostream>
#include <string>
#include <map>

#include <common.h>
#include <peopleStats.h>
#include <food.h>

#define HUNHDRED_GRAM 100

class Dish : public Stats {
    private:
        uint64_t amountOfPeople;
        std::map<Food, uint64_t, Stats::Compare> ingridients;

        void setAmountOfPeople(uint64_t amountOfPeople);

    public:
        const uint64_t minListItemsAmount;
        const uint64_t maxListItemsAmount;
        const uint64_t minAmountOfPeople;
        const uint64_t maxAmountOfPeople;

        Dish(std::string name, Food& food, uint64_t foodMass, uint64_t amountOfPeople=1);
        ~Dish(void);
        Dish& operator=(const Dish& right);
        void addFood(const Food& food, uint64_t foodMass);
        void removeFood(const Food& food);
        void changeAmountOfPeople(uint64_t amountOfPeople);
        uint64_t getAmountOfPeople(void) const;
        const std::map<Food, uint64_t, Stats::Compare>& getIngridientMap(void) const;
};

std::ostream& operator<<(std::ostream& out, const Dish& dish);

#endif // DISH_H
