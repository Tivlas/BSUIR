using System;




    public class Point
    {
        private string? want;
        private double x = 0, y = 0;

        public int BelongToArea(double x, double y)
        {
        double num = x * x + y * y;
        double diff = (x * x + y * y) * 0.00001;
            if (x * x + y * y < 49 && x * x + y * y > 9)
            {
                return 1;
            }
            else if (Math.Abs(num-49)<=diff || Math.Abs(num - 9) <= diff)
            {
                return 2;
            }
            else
            {
                return 3;
            }
        }
        private bool WannaContinue()
        {
            Console.Write("Хотите завершить работу с программой(введите да/нет): ");
            want = Console.ReadLine();
            switch (want)
            {
                case "да":
                    return true;
                case "нет":
                    return false;
                default:
                    Console.WriteLine("Вы не ввели ни 'да', ни 'нет'");
                    return WannaContinue();
            }
        }
        public bool Menu()
        {
            Console.Write("Введите абсциссу: ");
            while (!double.TryParse(Console.ReadLine(), out x)/* && !double.TryParse(Console.ReadLine(), out y)*/)
            {
                Console.WriteLine("Вы вели некорректные данные. Попробуйте снова");
            }
            Console.Write("Введите ординату: ");
            while (!double.TryParse(Console.ReadLine(), out y))
            {
                Console.WriteLine("Вы вели некорректные данные. Попробуйте снова");
            }
            int situation = BelongToArea(x, y);
            switch (situation)
            {
                case 1:
                    Console.WriteLine("Да\n");
                    break;
                case 2:
                    Console.WriteLine("На границе\n");
                    break;
                case 3:
                    Console.WriteLine("Нет\n");
                    break;
            }
            if (WannaContinue())
            {
                return true;
            }
            else
            {
                return false;
            }

        }
    }

