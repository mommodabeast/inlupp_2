# inlupp_2


Prerequisites
Before running the program, ensure that you have the following installed on your system:


GCC Compiler: The program is written in C, and you need a C compiler to build and run it.
CUnit: This is a unit testing framework for C. It is used to run tests for the program logic.
Building and Running the Program
To build and run the Warehouse Management System, follow these steps:


Open a terminal in the project directory.


Run the following command to compile the logic and create an executable for the program:


"make logic_run"
After successfully compiling, run the program using the following command:


./run
You will see the Warehouse Management System menu. Follow the on-screen instructions to perform various actions.


Using the Warehouse Management System
The program provides a menu-driven interface for interacting with the Warehouse Management System. Here is a quick overview of the available actions:


Add Merchandise (A/a): Add new merchandise to the warehouse.
Replenish Merchandise (P/p): Replenish the stock of existing merchandise.
List Merchandise (L/l): Display a list of all merchandise in the warehouse.
Show Stock (S/s): Show the current stock of merchandise.
Create Cart (C/c): Create a new shopping cart.
Remove Cart (R/r): Remove an existing shopping cart.
Add to Cart (+): Add merchandise to a shopping cart.
Remove from Cart (-): Remove merchandise from a shopping cart.
Calculate Cost (=): Calculate the total cost of items in a shopping cart.
Checkout (O/o): Complete the checkout process.
Exit (Q/q): Exit the program.


Testing
If you want to run tests for the program logic, you can use the following commands:


Run logic tests:


"make test_logic"
Run logic tests with Valgrind for memory checks:


"make logic_run_valgrind"


Cleaning Up
To clean up the compiled files and executable, use the following command:


"make clean"
This will remove the run executable and any temporary files generated during the compilation process.


Enjoy using the Warehouse Management System! If you encounter any issues or have questions, feel free to reach out for assistance.
