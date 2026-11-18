# ShelfSync — Smart Library Management System

A complete, professional, beginner-friendly library management system written in C11. Features separate Customer and Staff modes with persistent binary file storage.

---

## 📖 Project Description

ShelfSync is a console-based library management system designed to handle all core library operations including book management, customer registration, book issuing/returning, request tracking, and overdue monitoring. The system uses a menu-driven interface with proper data persistence through binary files.

---

## ✨ Features

### 👥 Dual User Modes
- **Customer Mode**: Browse books, request books, view issued/returned books, track due dates
- **Staff Mode**: Full library management including books, customers, requests, issues, and statistics

### 📚 Book Management
- Add books with unique ID, title, author, category, and copies
- View all books in formatted tables
- Search by ID, title, or author (partial, case-insensitive)
- Update book details
- Soft delete (preserves history)

### 👤 Customer Management
- Register customers with auto-generated unique 6-digit library codes
- Search customers by code or name
- View complete customer profiles (issued books, due dates, overdue, requests, history)

### 📋 Request Workflow
- Customers request books (status: PENDING)
- Staff approve/reject requests
- Approved requests automatically issue books (reduce stock, set due dates)
- Track request statuses: PENDING → APPROVED/ISSUED → COMPLETED or REJECTED

### 🔄 Issue & Return
- Issue books with automatic due date calculation (14 days)
- Return books with confirmation
- Automatic overdue detection
- Complete issue/return history

### 📊 Library Statistics
- Total books, copies, available copies
- Currently issued books
- Pending requests
- Registered customers
- Overdue books

### 🔐 Authentication
- Staff login (username/password, max 3 attempts)
- Customer login (name + 6-digit code)
- Staff password change

### 💾 Data Persistence
- Binary file storage for all data
- Automatic save after every successful operation
- Data survives program restarts

---

## 🛠️ How to Compile and Run

### Prerequisites
- GCC compiler (C11 support)
- Make (optional, for using Makefile)

### Using Makefile (Recommended)
```bash
# Build the project
make

# Run the program
make run

# Clean build artifacts
make clean

# Debug build
make debug

# Release build (optimized)
make release
```

### Manual Compilation
```bash
# Windows (PowerShell)
gcc -std=c11 -Wall -Wextra -Werror -I./src src/*.c -o ShelfSync.exe
.\ShelfSync.exe

# Linux/macOS
gcc -std=c11 -Wall -Wextra -Werror -I./src src/*.c -o ShelfSync
./ShelfSync
```

---

## 🔑 Default Login Credentials

### Staff (Admin)
```
Username: admin
Password: admin123
```

### Customer
Customers must be registered by staff first. After registration, they receive a unique 6-digit library code.

**Example Customer Registration:**
1. Staff logs in
2. Selects "Register New Customer"
3. Enters customer name (e.g., "John Doe")
4. System generates code (e.g., "742918")
5. Customer logs in with:
   - Name: John Doe
   - Code: 742918

---

## 📁 File Structure

```
ShelfSync/
├── src/
│   ├── main.c          # Entry point, menus, program flow
│   ├── auth.c/.h       # Staff/customer login, password change
│   ├── customer.c/.h   # Customer registration, search, profiles
│   ├── book.c/.h       # Book CRUD, search, display
│   ├── issue.c/.h      # Issue/return, due dates, overdue
│   ├── request.c/.h    # Request workflow (customer + staff)
│   ├── storage.c/.h    # Binary file I/O (load/save)
│   └── utils.c/.h      # Input validation, strings, dates, formatting
├── data/               # Binary data files (auto-created)
│   ├── books.dat
│   ├── customers.dat
│   ├── issues.dat
│   ├── requests.dat
│   └── staff.dat
├── Makefile            # Build automation
└── README.md           # This file
```

---

## 📋 Module Responsibilities

| Module | Responsibility |
|--------|---------------|
| `main.c` | Program entry, welcome screen, main menu, customer/staff menu loops |
| `auth.c` | Staff login (3 attempts), customer login (name+code), password change |
| `customer.c` | Register customers, generate unique 6-digit codes, search, display profiles |
| `book.c` | Add/view/search/update/delete books, formatted table display |
| `issue.c` | Issue books, return books, view issued/returned, overdue detection |
| `request.c` | Customer requests, staff approve/reject, request status tracking |
| `storage.c` | Load/save all binary data files (books, customers, issues, requests, staff) |
| `utils.c` | Safe input, string handling, date operations, formatted output |

---

## 🏗️ Data Structures

```c
// Core structures (defined in utils.h)
struct Book {
    int bookId;
    char title[50];
    char author[50];
    char category[50];
    int totalCopies;
    int availableCopies;
    int isActive;          // Soft delete flag
};

struct Customer {
    int customerId;
    char name[50];
    char uniqueCode[7];    // 6-digit code
    int isActive;
};

struct IssueRecord {
    int issueId;
    int bookId;
    int customerId;
    Date issueDate;
    Date dueDate;
    Date returnDate;
    int status;            // ISSUED, RETURNED, OVERDUE
};

struct BookRequest {
    int requestId;
    int bookId;
    int customerId;
    Date requestDate;
    int status;            // PENDING, APPROVED, REJECTED, COMPLETED
};

struct Staff {
    char username[50];
    char password[50];
    char fullName[50];
    int isActive;
};
```

