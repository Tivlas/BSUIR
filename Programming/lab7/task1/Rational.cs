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

        public static Rational operator +(Rational first, int second)
        {
            int numerator = first.numerator + second * first.denominator;
            int denominator = first.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        public static Rational operator +(int first, Rational second)
        {
            int numerator = first * second.denominator + second.numerator;
            int denominator = second.denominator;
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

        public static Rational operator -(Rational first, int second)
        {
            int numerator = first.numerator - second * first.denominator;
            int denominator = first.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        public static Rational operator -(int first, Rational second)
        {
            int numerator = first * second.denominator - second.numerator;
            int denominator = second.denominator;
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

        public static Rational operator *(Rational first, int second)
        {
            int numerator = first.numerator * second;
            int denominator = first.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        public static Rational operator *(int first, Rational second)
        {
            int numerator = first * second.numerator;
            int denominator = second.denominator;
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

        public static Rational operator /(Rational first, int second)
        {
            int numerator = first.numerator;
            int denominator = first.denominator * second;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        public static Rational operator /(int first, Rational second)
        {
            int numerator = first * second.denominator;
            int denominator = second.numerator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }

        // инкремент и декремент
        public static Rational operator ++(Rational first)
        {
            int numerator = first.numerator + first.denominator;
            int denominator = first.denominator;
            Rational result = new Rational(numerator, denominator);
            result.reduceFraction();
            return result;
        }
        
        public static Rational operator --(Rational first)
        {
            int numerator = first.numerator - first.denominator;
            int denominator = first.denominator;
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
        public static bool operator true(Rational first)
        {
            return first.numerator != 0;
        }

        public static bool operator false(Rational first)
        {
            return first.numerator == 0;
        }

        // преобразование в строку
        public override string ToString()
        {
            return numerator + "/" + denominator;
        }

        
    }
}
