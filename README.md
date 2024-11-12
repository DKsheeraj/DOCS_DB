# DOCS_DB

DOCS_DB is a key-value store implemented in C++. The keys in the Redis store are strings, and the values are records of students with the following fields:
- Last name
- First name
- Year of birth
- City
- Number of current coins


## Build

The project can be built using CMake version 3.15 or higher.
1. Use make build to build all the necessary
2. Navigate to build directory and execute DOCS_DB
3. For now only select hash table (1).
4. Use INFO command to see all the commands implemented

Just describing basic commands below

## Usage


### SET

```
SET key1 Arjun Vasudev 2003 India 60 
> OK
 
```
An example of using the SET command to create a record with a time limit in seconds. 
```
SET key1 Arjun Vasudev 2003 India 60 EX 60 
> OK
```

### GET

```
GET key1
```

### EXISTS

This command checks if a record with the given key exists. It returns true if the object exists or false if it doesn't:
```
EXISTS key1
> true
```

### DEL

This command deletes the key and the corresponding value, then returns true if the record was successfully deleted, otherwise false:
```
DEL key1
> true
```