---

## 🔄 Key Workflows

### Customer Request → Staff Approval → Issue
```
Customer logs in → Searches books → Requests book (PENDING)
                                    ↓
Staff logs in → Views pending requests → Approves request
                                    ↓
System: Creates issue record, reduces stock, sets due date (14 days)
                                    ↓
Request status: PENDING → APPROVED/ISSUED
```

### Book Return
```
Staff selects "Mark Book as Returned" → Enters customer code
                                    ↓
Shows customer's issued books → Staff selects book → Confirms
                                    ↓
System: Increases stock, updates issue status to RETURNED, sets return date
```

---

## ⚠️ Limitations

1. **Fixed array sizes**: MAX_BOOKS=100, MAX_CUSTOMERS=100, MAX_RECORDS=200
2. **Single staff account**: No multi-staff support with roles
3. **Console only**: No GUI or web interface
4. **Basic date handling**: No comprehensive leap year validation in all cases
5. **No fine calculation**: Overdue detection only, no monetary fines
6. **No backup/restore**: Manual file copy needed for backups
7. **Windows-centric**: Uses `system("cls")` and `_mkdir` (easily portable)

---

## 🚀 Future Improvements

- [ ] Dynamic memory allocation (linked lists) for unlimited records
- [ ] Multiple staff accounts with roles (admin, librarian, assistant)
- [ ] Book reservation queue with priority handling
- [ ] Fine calculation for overdue books
- [ ] Export reports to CSV/text files
- [ ] ISBN support and book metadata enrichment
- [ ] Category management (add/edit/delete categories)
- [ ] Backup and restore functionality
- [ ] Search pagination for large datasets
- [ ] GUI version (GTK/Qt) or web interface
- [ ] Audit trail/logging for all operations
- [ ] Book recommendations based on history

---

## 🧪 Sample Test Cases

| Test Case | Steps | Expected Result |
|-----------|-------|-----------------|
| TC-01 | Staff login with admin/admin123 | Success, staff menu shown |
| TC-02 | Staff login with wrong password 3x | Lockout, return to main menu |
| TC-03 | Add book with duplicate ID | Error: "Book ID already exists" |
| TC-04 | Add book with empty title | Error: "Title cannot be empty" |
| TC-05 | Add book with 0 copies | Error: "Copies must be positive" |
| TC-06 | Register customer "John Doe" | Success, shows 6-digit code |
| TC-07 | Register another "John Doe" | Different unique code generated |
| TC-08 | Customer login with correct code/name | Success, customer menu shown |
| TC-09 | Customer login with wrong name | Error: "Name does not match code" |
| TC-10 | Customer requests available book | Request saved as PENDING |
| TC-11 | Customer requests same book twice | Error: "Already requested" |
| TC-12 | Staff approves request with copies | Book issued, copies--, status APPROVED |
| TC-13 | Staff approves request with 0 copies | Error: "No copies available" |
| TC-14 | Staff marks book returned | Copies++, status RETURNED, return date set |
| TC-15 | View overdue books after due date | Shows overdue books with status OVERDUE |
| TC-16 | Program exit and restart | All data persisted from .dat files |

---

## ❓ Viva Questions & Answers

| Question | Answer |
|----------|--------|
| **Why binary files over text files?** | Binary preserves exact struct layout, faster I/O, no parsing needed, smaller files. |
| **What is soft delete and why use it?** | Setting `isActive=0` instead of removing from array. Preserves issue/request history referencing the book ID. |
| **How do you prevent duplicate library codes?** | Generate random 6-digit code, loop through all customers to check uniqueness, regenerate if collision. |
| **How does case-insensitive search work?** | Convert both search term and target string to lowercase, then use `strstr()`. |
| **Why separate request and issue records?** | Request = customer intent (PENDING). Issue = physical loan (ISSUED/RETURNED). Decouples approval from availability. |
| **How do you handle concurrent access?** | Single-user console app — no concurrency needed. All operations sequential in one process. |
| **What happens if power fails during save?** | Risk of partial write. Mitigation: write to temp file, then rename (atomic on POSIX). Not implemented in v1. |
| **How do you validate date input?** | Use `getCurrentDate()` for system dates. For manual entry, validate day/month/year ranges and leap years. |
| **Why `fgets()` over `scanf("%s")`?** | `fgets()` reads full line including spaces; `scanf("%s")` stops at whitespace. |
| **How do you ensure data consistency across modules?** | All modules share global arrays in `storage.c`. Every successful operation calls relevant `saveX()` immediately. |

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 👨‍💻 Author

**ShelfSync Team** - A C programming project demonstrating structured programming, file handling, and system design.

---

*Built with C11, compiled with GCC, tested on Windows/Linux/macOS.*