using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

                denominator = value;
            }
        }
        public int GCD(int a, int b)
        {
            a = Math.Abs(a);
            b = Math.Abs(b);
            if (a < b)
            {
                int temp = a;
                a = b;
                b = temp;
            }
            return b != 0 ? GCD(b, a % b) : a;
        }

        public void reduceFraction()
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
            result.reduceFraction();
            return result;
        }

        public static Rational operator +(Rational fraction, int number)
        {
            int numerator = fraction.numerator + number * fraction.denominator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        public static Rational operator +(int number, Rational fraction)
        {
            int numerator = number * fraction.denominator + fraction.numerator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        // вычитание
        public static Rational operator -(Rational first, Rational second)
        {
            int numerator = first.numerator * second.denominator - second.numerator * first.denominator;
            int denominator = first.denominator * second.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        public static Rational operator -(Rational fraction, int number)
        {
            int numerator = fraction.numerator - number * fraction.denominator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        public static Rational operator -(int number, Rational fraction)
        {
            int numerator = number * fraction.denominator - fraction.numerator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        // умножение
        public static Rational operator *(Rational first, Rational second)
        {
            int numerator = first.numerator * second.numerator;
            int denominator = first.denominator * second.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        public static Rational operator *(Rational fraction, int number)
        {
            int numerator = fraction.numerator * number;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        public static Rational operator *(int number, Rational fraction)
        {
            int numerator = number * fraction.numerator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        // деление
        public static Rational operator /(Rational first, Rational second)
        {
            int numerator = first.numerator * second.denominator;
            int denominator = first.denominator * second.numerator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        public static Rational operator /(Rational fraction, int number)
        {
            int numerator = fraction.numerator;
            int denominator = fraction.denominator * number;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        public static Rational operator /(int number, Rational fraction)
        {
            int numerator = number * fraction.denominator;
            int denominator = fraction.numerator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        // инкремент и декремент
        public static Rational operator ++(Rational fraction)
        {
            int numerator = fraction.numerator + fraction.denominator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }
        
        public static Rational operator --(Rational fraction)
        {
            int numerator = fraction.numerator - fraction.denominator;
            int denominator = fraction.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
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
