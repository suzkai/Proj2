# XMLReader

## 📌 Overview
The `CXMLReader` class is responsible for **reading and parsing XML entities** from a given `CDataSource`. It uses the **Expat** library for XML parsing and supports reading **start elements, end elements, and character data**.

Additionally, if a `CDataSink` is provided, `CXMLReader` can also **write XML entities**.

---

## 📌 **Class: `CXMLReader`**
### **Constructor**
```cpp
CXMLReader(std::shared_ptr<CDataSource> src);
CXMLReader(std::shared_ptr<CDataSource> src, std::shared_ptr<CDataSink> sink);
