# CDSVReader Class Documentation

The `CDSVReader` class is responsible for reading and parsing Delimited Separated Values (DSV) files (e.g., CSV files). It reads data row by row and converts it into a `std::vector<std::string>` format.

## Constructor

### `CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)`

- **src**: A shared pointer to a `CDataSource` object, which specifies the data source to read from.
- **delimiter**: The delimiter used to separate values in the file (typically `,` for CSV, `\t` for TSV).

## Functions

### `bool CDSVReader::ReadRow(std::vector<std::string>& row)`

Reads a single row from the data source and stores the resulting column values in the provided `row` vector.

**Parameters:**
- `row`: A reference to a `std::vector<std::string>` to store the parsed row data.

**Returns:**
- `true` if the row was successfully read.
- `false` if no more rows are available (e.g., end of file).

**Example:**
```cpp
std::vector<std::string> row;
CDSVReader reader(dataSource, ',');
while (reader.ReadRow(row)) {
    for (const auto& cell : row) {
        std::cout << cell << " ";
    }
    std::cout << std::endl;
}
