#include <cppconn/driver.h> 
#include <cppconn/exception.h> 
#include <cppconn/statement.h> 
#include <cppconn/prepared_statement.h>
#include <iostream> 
#include <mysql_connection.h> 
#include <mysql_driver.h> 
#include <stdlib.h>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include <conio.h>
using namespace std;
sql::mysql::MySQL_Driver* driver;
sql::Connection* con;


//Function to initialize database connection

void initDB()
{
	driver = sql::mysql::get_mysql_driver_instance();

	//connection to database

	con = driver->connect("tcp://127.0.0.1:3306", "root", "rt78f%lc");
	
	//connecting to specific database
	
	con->setSchema("Management");
	cout << "\t\t\tConnected succesfully\n\n";
}

//execute a sql query and returns a resultset
sql::ResultSet* executeQuery(const string& query) {
	try {
		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* res = stmt->executeQuery(query);
		delete stmt;
		return res;
	}
	catch (sql::SQLException& e) {
		cout << "Error: " << e.what() << endl;
		return nullptr; // Return nullptr on error
	}
}

sql::ResultSet* executeQuery(const string& query,const vector<sql::SQLString>& params) {
		try {
			sql::PreparedStatement* pstmt = con->prepareStatement(query);

			// Bind parameters to the prepared statement
			for (size_t i = 0; i < params.size(); ++i) {
				pstmt->setString(i + 1, params[i]);
			}

			// Execute the query and return the result set
			sql::ResultSet* res = pstmt->executeQuery();
			delete pstmt;
			return res;
		}
		catch (sql::SQLException& e) {
			cout << "Error: " << e.what() << endl;
			return nullptr;
		}
	}



void executeUpdate(const string& query, vector<sql::SQLString>& params) {
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		for (size_t i = 0; i < params.size(); ++i) {
			pstmt->setString(i + 1, params[i]);
		}
		pstmt->executeUpdate();
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "error!" << e.what();
	}
}


// Function to close the database connection
void closeDB() {
	if (con) {
		try {
			con->close();  // Close the connection
		}
		catch (sql::SQLException& e) {
			cout << "Error closing the database: " << e.what() << endl;
		}
		delete con;  // Delete the connection object
		con = nullptr;
	}
}



typedef struct {
	int date;
	int month;
	int year;
}date;

class books {
	int id;
	string name;
	string author;
	int price;
	int qty;
	double newprice;
public: void addbookdetails();
	    void updateprice();
		void search();
		void display();
};


class suppliers {
	int id;
	string name;
	string address;
	string address_city;
	string state;
	long int contactinfo;
public:void addsupplier();
	  void removesupplier();
	  void search();
};


class purchases {
	int order_id;
	int book_id;
	int sup_id;
	int qty;
	date dt_ordered;
	int eta;
	char received;
	int inv;
public: void new_ord();
	  void view();
	  void mark_receiv();
	  void mark_cancel();
};


class employees {
	int emp_id;
	string emp_name;
	string position;
	double salary;
	string hiredate;
public: bool exist_emp(int emp_id);
	  void add_emp();
	  void update();
	  void search();
	  void assign_migration();
	  void display();
	  void update_sal();
};


class members {
	int member_id;
	string name;
	long int phno;
	string address;
	string state;
	int id;
	string beg_date;
	string end_date;
	bool valid;
public: void add_mem();
	  void refresh();
	  void search();
};


class sales {
	int invoice_id;
	int member_id;
	int book_id;
	int qty;
	int amount;
	string date_s;
	double total_price;
	double total_sales;
public:
	void add();
	void find_total_sales();
};


