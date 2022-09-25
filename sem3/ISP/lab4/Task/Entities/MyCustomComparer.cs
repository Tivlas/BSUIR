namespace Task
{
    public class MyCustomComparer : IComparer<StoreGood>
    {
        public int Compare(StoreGood? x, StoreGood? y)
        {
            return string.Compare(x?.Name, y?.Name, StringComparison.Ordinal);
        }
    }
}
