using System;
using System.Data;
namespace Task4
{
    class ShopDBDataContext : IShopDB
    {
        public ShopDBDataContext()
        {
            #region Orders
            DataColumn OrderID = GetNewColumn("OrderID", Type.GetType("System.Int32"), true, false, true, 1, 1);
            DataColumn CustomerNo = GetNewColumn("CustomerNo", Type.GetType("System.Int32"), false, true);
            DataColumn OrderDate = GetNewColumn("OrderDate", Type.GetType("System.DateTime"));
            DataColumn EmployeeID = GetNewColumn("EmployeeID", Type.GetType("System.Int32"), false, true);
            AddColumns(Orders, OrderID, CustomerNo, OrderDate, EmployeeID);
            Orders.PrimaryKey = new DataColumn[] { Orders.Columns["OrderID"] };
            ShopDB.Tables.Add(Orders);
            #endregion
            #region Customers
            CustomerNo = null;
            CustomerNo = GetNewColumn("CustomerNo", Type.GetType("System.Int32"), true, false, true, 1, 1);
            DataColumn FName = GetNewColumn("FName", Type.GetType("System.String"));
            DataColumn LName = GetNewColumn("LName", Type.GetType("System.String"));
            DataColumn MName = GetNewColumn("MName", Type.GetType("System.String"), false, true);
            DataColumn Address1 = GetNewColumn("Address1", Type.GetType("System.String"));
            DataColumn Address2 = GetNewColumn("Address2", Type.GetType("System.String"), false, true);
            DataColumn City = GetNewColumn("City", Type.GetType("System.String"));
            DataColumn Phone = GetNewColumn("Phone", Type.GetType("System.String"));
            DataColumn DateInSystem = GetNewColumn("DateInSystem", Type.GetType("System.DateTime"), false, true);
            AddColumns(Customers, CustomerNo, FName, LName, MName, Address1, Address2, City, Phone, DateInSystem);
            Customers.PrimaryKey = new DataColumn[] { Customers.Columns["CustomerNo"] };
            ShopDB.Tables.Add(Customers);
            #endregion
            #region Employees
            EmployeeID = null;
            FName = null;
            LName = null;
            MName = null;
            EmployeeID = GetNewColumn("EmployeeID", Type.GetType("System.Int32"), true, false, false);
            FName = GetNewColumn("FName", Type.GetType("System.String"));
            LName = GetNewColumn("LName", Type.GetType("System.String"));
            MName = GetNewColumn("MName", Type.GetType("System.String"));
            DataColumn Salary = GetNewColumn("Salary", Type.GetType("System.Decimal"));
            DataColumn PriorSalary = GetNewColumn("PriorSalary", Type.GetType("System.Decimal"));
            DataColumn HireDate = GetNewColumn("HireDate", Type.GetType("System.DateTime"));
            DataColumn TerminationDate = GetNewColumn("TerminationDate", Type.GetType("System.DateTime"), false, true);
            DataColumn ManagerEmpID = GetNewColumn("ManagerEmpID", Type.GetType("System.Int32"), false, true);
            AddColumns(Employees, EmployeeID, FName, LName, MName, Salary, PriorSalary, HireDate, TerminationDate, ManagerEmpID);
            Employees.PrimaryKey = new DataColumn[] { Employees.Columns["EmployeeID"] };
            ShopDB.Tables.Add(Employees);
            #endregion
            #region OrderDetails
            OrderID = null;
            OrderID = GetNewColumn("OrderID", Type.GetType("System.Int32"));
            DataColumn LineItem = GetNewColumn("LineItem", Type.GetType("System.Int32"));
            DataColumn ProdID = GetNewColumn("ProdID", Type.GetType("System.Int32"), false, true);
            DataColumn Qty = GetNewColumn("Qty", Type.GetType("System.Int32"));
            DataColumn UnitPrice = GetNewColumn("UnitPrice", Type.GetType("System.Decimal"));
            DataColumn TotalPrice = GetNewColumn("TotalPrice", Type.GetType("System.Decimal"), false, true);
            AddColumns(OrderDetails, OrderID, LineItem, ProdID, Qty, UnitPrice, TotalPrice);
            OrderDetails.PrimaryKey = new DataColumn[] { OrderDetails.Columns["OrderID"], OrderDetails.Columns["LineItem"] };
            ShopDB.Tables.Add(OrderDetails);
            #endregion
            #region Products
            ProdID = null;
            UnitPrice = null;
            ProdID = GetNewColumn("ProdID", Type.GetType("System.Int32"), true, false, true);
            DataColumn Description = GetNewColumn("Description", Type.GetType("System.String"));
            UnitPrice = GetNewColumn("UnitPrice", Type.GetType("System.Decimal"), false, true);
            DataColumn Weight = GetNewColumn("Weight", Type.GetType("System.Decimal"), false, true);
            AddColumns(Products, ProdID, Description, UnitPrice, Weight);
            Products.PrimaryKey = new DataColumn[] { Products.Columns["ProdID"] };
            ShopDB.Tables.Add(Products);
            #endregion
            #region FK
            ForeignKeyConstraint fk = new ForeignKeyConstraint(Customers.Columns["CustomerNo"], Orders.Columns["CustomerNo"])
            {
                ConstraintName = "fk_OrdersToCustomers",
                DeleteRule = Rule.SetNull,
                UpdateRule = Rule.Cascade
            };
            ShopDB.Tables["Orders"].Constraints.Add(fk);
            fk = null;
            fk = new ForeignKeyConstraint(Orders.Columns["OrderID"], OrderDetails.Columns["OrderID"])
            {
                ConstraintName = "fk_OrderDetailsToOrders",
                DeleteRule = Rule.Cascade,
                UpdateRule = Rule.Cascade
            };
            ShopDB.Tables["OrderDetails"].Constraints.Add(fk);
            fk = null;
            fk = new ForeignKeyConstraint(Employees.Columns["EmployeeID"], Orders.Columns["EmployeeID"])
            {
                ConstraintName = "fk_OrdersToEmloyees",
                DeleteRule = Rule.SetNull,
                UpdateRule = Rule.Cascade
            };
            ShopDB.Tables["Orders"].Constraints.Add(fk);
            fk = null;
            fk = new ForeignKeyConstraint(Products.Columns["ProdID"], OrderDetails.Columns["ProdID"])
            {
                ConstraintName = "fk_OrderDetailsToProducts",
                DeleteRule = Rule.SetNull,
                UpdateRule = Rule.Cascade
            };
            ShopDB.Tables["OrderDetails"].Constraints.Add(fk);
            #endregion
            #region Relations
            ShopDB.Relations.Add("OrdersToCustomers", Customers.Columns["CustomerNo"], Orders.Columns["CustomerNo"]);
            ShopDB.Relations.Add("OrderDetailsToOrders", Orders.Columns["OrderID"], OrderDetails.Columns["OrderID"]);
            ShopDB.Relations.Add("OrdersToEmloyees", Employees.Columns["EmployeeID"], Orders.Columns["EmployeeID"]);
            ShopDB.Relations.Add("OrderDetailsToProducts", Products.Columns["ProdID"], OrderDetails.Columns["ProdID"]);
            #endregion
        }

        public DataSet ShopDB { get; } = new DataSet("ShopDB");
        public DataTable Orders { get; } = new DataTable("Orders");
        public DataTable Customers { get; } = new DataTable("Customers");
        public DataTable Employees { get; } = new DataTable("Employees");
        public DataTable OrderDetails { get; } = new DataTable("OrderDetails");
        public DataTable Products { get; } = new DataTable("Products");

        private DataColumn GetNewColumn(string colName, Type colType, bool unique = false, bool canNull = false, bool ai = false, int aiSeed = 1, int aiStep = 1)
        {
            DataColumn col = new DataColumn(colName, colType);
            col.Unique = unique == true ? true : false;
            col.AllowDBNull = canNull == true ? true : false;
            col.AutoIncrement = ai == true ? true : false;
            if (ai)
            {
                col.AutoIncrementSeed = aiSeed;
                col.AutoIncrementStep = aiStep;
            }
            return col;
        }

        private void AddColumns(DataTable dt, params DataColumn[] ps)
        {
            foreach (DataColumn col in ps)
            {
                dt.Columns.Add(col);
            }
        }
    }
}
