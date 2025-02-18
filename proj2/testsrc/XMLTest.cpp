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

TEST(XMLWriterTest, WriteBasicXML) {
    auto dataSink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(dataSink);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;  // ✅ Ensure it's a StartElement
    entity.DNameData = "test";
    
    writer.WriteEntity(entity);

    std::string expected_output = "<test/>";
    EXPECT_EQ(dataSink->String(), expected_output);
}

TEST(XMLWriterTest, Nested) {
    auto dataSink = std::make_shared<CStringDataSink>();
    CXMLWriter writer(dataSink);

    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    entity.DNameData = "test";

    writer.WriteEntity(entity);
    std::string expected_output = "<test/>";
    EXPECT_EQ(dataSink->String(), expected_output);

    SXMLEntity nest;
    nest.DType = SXMLEntity::EType::StartElement;
    nest.DNameData = "nest";

    writer.WriteEntity(nest);
    std::string expected_output1 = "<test/><nest/>";
    EXPECT_EQ(dataSink->String(), expected_output1);
}



