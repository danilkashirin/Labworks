#include "Column.h"
#include "MyCoolDB.h"

MyCoolDB TestDB() {
  MyCoolDB db;

  db.Request("CREATE TABLE Books ("
             "    id    INT,"
             "    book_name  VARCHAR,"
             "    author  VARCHAR,"
             "    price   DOUBLE,"
             "    available  BOOL"
             ");");
  db.Request("INSERT INTO Books(id, book_name, author, price, available) VALUES (1, 'M and M', 'Bulgakov', 100.2, true)");
  db.Request("INSERT INTO Books(id, author, book_name) VALUES (2, 'Tolstoy', 'War and Peace')");
  db.Request("INSERT INTO Books(id, author, price) VALUES (3, 'Pushkin', 50.0)");
  return db;
}

int main() {

  MyCoolDB db = TestDB();

  db.Request("CREATE TABLE Characters ("
             "    id INT PRIMARY KEY,"
             "    author_id INT,"
             "    character VARCHAR"
             ")");

  db.Request("INSERT INTO Characters(id, author_id, character) VALUES (1, 1, 'Master')");
  db.Request("INSERT INTO Characters(id, author_id, character) VALUES (2, 1, 'Voland')");
  db.Request("INSERT INTO Characters(id, author_id, character) VALUES (3, 1, 'Azazelo')");
  db.Request("INSERT INTO Characters(id, author_id, character) VALUES (4, 2, 'Bezuhov')");
  db.Request("INSERT INTO Characters(id, author_id, character) VALUES (5, 2, 'Bolkonskiy')");
  db.Request("INSERT INTO Characters(id, author_id, character) VALUES (6, 2, 'Rostova')");


}
