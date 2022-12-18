namespace TaskLib
{
    public class Car
    {
        public int ID { get; set; } = 0;
        public string Name { get; set; } = "No Name";
        public int EngineCapacity { get; set; } = 0;

        public Car(int id, string name, int engineCapacity)
        {
            ID = id;
            Name = name;
            EngineCapacity = engineCapacity;
        }

        public Car()
        {
        }
    }
}
