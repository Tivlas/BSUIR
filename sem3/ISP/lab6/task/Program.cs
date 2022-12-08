using System.Reflection;
using System.Runtime.Loader;

namespace task
{
    public class Program
    {
        static void Main(string[] args)
        {
            IEnumerable<Employee> employees = new List<Employee>
            {
                new Employee { Name = "Вадим", Age = 19, IsMarried = false },
                new Employee { Name = "Саша", Age = 19, IsMarried = true },
                new Employee { Name = "Паша", Age = 19, IsMarried = false },
                new Employee { Name = "Дана", Age = 18, IsMarried = false },
                new Employee { Name = "Эмилия", Age = 19, IsMarried = false }
            };
            var context = new AssemblyLoadContext("FileServiceLib.dll", isCollectible: true);
            //Assembly assembly = context.LoadFromAssemblyPath("D:\\University\\BSUIR\\sem3\\ISP\\lab6\\task\\bin\\Debug\\net6.0\\FileServiceLib.dll");
            Assembly assembly = context.LoadFromAssemblyName(new AssemblyName("FileServiceLib.dll"));

            Type? type = assembly.GetType("FileServiceLib.FileService`1")?.MakeGenericType(typeof(Employee));
            if (type != null)
            {
                var fileService = Activator.CreateInstance(type);
                MethodInfo? saveMethod = type.GetMethod("SaveData");
                MethodInfo? readMethod = type.GetMethod("ReadFile");
                saveMethod?.Invoke(fileService, new object[] { employees, "employees.json" });
                var result = (IEnumerable<Employee>?)readMethod?.Invoke(fileService, new object[] { "employees.json" });
                foreach (var item in result)
                {
                    Console.WriteLine(item);
                }
            }
            context.Unload();
        }
    }
}