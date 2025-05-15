#include "UserOrders.h"
#include <algorithm>
#include <vector>

// Constructor
UserOrders::UserOrders(const std::string& name) : buyerName(name) {}

// Getter for buyerName
std::string UserOrders::getBuyerName() const {
    return buyerName;
}

std::string UserOrders::getBuyerRealName() const {
    return buyerRealName;
}

void UserOrders::setBuyerRealName(std::string name) {
    buyerRealName = name;
}

void UserOrders::addAuctionGroup(const std::string& groupName)
{
    if (aucNumberItemsGroups.find(groupName) == aucNumberItemsGroups.end()) {
        aucNumberItemsGroups[groupName] = std::vector<int>();
    }
}

void UserOrders::addNumToAuctionGroup(const std::string& groupName, int item)
{
    aucNumberItemsGroups[groupName].push_back(item);
}

// Add to Auction Numbers
void UserOrders::addToAucNumberItems(int item) {
    aucNumberItems.push_back(item);
}

// Add to Auction Items
void UserOrders::addToAucTextItems(const std::string& item) {
    // If item already exist, then add one to the current count of that item
    // and then return
    for (auto& aucItem : aucTextItems) {
        if (aucItem.itemName == item) {
            aucItem.count += 1;
            return;
        }
    }

    // If item an new item, then create an object for that item
    aucTextItems.push_back({ item, 1});
}

std::vector<int> UserOrders::getGroupAuctionNumbers(const std::string& groupName) const
{
    auto it = aucNumberItemsGroups.find(groupName);
    if (it != aucNumberItemsGroups.end()) {
        return it->second;
    }
    return {};
}

std::unordered_map<std::string, std::vector<int>> UserOrders::getAllAuctionGroups() const
{
    return aucNumberItemsGroups;
}

// Get Auction Number Items
std::vector<int> UserOrders::getAucNumberItems() const {
    return aucNumberItems;
}

// Get Auction Text Items
std::vector<UserOrders::Items> UserOrders::getAucTextItems() const {
    return aucTextItems;
}

void UserOrders::merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<int> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        }
        else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < leftArr.size()) arr[k++] = leftArr[i++];
    while (j < rightArr.size()) arr[k++] = rightArr[j++];
}

void UserOrders::mergeSort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void UserOrders::sortAucNumberItems() {
    for (auto& pair : aucNumberItemsGroups) {
        std::vector<int>& items = pair.second;
        if (!items.empty()) {
            mergeSort(items, 0, items.size() - 1);
        }
    }
}

void UserOrders::mergeItems(std::vector<Items>& arr, int left, int mid, int right) {
    std::vector<Items> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<Items> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i].itemName <= rightArr[j].itemName) {
            arr[k++] = leftArr[i++];
        }
        else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < leftArr.size()) arr[k++] = leftArr[i++];
    while (j < rightArr.size()) arr[k++] = rightArr[j++];
}

void UserOrders::mergeSortItems(std::vector<Items>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSortItems(arr, left, mid);
    mergeSortItems(arr, mid + 1, right);
    mergeItems(arr, left, mid, right);
}

void UserOrders::sortAll() {
    if (!aucNumberItemsGroups.empty()) {
        sortAucNumberItems();
    }
    if (!aucTextItems.empty()) {
        mergeSortItems(aucTextItems, 0, aucTextItems.size() - 1);
    }
}

