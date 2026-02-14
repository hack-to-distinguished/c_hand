# Project Overview

This project features a messaging service system and an HTTP web server, both built in C. These components will be integrated into [`Tank Squared`](https://github.com/ChristianHrs/tank_squared) to enable real-time in-game chat, custom player-hosted lobbies, and future database-driven features such as player statistics tracking.

The next major step is to develop a database management system from scratch to track player statistics and manage messaging. This system will also be integrated with the other components, resulting in a fully unified implementation within Tank Squared.

**COMPONENTS:**
- Messaging Service (in active development)
- Web Server (Completed)
- Database Management System (in active development)

---

## Table of Contents
- [Simple HTTP_C Web Server](#simple-http_c-web-server)
  - [State Machine Diagram](#state-machine-diagram)
- [Benchmarking Results](#benchmarking-results)
  - [Test Parameters](#test-parameters)
  - [Results](#results)
  - [Observations](#observations)
- [Messaging System](#messaging-system)
- [SDBMS (Simple Database Management System)](#sdbms-simple-database-management-system)
- [SDBMS Syntax Guide](#sdbms-syntax-guide)
- [Project Plan](#project-plan)
  - [Phase 1: TCP Echo Server](#phase-1-tcp-echo-server)
  - [Phase 2: HTTP Server](#phase-2-http-server)
  - [Phase 3: Messaging](#phase-3-messaging)
  - [Phase 4: Database Management System (SDBMS)](#phase-4-database-management-system-sdbms)
  - [Phase 5: Integration & Expansion](#phase-5-integration--expansion)
- [How to Compile and Run the HTTP Web Server](#how-to-compile-and-run-the-http-web-server)
- [How to Compile and Run the Messaging System](#how-to-compile-and-run-the-messaging-system)
- [How to Compile and Run the SDBMS](#how-to-compile-and-run-the-sdbms)

---

## Simple HTTP_C Web Server  

A custom-built HTTP/1.1 web server written from scratch in C, designed for **high performance and scalability**.  

**Key Features:**
- Thread pool implementation for efficiently handling multiple simultaneous client connections.
- Stateful HTTP request parsing using a **state machine** for precise and robust request handling.
- Supports both GET + POST methods.

### State Machine Diagram

<img width="688" height="667" alt="HTTP Request State Machine" src="https://github.com/user-attachments/assets/98b77725-f7ef-4b65-8e52-3d474b0f31fb" />

---

## Benchmarking Results

We evaluated the server using [`wrk`](https://github.com/wg/wrk), a modern HTTP benchmarking tool. All tests were run locally on `127.0.0.1:8080` over 30-second durations.  

### Test Parameters
- **Endpoint tested:** `/static/index.html`  
- **HTTP method:** GET  
- **Duration:** 30 seconds per test  
- **Threads & Connections:** varied to simulate different levels of concurrency

**`/static/index.html`**:
<img width="1625" height="945" alt="image" src="https://github.com/user-attachments/assets/7bcddd8d-2e68-469c-99a4-62340721a2b2" />

### Results

| Threads | Connections | Duration | Requests/sec | Avg Latency | Stdev Latency | Max Latency | Requests Completed | Transfer/sec | Notes |
|--------|------------|---------|-------------|------------|---------------|------------|-----------------|--------------|-------|
| 4      | 100        | 30s     | 34,256      | 2.25ms     | 1.00ms        | 16.43ms    | 1,028,378       | 72.17MB      | Baseline, low concurrency |
| 8      | 1000       | 30s     | 31,521      | 8.91ms     | 42.14ms       | 834.18ms   | 948,533         | 66.40MB      | High concurrency, higher latency and variance |

### Observations
- Moderate concurrency (4 threads, 100 connections) achieves **high throughput (~34k req/s)** with low average latency (~2.25ms).  
- Under very high concurrency (8 threads, 1000 connections), throughput remains strong, but **latency and variability increase significantly**, highlighting limits under extreme load.  
- Minimal socket errors indicate **stable connection handling** even under stress.  

These results demonstrate that the server is **efficient at handling static file requests**, with performance naturally degrading under extreme concurrency.

---

## Messaging System  

A real-time messaging platform leveraging the **WebSocket protocol** for persistent, bidirectional communication.  

**Key Features:**
- TypeScript-based frontend for seamless message sending and receiving.
- Supports group messaging through the server.

**Messaging Page (browser view):**
<img width="2739" height="1540" alt="image" src="https://github.com/user-attachments/assets/d0f72822-17a8-4aeb-a669-d9ff1a166866" />

---

## SDBMS (Simple Database Management System)

A simple database management system developed from scratch in C.  

Currently, both a tokenizer, and parser has been implemented.

**Key Features:**
- Tokenizer: Converts SQL input into structured tokens (keywords, identifiers, symbols, etc.) for the parser.
- Parser: Converts tokens into ASTs and syntactically checks user input.
- Execution Engine: In development

**Goal:**  
To gain a deeper understanding of how databases work internally and to eventually implement SDBMS as the main database used for the network backend in conjunction with *Tank Squared* and the *Messaging System*.

---

## SDBMS Syntax Guide

### Overview
This guide describes the supported SQL syntax for database operations including SELECT, INSERT, UPDATE, and DELETE statements.

### General Rules
- All statements must end with a semicolon (`;`)
- Identifiers (table and column names) **cannot contain underscores** (`_`)
- Keywords are case-insensitive (SELECT, select, Select are equivalent)
- String literals use single quotes: `'example'`

### Statement Types

#### SELECT Statement
Retrieve data from one or more tables.

```sql
db > SELECT column1, column2 
   > FROM table1, table2
   > WHERE condition
   > ORDER BY column1 ASC, column2 DESC;
```

**Components:**
- **SELECT clause**: Specify columns or `*` for all columns
- **FROM clause**: List one or more tables (comma-separated)
- **WHERE clause** (optional): Filter results using conditions
- **ORDER BY clause** (optional): Sort results by columns with ASC (ascending) or DESC (descending)

**Note:** Column aliasing with AS is not currently supported.

#### INSERT Statement
Add new records to a table.

```sql
db > INSERT INTO tablename (column1, column2, column3)
   > VALUES (value1, value2, value3);
```

#### UPDATE Statement
Modify existing records in a table.

```sql
db > UPDATE tablename
   > SET column1 = value1, column2 = value2
   > WHERE condition;
```

#### DELETE Statement
Remove records from a table.

```sql
db > DELETE FROM tablename
   > WHERE condition;
```

### Expressions and Operators

#### Arithmetic Operators
- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division

**Precedence:** Multiplication and division evaluate before addition and subtraction. Use parentheses to override precedence.

```sql
db > SELECT salary * 1.1 + bonus FROM employees;
db > SELECT (salary + bonus) * 1.1 FROM employees;
```

#### Comparison Operators
- `=` Equal to
- `!=` Not equal to
- `<` Less than
- `<=` Less than or equal to
- `>` Greater than
- `>=` Greater than or equal to

#### Logical Operators
- `AND` Both conditions must be true
- `OR` At least one condition must be true

Use parentheses to group conditions for complex logic:
```sql
> WHERE (age > 18 AND status = 'active') OR role = 'admin'
```

**Note:** Pattern matching with LIKE is not currently supported.

### Identifiers and Literals

#### Column References
- **Simple**: `columnname`
- **Qualified**: `tablename.columnname`

Remember: No underscores allowed in identifiers.

#### Literals
- **String**: `'text value'`
- **Integer**: `42` or `-54`
- **Float**: `3.14` or `-2.43`
- **NULL**: `NULL` keyword

#### Functions
Standard function call syntax:
```sql
> functionname(arg1, arg2, ...)
```

### Special Commands

#### EXIT
Exit the SQL environment:
```sql
db > EXIT;
```

### Examples

```sql
-- Select all employees earning over 50000
db > SELECT * FROM employees WHERE salary > 50000;

-- Select with multiple conditions and logical operators
db > SELECT name, department FROM employees
   > WHERE (salary > 40000 AND status = 'active') OR role = 'manager'
   > ORDER BY name ASC;

-- Insert a new employee (note: no underscores in column names)
db > INSERT INTO employees (name, salary, department)
   > VALUES ('John Doe', 55000, 'Engineering');

-- Update employee salary with arithmetic expression
db > UPDATE employees
   > SET salary = salary * 1.1
   > WHERE department = 'Sales';

-- Delete inactive employees
db > DELETE FROM employees
   > WHERE status = 'inactive';

-- Complex SELECT with multiple tables and arithmetic
db > SELECT 
   > employees.name,
   > employees.salary * 1.15 + bonus.amount,
   > performance.rating
   > FROM employees, bonus, performance
   > WHERE employees.id = bonus.employeeid
   > AND employees.id = performance.employeeid
   > AND performance.rating >= 4.0
   > ORDER BY employees.salary DESC;
```

---

## Project Plan

### Phase 1: TCP Echo Server
- [x] Create a socket  
- [x] Bind to a port  
- [x] Listen and accept connections  
- [x] Receive data and send it back (echo)  

### Phase 2: HTTP Server
- [x] Listen on TCP port  
- [x] Apply HTTP protocol to incoming and outgoing data  
- [x] Large data streaming  

### Phase 3: Messaging
- [x] Enable users to send messages to the server  
- [ ] Create end-to-end messaging without using the server as a middleman  
- [x] Group chat messaging via server redirection  
- [ ] Ability to select which client to message  
- [x] GUI for messaging  
- [ ] Host group chat online with a 6-hour timeout between messages  

### Phase 4: Database Management System (SDBMS)
- [x] Implement tokenizer for SQL command parsing  
- [x] Implement parser to validate and interpret tokens  
- [ ] Create execution engine for running basic SQL-like commands  
- [ ] Integrate SDBMS as the backend database for the messaging system  
- [ ] Integrate SDBMS with **Tank Squared** for persistent multiplayer data  

### Phase 5: Integration & Expansion
- [ ] Use messaging system within **Tank Squared** (multiplayer mode)  
- [ ] Self-host the messaging software  
- [ ] Implement blockchain-based decentralized messaging  
- [ ] Integrate packet streaming into **Tank Squared** for real-time multiplayer  


---

## How to Compile and Run the HTTP Web Server

```bash
# Compile
make all

# Run on default port 8080
./threadpoolserver
```

## How to Compile and Run the Messaging System

```bash
# Compile
make all

# Run on default port 8080
./web_server/server 127.0.0.1

# Run one or more instances of the frontend
cd frontend

# Install the dependencies
npm install

# Run the program and navigate to the URL
npx vite
```

## How to Compile and Run the SDBMS

```bash
# Compile
make all

./sdbms
```
