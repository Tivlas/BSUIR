using MauiCalculator.Lab3.Entities;
using SQLite;

namespace MauiCalculator.Lab3.Services
{

    public class SQLiteService : IDbService
    {
        private string _dbPath;
        private SQLiteConnection _conn;
        public void Init()
        {
            if (_conn != null) return;
            _dbPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "lab3.db");

            _conn = new SQLiteConnection(_dbPath);

            _conn.DropTable<JobResponsibilities>();
            _conn.DropTable<EmployeePosition>();
            _conn.CreateTable<EmployeePosition>();
            _conn.CreateTable<JobResponsibilities>();

            List<EmployeePosition> employeePositions = new List<EmployeePosition> {
                new EmployeePosition { Name = "CEO", Salary = 100000 },
                new EmployeePosition { Name = "Manager", Salary = 50000 },
                new EmployeePosition { Name = "Employee", Salary = 10000 }
            };
            List<JobResponsibilities> jobResponsibilities = new List<JobResponsibilities>
            {
                new JobResponsibilities { Description = "Managing assets", EmployeePositionID = 1 },
                new JobResponsibilities { Description = "Making major decisions", EmployeePositionID = 1 },
                new JobResponsibilities { Description = "Setting goals", EmployeePositionID = 1 },
                new JobResponsibilities { Description = "Monitor company perfomance", EmployeePositionID = 1 },
                new JobResponsibilities { Description = "Setting precedence for the working culture and environment", EmployeePositionID = 1 },

                new JobResponsibilities { Description = "Manage the software development team", EmployeePositionID = 2 },
                new JobResponsibilities { Description = "Control troject timelines", EmployeePositionID = 2 },
                new JobResponsibilities { Description = "Hiring and recruiting", EmployeePositionID = 2 },
                new JobResponsibilities { Description = "Manage the tools", EmployeePositionID = 2 },
                new JobResponsibilities { Description = "Ensure software quality", EmployeePositionID = 2 },

                new JobResponsibilities { Description = "Writing and implementing efficient code", EmployeePositionID = 3 },
                new JobResponsibilities { Description = "Maintaining and upgrading existing systems", EmployeePositionID = 3 },
                new JobResponsibilities { Description = "Testing and evaluating new programs", EmployeePositionID = 3 },
                new JobResponsibilities { Description = "Designing programs", EmployeePositionID = 3 },
                new JobResponsibilities { Description = "Training users", EmployeePositionID = 3 }
            };

            _conn.InsertAll(employeePositions);
            _conn.InsertAll(jobResponsibilities);
        }
        public IEnumerable<EmployeePosition> GetEmployeePositions()
        {
            return _conn.Table<EmployeePosition>().ToList();
        }

        public IEnumerable<JobResponsibilities> GetJobResponsibilities(int employeePositionID)
        {
            return _conn.Table<JobResponsibilities>().Where(x => x.EmployeePositionID == employeePositionID).ToList();
        }
    }
}
