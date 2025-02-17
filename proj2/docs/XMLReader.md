
---

### **3. `CXMLReader.md`**

```markdown
# CXMLReader Class Documentation

The `CXMLReader` class is designed to parse XML data. It uses the Expat library to process XML elements and attributes from a data source, and returns each parsed element as an `SXMLEntity`.

## Constructor

### `CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src)`

- **src**: A shared pointer to a `CDataSource` object, specifying the XML data source.

## Functions

### `bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata = false)`

Reads an XML entity from the data source and stores it in the provided `entity` object.

**Parameters:**
- `entity`: A reference to an `SXMLEntity` object where the parsed entity will be stored.
- `skipcdata`: If `true`, CDATA sections will be skipped. If `false`, they will be included.

**Returns:**
- `true` if the entity was successfully read.
- `false` if no more entities are available (end of file).

**Example:**
```cpp
SXMLEntity entity;
CXMLReader reader(dataSource);
while (reader.ReadEntity(entity)) {
    std::cout << "Element: " << entity.DNameData << std::endl;
}
