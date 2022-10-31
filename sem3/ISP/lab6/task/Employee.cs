namespace task
{
    public class Employee
    {
        public string Name { get; set; } = "Default-name";
        public int Age { get; set; } = 0;
        public bool IsMarried { get; set; } = false;

        public override string ToString()
        {
            return $"Name: {Name}, Age: {Age}, IsMarried: {IsMarried}";
        }
    }
}
