#include "MyCoolDB.h"
#include "gtest/gtest.h"

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

TEST(DB_Test, Create) {
  MyCoolDB db;

  db.Request("CREATE TABLE Books ("
             "    id    INT,"
             "    book_name  VARCHAR,"
             "    author  VARCHAR,"
             "    price   DOUBLE,"
             "    available  BOOL"
             ");");
}

TEST(DB_Test, Insert) {
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

  Result result = db.RequestQuery("SELECT * FROM Books");
  ASSERT_EQ(result.GetAmount(), 3);

  result.Next();
  ASSERT_EQ(result.Get<int>("id").GetValue(), 1);
  ASSERT_EQ(result.Get<std::string>("author").GetValue(), "Bulgakov");
  ASSERT_EQ(result.Get<std::string>("book_name").GetValue(), "M and M");
  ASSERT_EQ(result.Get<double>("price").GetValue(), 100.2);
  ASSERT_EQ(result.Get<bool>("available").GetValue(), true);

  result.Next();
  ASSERT_EQ(result.Get<int>("id").GetValue(), 2);
  ASSERT_EQ(result.Get<std::string>("author").GetValue(), "Tolstoy");
  ASSERT_EQ(result.Get<std::string>("book_name").GetValue(), "War and Peace");
  ASSERT_TRUE(result.Get<double>("price").IsNull());
  ASSERT_TRUE(result.Get<bool>("available").IsNull());

  result.Next();
  ASSERT_EQ(result.Get<int>("id").GetValue(), 3);
  ASSERT_EQ(result.Get<std::string>("author").GetValue(), "Pushkin");
  ASSERT_TRUE(result.Get<std::string>("book_name").IsNull());
  ASSERT_EQ(result.Get<double>("price").GetValue(), 50.0);
  ASSERT_TRUE(result.Get<bool>("available").IsNull());

  ASSERT_FALSE(result.Next());
}

TEST(DB_Test, Select) {
  MyCoolDB db = TestDB();

  db.Request("INSERT INTO Books(id, author, book_name, available) VALUES (4, 'Bulgakov', 'Morphine', false)");

  Result result_1 = db.RequestQuery("SELECT id, available, price FROM Books WHERE author = 'Bulgakov'");
  ASSERT_EQ(result_1.GetAmount(), 2);
  ASSERT_EQ(result_1.Get<int>("id").GetValue(), 1);
  ASSERT_EQ(result_1.Get<bool>("available").GetValue(), true);
  result_1.Next();
  ASSERT_EQ(result_1.Get<int>("id").GetValue(), 4);
  ASSERT_EQ(result_1.Get<double>("price").IsNull(), true);

  Result result_2 = db.RequestQuery("SELECT author FROM Books WHERE id <= 2");
  ASSERT_EQ(result_2.GetAmount(), 2);
  ASSERT_EQ(result_2.Get<std::string>("author").GetValue(), "Bulgakov");
  result_2.Next();
  ASSERT_EQ(result_2.Get<std::string>("author").GetValue(), "Tolstoy");
  ASSERT_FALSE(result_2.Next());

  Result result_3 = db.RequestQuery("SELECT book_name, FROM Books WHERE price >= 70");
  ASSERT_EQ(result_3.GetAmount(), 1);
  ASSERT_EQ(result_3.Get<std::string>("book_name").GetValue(), "M and M");
  ASSERT_FALSE(result_3.Next());

  Result result_4 = db.RequestQuery("SELECT id FROM Books WHERE id < 3 OR available IS NULL");
  ASSERT_EQ(result_4.GetAmount(), 3);
  result_4.Next();
  ASSERT_EQ(result_4.Get<int>("id").GetValue(), 1);
  result_4.Next();
  ASSERT_EQ(result_4.Get<int>("id").GetValue(), 2);
  result_4.Next();
  ASSERT_EQ(result_4.Get<int>("id").GetValue(), 3);
  ASSERT_FALSE(result_3.Next());

  Result result_5 = db.RequestQuery("SELECT id, author FROM Books WHERE author != 'Tolstoy' AND author != 'Pushkin'");
  ASSERT_EQ(result_5.GetAmount(), 2);
  ASSERT_EQ(result_5.Get<int>("id").GetValue(), 1);
  ASSERT_EQ(result_5.Get<std::string>("author").GetValue(), "Bulgakov");
  ASSERT_TRUE(result_5.Next());
}

TEST(DB_Test, Update) {
  MyCoolDB db = TestDB();

  db.Request("UPDATE Books SET available = false, price = 300 WHERE book_name = 'War and Peace'");
  db.Request("UPDATE Books SET available = false WHERE id >= 2");
  Result result = db.RequestQuery("SELECT * FROM Books WHERE available = false");
  ASSERT_EQ(result.GetAmount(), 2);

  result.Next();
  ASSERT_EQ(result.Get<int>("id").GetValue(), 2);
  ASSERT_EQ(result.Get<double>("price").GetValue(), 300);
  ASSERT_EQ(result.Get<bool>("available").GetValue(), false);

  result.Next();
  ASSERT_EQ(result.Get<int>("id").GetValue(), 3);
  ASSERT_FALSE(result.Get<bool>("available").IsNull());
}

