#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <ctime>

using namespace std;

map<string, int> inventory;

struct intake_record {
    string date;
    string time;
    string medicine;
    int quantity;
};

vector<intake_record> intake;

string get_date() {
    time_t current_time = time(0);
    tm* now = localtime(&current_time);

    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", now);
    return buffer;
}

void load_data() {
    ifstream file("medicine_data.txt");

    string type;
    while (file >> type) {
        if (type == "INVENTORY") {
            string medicine;
            int quantity;
            file >> medicine >> quantity;
            inventory[medicine] = quantity;
            
        } else if (type == "INTAKE") {
            intake_record r;
            file >> r.date >> r.time >> r.medicine >> r.quantity;
            intake.push_back(r);
        }
    }

    file.close();
}

void save_data() {
    ofstream file("medicine_data.txt");

   map<string, int>::iterator item;

	for (item = inventory.begin(); item != inventory.end(); item++) {
    	file << "INVENTORY " << item -> first << " " << item -> second << endl;
	}

    for (int i = 0; i < intake.size(); i++) {
        file << "INTAKE "
             << intake[i].date << " "
             << intake[i].time << " "
             << intake[i].medicine << " "
             << intake[i].quantity << endl;
    }

    file.close();
}

void record_intake() {
    string medicine, time;
    int quantity;
    
    cout << "\n- Please use lowercase letters only (Ex. biogesic) -\n";

    cout << "Medicine: ";
    cin >> medicine;

    cout << "Time: ";
    cin >> time;

    cout << "Quantity: ";
    cin >> quantity;

    if (inventory.count(medicine) == 0) {
        cout << "Medicine not found.\n\n";
        return;
    }

    if (inventory[medicine] < quantity) {
        cout << "Not enough stock.\n\n";
        return;
    }

    intake_record r;
    r.date = get_date();
    r.time = time;
    r.medicine = medicine;
    r.quantity = quantity;

    intake.push_back(r);
    inventory[medicine] -= quantity;

    save_data();

    cout << "Recorded!\n\n";
}

void update_medicine() {
    string medicine;
    int quantity;
    
    cout << "\n- Please use lowercase letters only (Ex. biogesic) -\n";

    cout << "Medicine: ";
    cin >> medicine;

    cout << "Quantity: ";
    cin >> quantity;

    inventory[medicine] += quantity;

    save_data();

    cout << "Updated!\n\n";
}

void view_inventory() {
    cout << "\nINVENTORY:\n";

    map<string, int>::iterator item;
    for (item = inventory.begin(); item != inventory.end(); item++) {
        cout << item -> first << " - " << item ->  second << "\n";
    }
    
    cout << endl;
}

void view_intake() {
    string today = get_date();
    cout << "\nTODAY (" << today << "):\n";

    int count = 0;

    for (int i = 0; i < intake.size(); i++) {
        if (intake[i].date == today) {
            cout << intake[i].time << " - " << intake[i].medicine << " (" << intake[i].quantity << ")\n";
            count++;
        }
    }
    
    cout << endl;

    if (count == 0) {
        cout << "No records\n\n";
    }
}

int main() {
    load_data();

    int choice;
    
    cout << "Hello! Welcome to your medicine inventory and intake manager. \nTo get started, Please pick a number.\n\n";

    while (true) {
        cout << "===============================================================\n";
        cout << "1. Record Intake\n";
        cout << "2. Update Medicine\n";
        cout << "3. View Inventory\n";
        cout << "4. View Today Intake\n";
        cout << "5. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            record_intake();
            
        } else if (choice == 2) {
            update_medicine();
            
        } else if (choice == 3) {
            view_inventory();
            
        } else if (choice == 4) {
            view_intake();
            
        } else if (choice == 5) {
            break;
            
        } else {
            cout << "Invalid\n\n";
        }
    }

    return 0;
}
