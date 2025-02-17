### **4. `CXMLWriter.md`**

```markdown
# CXMLWriter Class Documentation

The `CXMLWriter` class is responsible for writing XML data to a sink. It generates XML elements, attributes, and text content, and formats them according to XML standards.

## Constructor

### `CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)`

- **sink**: A shared pointer to a `CDataSink` object, where the generated XML will be written.

## Functions

### `bool CXMLWriter::WriteEntity(const SXMLEntity &entity)`

Writes an XML entity (start element, end element) to the sink.

**Parameters:**
- `entity`: The XML entity (start or end element) to be written.

**Returns:**
- `true` if the entity was successfully written.
- `false` if an error occurred.

**Example:**
```cpp
SXMLEntity entity;
CXMLWriter writer(sink);
entity.DType = SXMLEntity::EType::StartElement;
entity.DNameData = "book";
writer.WriteEntity(entity);
