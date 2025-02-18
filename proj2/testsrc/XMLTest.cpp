#include "gtest/gtest.h"
#include "XMLReader.h"
#include "XMLWriter.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(XMLReaderTest, ReadBasicXML) {
    std::string xml_data = "<root><child key='value'>Text</child></root>";
    auto dataSource = std::make_shared<CStringDataSource>(xml_data);
    CXMLReader reader(dataSource);

    SXMLEntity entity;
    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DNameData, "root");
    EXPECT_TRUE(reader.ReadEntity(entity));
    EXPECT_EQ(entity.DNameData, "child");
    EXPECT_EQ(entity.AttributeValue("key"), "value");
}

TEST(XMLWriterTest, WriteOSMElement) {
    auto dataSink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(dataSink);

    // Start with the opening <osm> tag
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    entity.DNameData = "osm";
    entity.DAttributes.push_back({"version", "0.6"});
    entity.DAttributes.push_back({"generator", "osmconvert 0.8.5"});

    writer.WriteEntity(entity);

    // Now, write the closing </osm> tag
    entity.DType = SXMLEntity::EType::EndElement;
    entity.DNameData = "osm";
    writer.WriteEntity(entity);

    // The expected output should match the exact format, without any newlines or tabs
    std::string expected_output = "<osm version=\"0.6\" generator=\"osmconvert 0.8.5\"></osm>";

    // Debug output (optional)
    std::cout << "Actual XML Output: " << dataSink->String() << std::endl;

    EXPECT_EQ(dataSink->String(), expected_output);
}
