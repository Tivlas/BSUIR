namespace task1;
class Program
{
    static void Main(string[] args)
    {
        Rational fr1 = new Rational(1, 2); // fr ~ fraction
        Rational fr2 = new Rational(1, 3);
        Rational fr3 = new Rational(3, 4);
        Rational fr4 = new Rational(16, 72);
        Console.WriteLine("fr1: " + fr1);
        Console.WriteLine("fr2: " + fr2);
        Console.WriteLine("fr3: " + fr3);
        Console.WriteLine("fr4: " + fr4);
        Console.WriteLine();

        Console.WriteLine("fr1 + fr2: " + (fr2 + fr1));
        Console.WriteLine("fr1 + 4: " + (fr1 + 4));
        Console.WriteLine("4 + fr1: " + (4 + fr1));
        Console.WriteLine();

        Console.WriteLine("fr1 - fr2: " + (fr2 - fr1));
        Console.WriteLine("fr1 - 4: " + (fr1 - 4));
        Console.WriteLine("4 - fr1: " + (4 - fr1));
        Console.WriteLine();

        Console.WriteLine("fr1 * fr2: " + (fr2 * fr1));
        Console.WriteLine("fr1 * 4: " + (fr1 * 4));
        Console.WriteLine("4 * fr1: " + (4 * fr1));
        Console.WriteLine();

        Console.WriteLine("fr1 / fr2: " + (fr2 / fr1));
        Console.WriteLine("fr1 / 4: " + (fr1 / 4));
        Console.WriteLine("4 / fr1: " + (4 / fr1));
        Console.WriteLine();

        Console.WriteLine("fr1 == fr2: " + (fr2 == fr1));
        Console.WriteLine("fr1 != fr2: " + (fr2 != fr1));
        Console.WriteLine("fr1 < fr2: " + (fr2 < fr1));
        Console.WriteLine("fr1 > fr2: " + (fr2 > fr1));
        Console.WriteLine();

        Console.WriteLine("fr1--: " + (fr1--));
        Console.WriteLine("--fr1: " + --fr1);

        Console.WriteLine("fr2++: " + (fr2++));
        Console.WriteLine("++fr2: " + ++fr2);
        Console.WriteLine();

        fr4.ReduceFraction();
        Console.WriteLine("Reduce fraction fr4: " + fr4);
        Console.WriteLine();

        if (fr1)
        {
            Console.WriteLine("fr1 is true");
        }
        else
        {
            Console.WriteLine("fr1 is false");
        }

        int temp1 = (int)fr4;
        fr4.Numerator = 6;
        fr4.Denominator = 3;
        int temp2 = (int)fr4;
        Console.WriteLine("fr4: " + fr4);
        Rational fr5 = (Rational)temp2;
        Console.WriteLine("fr5: " + fr5);
        Console.WriteLine();
    }
}
