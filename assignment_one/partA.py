#1. Basic array operations

arr = [10, 20, 30, 40, 50]

# Access
print("Element at index 2:", arr[2])

# Update
arr[2] = 35
print("After update:", arr)

# Insert
arr.append(60)
print("After insertion:", arr)

# Delete
arr.remove(20)
print("After deletion:", arr)

# Traversal
print("Array elements:")
for element in arr:
    print(element)

print("=======================================================")

#2. Traversal of 1D, 2D and 3D arrays + base address
#dimensional arrayarr1 = [10, 20, 30, 40, 50]

arr1 = [10, 20, 30, 40, 50]

print("1D Array:")
for i in range(len(arr1)):
    print(arr1[i])

print("Base address:", id(arr1[0]))

print("=======================================================")

arr2 = [
    [10, 20, 30],
    [40, 50, 60]
]

print("\n2D Array:")
for i in range(len(arr2)):
    for j in range(len(arr2[i])):
        print(arr2[i][j], end=" ")
    print()

print("Base address:", id(arr2[0][0]))

print("=======================================================")

arr3 = [
    [
        [1, 2],
        [3, 4]
    ],
    [
        [5, 6],
        [7, 8]
    ]
]

print("\n3D Array:")
for i in range(len(arr3)):
    for j in range(len(arr3[i])):
        for k in range(len(arr3[i][j])):
            print(arr3[i][j][k], end=" ")
        print()
    print()

print("Base address:", id(arr3[0][0][0]))

#merge 2 one dimensional arrays
arr1 = [1, 2, 3, 4, 5]
arr2 = [6, 7, 8, 9, 10]

merged = arr1 + arr2

print("First array:", arr1)
print("Second array:", arr2)
print("Merged array:", merged)

print("========================================================")

#Minimum and maximum elemets with their locations
arr = [
    [10, 25, 3],
    [45, 2, 18],
    [30, 50, 7]
]


def find_min(arr):
    minimum = arr[0][0]
    row = 0
    column = 0

    for i in range(len(arr)):
        for j in range(len(arr[i])):
            if arr[i][j] < minimum:
                minimum = arr[i][j]
                row = i
                column = j

    return minimum, row, column


def find_max(arr):
    maximum = arr[0][0]
    row = 0
    column = 0

    for i in range(len(arr)):
        for j in range(len(arr[i])):
            if arr[i][j] > maximum:
                maximum = arr[i][j]
                row = i
                column = j

    return maximum, row, column


minimum, min_row, min_column = find_min(arr)
maximum, max_row, max_column = find_max(arr)

print("Minimum:", minimum)
print("Location: row", min_row, "column", min_column)

print("Maximum:", maximum)
print("Location: row", max_row, "column", max_column)

print("========================================================")

