namespace numbers
{
    public class Numbers
    {
        public static double max(double x, double y)
        {
            return x > y ? x : y;
        }

        public static double calc(double x, double y)
        {
            return Numbers.max(x, 2 * y + x) - Numbers.max(7 * x + 2 * y, y);
        }
    }
}
