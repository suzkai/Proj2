#include "gtest/gtest.h"
#include "DSVReader.h"
#include "DSVWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

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

TEST(DSVWriterTest, WriteSimpleRow) {
    auto dataSink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(dataSink, ',');

    std::vector<std::string> row = {"one", "two", "three"};
    EXPECT_TRUE(writer.WriteRow(row));

    std::string expected_output = "one,two,three\n";
    EXPECT_EQ(dataSink->String(), expected_output);
}
