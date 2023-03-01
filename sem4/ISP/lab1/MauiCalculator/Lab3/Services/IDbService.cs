using MauiCalculator.Lab3.Entities;

namespace MauiCalculator.Lab3.Services
{
    public interface IDbService
    {
        void Init();
        IEnumerable<EmployeePosition> GetEmployeePositions();
        IEnumerable<JobResponsibilities> GetJobResponsibilities(int employeePositionID);
    }
}
