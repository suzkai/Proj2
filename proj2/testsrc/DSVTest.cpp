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

// > Values that have either the delimiter, double quote character ‘"’, or newline must be
// quoted with double quotes.
// > Double quote characters in the cell must be replaced with two double quotes.
// > An empty line is a valid row where there are no values.
// > A double quote specified as a delimiter is interpreted as a comma ‘,’.

TEST(DSVWriterTest, WT1){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter write(Sink, '/', false);

    const std::vector<std::string> &row = {"ECS34", "\"ECS34\"", "ECS\n34", "ECS/34"};
    EXPECT_TRUE(write.WriteRow(row));

    std::string ret = Sink->String();
    EXPECT_EQ(ret, "ECS34/\"\"\"ECS34\"\"\"/\"ECS\n34\"/\"ECS/34\"\n");
}
TEST(DSVWriterTest, WT2){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter write(Sink, '/', false);

    const std::vector<std::string> &row = {"", "", "", ""};
    EXPECT_TRUE(write.WriteRow(row));

    std::string ret = Sink->String();
    EXPECT_EQ(ret, "\n");
}

TEST(DSVWriterTest, WT3){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter write(Sink, '/', false);

    const std::vector<std::string> &row = {"e/", "c/", "s/", "34/"};
    EXPECT_TRUE(write.WriteRow(row));

    std::string ret = Sink->String();
    EXPECT_EQ(ret, "\"e/\"/\"c/\"/\"s/\"/\"34/\"\n");
}

TEST(DSVWriterTest, WT4){
    auto Sink = std::make_shared<CStringDataSink>();
    CDSVWriter write(Sink, ',', true);

    const std::vector<std::string> &row = {"e", "c", "s", "34"};
    EXPECT_TRUE(write.WriteRow(row));

    std::string ret = Sink->String();
    EXPECT_EQ(ret, "\"e\",\"c\",\"s\",\"34\"\n");
}