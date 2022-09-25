namespace Task
{
    public class StoreGood
    {
        public string? Name { get; set; }
        public int Cost { get; set; }
        public bool IsAtDiscount { get; set; }

        public StoreGood(string name, int cost, bool isAtDiscount)
        {
            Name = name;
            Cost = cost;
            IsAtDiscount = isAtDiscount;
        }

        public StoreGood()
        {
        }

        public override string ToString()
        {
            return $"Name: {Name}, cost: {Cost}, isAtDiscount: {IsAtDiscount}.";
        }
    }
}
