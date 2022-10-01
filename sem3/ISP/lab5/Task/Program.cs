using lab5.Domain;
using Serializer;

namespace Task
{
    class Program
    {
        static void Main(string[] args)
        {
            var companies = CreateListOfCompanies();
            Serializer.Serializer serializer = new();
            serializer.SerializeByLinq(companies, "companiesByLing.xml");
            serializer.SerializeByXml(companies, "companiesByXml.xml");
            var companies1 = serializer.DeSerializeByLinq("companiesByLing.xml");
            var companies2 = serializer.DeSerializeByXml("companiesByXml.xml");
            Console.WriteLine("---------------------Исходные данные---------------------");
            foreach (var company in companies)
            {
                Console.WriteLine(company);
            }

            Console.WriteLine("---------------------Данные после десериализации ling---------------------");
            foreach (var company in companies1)
            {
                Console.WriteLine(company);
            }

            Console.WriteLine("---------------------Данные после десериализации xml---------------------");
            foreach (var company in companies2)
            {
                Console.WriteLine(company);
            }
        }

        static List<Company> CreateListOfCompanies()
        {
            List<Company> companies = new List<Company>
            {
                new Company
                {
                    Name = "Company1",
                    HrDepartment = new HrDepartment
                    {
                        Employees = new List<Employee>
                        {
                            new Employee
                            {
                                Name = "Employee1"
                            },
                            new Employee
                            {
                                Name = "Employee2"
                            }
                        },
                        Vacancies = new List<string>
                        {
                            "Vacancy1",
                            "Vacancy2"
                        }
                    }
                },
                new Company
                {
                    Name = "Company2",
                    HrDepartment = new HrDepartment
                    {
                        Employees = new List<Employee>
                        {
                            new Employee
                            {
                                Name = "Employee3"
                            },
                            new Employee
                            {
                                Name = "Employee4"
                            }
                        },
                        Vacancies = new List<string>
                        {
                            "Vacancy3",
                            "Vacancy4"
                        }
                    }
                },
                new Company
                {
                    Name = "Company3",
                    HrDepartment = new HrDepartment
                    {
                        Employees = new List<Employee>
                        {
                            new Employee
                            {
                                Name = "Employee5"
                            },
                            new Employee
                            {
                                Name = "Employee6"
                            }
                        },
                        Vacancies = new List<string>
                        {
                            "Vacancy5",
                            "Vacancy6"
                        }
                    }
                },
                new Company
                {
                    Name = "Company4",
                    HrDepartment = new HrDepartment
                    {
                        Employees = new List<Employee>
                        {
                            new Employee
                            {
                                Name = "Employee7"
                            },
                            new Employee
                            {
                                Name = "Employee8"
                            }
                        },
                        Vacancies = new List<string>
                        {
                            "Vacancy7",
                            "Vacancy8"
                        }
                    }
                },
                new Company
                {
                    Name = "Company5",
                    HrDepartment = new HrDepartment
                    {
                        Employees = new List<Employee>
                        {
                            new Employee
                            {
                                Name = "Employee9"
                            },
                            new Employee
                            {
                                Name = "Employee10"
                            }
                        },
                        Vacancies = new List<string>
                        {
                            "Vacancy9",
                            "Vacancy10"
                        }
                    }
                }
            };
            return companies;
        }
    }
}

