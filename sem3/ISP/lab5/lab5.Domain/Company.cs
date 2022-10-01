namespace lab5.Domain
{
    public class Company
    {
        private HrDepartment? _hrDepartment = new();

        public HrDepartment? HrDepartment { get => _hrDepartment; init => _hrDepartment = value; }

        public string? Name { get; set; } = "Company";

        public int NumberOfEmployees { get => _hrDepartment!.NumberOfEmployees; }

        public void AddEmployee(Employee employee)
        {
            _hrDepartment?.AddEmployee(employee);
        }

        public void RemoveEmployee(Employee employee)
        {
            _hrDepartment?.RemoveEmployee(employee);
        }

        public void AddVacancy(string vacancy)
        {
            _hrDepartment?.AddVacancy(vacancy);
        }

        public void RemoveVacancy(string vacancy)
        {
            _hrDepartment?.RemoveVacancy(vacancy);
        }

        public override string ToString()
        {
            return $"Company name: {Name}.\n{_hrDepartment}\n";
        }
    }
}
