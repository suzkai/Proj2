#include "gtest/gtest.h"
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

// > Values that have either the delimiter, double quote character ‘"’, or newline must be
// quoted with double quotes.
// > Double quote characters in the cell must be replaced with two double quotes.
// > An empty line is a valid row where there are no values.
// > A double quote specified as a delimiter is interpreted as a comma ‘,’.

TEST(DSVReaderTest, ReadSimpleRow) {
    std::string input_data = "one,two,three\nfour,five,six\n";
    auto dataSource = std::make_shared<CStringDataSource>(input_data);
    CDSVReader reader(dataSource, ',');

    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "one");
    EXPECT_EQ(row[1], "two");
    EXPECT_EQ(row[2], "three");
}

TEST(DSVReaderTest, RT1) {
    std::string input_data = "\"15 April\",\"Muniz, Alvin \"\"Hank\"\"\",\"A\"";
    auto dataSource = std::make_shared<CStringDataSource>(input_data);
    CDSVReader reader(dataSource, ',');

    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "15 April");
    EXPECT_EQ(row[1], "Muniz, Alvin \"Hank\"");
    EXPECT_EQ(row[2], "A");
}

TEST(DSVReaderTest, RT2) {
    std::string input_data = "\"\",\"\",\"\"";
    auto dataSource = std::make_shared<CStringDataSource>(input_data);
    CDSVReader reader(dataSource, ',');

    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "");
    EXPECT_EQ(row[1], "");
    EXPECT_EQ(row[2], "");
}

TEST(DSVReaderTest, RT3) {
    std::string input_data = "\"....\n.....\n\"";
    auto dataSource = std::make_shared<CStringDataSource>(input_data);
    CDSVReader reader(dataSource, ',');

    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_FALSE(reader.End());
}

// Only thing your reader is missing is if there is a \n at end
// **I think**
TEST(DSVReaderTest, RT4) {
    std::string input_data = "\".\"\"hi\"\"!\"\n";
    auto dataSource = std::make_shared<CStringDataSource>(input_data);
    CDSVReader reader(dataSource, ',');

    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row[0], ".\"hi\"!");
    EXPECT_TRUE(reader.End());
}

TEST(DSVReaderTest, RT5) {
    std::string input_data = "My name is \"\"Bob\"\"!";
    auto dataSource = std::make_shared<CStringDataSource>(input_data);
    CDSVReader reader(dataSource, ',');

    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row[0], "My name is \"Bob\"!");
    EXPECT_TRUE(reader.End());
}