void books::addbookdetails() {
	cin.ignore();
	cout << "\n\n\nEnter the id:";
	cin >> id;
	cout << "\nEnter the name of the book:";
	cin >> name;
	cout << "\nEnter the name of the author:";
	cin >> author;
	cout << "\nEnter the price:";
	cin >> price;
	cout << "\nEnter the qty received:";
	cin >> qty;

		string query = "INSERT INTO books(id,name,author,price,qty) VALUES(?,?,?,?,?)";
		vector<sql::SQLString> params = { to_string(id),name,author,to_string(price),to_string(qty) };
		executeUpdate(query, params);
		cout << "Book details added successfully";
}


void books::updateprice()
{
	char choice;
	cout << "\n\n\n\nEnter the id of the book for update in price:";
	cin >> id;
	cout << "\nEnter the new price:";
	cin >> newprice;
	
		string query = "UPDATE books SET price=? WHERE id=?";
		vector<sql::SQLString> params = { to_string(newprice),to_string(id) };
		executeUpdate(query, params);
			cout << "\nPrice updated successfully";
	}


void books::search()
{
	int book_id;
	cout << "\n\n\n\nEnter the book ID to search:";
	cin >> book_id;
	
	string query="SELECT * FROM books WHERE id = ? ";
	vector<sql::SQLString> params = { to_string(book_id) };
    sql::ResultSet *res=executeQuery(query,params);
	if (res!=NULL && res->next()) {
		cout << "\nBook Details:\n";
		cout << "ID:" << res->getInt("id") << endl;
		cout << "BOOK Name:" << res->getString("name") << endl;
		cout << "Author:" << res->getString("author") << endl;
		cout << "Quantity:" << res->getInt("qty") << endl;
		cout << "Price:" << res->getInt("price") << endl;
	
	}
	else {
		cout << "\nNo book found\n";
	}
	delete res;
}


void books :: display() {
	try {
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("SELECT *FROM books");
		sql::ResultSet* res = pstmt->executeQuery();
		cout << "\nAvailable Books:\n";
		while (res->next()) {
			cout << "ID:" << res->getInt("id") << endl;
			cout << "BOOK Name:" << res->getString("name") << endl;
			cout << "Author:" << res->getString("author") << endl;
			cout << "Quantity:" << res->getInt("qty") << endl;
			cout << "Price:" << res->getInt("price") << endl;
		}
		delete res;
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cout << "\nError:" << e.what();
	}
}


void suppliers::addsupplier()
{
	cin.ignore();
	cout << "\n\n\nEnter the supplier name: ";
	cin >> name;
	cout << "\nEnter the supplier ID:";
	cin >> id;
	cout << "\nEnter the Address of Supplier:";
	cin >> address;
	cin >> address_city;
	cout << "\nEnter the state of supplier:";
	cin >> state;
	cout << "\nEnter the contact Number:";
	cin >> contactinfo;
	string query = "INSERT INTO suppliers(name,id,address,address_city,state,contactinfo) VALUES(?,?,?,?,?,?)";
	vector<sql::SQLString> params = { name,to_string(id),address,address_city,state ,to_string(contactinfo)};
	executeUpdate(query, params);
	cout << "Supplier inserted successfully";
}


void suppliers::removesupplier() {
	int supplier_id;
	cout << "\nEnter the Supplier Id to remove:";
	cin >> supplier_id;
	string query = "DELETE FROM suppliers WHERE id=?";
	vector<sql::SQLString> params = {to_string(supplier_id) };
	executeUpdate(query, params);
	cout << "Supplier removed successfully";
}


void suppliers::search() {
	int supplier_id;
	cout << "\nEnter the Supplier Id to search:";
	cin >> supplier_id;
	string query = "SELECT*FROM suppliers WHERE id=?";
	vector<sql::SQLString> params = { to_string(supplier_id) };
	sql::ResultSet *res=executeQuery(query,params);
	if (res && res->next()) {
		cout << "\nSupplier Details:\n";
		cout << "ID: " << res->getInt("id") << "\n";
		cout << "Name: " << res->getString("name") << "\n";
		cout << "Address:" << res->getString("address") << "\n";
		cout << "Contact Info: " << res->getInt("contactinfo") << "\n";
	}
	else {
		cout << "No supplier found with the given ID.\n";
	}

	delete res;
}


