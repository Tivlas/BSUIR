namespace lab5.Domain
{
    public class HrDepartment
    {
        private List<string>? _vacancies = new();

        private List<Employee>? _employees = new();

        public List<string>? Vacancies { get => _vacancies; init => _vacancies = value; }

        public List<Employee>? Employees { get => _employees; init => _employees = value; }

        public int NumberOfEmployees { get => _employees!.Count; }

        public void AddVacancy(string vacancy)
        {
            _vacancies?.Add(vacancy);
        }

        public void AddEmployee(Employee employee)
        {
            _employees?.Add(employee);
        }

        public void RemoveVacancy(string vacancy)
        {
            _vacancies?.Remove(vacancy);
        }

        public void RemoveEmployee(Employee employee)
        {
            _employees?.Remove(employee);
        }

        public override string ToString()
        {
            return $"Vacancies: {string.Join(", ", _vacancies!)}\nEmployees: {string.Join(", ", _employees!)}";
        }
    }

    public class Employee
    {
        public string? Name { get; set; } = "Employee";

        public override bool Equals(object? obj)
        {
            return obj is Employee employee && Name == employee.Name;
        }

        public override int GetHashCode()
        {
            return Name!.GetHashCode();
        }

        public override string ToString()
        {
            return $"Employee name: {Name}";
        }
    }
}
