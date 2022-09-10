using System;

    public class Number
    {
        private string? want;
        public bool EndsWith7(int n)
        {
            if ((n - 7) % 10 == 0)
            {
                return true;
            }
            return false;
        }
        private bool WannaContinue()
        {
            Console.Write("Do you wanna stop using the programm(enter yes/no): ");
            want = Console.ReadLine();
            switch (want)
            {
                case "yes":
                    return true;
                case "no":
                    return false;
                default:
                    Console.WriteLine("You didn't enter either yes or no!");
                    return WannaContinue();
            }
        }
        public bool Menu()
        {
            Console.Write("Enter an integer number: ");
            int num = 0;
            while (!int.TryParse(Console.ReadLine(), out num))
            {
                Console.WriteLine("This is not a number!");
            }
            if (EndsWith7(num))
            {
                Console.WriteLine("The entered number ends with 7\n");
            }
            else
            {
                Console.WriteLine("The entered number doesn't end with 7\n");
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