void purchases::new_ord() {
	cout << "\n\n\n\nEnter the book id:";
	cin >> book_id;
	cout << "\nEnter the supplier id:";
	cin >> sup_id;
	cout << "Enter the qty ordered:";
	cin >> qty;
	cout << "Estimated expected delivery(in days):";
	cin >> eta;

	string bookQuery = "SELECT * FROM books WHERE id = ?";
	vector<sql::SQLString> bookParams = { to_string(book_id) };
	sql::ResultSet* bookRes = executeQuery(bookQuery,bookParams);
	if (!bookRes || !bookRes->next()) {
		cout << "Invalid book ID. Please check and try again." << endl;
		delete bookRes;
		return;
	}
	delete bookRes;

	// Check if the supplier exists in the suppliers table
	string supplierQuery = "SELECT * FROM suppliers WHERE id = ?";
	vector<sql::SQLString> supParams = { to_string(sup_id) };
	sql::ResultSet* supRes = executeQuery(supplierQuery,supParams);
	if (!supRes || !supRes->next()) {
		cout << "Invalid supplier ID. Please check and try again." << endl;
		delete supRes;
		return;
	}
	delete supRes;

	string query = "INSERT INTO purchases(book_id,sup_id,qty,eta) VALUES(?,?,?,?)";
	vector<sql::SQLString> params = { to_string(book_id),to_string(sup_id),to_string(qty),to_string(eta)};
	executeUpdate(query, params);
	cout << "\n!!!New order added!!!";
}


void purchases::mark_receiv() {
	cout << "\nEnter the order id to be received:";
	cin >> order_id;
	cout << "\nEnter Y if order have been received:";
	cin >> received;
	if (received == 'Y' || received == 'y') {
		string query = "UPDATE purchases SET received=TRUE  WHERE order_id=?";
		vector<sql::SQLString> params = {to_string(order_id) };
		executeUpdate(query, params);
		cout << "Received marked successfully";
	}
	else {
		cout << "\n\n\nNot received";
	}
}


void purchases::view() {
	string query = "SELECT * FROM purchases";
	sql::ResultSet* res = executeQuery(query);
	if (res) {
		cout << "\nPurchases:\n";
		while (res->next()) {
			cout << "Order ID:" << res->getInt("order_id") << "\n";
			cout << "Book ID: " << res->getInt("book_id") << "\n ";
			cout<< "Supplier ID: " << res->getInt("sup_id") << "\n ";
			cout<< "Order Date: " << res->getString("dt_ordered") << "\n ";
			cout<< "Quantity: " << res->getInt("qty") << "\n ";
			cout<< "Received: " << res->getString("received") << "\n";
		}
	}
	delete res;
}


void purchases::mark_cancel() {
	cout << "\nEnter the order id to be received:";
	cin >> order_id;
	cout << "\nEnter N (if you want to cancel the order):";
	cin >> received;
	if (received == 'N' || received == 'n') {
		string query = "UPDATE purchases SET received=FALSE  WHERE order_id=?";
		vector<sql::SQLString> params = { to_string(order_id) };
		executeUpdate(query, params);
		cout << "Cancelled marked successfully";
	}
}


bool employees::exist_emp(int emp_id) {
	    string query = "SELECT COUNT(*) FROM employees WHERE emp_id = ?";
		vector<sql::SQLString> params = { to_string(emp_id) };
		sql::ResultSet* res = executeQuery(query,params);

		if (res && res->next()) {
			bool exists = res->getInt(1) > 0;
			delete res;
			return exists;  // Employee exists
		}
		delete res;
		return false;  // Employee not found
	}


