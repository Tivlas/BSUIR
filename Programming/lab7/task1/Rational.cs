namespace task1
{
    public class Rational
    {
        private int numerator = 0;
        private int denominator = 1;

        public Rational(int numerator, int denominator)
        {
            this.numerator = numerator;
            this.denominator = denominator;
        }

        public int Numerator
        {
            get
            {
                return numerator;
            }
            set
            {
                numerator = value;
            }
        }

        public int Denominator
        {
            get
            {
                return denominator;
            }
            set
            {
                if (value == 0)
                {
                    denominator = 1;
                    throw new ArgumentException("Denominator can't be zero");
                }
                denominator = value;
            }
        }

        // НОД
        public int GCD(int a, int b)
        {
            return b != 0 ? Math.Abs(GCD(b, a % b)) : a;
        }

        // сокращение дроби
        public void ReduceFraction()
        {

            int gcd = GCD(numerator, denominator);
            numerator /= gcd;
            denominator /= gcd;
        }

        // сложение
        public static Rational operator +(Rational first, Rational second)
        {
            int numerator = first.numerator * second.denominator + second.numerator * first.denominator;
            int denominator = first.denominator * second.denominator;
            Rational result = new Rational(numerator, denominator);
            result.ReduceFraction();
            return result;
        }

        public static Rational operator +(int number, Rational fraction)
        {
            int numerator = number * fraction.denominator + fraction.numerator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.ReduceFraction();
            return result;
        }

        // вычитание
        public static Rational operator -(Rational first, Rational second)
        {
            int numerator = first.numerator * second.denominator - second.numerator * first.denominator;
            int denominator = first.denominator * second.denominator;
            Rational result = new Rational(numerator, denominator);
            result.ReduceFraction();
            return result;
        }

        public static Rational operator -(int number, Rational fraction)
        {
            int numerator = number * fraction.denominator - fraction.numerator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.ReduceFraction();
            return result;
        }

        // умножение
        public static Rational operator *(Rational first, Rational second)
        {
            int numerator = first.numerator * second.numerator;
            int denominator = first.denominator * second.denominator;
            Rational result = new Rational(numerator, denominator);
            result.ReduceFraction();
            return result;
        }

        public static Rational operator *(int number, Rational fraction)
        {
            int numerator = number * fraction.numerator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.ReduceFraction();
            return result;
        }

        // деление
        public static Rational operator /(Rational first, Rational second)
        {
            int numerator = first.numerator * second.denominator;
            int denominator = first.denominator * second.numerator;
            if (denominator == 0)
            {
                numerator = 0;
                denominator = 1;
                throw new DivideByZeroException("Can't divide by zero");
            }
            Rational result = new Rational(numerator, denominator);
            result.ReduceFraction();
            return result;
        }

        public static Rational operator /(int number, Rational fraction)
        {
            int numerator = number * fraction.denominator;
            int denominator = fraction.numerator;
            if (denominator == 0)
            {
                numerator = 0;
                denominator = 1;
                throw new DivideByZeroException("Can't divide by zero");
            }
            Rational result = new Rational(numerator, denominator);
            result.ReduceFraction();
            return result;
        }

        // инкремент и декремент
        public static Rational operator ++(Rational fraction)
        {
            int numerator = fraction.numerator + fraction.denominator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.ReduceFraction();
            return result;
        }

        public static Rational operator --(Rational fraction)
        {
            int numerator = fraction.numerator - fraction.denominator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.ReduceFraction();
            return result;
        }

        // сравнение
        public static bool operator ==(Rational first, Rational second)
        {
            return first.numerator == second.numerator && first.denominator == second.denominator;
        }

        public static bool operator !=(Rational first, Rational second)
        {
            return first.numerator != second.numerator || first.denominator != second.denominator;
        }

        public static bool operator >(Rational first, Rational second)
        {
            return first.numerator * second.denominator > second.numerator * first.denominator;
        }

        public static bool operator <(Rational first, Rational second)
        {
            return first.numerator * second.denominator < second.numerator * first.denominator;
        }

        // операторы true и false
        public static bool operator true(Rational fraction)
        {
            return fraction.numerator != 0;
        }

        public static bool operator false(Rational fraction)
        {
            return fraction.numerator == 0;
        }

        // преобразование в int и обратно
        public static explicit operator int(Rational fraction)
        {
            return fraction.numerator / fraction.denominator;
        }

        public static implicit operator Rational(int number)
        {
            return new Rational(number, 1);
        }

        // преобразование в строку
        public override string ToString()
        {
            return numerator + "/" + denominator;
        }
    }
}
