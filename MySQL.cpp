#include<iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

using namespace std;

void viewMenu(sql::Connection* con) {
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT dish.ID, dish.name, dish.price, category.name as catname, origin.name as oriname FROM dish,category,origin WHERE available = 1 and category.ID = dish.categoryID and origin.ID = dish.originID");

    cout << "Available Menu:" << endl;
    while (res->next()) {
        cout << "ID: " << res->getInt("ID") << endl;
        cout << "Name: " << res->getString("name") << endl;
        cout << "Price: $" << res->getInt("price") << endl;
        cout << "Category: " << res->getString("catname") << endl;
        cout << "Origin: " << res->getString("oriname") << endl;
        cout << "=====================" << endl;
    }

    delete res;
    delete stmt;
}

void listCategories(sql::Connection* con) {
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT ID, name FROM category");

    cout << "Available Categories:" << endl;
    while (res->next()) {
        cout << "ID: " << res->getInt("ID") << " - Name: " << res->getString("name") << endl;
    }
    cout << "=====================" << endl;

    delete res;
    delete stmt;
}

void searchMenuByCategoryName(sql::Connection* con) {
    listCategories(con);

    string categoryName;
    cout << "Enter the name of the Category to search for: ";
    cin >> categoryName;

    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT dish.ID, dish.name, dish.price, origin.name as oriname FROM dish, category, origin WHERE category.name = ? AND dish.categoryID = category.ID AND dish.originID = origin.ID AND dish.available = 1");
    pstmt->setString(1, categoryName);
    sql::ResultSet* res = pstmt->executeQuery();

    cout << "Menu (Category: " << categoryName << "):" << endl;
    while (res->next()) {
        cout << "ID: " << res->getInt("ID") << endl;
        cout << "Name: " << res->getString("name") << endl;
        cout << "Price: $" << res->getDouble("price") << endl;
        cout << "Origin: " << res->getString("oriname") << endl; 
        cout << "=====================" << endl;
    }

    delete res;
    delete pstmt;
}

void listOrigins(sql::Connection* con) {
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT ID, name FROM origin");

    cout << "Available Origins:" << endl;
    while (res->next()) {
        cout << "ID: " << res->getInt("ID") << " - Name: " << res->getString("name") << endl;
    }
    cout << "=====================" << endl;

    delete res;
    delete stmt;
}

void searchMenuByOriginName(sql::Connection* con) {
    listOrigins(con);

    string originName;
    cout << "Enter the name of the Origin to search for: ";
    cin.ignore();
    getline(cin, originName); 

    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT dish.ID, dish.name, dish.price, category.name as catname FROM dish, origin, category WHERE origin.name = ? AND dish.originID = origin.ID AND dish.categoryID = category.ID AND dish.available = 1");
    pstmt->setString(1, originName);
    sql::ResultSet* res = pstmt->executeQuery();

    cout << "Menu (Origin: " << originName << "):" << endl;
    while (res->next()) {
        cout << "ID: " << res->getInt("ID") << endl;
        cout << "Name: " << res->getString("name") << endl;
        cout << "Price: $" << res->getDouble("price") << endl;
        cout << "Category: " << res->getString("catname") << endl;
        cout << "=====================" << endl;
    }

    delete res;
    delete pstmt;
}

void listDishes(sql::Connection* con) {
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT ID, name FROM dish");

    cout << "Dishes list:" << endl;
    while (res->next()) {
        cout << "ID: " << res->getInt("ID") << " - Name: " << res->getString("name") << endl;
    }
    cout << "=====================" << endl;

    delete res;
    delete stmt;
}

void viewDishDetails(sql::Connection* con) {
    listDishes(con);

    int dishID;
    cout << "Enter Dish ID: ";
    cin >> dishID;

    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT dish.name, dish.price, dish.available,category.name AS catname, origin.name AS oriname FROM dish, category, origin WHERE dish.ID = ? AND category.ID = dish.categoryID AND origin.ID = dish.originID ");
    pstmt->setInt(1, dishID);
    sql::ResultSet* res = pstmt->executeQuery();

    if (res->next()) {
        cout << "Dish Details (ID: " << dishID << "):" << endl;
        cout << "Name: " << res->getString("name") << endl;
        cout << "Price: $" << res->getDouble("price") << endl;
        cout << "Available: " << (res->getInt("available") ? "Yes" : "No") << endl;
        cout << "Category: " << res->getString("catname") << endl;
        cout << "Origin: " << res->getString("oriname") << endl;

        sql::PreparedStatement* pstmtIngredients = con->prepareStatement("SELECT ingredient.name FROM ingredient, recipe WHERE recipe.dishID = ? AND ingredient.ID = recipe.ingredientID");
        pstmtIngredients->setInt(1, dishID);
        sql::ResultSet* resIngredients = pstmtIngredients->executeQuery();

        cout << "Ingredients: ";
        bool first = true;
        while (resIngredients->next()) {
            if (!first) {
                cout << ", ";
            }
            cout << resIngredients->getString("name");
            first = false;
        }
        cout << endl << "=====================" << endl;

        delete resIngredients;
        delete pstmtIngredients;
    }
    else {
        cout << "Dish with ID " << dishID << " not found." << endl;
    }

    delete res;
    delete pstmt;
}
void listIngredients(sql::Connection* con) {
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT ID, name FROM ingredient");

    cout << "Available Ingredients:" << endl;
    while (res->next()) {
        cout << "ID: " << res->getInt("ID") << " - Name: " << res->getString("name") << endl;
    }
    cout << "=====================" << endl;

    delete res;
    delete stmt;
}

void viewIngredientDetails(sql::Connection* con) {
    listIngredients(con);

    int ingredientID;
    cout << "Enter Ingredient ID: ";
    cin >> ingredientID;

    sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM ingredient WHERE ID = ?");
    pstmt->setInt(1, ingredientID);
    sql::ResultSet* res = pstmt->executeQuery();

    if (res->next()) {
        cout << "Ingredient Details (ID: " << ingredientID << "):" << endl;
        cout << "Name: " << res->getString("name") << endl;
        cout << "Quantity: " << res->getInt("quantity") << endl;
        cout << "Imported Date: " << res->getString("imported_date") << endl;
        cout << "Expired Date: " << res->getString("expired_date") << endl;
        cout << "Unit: " << res->getString("unit") << endl;
        cout << "=====================" << endl;
    }
    else {
        cout << "Ingredient with ID " << ingredientID << " not found." << endl;
    }

    delete res;
    delete pstmt;
}

int main() {
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "root");

    con->setSchema("emenu");

    int choice;
    int dishID, ingredientID;

    string categoryName, originName;

    do {
        cout << "Menu Viewer" << endl;
        cout << "1. View Menu" << endl;
        cout << "2. Search Menu by Category" << endl;
        cout << "3. Search Menu by Origin" << endl;
        cout << "4. View Dish Details" << endl;
        cout << "5. View Ingredient Details" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            viewMenu(con);
            break;
        case 2:
            searchMenuByCategoryName(con);
            break;
        case 3:
            searchMenuByOriginName(con);
            break;
        case 4:
            viewDishDetails(con);
            break;
        case 5:
            viewIngredientDetails(con);
            break;
        case 6:
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;
    } while (choice != 0);

    delete con;

    return 0;
}