void employees::add_emp() {

		cout << "Enter employee ID: ";
		cin >> emp_id;

		if (exist_emp(emp_id)) {
			cout << "Employee with ID " << emp_id << " already exists. Cannot add duplicate.\n";
			return;
		}

		cin.ignore(); // Clear the input buffer
		cout << "Enter employee name: ";
		getline(cin, emp_name);
		cout << "Enter employee position: ";
		getline(cin, position);
		cout << "Enter employee salary: ";
		cin >> salary;
		cin.ignore(); // Clear the buffer again
		cout << "Enter hire date (YYYY-MM-DD): ";
		cin >> hiredate;

		string query = "INSERT INTO employees (emp_id, emp_name, position, salary, hiredate) VALUES (?, ?, ?, ?, ?)";
		vector<sql::SQLString> params = { to_string(emp_id), emp_name, position, to_string(salary), hiredate };
		executeUpdate(query, params);

		cout << "Employee added successfully.\n";
}


void employees::search() {
		int emp_id;

		cout << "Enter the employee ID to search: ";
		cin >> emp_id;

		if (!exist_emp(emp_id)) {
			cout << "Employee not found. You can add this employee.\n";
			add_emp();  
			return;
		}

		string query = "SELECT * FROM employees WHERE emp_id = ?";
	    vector<sql::SQLString> params = { to_string(emp_id) };
		sql::ResultSet* res = executeQuery(query,params);

		if (res && res->next()) {
			cout << "\nEmployee Details:\n";
			cout << "ID: " << res->getInt("emp_id") << "\n";
			cout << "Name: " << res->getString("emp_name") << "\n";
			cout << "Position: " << res->getString("position") << "\n";
			cout << "Salary: " << res->getDouble("salary") << "\n";
			cout << "Hire Date: " << res->getString("hiredate") << "\n";
		}
		else {
			cout << "No employee found with the given ID.\n";
		}

		delete res;
	}



	void employees::update(){

		cout << "Enter the employee ID to update: ";
		cin >> emp_id;

		if (!exist_emp(emp_id)) {
			cout << "Employee not found. You can add this employee.\n";
			add_emp(); 
			return;
		}

		cin.ignore(); // Clear input buffer
		cout << "Enter new employee name: ";
		getline(cin, emp_name);
		cout << "Enter new employee position: ";
		getline(cin, position);
		cout << "Enter new employee salary: ";
		cin >> salary;
		cin.ignore();
		cout << "Enter new hire date (YYYY-MM-DD): ";
		cin>> hiredate;

		string query = "UPDATE employees SET emp_name = ?, position = ?, salary = ?, hiredate = ? WHERE emp_id = ?";
		vector<sql::SQLString> params = { emp_name, position, to_string(salary), hiredate,to_string(emp_id) };
		executeUpdate(query, params);

		cout << "Employee updated successfully.\n";
	}
	void employees::update_sal() {
		cout << "\nEnter the id to update the salary of an employee:";
		cin >> emp_id;
		cout << "Enter the updated salary:";
		cin >> salary;
		string query = "UPDATE employees SET salary=? WHERE emp_id=?";
		vector<sql::SQLString> params = { to_string(salary),to_string(emp_id) };
		executeUpdate(query, params);
		cout << "\nSalary update successfully\n";
	}
	void employees::assign_migration() {
		char mgr_state;
		string mgr_status;
		cout << "\nEnter your ID for verification:";
		cin >> emp_id;
		cout << "\nEnter y or n to grant manager rights:";
		cin >> mgr_state;
		string query = "SELECT mgr_state FROM employees where emp_id=? ";
		vector<sql::SQLString> params = { to_string(emp_id)};
		sql::ResultSet* res = executeQuery(query,params);
		if (res->next()) {
			mgr_status = res->getString(1);
			if (mgr_status == "Y" || mgr_status == "y") {
				cout << "\nEmployee already has manager rights...exiting...\n";
				// Fetch the mgr_state from the result set

			}
			else {
				cout << "\nYou do not have manager rights\n" << endl;
				cout << "Do you want to assign manager rights(Y/N):";
				cin >> mgr_state;
				if (mgr_state == 'Y' || mgr_state == 'y')
					query = "UPDATE employees SET mgr_state='Y' where emp_id=? ";
				params = { to_string(emp_id) };
				executeUpdate(query, params);
				cout << endl << endl;
				cout << "Manager status updated successfully." << endl;
			}
		}
		else {
			cout << "\nEmployee not found";
		}
		delete res;

	}


	void employees::display() {
		string query = "SELECT * FROM employees";
		sql::ResultSet* res = executeQuery(query);
		if (res && res->next()) {
			cout << "\nEmployee Details:\n";
			cout << "ID: " << res->getInt("emp_id") << "\n";
			cout << "Name: " << res->getString("emp_name") << "\n";
			cout << "Position: " << res->getString("position") << "\n";
			cout << "Salary: " << res->getDouble("salary") << "\n";
			cout << "Hire Date: " << res->getString("hiredate") << "\n";
		}
		else {
			cout << "\nNo employee found with the given ID.\n";
		}

		delete res;
		}


	void members::add_mem() {
		cout << endl << "Enter the member id:";
		cin >> id;
		cout << endl << "Enter the name of the member:";
		cin >> name;
		cout << "\nEnter phone number:";
		cin >> phno;
		cout << "\nEnter address:";
		cin >> address;
		cout << "\nEnter the name of state:";
		cin >> state;
		cout << "\nEnter beginning date:";
		cin >> beg_date;
		cout << "\nEnter end date:";
		cin >> end_date;
		string query = "INSERT INTO members(id,name,phno,address,state,beg_date,end_date) VALUES(?,?,?,?,?,?,?)";
		vector<sql::SQLString> params = { to_string(id),name,to_string(phno),address,state,beg_date,end_date };
		executeUpdate(query, params);
		query = "SELECT id FROM members WHERE phno=?";
		params = { to_string(phno) };
		sql::ResultSet* res = executeQuery(query,params);
		if (res && res->next()) {
			cout << "\n\nMembers added successfully\n\n";
		}
		delete res;
}


	void members::refresh() {
		cout << "Is the member valid? (1 for Yes, 0 for No): ";
		cin >> valid;
		string query = "UPDATE members SET valid=? WHERE end_date<=curdate();";
		vector<sql::SQLString> params = { to_string(valid) };
		executeUpdate(query, params);
	}


	void members::search() {
		int member_id;
		cout << "\nEnter member id:";
		cin >> member_id;
		string query = "SELECT * FROM members WHERE id=?";
		vector<sql::SQLString> params = { to_string(member_id) };
		sql::ResultSet* res = executeQuery(query,params);
		if (res && res->next()) {
			cout << endl << "Member details:" << endl;
			cout << "Name:" << res->getString("name") << endl;
			cout << "Address:" << res->getString("address") << endl;
			cout << "State:" << res->getString("state") << endl;
			cout << "Contact number:" << res->getInt("phno") << endl;
		}
		else {
			cout <<endl<< "No member found!!";
		}
		delete res;
	}


	void sales::add() {
			cout << endl << "Enter member id:";
			cin >> member_id;
			cout << endl << "Enter the book ID:";
			cin >> book_id;
			cout << endl << "Enter the quantity:";
			cin >> qty;
			cout << endl << "Enter the sales date:";
			cin >> date_s;

			// Fetch the price from the books table
			string query = "SELECT price FROM books WHERE id=?";
			vector<sql::SQLString> params = { to_string(book_id) };

			sql::ResultSet* res = executeQuery(query,params); // Execute the query

			if (res && res->next()) { // Check if the result is valid and move to the first record
				int price_per_unit = res->getInt("price"); // Get the price
				cout << "Price per unit: " << price_per_unit << endl; 

				// Ensure the quantity is positive
				if (qty <= 0) {
					cout << "Invalid quantity. Must be greater than 0." << endl;
					delete res;
					return;
				}

				// Calculate total amount
				amount = price_per_unit * qty;
				cout << "The bill amount: " << amount << endl; // Display the amount

				if (amount < 0) {
					cout << "Error: Negative bill amount!" << endl;
				}
			}
			else {
				cout << "Invalid book id or no price available." << endl;
				delete res;
				return;
			}
			delete res; 

			// Insert the sales record with current date
			query = "INSERT INTO sales(member_id, book_id, qty, amount, date_s) VALUES(?,?,?,?,?)";
			params = { to_string(member_id), to_string(book_id), to_string(qty), to_string(amount),date_s };

			executeUpdate(query, params); // Execute the insert query

			// Fetch and display the last inserted invoice id
			query = "SELECT LAST_INSERT_ID() AS invoice_id";
			res = executeQuery(query);
			if (res!=NULL && res->next()) {
			   int invoice_id = res->getInt("invoice_id");
				cout << "The invoice id for the bill is: " << invoice_id << endl;
			}
			delete res;
		}


	void sales::find_total_sales() {
		string query = "SELECT SUM(amount) AS total_sales FROM sales WHERE YEAR(date_s)=YEAR(curdate());";
		sql::ResultSet* res = executeQuery(query);
		if (res != NULL && res->next()) {
			double total_sales = res->getDouble("total_sales");
			cout << endl << "Total sales this year:" << total_sales;
		}
		delete res;
	}


	void book_menu();
	void sup_menu();
	void pur_menu();
	void emp_menu();
	void mem_menu();
	void sal_menu();

	// main menu

	void main_menu()
	{
		int c;
		cout << "*************************************************" << endl;
		cout << "         BOOKSHOP MANGEMENT SYSTEM" << endl;
		cout << "*************************************************" << endl;
		cout << "   1. BOOKS" << endl;
		cout << "   2. SUPPLIERS" << endl;
		cout << "   3. PURCHASES" << endl;
		cout << "   4. EMPLOYEES" << endl;
		cout << "   5. MEMBERS" << endl;
		cout << "   6. SALES" << endl;
		cout << "   7. EXIT" << endl << endl << endl;
		cout << "Enter Your Choice : ";
		cin >> c;
		switch (c)
		{
		case 1:
			system("cls");
			book_menu();
			break;
		case 2:
			system("cls");
			sup_menu();
			break;
		case 3:
			system("cls");
			pur_menu();
			break;
		case 4:
			system("cls");
			emp_menu();
			break;
		case 5:
			system("cls");
			mem_menu();
			break;
		case 6:
			system("cls");
			sal_menu();
			break;
		case 7:
			exit(0);
		default:
			system("cls");
			cout << "Wrong Input" << endl << endl << "Invalid input";
			break;
		}
		return;
	}

	// book menu

	void book_menu()
	{
		int c;
		books b;
		cout << "*************************************************" << endl;
		cout << "                  BOOK MENU" << endl;
		cout << "*************************************************" << endl;
		cout << "   1. ADD" << endl;
		cout << "   2. UPDATE PRICE" << endl;
		cout << "   3. SEARCH" << endl;
		cout << "   4. DISPLAY ALL" << endl;
		cout << "   5. RETURN TO MAIN MENU" << endl << endl << endl;
		cout << "Enter Your Choice : ";
		cin >> c;
		switch (c)
		{
		case 1:
			b.addbookdetails();
			break;
		case 2:
			b.updateprice();
			break;
		case 3:
			b.search();
			break;
		case 4:
			b.display();
			break;
		case 5:
			main_menu();
			return;
			break;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
		}
		return;
	}

	// supp menu

	void sup_menu()
	{
		int c;
		suppliers s;
		cout << "*************************************************" << endl;
		cout << "                SUPPLIER MENU" << endl;
		cout << "*************************************************" << endl;
		cout << "   1. ADD" << endl;
		cout << "   2. REMOVE" << endl;
		cout << "   3. SEARCH" << endl;
		cout << "   4. RETURN TO MAIN MENU" << endl << endl << endl;
		cout << "Enter Your Choice : ";
		cin >> c;
		switch (c)
		{
		case 1:
			s.addsupplier();
			break;
		case 2:
			s.removesupplier();
			break;
		case 3:
			s.search();
			break;
		case 4:
			main_menu();
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
		}
	}

	// purchase menu

	void pur_menu()
	{
		int c;
		purchases p;
		cout << "*************************************************" << endl;
		cout << "                PURCHASES MENU" << endl;
		cout << "*************************************************" << endl;
		cout << "   1. New Order" << endl;
		cout << "   2. View All" << endl;
		cout << "   3. Cancel Order" << endl;
		cout << "   4. Recieved Order" << endl;
		cout << "   5. RETURN TO MAIN MENU" << endl << endl << endl;
		cout << "Enter Your Choice : ";
		cin >> c;
		switch (c)
		{
		case 1:
			p.new_ord();
			break;
		case 2:
			p.view();
			break;
		case 3:
			p.mark_cancel();
			break;
		case 4:
			p.mark_receiv();
			break;
		case 5:
			main_menu();
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
		}
	}

	// emp_menu

	void emp_menu()
	{
		int c;
		employees e;
		cout << "*************************************************" << endl;
		cout << "                 EMPLOYEE MENU" << endl;
		cout << "*************************************************" << endl;
		cout << "   1. New Employee" << endl;
		cout << "   2. Search Employee" << endl;
		cout << "   3. Assign Manager" << endl;
		cout << "   4. View All" << endl;
		cout << "   5. Update Salary" << endl;
		cout << "   6. Update" << endl;
		cout << "   7. RETURN TO MAIN MENU" << endl << endl << endl;
		cout << "Enter Your Choice : ";
		cin >> c;
		switch (c)
		{
		case 1:
			e.add_emp();
			break;
		case 2:
			e.search();
			break;
		case 3:
			e.assign_migration();
			break;
		case 4:
			e.display();
			break;
		case 5:
			e.update_sal();
			break;
		case 6:
			e.update();
			break;
		case 7:
			main_menu();
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
		}
	}


	// mem menu

	void mem_menu()
	{
		int c;
		members m;
		cout << "*************************************************" << endl;
		cout << "                 MEMBERS MENU" << endl;
		cout << "*************************************************" << endl;
		cout << "   1. New Member" << endl;
		cout << "   2. Search Member" << endl;
		cout << "   3. RETURN TO MAIN MENU" << endl << endl << endl;
		cout << "Enter Your Choice : ";
		cin >> c;
		switch (c)
		{
		case 1:
			m.add_mem();
			break;
		case 2:
			m.search();
			break;
		case 3:
			main_menu();
		case 4:
			m.refresh();
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
		}
	}

	// sal_menu

	void sal_menu()
	{
		int c;
		sales s;
		cout << "*************************************************" << endl;
		cout << "                 SALES MENU" << endl;
		cout << "*************************************************" << endl;
		cout << "   1. Add New Bill" << endl;
		cout << "   2. Total Sales Of The Year" << endl;
		cout << "   3. RETURN TO MAIN MENU" << endl << endl << endl;
		cout << "Enter Your Choice : ";
		cin >> c;
		switch (c)
		{
		case 1:
			s.add();
			break;
		case 2:
			s.find_total_sales();
			break;
		case 3:
			main_menu();
			return;
		default:
			cout << "Wrong Input" << endl << "Invalid input";
			break;
		}
	}
	int main()
	{
		initDB();
		main_menu();
		closeDB();
		return 0;
	}