TEST(DB_Test, Delete) {
  MyCoolDB db = TestDB();

  db.Request("DELETE FROM Books WHERE id <= 2");
  Result result1 = db.RequestQuery("SELECT * FROM Books");
  ASSERT_EQ(result1.GetAmount(), 1);

  db.Request("DELETE FROM Books");
  Result result2 = db.RequestQuery("SELECT * FROM Books");
  ASSERT_EQ(result2.GetAmount(), 0);
}

TEST(DB_Test, DropTable) {
  MyCoolDB db = TestDB();

  db.Request("CREATE TABLE TestTable ("
             "    test INT"
             ")");

  ASSERT_EQ(db.GetTableNames(), std::vector<std::string>({"Books", "TestTable"}));
  db.Request("DROP TABLE Books");
  ASSERT_EQ(db.GetTableNames(), std::vector<std::string>({"TestTable"}));
  db.Request("DROP TABLE TestTable");
  ASSERT_EQ(db.GetTableNames(), std::vector<std::string>());
}

TEST(DB_Test, PrimaryKey) {
  MyCoolDB db = TestDB();

  db.Request("CREATE TABLE Characters ("
             "    id INT PRIMARY KEY,"
             "    author VARCHAR"
             ")");

  db.Request("INSERT INTO Characters(id, author) VALUES (1, 'Bulgakov M.A.')");
  db.Request("INSERT INTO Characters(id, author) VALUES (2, 'Tolstoy L.N.')");
}

TEST(DB_Test, ForeignKey) {
  MyCoolDB db = TestDB();

  db.Request("CREATE TABLE Characters ("
             "    id INT PRIMARY KEY,"
             "    author_id INT,"
             "    author VARCHAR,"
             "    FOREIGN KEY (author_id) REFERENCES Books(id)"
             ")");

  db.Request("INSERT INTO Characters(id, author_id, author) VALUES (1, 1, 'Bulgakov M.A.')");
  db.Request("INSERT INTO Characters(id, author_id, author) VALUES (2, 1, 'Bulgakov M.')");
  db.Request("INSERT INTO Characters(id, author_id, author) VALUES (3, 1, 'Bulgakov Mihail')");
  db.Request("INSERT INTO Characters(id, author_id, author) VALUES (4, 2, 'Tolstoy L.N.')");
  db.Request("INSERT INTO Characters(id, author_id, author) VALUES (5, 2, 'Tolstoy L.')");
  db.Request("INSERT INTO Characters(id, author_id, author) VALUES (6, 2, 'Tolstoy Lev')");
}

TEST(DB_Test, SaveLoad) {
  MyCoolDB test_db = TestDB();
  test_db.Save("/Users/danilkasirin/projects_C/Programming/labwork-12-danilkashirin/my_sql/test.db");

  MyCoolDB new_db;
  new_db.Load("/Users/danilkasirin/projects_C/Programming/labwork-12-danilkashirin/my_sql/test.db");

  Result result = new_db.RequestQuery("SELECT * FROM Books");
  ASSERT_EQ(result.GetAmount(), 3);

  ASSERT_EQ(result.Get<int>("id").GetValue(), 1);
  ASSERT_EQ(result.Get<std::string>("author").GetValue(), "Bulgakov");
  ASSERT_EQ(result.Get<std::string>("book_name").GetValue(), "M and M");
  ASSERT_EQ(result.Get<double>("price").GetValue(), 100.2);
  ASSERT_EQ(result.Get<bool>("available").GetValue(), true);

  result.Next();
  ASSERT_EQ(result.Get<int>("id").GetValue(), 2);
  ASSERT_EQ(result.Get<std::string>("author").GetValue(), "Tolstoy");
  ASSERT_EQ(result.Get<std::string>("book_name").GetValue(), "War and Peace");
  ASSERT_TRUE(result.Get<double>("price").IsNull());
  ASSERT_TRUE(result.Get<bool>("available").IsNull());

  result.Next();
  ASSERT_EQ(result.Get<int>("id").GetValue(), 3);
  ASSERT_EQ(result.Get<std::string>("author").GetValue(), "Pushkin");
  ASSERT_TRUE(result.Get<std::string>("book_name").IsNull());
  ASSERT_EQ(result.Get<double>("price").GetValue(), 50.0);
  ASSERT_TRUE(result.Get<bool>("available").IsNull());

  ASSERT_FALSE(result.Next());
}

