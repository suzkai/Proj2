# XMLWriter

## Overview
The `CXMLWriter` class is responsible for writing XML entities to a provided `CDataSink`. It handles **start elements**, **end elements**, and **character data** while ensuring proper XML formatting.

---

## 📌 **Class: `CXMLWriter`**
### **Constructor**
```cpp
CXMLWriter(std::shared_ptr<CDataSink> sink);
