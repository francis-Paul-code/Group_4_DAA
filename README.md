# Group_4_DAA

## Supported stepped array algorithms

- **Bubble Sort:** each Step compares one adjacent pair and swaps it if needed.
- **Binary Search:** each Step examines one midpoint; enter an ascending array and a target before applying.

Select an algorithm, enter the data (and a search target for Binary Search), choose **Apply data / Reset**, then use **Step** or **Start**. Both controls use the same advance callback. Binary Search rejects arrays that are not in ascending order.

The algorithm counters exclude visualization work; redrawing the entire array costs O(n) UI work per step.

Build and run the algorithm step tests with:

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