TEST(DB_Test, Join) {
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

  Result result_1 = db.RequestQuery("SELECT * FROM Books INNER JOIN Characters ON Characters.author_id = Books.id");
  ASSERT_EQ(result_1.GetAmount(), 6);
  result_1.Next();
  ASSERT_EQ(result_1.Get<int>("Characters.id").GetValue(), 1);
  ASSERT_EQ(result_1.Get<int>("Books.id").GetValue(), 1);
  ASSERT_EQ(result_1.Get<std::string>("Books.book_name").GetValue(), "M and M");
  ASSERT_EQ(result_1.Get<std::string>("Characters.character").GetValue(), "Master");

  result_1.Next();
  ASSERT_EQ(result_1.Get<int>("Characters.id").GetValue(), 2);
  ASSERT_EQ(result_1.Get<int>("Books.id").GetValue(), 1);
  ASSERT_EQ(result_1.Get<std::string>("Books.book_name").GetValue(), "M and M");
  ASSERT_EQ(result_1.Get<std::string>("Characters.character").GetValue(), "Voland");

  result_1.Next();
  ASSERT_EQ(result_1.Get<int>("Characters.id").GetValue(), 3);
  ASSERT_EQ(result_1.Get<int>("Books.id").GetValue(), 1);
  ASSERT_EQ(result_1.Get<std::string>("Books.book_name").GetValue(), "M and M");
  ASSERT_EQ(result_1.Get<std::string>("Characters.character").GetValue(), "Azazelo");

  result_1.Next();
  ASSERT_EQ(result_1.Get<int>("Characters.id").GetValue(), 4);
  ASSERT_EQ(result_1.Get<int>("Books.id").GetValue(), 2);
  ASSERT_EQ(result_1.Get<std::string>("Books.book_name").GetValue(), "War and Peace");
  ASSERT_EQ(result_1.Get<std::string>("Characters.character").GetValue(), "Bezuhov");

  result_1.Next();
  ASSERT_EQ(result_1.Get<int>("Characters.id").GetValue(), 5);
  ASSERT_EQ(result_1.Get<int>("Books.id").GetValue(), 2);
  ASSERT_EQ(result_1.Get<std::string>("Books.book_name").GetValue(), "War and Peace");
  ASSERT_EQ(result_1.Get<std::string>("Characters.character").GetValue(), "Bolkonskiy");

  result_1.Next();
  ASSERT_EQ(result_1.Get<int>("Characters.id").GetValue(), 6);
  ASSERT_EQ(result_1.Get<int>("Books.id").GetValue(), 2);
  ASSERT_EQ(result_1.Get<std::string>("Books.book_name").GetValue(), "War and Peace");
  ASSERT_EQ(result_1.Get<std::string>("Characters.character").GetValue(), "Rostova");
  ASSERT_FALSE(result_1.Next());


  db.Request("INSERT INTO Characters(id, author_id, character) VALUES (7, 4, 'Karenina')");
  Result result_2 = db.RequestQuery("SELECT * FROM Books LEFT JOIN Characters ON Characters.author_id = Books.id");
  ASSERT_EQ(result_2.GetAmount(), 7);
  for (int i = 1; i <= 7; i++) result_2.Next();

  ASSERT_TRUE(result_2.Get<int>("Characters.id").IsNull());
  ASSERT_TRUE(result_2.Get<int>("Characters.author_id").IsNull());
  ASSERT_TRUE(result_2.Get<std::string>("Characters.character").IsNull());
  ASSERT_EQ(result_2.Get<int>("Books.id").GetValue(), 3);

  Result result_3 = db.RequestQuery("SELECT * FROM Characters RIGHT JOIN Books ON Characters.author_id = Books.id");
  ASSERT_EQ(result_3.GetAmount(), 7);
  for (int i = 1; i <= 7; i++) result_3.Next();
  ASSERT_TRUE(result_3.Get<int>("Characters.id").IsNull());
  ASSERT_TRUE(result_3.Get<int>("Characters.author_id").IsNull());
  ASSERT_TRUE(result_3.Get<std::string>("Characters.character").IsNull());
  ASSERT_EQ(result_3.Get<int>("Books.id").GetValue(), 3);



  Result result_4 = db.RequestQuery("SELECT * FROM Books INNER JOIN Characters ON Characters.author_id = Books.id "
                                      "WHERE Books.id >= 2 OR Characters.character = 'Azazelo'");
  ASSERT_EQ(result_4.GetAmount(), 4);
  std::cout << result_4;
  result_4.Next();
  ASSERT_EQ(result_4.Get<int>("Characters.id").GetValue(), 3);
  ASSERT_EQ(result_4.Get<int>("Books.id").GetValue(), 1);
  ASSERT_EQ(result_4.Get<std::string>("Books.book_name").GetValue(), "M and M");
  ASSERT_EQ(result_4.Get<std::string>("Characters.character").GetValue(), "Azazelo");

  result_4.Next();
  ASSERT_EQ(result_4.Get<int>("Characters.id").GetValue(), 4);
  ASSERT_EQ(result_4.Get<int>("Books.id").GetValue(), 2);
  ASSERT_EQ(result_4.Get<std::string>("Books.book_name").GetValue(), "War and Peace");
  ASSERT_EQ(result_4.Get<std::string>("Characters.character").GetValue(), "Bezuhov");
}
