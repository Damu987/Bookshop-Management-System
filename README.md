# Bookshop-Management-System
Create a Mysql Database
-Database Name:Management
And Tables:
Books
 int id
//Primary key
   string name
   string author
   int price
   int qty
suppliers
 int id
 //primary key
   string name
   long int phn
   string addr_line1
   string addr_line2
   string addr_city
   string addr_state
  Purchases
   int ord_id
   //primary key
    int book_id
    //FK ref(books)
     int sup_id
    //FK ref(suppliers)
     int qty
     date dt_ordered
     int eta
     char received
    //check (T or C or F) def (F)
     int inv
     employees
       int id
      //primary key
        string name
        string addr_line1
        string addr_line2
        string addr_city
        string addr_state
        long int phn
        date
      date_of_joining
       long int salary
       string mgr_status
      //check(T or F) def f

    members
     int id
    //primary key
     string name 
     string addr_line1
     string addr_line2
     string addr_city
     string addr_state
     long int phn
     date beg_date
     date end_date
     string valid

    sales
     int invoice_id
    //primary key
     int member_id
    //FK ref member(id)
      int book_id
    //FK ref books(id)
     int qty
     int amount
     date date_s
